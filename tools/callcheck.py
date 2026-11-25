"""Compares, per rewritten function, which functions the original calls with
which functions its rewrite calls.

Original: the functions its machine code calls (CALL rel32) or tail-jumps
to, decoded from the exe between its start and the next function (byte-wise,
so a stray E8 / E9 inside an instruction can add a false callee). Rewrite: the functions its body calls by name (rewrites resolve to
their exe address through RT3_REPLACE; crt:: and jpeg:: entries through
src/core/crt.h / jpeglib.h), raw exe addresses it casts and calls, and the
same for file-local helpers it calls (followed transitively).

A callee only on one side points at a possible difference (a missing check,
a different CRT function such as strcmp for stricmp, an extra call). Many
are harmless: an original helper the rewrite inlines, or a call the original
compiler inlined. Lists the functions with differences, most first.

A callee one side reaches through helpers the other side inlined is not
reported (--direct reports those too).

    python tools/callcheck.py [--all] [--direct] [name-or-address ...]
"""
import re
import sys
from pathlib import Path

from replacements import REPLACEMENT_RE

ROOT = Path(__file__).resolve().parents[1]


def exe_callees(starts):
    """Per function start: the function starts its code calls (CALL rel32) or
    tail-jumps to (JMP rel32 out of the function), scanning up to the next
    function, so code reached through switch tables counts too."""
    import struct
    import stockgame
    from pe import find_exe
    d = open(find_exe(stockgame.MIRROR), "rb").read()
    pe = struct.unpack_from("<I", d, 0x3c)[0]
    n = struct.unpack_from("<H", d, pe + 6)[0]
    opt = struct.unpack_from("<H", d, pe + 20)[0]
    secs = [struct.unpack_from("<8sIIII", d, pe + 24 + opt + i * 40) for i in range(n)]
    _, vs, va, rs, ro = [s for s in secs if s[0].startswith(b".text")][0]
    base = 0x400000 + va
    known = set(starts)
    order = sorted(starts)
    out = {}
    for k, a in enumerate(order):
        end = order[k + 1] if k + 1 < len(order) else a + 0x1000
        got = set()
        for at in range(a, end - 4):
            op = d[ro + at - base]
            if op in (0xE8, 0xE9):
                t = at + 5 + struct.unpack_from("<i", d, ro + at - base + 1)[0]
                if t in known and t != a and not (a <= t < end):
                    got.add(t)
            # A function's address as an immediate (a callback, a handler table).
            v = struct.unpack_from("<I", d, ro + at - base)[0]
            if v in known and v != a:
                got.add(v)
        out[a] = got
    # A thunk (one JMP to another function) stands for its target.
    thunk = {a: t for a in order for t in out[a]
             if d[ro + a - base] == 0xE9 and a + 5 + struct.unpack_from("<i", d, ro + a - base + 1)[0] == t}
    return {a: {thunk.get(c, c) for c in cs} for a, cs in out.items()}, thunk


def load():
    starts = [int(l.split("\t")[0], 16) for l in (ROOT / "decomp" / "index.tsv").read_text().splitlines()]
    callees, thunk = exe_callees(starts)
    # A body chunk Ghidra listed separately belongs to its parent (fragments.py).
    from fragments import FRAGMENTS
    for frag, parent in FRAGMENTS.items():
        callees[parent] = (callees.get(parent, set()) | callees.get(frag, set())) - {frag}
    lib = {}
    for h, macro in (("crt.h", "RT3_CRT"), ("jpeglib.h", "RT3_JPEG")):
        for m in re.finditer(macro + r"\([^,]+,\s*(\w+),.*?(0x[0-9a-fA-F]+)\)", (ROOT / "src" / "core" / h).read_text()):
            lib[m.group(1)] = int(m.group(2), 16)
    lib = {k: thunk.get(v, v) for k, v in lib.items()}  # operator new is a thunk, for one
    src = {p: p.read_text(encoding="utf-8") for p in (ROOT / "src").rglob("*.cpp")}
    reg, where = {}, {}
    for p, t in src.items():
        for m in REPLACEMENT_RE.finditer(t):
            a = int(m.group(1), 16)
            if a not in thunk:  # a name for a thunk and its target: the target
                reg[m.group(2)] = a
            where.setdefault(int(m.group(1), 16), (m.group(2), p))
    return callees, lib, src, reg, where


# A definition starts at column 0 (not a comment, directive or indented call).
DEF_START = re.compile(r"^(?![ \t#/])[^\n;()]*?\b([A-Za-z_]\w*)\s*\(", re.M)
_defs = {}


