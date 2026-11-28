"""Byte matching (phase 2): how close a compiled function is to the original.

    python tools/matchdiff.py <addr> [--obj=<file.obj>] [--src=<file.cpp>] [--sym=<name>] [--cl=<compiler>] [--flags=...] [--diff]
    python tools/matchdiff.py --all <file.cpp> [--cl=...] [--flags=...]     (every RT3_REPLACE of the file)

Compiles the source file (or takes an .obj), finds the function in the object's
COFF symbol table (the name registered with RT3_REPLACE for <addr>, or --sym),
and compares its machine code with the original's bytes in the exe (size from
decomp/index.tsv), instruction by instruction with capstone:
- operands the linker fills in (relocations in the .obj; absolute addresses
  of the image in the original) compare as symbols, not values;
- branch targets compare as offsets from the function start.
Prints a score (matching instructions / instructions of the longer side, 1.0
= byte matching apart from relocated operands) and with --diff the differing
instructions side by side.

The original was built with Visual C++ .NET 2002 (cl 13.00.9466). Until that
compiler is installed, --cl defaults to the Visual Studio 2019 cl (from
vcvars32), so scores are only a rough guide; --cl=<path to cl.exe> switches.
"""
import difflib
import os
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

import capstone

from pe import PE
from replacements import REPLACEMENT_RE

ROOT = Path(__file__).resolve().parents[1]
VCVARS = r"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"
# Flags close to the original build: speed optimisation, x87 floating point.
# (VC7.0's /G6 /Ox /Gy are the likely ones; VC2019 has no /G6.)
DEFAULT_FLAGS = "/nologo /c /O2 /Oy /Gy /GS- /MT /EHsc /Isrc /D_CRT_SECURE_NO_WARNINGS /arch:IA32 /fp:precise /std:c++17"


def compile_obj(src, cl, flags):
    out = Path(tempfile.gettempdir()) / "rt3_matchdiff"
    out.mkdir(exist_ok=True)
    obj = out / (Path(src).stem + ".obj")
    if cl:
        cmd = f'"{cl}" {flags} "{src}" /Fo"{obj}"'
    else:
        cmd = f'call "{VCVARS}" >nul 2>nul && cl {flags} "{src}" /Fo"{obj}"'
    bat = out / "cc.bat"
    bat.write_text(f"@echo off\ncd /d \"{ROOT}\"\n{cmd}\n")
    p = subprocess.run(["cmd", "/c", str(bat)], capture_output=True, text=True, errors="replace")
    if not obj.exists() or p.returncode:
        sys.exit("compile failed:\n" + p.stdout[-3000:] + p.stderr[-2000:])
    return obj


class Coff:
    """The parts of a COFF object needed here: sections, relocations, symbols."""

    def __init__(self, path):
        d = self.d = Path(path).read_bytes()
        machine, nsec, _, symptr, nsym, optsz, _ = struct.unpack_from("<HHIIIHH", d, 0)
        self.secs = []
        for i in range(nsec):
            o = 20 + optsz + i * 40
            name = d[o:o + 8].rstrip(b"\0").decode("latin-1")
            size, raw, relptr, _, nrel = struct.unpack_from("<IIIIH", d, o + 16)
            relocs = [struct.unpack_from("<IIH", d, relptr + 10 * k) for k in range(nrel)]
            self.secs.append((name, size, raw, relocs))
        strtab = symptr + nsym * 18
        self.syms = []
        k = 0
        while k < nsym:
            o = symptr + k * 18
            raw_name = d[o:o + 8]
            if raw_name[:4] == b"\0\0\0\0":
                off = struct.unpack_from("<I", raw_name, 4)[0]
                name = d[strtab + off:d.index(b"\0", strtab + off)].decode("latin-1")
            else:
                name = raw_name.rstrip(b"\0").decode("latin-1")
            value, secno, typ, cls, naux = struct.unpack_from("<IhHBB", d, o + 8)
            self.syms.append((name, value, secno, typ, cls))
            k += 1 + naux

    def function(self, wanted):
        """(bytes, relocated offsets) of the function whose decorated name holds `wanted`."""
        cands = [s for s in self.syms if s[2] > 0 and s[3] == 0x20 and re.search(r"(^|[?_@])" + re.escape(wanted) + r"@", s[0])]
        if not cands:
            cands = [s for s in self.syms if s[2] > 0 and s[3] == 0x20 and wanted in s[0]]
        if not cands:
            sys.exit(f"no function symbol matching {wanted!r} in the object")
        name, value, secno, _, _ = cands[0]
        sname, size, raw, relocs = self.secs[secno - 1]
        ends = sorted(v for n, v, s, t, c in self.syms if s == secno and v > value)
        end = ends[0] if ends else size
        code = self.d[raw + value:raw + end]
        # The compiler pads a function's section to 16 bytes (nop / int3): not part of it.
        while code and code[-1] in (0x90, 0xCC) and len(code) > 1:
            code = code[:-1]
        rel = set()
        for va, _, _ in relocs:
            if value <= va < end:
                rel.update(range(va - value, va - value + 4))
        return name, code, rel


