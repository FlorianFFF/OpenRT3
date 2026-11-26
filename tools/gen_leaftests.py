"""Generates tests/test_leaves.cpp: one differential test over every rewritten
leaf function (the original calls nothing, or only pure C runtime functions).

A leaf can only read its arguments, memory and globals, so it is checked
generically: both versions get the same arguments (pointers to buffers of
random words, which are themselves pointers into the buffers, small numbers
or random bits; small integers; floats) and must return the same value (EAX,
and the x87 result for float returns) and leave every buffer the same. A
fault counts as a result: both must fault alike.

The argument kinds come from the rewrite's own signature (thiscall methods
are written as __fastcall(this, int, ...), so ECX / EDX / stack follow the
declaration). The rewrite itself is found through hook::Find at run time.

    python tools/gen_leaftests.py
"""
import re
import struct
from pathlib import Path

from pe import find_exe
from replacements import ORIGINAL_ONLY, REPLACEMENT_RE

ROOT = Path(__file__).resolve().parents[1]
# C runtime functions a leaf may call: pure (no heap, files, or state).
PURE_CRT = {
    0x5a10d0,  # ftol
    0x5a57cf, 0x5a5f26, 0x5a20b0, 0x5a18a0, 0x5a1ff0, 0x5a19c4, 0x5a1df0, 0x5a330d,  # string functions, sprintf
    0x5a152e, 0x5a1634, 0x5a1390, 0x5a13e4, 0x5a1b60, 0x5a1270,  # fabs sqrt fmod sin pow atan
}

# Results where only part of EAX is the result: the original leaves the rest
# as it was, and every caller reads only that part.
RESULT = {
    0x4490a0: "B",  # BrightnessOf: AL (all callers in 0x44a9a0 store AL)
    0x531f90: "V",  # Sound_RegisterEos: EAX untouched on the early paths; both callers ignore it
}

# Leaves left out, with the reason.
SKIP = {
    # The original's __try uses the exe's C runtime SEH handler, which notes the
    # unwind target in its own debugger hook data (0x62a35c, _NLG_Notify) when
    # the probed instruction faults; ours uses our runtime's. Not game state.
    0x55cd40: "Cpu_ProbeFeature",
}

INTS = {"int", "int8_t", "int16_t", "int32_t", "uint8_t", "uint16_t", "uint32_t", "bool", "char",
        "unsigned", "long", "short", "DWORD", "BOOL", "uint", "size_t", "unsigned int", "signed char",
        "unsigned char", "unsigned short", "unsigned long", "HRESULT"}


def kind(param):
    """'p' pointer, 'i' integer, 'f' float, 'd' double (two stack words), None unknown."""
    p = re.sub(r"\b(const|volatile|struct|enum)\b", " ", param)
    p = re.sub(r"/\*.*?\*/", " ", p).strip()
    if not p or p == "void":
        return ""
    if "*" in p or "&" in p or "(" in p:
        return "p"
    base = re.sub(r"\s+\w+$", "", p).strip() if len(p.split()) > 1 else p
    if base == "float":
        return "f"
    if base == "double":
        return "d"
    if base in INTS:
        return "i"
    return None


def ret_kind(t):
    t = re.sub(r"\b(RT3_NOINLINE|static|inline|const|__declspec\(noinline\))\b", " ", t).strip()
    if t == "void":
        return "V"
    if t in ("float", "double"):
        return "F"
    if "*" in t:
        return "R"
    if t in ("uint8_t", "int8_t", "bool", "char", "unsigned char", "signed char"):
        return "B"
    if t in ("uint16_t", "int16_t", "short", "unsigned short"):
        return "W"
    if t in INTS:
        return "R"
    return None


def spec_for(cc, params):
    """[V|F|B|W|R] then ECX, EDX, then the stack words."""
    ks = [kind(p) for p in params]
    if any(k is None for k in ks):
        return None
    ks = [k for k in ks if k != ""]
    regs, stack = ["-", "-"], []
    if cc == "__fastcall":
        r = 0
        for k in ks:
            if r < 2 and k in ("p", "i"):
                regs[r] = k
                r += 1
            else:
                stack.append(k)
    else:
        stack = ks
    words = []
    for k in stack:
        words += ["d", "-"] if k == "d" else [k]  # a double: low word drawn as 'd', high word follows
    if len(words) > 8:
        return None
    return "".join(regs) + "".join(words)


