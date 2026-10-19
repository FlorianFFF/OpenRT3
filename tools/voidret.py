"""Checks rewritten functions declared `void` for a live EAX at return.

The decompiler often shows constructors and setters as void although they
return `this` or a value in EAX, and callers use it. For every RT3_REPLACE'd
function whose C++ return type is void, this prints the instructions that set
EAX last before each RET (via the GhidraMCP HTTP server on 127.0.0.1:8080).

    python tools/voidret.py
"""
import re
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
EAX_WRITE = re.compile(r"^(MOV|XOR|LEA|POP|MOVZX|MOVSX|OR|AND|SUB|ADD|INC|DEC|SETNZ|SETZ|IMUL|SAR|SHL|SHR|NEG)\s+(EAX|AX|AL),|^CALL\b")


def disasm(addr):
    url = f"http://127.0.0.1:8080/disassemble_function?address=0x{addr:x}"
    lines = urllib.request.urlopen(url, timeout=60).read().decode().splitlines()
    out = []
    for l in lines:
        if ":" in l:
            a, ins = l.split(":", 1)
            out.append((a.strip(), ins.strip()))
    return out


def void_replacements():
    found = []
    for p in sorted((ROOT / "src").rglob("*.cpp")):
        text = p.read_text(encoding="utf-8")
        for m in re.finditer(r"RT3_REPLACE\(\s*0x([0-9a-fA-F]+)\s*,\s*(\w+)\s*\)", text):
            addr, name = int(m.group(1), 16), m.group(2)
            d = re.search(r"\n\s*(?:static\s+)?void\s+(?:__\w+\s+)?" + name + r"\s*\(", text)
            if d:
                found.append((addr, name))
    return found


def callers(addr):
    out = []
    for line in (ROOT / "decomp" / "index.tsv").read_text().splitlines():
        a, _, _, cs = (line.split("\t") + [""])[:4]
        if f"{addr:08x}" in cs.split(","):
            out.append(int(a, 16))
    return out


READS_EAX = re.compile(r"\b(EAX|AX|AL)\b")


def uses_after_call(addr):
    """Call sites that read EAX before overwriting it."""
    hits = []
    target = f"CALL 0x{addr:08x}"
    for c in callers(addr):
        ins = disasm(c)
        for i, (a, op) in enumerate(ins):
            if op != target:
                continue
            for a2, op2 in ins[i + 1: i + 6]:
                dst_src = op2.split(None, 1)
                if len(dst_src) < 2:
                    if op2.startswith(("RET", "LEAVE")):
                        hits.append(f"{a}->{op2} (returns EAX on)")
                    break
                mnem, args = dst_src
                parts = [s.strip() for s in args.split(",")]
                src_reads = any(READS_EAX.search(p) for p in parts[1:]) or (
                    mnem in ("TEST", "CMP", "PUSH") and READS_EAX.search(args))
                dst_is_eax = parts and re.fullmatch(r"(EAX|AX|AL)", parts[0]) and mnem in ("MOV", "XOR", "LEA", "POP", "MOVZX", "MOVSX")
                if src_reads or (READS_EAX.search(parts[0]) and "[" in parts[0]):
                    hits.append(f"{a}: {op2}")
                    break
                if dst_is_eax or mnem.startswith(("CALL", "J", "RET")):
                    break
    return hits


def main():
    for addr, name in void_replacements():
        hits = uses_after_call(addr)
        if hits:
            print(f"{addr:08x} {name:34} CALLERS USE EAX: {hits[:4]}")
    return
    for addr, name in void_replacements():
        ins = disasm(addr)
        notes = []
        for i, (a, op) in enumerate(ins):
            if op.startswith("RET"):
                # last EAX writer before this RET in straight-line order
                for j in range(i - 1, max(-1, i - 12), -1):
                    if EAX_WRITE.match(ins[j][1]):
                        notes.append(f"{ins[j][1]}")
                        break
                    if ins[j][1].startswith(("JMP", "RET")):
                        break
        first = ins[0][1] if ins else ""
        flag = "THIS" if first.startswith("MOV EAX,ECX") else ""
        live = [n for n in notes if not n.startswith("CALL")]
        if flag or live:
            print(f"{addr:08x} {name:34} {flag:5} {sorted(set(live))}")


if __name__ == "__main__":
    main()
