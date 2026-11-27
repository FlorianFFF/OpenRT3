"""Static check: does each replacement pop the same number of stack bytes as
the original? (A wrong calling convention corrupts the caller's stack.)

For every RT3_REPLACE(addr, Fn), finds Fn's definition, works out the stack
purge its convention implies, and compares with Ghidra's purge size for the
original (decomp/conv.tsv).

    python tools/checkconv.py
"""
import re
from pathlib import Path
from replacements import REPLACEMENT_RE

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
    words = re.findall(r"\w+", p)  # whole words: "int32_t doubleKind" is 4 bytes
    return 8 if any(t in words for t in EIGHT) else 4


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


KEEP = re.compile(r"RT3_KEEP_ECX_EDX\(\s*\w+\s*,\s*(\d+)\s*\)")


def find_def(name, text, texts):
    pat = re.compile(r"(__declspec\(naked\)[^\n(]*?)?(__fastcall|__stdcall|__cdecl)?\s*\**\s*\b" + name + r"\s*\(([^)]*)\)\s*\{")
    for t in [text] + list(texts.values()):
        m = pat.search(t)
        if m:
            return m, t
    return None, None


def main():
    purge = {}
    for line in (ROOT / "decomp" / "conv.tsv").read_text().splitlines():
        a, _, p, *_ = line.split("\t")
        purge[int(a, 16)] = int(p)
    texts = {path: path.read_text(encoding="utf-8") for path in sorted((ROOT / "src").rglob("*.cpp"))}
    bad = 0
    seen = {}
    for path, text in texts.items():
        for m in REPLACEMENT_RE.finditer(text):
            a = int(m.group(1), 16)
            if a in seen:
                print(f"DUP {a:08x}: replaced in {seen[a]} and {path.name}")
                bad += 1
            seen[a] = path.name
    for path, text in texts.items():
        for m in REPLACEMENT_RE.finditer(text):
            addr, name = int(m.group(1), 16), m.group(2)
            want = purge.get(addr)
            d, t = find_def(name, text, texts)
            if not d:
                print(f"?? {addr:08x} {name}: definition not found")
                continue
            if want is None or want == 2147483647:
                continue  # unknown (tail-calling thunks)
            if d.group(1):  # naked entry: its own RET (or the keep-registers macro) decides the purge
                body = t[d.end():]
                body = body[:body.find("\n}")]
                k = KEEP.search(body)
                if k:
                    rets = {int(k.group(1)) * 4}
                else:
                    rets = {int(r, 0) if r else 0 for r in re.findall(r"\bret\b\s*(0x[0-9a-fA-F]+|\d+)?", body)}
                if rets and rets != {want}:
                    bad += 1
                    print(f"MISMATCH {addr:08x} {name}: original pops {want}, naked rewrite returns with {sorted(rets)}  [{path.relative_to(ROOT)}]")
                continue
            conv = d.group(2) or "__cdecl"
            params = [p for p in d.group(3).split(",")] if d.group(3).strip() else []
            got = implied_purge(conv, params)
            if want != got:
                bad += 1
                print(f"MISMATCH {addr:08x} {name}: original pops {want}, rewrite ({conv}, {len(params)} params) pops {got}"
                      f"  [{path.relative_to(ROOT)}]")
    bad += bare_ecx_calls(texts)
    bad += cast_calls(texts, purge)
    print(f"{bad} mismatches")
    return 1 if bad else 0


CAST_CALL = re.compile(r"\(\(\s*[\w:\*\s<>]+?\(\s*(__fastcall|__stdcall|__cdecl|__thiscall)?\s*\*\s*\)\s*\(([^()]*)\)\s*\)\s*(0x[0-9a-fA-F]{6})\s*\)")


def cast_calls(texts, purge):
    """Calls of an exe address through a cast, ((ret(conv*)(params))0xADDR)(...):
    the cast's convention and parameters must pop what the original pops (a
    value meant for the stack passed in EDX corrupts the stack on return)."""
    bad = 0
    for path, t in texts.items():
        for m in CAST_CALL.finditer(t):
            addr = int(m.group(3), 16)
            want = purge.get(addr)
            if want is None or want == 2147483647:
                continue
            conv = m.group(1) or "__cdecl"
            params = [p for p in m.group(2).split(",")] if m.group(2).strip() else []
            if conv == "__thiscall":
                got = sum(s for p in params[1:] for s in [size_of(p)] if s is not None)
            else:
                got = implied_purge(conv, params)
            if got != want:
                bad += 1
                line = t.count("\n", 0, m.start()) + 1
                print(f"CAST CALL {addr:08x}: original pops {want}, the cast ({conv}, {len(params)} params) pops {got}"
                      f"  [{path.relative_to(ROOT)}:{line}]")
    return bad


def bare_ecx_calls(texts):
    """A naked stub declared without parameters that passes ECX on (a thiscall
    original, e.g. RT3_KEEP_ECX_EDX) must be called with ECX set: from C++
    through a __fastcall cast, never as a bare Name() (ECX is then whatever
    the previous call left)."""
    stubs = set()
    for t in texts.values():
        for m in re.finditer(r"__declspec\(naked\)[^{;]*?\b(\w+)\(\)\s*\{(.*?)\n?\}", t, re.S):
            if "ecx" in m.group(2).lower() or "KEEP_ECX" in m.group(2):
                stubs.add(m.group(1))
    bad = 0
    for path, t in texts.items():
        lines = t.splitlines()
        for n in stubs:
            for m in re.finditer(r"(?<![\w&>])" + n + r"\(\)", t):
                l = lines[t.count("\n", 0, m.start())]
                if "naked" in l or re.match(r"\s*(RT3_NOINLINE\s+)?(static\s+)?\w[\w:\*<>]*[\w:\*<> ]*\s\**" + n + r"\(\);", l):
                    continue  # its definition or a declaration
                bad += 1
                print(f"BARE CALL {n}() without ECX  [{path.relative_to(ROOT)}:{t.count(chr(10), 0, m.start()) + 1}]")
    return bad


if __name__ == "__main__":
    import sys
    sys.exit(main())
