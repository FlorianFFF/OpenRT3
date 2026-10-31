"""Static check: does each replacement pop the same number of stack bytes as
the original? (A wrong calling convention corrupts the caller's stack.)

For every RT3_REPLACE(addr, Fn), finds Fn's definition, works out the stack
purge its convention implies, and compares with Ghidra's purge size for the
original (decomp/conv.tsv).

    python tools/checkconv.py
"""
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
EIGHT = ("double", "uint64_t", "int64_t", "GameDate", "LONGLONG", "ULONGLONG")


STRUCTS = {"Mat16": 64}  # structs passed by value


def size_of(param):
    p = param.strip()
    if not p or p == "void":
        return None
    if "*" in p or "&" in p:
        return 4
    for name, size in STRUCTS.items():
        if p.split()[0] == name:
            return size
    return 8 if any(p.startswith(t) or (" " + t) in " " + p for t in EIGHT) else 4


def is_float(param):
    p = param.strip()
    return "*" not in p and "&" not in p and p.split()[0] in ("float", "double") if p else False


def implied_purge(conv, params):
    sized = [(s, is_float(p)) for p in params for s in [size_of(p)] if s is not None]
    sizes = [s for s, _ in sized]
    if conv == "__stdcall":
        return sum(sizes)
    if conv == "__fastcall":
        regs, stack = 0, 0
        for s, flt in sized:  # floats and structs never go in ECX/EDX
            if regs < 2 and s <= 4 and not flt:
                regs += 1
            else:
                stack += s
        return stack
    return 0  # __cdecl / default


def main():
    purge = {}
    for line in (ROOT / "decomp" / "conv.tsv").read_text().splitlines():
        a, _, p, *_ = line.split("\t")
        purge[int(a, 16)] = int(p)
    bad = 0
    seen = {}
    for path in sorted((ROOT / "src").rglob("*.cpp")):
        for m in re.finditer(r"RT3_REPLACE\(\s*0x([0-9a-fA-F]+)", path.read_text(encoding="utf-8")):
            a = int(m.group(1), 16)
            if a in seen:
                print(f"DUP {a:08x}: replaced in {seen[a]} and {path.name}")
                bad += 1
            seen[a] = path.name
    for path in sorted((ROOT / "src").rglob("*.cpp")):
        text = path.read_text(encoding="utf-8")
        for m in re.finditer(r"RT3_REPLACE\(\s*0x([0-9a-fA-F]+)\s*,\s*(\w+)\s*\)", text):
            addr, name = int(m.group(1), 16), m.group(2)
            d = re.search(r"(__fastcall|__stdcall|__cdecl)?\s*\**\s*\b" + name + r"\s*\(([^)]*)\)\s*\{", text)
            if not d:
                print(f"?? {addr:08x} {name}: definition not found")
                continue
            conv = d.group(1) or "__cdecl"
            params = [p for p in d.group(2).split(",")] if d.group(2).strip() else []
            want = purge.get(addr)
            got = implied_purge(conv, params)
            if want is None or want == 2147483647:
                continue  # unknown (tail-calling thunks)
            if want != got:
                bad += 1
                print(f"MISMATCH {addr:08x} {name}: original pops {want}, rewrite ({conv}, {len(params)} params) pops {got}"
                      f"  [{path.relative_to(ROOT)}]")
    print(f"{bad} mismatches")


if __name__ == "__main__":
    main()