def defs(path, text):
    """name -> (start, end) of the body of each function defined in text."""
    if path in _defs:
        return _defs[path]
    out = {}
    for m in DEF_START.finditer(text):
        i, depth = m.end() - 1, 0
        while i < len(text):  # past the parameter list
            if text[i] == "(":
                depth += 1
            elif text[i] == ")":
                depth -= 1
                if depth == 0:
                    break
            i += 1
        j = i + 1
        while j < len(text) and (text[j].isspace() or text.startswith("const", j)):
            j += 5 if text.startswith("const", j) else 1
        if j >= len(text) or text[j] != "{":
            continue
        depth = 0
        for k in range(j, len(text)):
            if text[k] == "{":
                depth += 1
            elif text[k] == "}":
                depth -= 1
                if depth == 0:
                    out.setdefault(m.group(1), (j, k + 1))
                    break
    _defs[path] = out
    return out


CALL = re.compile(r"(?:\b(crt|jpeg)::)?\b([A-Za-z_]\w*)\s*\(")
ADDR = re.compile(r"\)\s*(0x[45][0-9a-fA-F]{5})\s*\)\s*\(|Orig<[^>]*>\(\s*(0x[45][0-9a-fA-F]{5})")


def rewrite_calls(name, path, ctx, seen=None):
    callees, lib, src, reg, where = ctx
    seen = seen if seen is not None else set()
    seen.add(name)
    d = defs(path, src[path])
    if name not in d:
        return None
    b = src[path][d[name][0]:d[name][1]]
    out = set()
    for m in CALL.finditer(b):
        ns, n = m.group(1), m.group(2)
        if ns:
            if n in lib:
                out.add(lib[n])
        elif n in reg and n != name:
            out.add(reg[n])
        elif re.sub(r"_?Impl$", "", n) in reg and n.endswith("Impl"):  # a rewrite's body, called directly
            out.add(reg[re.sub(r"_?Impl$", "", n)])
        elif n not in seen and n not in reg and n in d:
            sub = rewrite_calls(n, path, ctx, seen)  # a file-local helper
            if sub:
                out |= sub
    for m in ADDR.finditer(b):
        a = m.group(1) or m.group(2)
        if a:
            out.add(int(a, 16))
    # Functions referenced without a call: tables of handlers, callbacks.
    for m in re.finditer(r"\b([A-Za-z_]\w*)\b", b):
        if m.group(1) in reg and m.group(1) != name:
            out.add(reg[m.group(1)])
    for m in re.finditer(r"\b0x([45][0-9a-fA-F]{5})\b", b):
        a = int(m.group(1), 16)
        if a in callees:
            out.add(a)
    return out


def reach(start, callees, depth):
    seen, frontier = set(start), set(start)
    for _ in range(depth):
        frontier = {c for f in frontier for c in callees.get(f, ())} - seen
        seen |= frontier
    return seen


def main():
    ctx = load()
    callees, lib, src, reg, where = ctx
    want = [a for a in sys.argv[1:] if not a.startswith("--")]
    names = {v: k for k, v in lib.items()}
    rows = []
    for a, (name, path) in sorted(where.items()):
        if want and name not in want and ("%06x" % a) not in want:
            continue
        if a not in callees:
            continue
        r = rewrite_calls(name, path, ctx)
        if r is None:
            continue
        # The C runtime only where it is a named library call (crt.h): the
        # compiler's own helpers (_ftol, _chkstk, ...) are not calls of the code.
        libs = set(lib.values())
        o = {c for c in callees[a] if c < 0x5a1000 or c in libs or 0x56c850 <= c < 0x5a1000}
        r = {c for c in r if c < 0x5a1000 or c in libs or 0x56c850 <= c < 0x5a1000}
        missing = o - r
        extra = {c for c in r - o if c < 0x5c8000}
        if "--direct" not in sys.argv:
            # One side calling a helper whose body the other side contains is
            # the same code: a rewrite-only callee the original reaches through
            # its callees, or an original-only callee the rewrite reaches, or
            # whose own callees the rewrite calls (inlined).
            ro = reach(o, callees, 3)
            rr = reach(r, callees, 3)
            extra = {c for c in extra if c not in ro}
            missing = {c for c in missing if c not in rr and not (callees.get(c) and callees[c] <= rr)}
        missing, extra = sorted(missing), sorted(extra)
        if missing or extra or "--all" in sys.argv:
            rows.append((len(missing) + len(extra), a, name, path, missing, extra))
    rows.sort(key=lambda x: -x[0])
    nm = lambda c: where[c][0] if c in where else names.get(c, "%06x" % c)
    for n, a, name, path, missing, extra in rows:
        print("%06x %-36s %s" % (a, name, path.relative_to(ROOT / "src").as_posix()))
        if missing:
            print("    only original: " + ", ".join(nm(c) for c in missing))
        if extra:
            print("    only rewrite:  " + ", ".join(nm(c) for c in extra))
    print("%d of %d functions differ" % (len(rows), len(where)), file=sys.stderr)


if __name__ == "__main__":
    main()
