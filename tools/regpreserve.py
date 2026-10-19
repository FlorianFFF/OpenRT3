"""Static check: do callers of a replaced function rely on ECX/EDX surviving
the call? The original compiler knew which registers each callee leaves
alone and sometimes skips reloading them (0x44d410 keeps using ECX after
calling 0x449e90). A C++ replacement clobbers ECX/EDX freely, so such
callees need a register-preserving wrapper (see hook/preserve.h).

For every CALL to a replaced address, scans the caller's instructions after
the call (fallthrough only) and reports a read of ECX/EDX before it is
written. A later CALL counts as a read only if that callee reads the
register on entry; CRT calls (>= 0x5a1000) and malloc/free copies never do.
`PUSH reg` whose slot is overwritten right away (stack reservation) is ignored.

    python tools/regpreserve.py
"""
import re, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SUB = {"ECX": ("ECX", "CX", "CL", "CH"), "EDX": ("EDX", "DX", "DL", "DH")}
COVER = {"CX": ("CX", "CL", "CH"), "CL": ("CL",), "CH": ("CH",), "DX": ("DX", "DL", "DH"), "DL": ("DL",), "DH": ("DH",)}
WRITE_OPS = {"MOV", "LEA", "MOVZX", "MOVSX", "POP"} | {"SET" + c for c in
             ("Z", "NZ", "L", "LE", "G", "GE", "C", "NC", "A", "AE", "B", "BE", "S", "NS", "P", "NP")}
NO_ECX_CALLS = {0x53b070, 0x53b080}

ASM = {}


def load(addr):
    if addr not in ASM:
        p = ROOT / "decomp" / f"{addr:08x}.asm"
        ASM[addr] = p.read_text().splitlines() if p.exists() else []
    return ASM[addr]


def names(text, r):
    return [n for n in SUB[r] if re.search(r"\b" + n + r"\b", text)]


ENTRY = {}


def reads_on_entry(addr, r, depth=0):
    """Sub-register names of r that function addr reads before writing them."""
    if addr >= 0x5a1000 or addr in NO_ECX_CALLS:
        return set()
    key = (addr, r)
    if key not in ENTRY:
        ENTRY[key] = set()  # recursion guard
        lines = [re.match(r"[0-9a-f]{8}\s+(.*)", l).group(1) for l in load(addr) if re.match(r"[0-9a-f]{8}\s", l)]
        if not lines:
            ENTRY[key] = {r} if r == "ECX" else set()
        else:
            hit = scan(lines, r, depth + 1, want_names=True)
            ENTRY[key] = hit or set()
    return ENTRY[key]


def scan(lines, r, depth=0, want_names=False):
    """Index of the first instruction that reads r before it is fully written, else None.
    With want_names, the set of sub-register names read instead of the index."""
    defined = set()
    def hit(i, ns):
        return set(ns) if want_names else i
    for i, ins in enumerate(lines[:60]):
        m = re.match(r"(\S+)\s*(.*)", ins)
        op, rest = m.group(1), m.group(2)
        base = op.split(".")[0]
        if base in ("RET", "JMP"):
            return None
        if base == "CALL":
            t = re.match(r"0x([0-9a-f]+)$", rest)
            if r == "ECX" and depth < 3 and t:
                need = reads_on_entry(int(t.group(1), 16), r, depth)
                if any(n not in defined for n in need):
                    return hit(i, need)
            # Indirect calls (imports cached in registers, COM methods, callbacks) are
            # assumed not to take ECX: vtable thiscalls reload it right before.
            return None  # callee clobbers both
        if ".REP" in op or base == "LOOP":
            if r == "ECX":
                return hit(i, ["ECX"])
            continue
        if base == "CDQ":
            if r == "EDX":
                return None
            continue
        if base in ("MUL", "IMUL") and "," not in rest:
            if r == "EDX":
                return None
        if base in ("DIV", "IDIV") and r == "EDX":
            return hit(i, ["EDX"])
        if base == "OR" and len(ops := [o.strip() for o in rest.split(",")]) == 2 and ops[0] == r and ops[1] == "0xffffffff":
            return None
        if ins in ("LEA ECX,[ECX]", "LEA EDX,[EDX]", "MOV EDI,EDI"):
            continue
        ops = [o.strip() for o in rest.split(",")] if rest else []
        if not ops:
            continue
        dst, srcs = ops[0], ops[1:]
        if base in ("XOR", "SUB", "SBB") and len(ops) == 2 and dst == ops[1] and dst == r:
            return None
        if base == "PUSH" and names(dst, r) and want_names and i == 0:
            continue  # PUSH ECX as a callee's first instruction reserves a local
        if base == "PUSH" and names(dst, r):
            # Stack reservation: the pushed slot is overwritten ([ESP] store)
            # before anything else is pushed or called.
            reserved = False
            for nxt in lines[i + 1:i + 20]:
                if re.match(r"(PUSH|CALL|POP|RET|J)", nxt):
                    break
                if re.match(r"(FSTP|FST|MOV|FISTP) dword ptr \[ESP\]", nxt):
                    reserved = True
                    break
            if reserved:
                continue
        read = [n for s in srcs for n in names(s, r)]
        if "[" in dst or base not in WRITE_OPS:
            read += names(dst, r)
        if any(n not in defined for n in read):
            return hit(i, [n for n in read if n not in defined])
        wn = names(dst, r) if "[" not in dst else []
        if wn and base in WRITE_OPS:
            if wn[0] == r:
                return None
            defined.update(COVER[wn[0]])
    return None