def index_sizes():
    sizes = {}
    for line in (ROOT / "decomp" / "index.tsv").read_text().splitlines():
        a, _, s, *_ = line.split("\t")
        sizes[int(a, 16)] = int(s)
    return sizes


def normalize(code, base, reloc=None, image=False):
    """Instructions as text with the linker-filled operands and branch targets made relative."""
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    out = []
    for ins in md.disasm(code, base):
        start = ins.address - base
        text = f"{ins.mnemonic} {ins.op_str}".strip()
        relocated = reloc is not None and any(start <= r < start + ins.size for r in reloc)
        if relocated and (ins.group(capstone.CS_GRP_JUMP) or ins.group(capstone.CS_GRP_CALL)):
            text = f"{ins.mnemonic} <sym>"
        elif ins.group(capstone.CS_GRP_JUMP) or ins.group(capstone.CS_GRP_CALL):
            if ins.operands and ins.operands[0].type == capstone.x86.X86_OP_IMM:
                target = ins.operands[0].imm
                inside = base <= target < base + len(code)
                text = f"{ins.mnemonic} " + (f"+{target - base:x}" if inside else "<sym>")
        elif relocated:  # (an object's unrelocated address can be 0: "[0]", "push 0")
            text = re.sub(r"0x[0-9a-f]+|(?<=\[)\d+(?=\])|(?<=[ ,])\d+$", "<sym>", text)
        else:  # absolute addresses of the game image (ours reads globals by address too)
            text = re.sub(r"0x([0-9a-f]{6,8})", lambda m: "<sym>" if 0x400000 <= int(m.group(1), 16) < 0xe00000 else m.group(0), text)
        out.append((start, ins.bytes.hex(), text))
    return out


def compare(addr, obj, sym, show):
    sizes = index_sizes()
    if addr not in sizes:
        sys.exit(f"{addr:#x} is not a function start in decomp/index.tsv")
    orig = PE().read(addr, sizes[addr])
    name, code, rel = Coff(obj).function(sym)
    a = normalize(orig, addr, image=True)
    b = normalize(code, 0, reloc=rel)
    sm = difflib.SequenceMatcher(a=[t for _, _, t in a], b=[t for _, _, t in b], autojunk=False)
    same = sum(bl.size for bl in sm.get_matching_blocks())
    score = same / max(len(a), len(b), 1)
    exact = orig == code
    print(f"{addr:06x} {sym}: score {score:.3f} ({same}/{max(len(a), len(b))} instructions; original {len(orig)} bytes, "
          f"ours {len(code)}){' BYTE MATCH' if exact else ''}")
    if show:
        for tag, i1, i2, j1, j2 in sm.get_opcodes():
            if tag == "equal":
                continue
            for k in range(max(i2 - i1, j2 - j1)):
                l = a[i1 + k][2] if i1 + k < i2 else ""
                r = b[j1 + k][2] if j1 + k < j2 else ""
                print(f"  {l:<44} | {r}")
    return score


def main():
    args = [x for x in sys.argv[1:] if not x.startswith("--")]
    opt = {x.split("=", 1)[0][2:]: (x.split("=", 1)[1] if "=" in x else True) for x in sys.argv[1:] if x.startswith("--")}
    cl, flags = opt.get("cl"), opt.get("flags", DEFAULT_FLAGS)
    regs = {}
    for p in (ROOT / "src").rglob("*.cpp"):
        for m in REPLACEMENT_RE.finditer(p.read_text(encoding="utf-8", errors="replace")):
            regs.setdefault(int(m.group(1), 16), (m.group(2), p))
    if "all" in opt:
        src = Path(args[0])
        obj = compile_obj(src, cl, flags)
        scores = []
        for a, (n, p) in sorted(regs.items()):
            if p.resolve() == src.resolve():
                try:
                    scores.append(compare(a, obj, n, False))
                except SystemExit as e:
                    print(f"{a:06x} {n}: {e}")
        if scores:
            print(f"{len(scores)} functions, mean score {sum(scores) / len(scores):.3f}, "
                  f"byte matching {sum(1 for s in scores if s == 1.0)}")
        return 0
    addr = int(args[0], 16)
    name, src = regs.get(addr, (None, None))
    sym = opt.get("sym", name)
    if not sym:
        sys.exit(f"no RT3_REPLACE for {addr:#x}; give --sym")
    obj = Path(opt["obj"]) if "obj" in opt else compile_obj(Path(opt.get("src", src)), cl, flags)
    compare(addr, obj, sym, "diff" in opt)
    return 0


if __name__ == "__main__":
    sys.exit(main())