def exe_code():
    """A function's machine code by address, from the exe file."""
    b = find_exe().read_bytes()
    pe = struct.unpack_from("<I", b, 0x3c)[0]
    n, opt = struct.unpack_from("<H", b, pe + 6)[0], struct.unpack_from("<H", b, pe + 20)[0]
    secs = []
    for i in range(n):
        o = pe + 24 + opt + 40 * i
        vs, va, rs, ro = struct.unpack_from("<IIII", b, o + 8)
        secs.append((0x400000 + va, min(vs, rs), ro))
    def code(addr, size):
        for va, sz, ro in secs:
            if va <= addr < va + sz:
                return b[ro + addr - va: ro + addr - va + size]
        return b""
    return code


def indirect_call(code):
    """Whether the code may contain CALL r/m32 (FF /2): it would call through the
    random test data. (A byte scan: it can also hit operand bytes.)"""
    return any(code[i] == 0xFF and (code[i + 1] & 0x38) == 0x10 for i in range(len(code) - 1))


def calls_out(code, addr, starts):
    """The functions the code calls or jumps to (rel32) outside itself: a tail
    jump or thunk is not in the call graph's callee list. (A byte scan.)"""
    out = set()
    for i in range(len(code) - 4):
        if code[i] in (0xE8, 0xE9):
            t = addr + i + 5 + struct.unpack_from("<i", code, i + 1)[0]
            if t in starts and not (addr <= t < addr + len(code)):
                out.add(t)
    return out


def main():
    code = exe_code()
    callees, size = {}, {}
    for line in (ROOT / "decomp" / "index.tsv").read_text().splitlines():
        f = line.split("\t")
        callees[int(f[0], 16)] = [int(c, 16) for c in f[3].split(",") if c] if len(f) > 3 and f[3] else []
        size[int(f[0], 16)] = int(f[2])
    cases = []
    for path in sorted((ROOT / "src").rglob("*.cpp")):
        text = path.read_text(encoding="utf-8")
        for m in REPLACEMENT_RE.finditer(text):
            addr, name = int(m.group(1), 16), m.group(2)
            if addr in ORIGINAL_ONLY or addr in SKIP or addr not in callees or any(c not in PURE_CRT for c in callees[addr]):
                continue
            c = code(addr, size[addr])
            if indirect_call(c) or calls_out(c, addr, callees) - PURE_CRT:
                continue
            d = re.search(r"^[ \t]*((?:RT3_NOINLINE\s+|static\s+|inline\s+)*[\w:<>\*\s]+?)\s*\b(__fastcall|__stdcall|__cdecl)?\s*\b"
                          + re.escape(name) + r"\s*\(([^)]*)\)\s*\{", text, re.MULTILINE)
            if not d:
                continue
            rt = RESULT.get(addr) or ret_kind(" ".join(d.group(1).split()))
            cc = d.group(2) or "__cdecl"
            params = [p.strip() for p in d.group(3).split(",")] if d.group(3).strip() else []
            s = spec_for(cc, params)
            if rt is None or s is None:
                continue
            cases.append((addr, name, rt + s))
    cases.sort()
    out = [
        "// Generated by tools/gen_leaftests.py: do not edit.",
        "// Every rewritten leaf function against its original on random inputs.",
        '#include "difftest.h"',
        '#include "leaftest.h"',
        "",
        "const LeafCase kLeafCases[] = {",
    ]
    out += [f'    {{ 0x{a:06x}, "{s}", "{n}" }},' for a, n, s in cases]
    out += ["};", f"const int kNumLeafCases = {len(cases)};", ""]
    (ROOT / "tests" / "test_leaves_table.cpp").write_text("\n".join(out), encoding="utf-8")
    print(f"{len(cases)} leaf functions")


if __name__ == "__main__":
    main()