def main():
    replaced = {}
    for path in (ROOT / "src").rglob("*.cpp"):
        text = path.read_text(encoding="utf-8")
        for m in re.finditer(r"RT3_REPLACE\(\s*0x([0-9a-fA-F]+)\s*,\s*(\w+)", text):
            if re.search(r"naked\)[^\n]*\b" + m.group(2) + r"\(", text):
                continue  # already wrapped with RT3_KEEP_ECX_EDX
            replaced[int(m.group(1), 16)] = m.group(2)
    hits = {}
    for asm in sorted((ROOT / "decomp").glob("*.asm")):
        raw = asm.read_text().splitlines()
        lines = [re.match(r"([0-9a-f]{8})\s+(.*)", l) for l in raw]
        lines = [m for m in lines if m]
        for i, m in enumerate(lines):
            c = re.match(r"CALL 0x([0-9a-f]{8})$", m.group(2))
            if not c or int(c.group(1), 16) not in replaced:
                continue
            callee = int(c.group(1), 16)
            after = [x.group(2) for x in lines[i + 1:]]
            for r in ("ECX", "EDX"):
                j = scan(after, r)
                if j is not None:
                    hits.setdefault((callee, r), []).append(f"{m.group(1)}->{lines[i + 1 + j].group(1)}: {after[j]}")
    new = 0
    for (callee, r), sites in sorted(hits.items()):
        sites = [x for x in sites if (r, x.split("->")[0]) not in REVIEWED]
        if not sites:
            continue
        new += 1
        print(f"{callee:08x} {replaced[callee]:32s} {r} kept by {len(sites)}: {sites[:3]}")
    print(f"regpreserve: {new} unreviewed (callee, register) pairs")
    sys.exit(1 if new else 0)


# Call sites checked by hand: the register is not really relied on (masked
# partial writes, stack reservation pushes, 8-byte struct returns in EDX:EAX,
# callbacks/imports that ignore ECX).
REVIEWED = {(r, a) for r, a in [
    ("ECX", "004803b8"), ("ECX", "0050de1a"), ("ECX", "004e38c5"), ("ECX", "0041b6a5"), ("ECX", "0041b6dc"),
    ("ECX", "0046f2b9"), ("ECX", "0041b82c"), ("ECX", "0042797b"), ("ECX", "00427815"), ("ECX", "00427893"),
    ("ECX", "00426a6a"), ("ECX", "00426ae9"), ("ECX", "0046ea38"), ("ECX", "00427413"), ("ECX", "004e71ea"),
    ("ECX", "00427b24"), ("ECX", "0045963b"), ("ECX", "004a8e1c"), ("ECX", "004c039e"), ("ECX", "0050a932"),
    ("ECX", "0043c418"), ("ECX", "0043c44f"), ("ECX", "004f376b"), ("ECX", "004e79c3"), ("ECX", "004a8a7f"),
    ("ECX", "00552d41"), ("ECX", "005395ff"), ("ECX", "004aa5c2"), ("ECX", "0048350f"),
    ("EDX", "0040abc7"), ("EDX", "00428703"), ("EDX", "0042871e"), ("EDX", "00409ea2"), ("EDX", "0040ab81"),
    ("EDX", "00439342"), ("EDX", "0046ba59"), ("EDX", "00437112"), ("EDX", "00443fda"), ("ECX", "004fef8a"), ("ECX", "00455ebd"), ("ECX", "0044f876"), ("EDX", "00481c74"), ("EDX", "00481c16"), ("ECX", "0044716e"), ("ECX", "004471c4"), ("EDX", "00447143"), ("EDX", "00447199"),
]}


if __name__ == "__main__":
    main()
