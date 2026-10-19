"""Everything needed to rewrite a function, in one call:
decompile, stack purge (callee-popped bytes), callers, callees, done-state,
and any mention in the full_reverse notes.

    python tools/fn.py 0x51db80 0x51d5f0 ...   [--notes] [--brief]

Calling convention from purge: 0 = __cdecl (or no stack args); >0 = callee
pops: __stdcall, or __fastcall/__thiscall for the stack part. In the
decompile, no convention keyword means __stdcall when purge > 0.
"""
import re
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DEC = ROOT / "decomp"


def main():
    sys.stdout.reconfigure(encoding="utf-8", errors="replace")
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    notes = "--notes" in sys.argv
    brief = "--brief" in sys.argv
    show_asm = "--asm" in sys.argv
    purge, callees, callers, size = {}, {}, defaultdict(list), {}
    for line in (DEC / "conv.tsv").read_text().splitlines():
        a, _, p, *_ = line.split("\t")
        purge[int(a, 16)] = int(p)
    for line in (DEC / "index.tsv").read_text().splitlines():
        a, name, sz, cs = (line.split("\t") + [""])[:4]
        a = int(a, 16)
        size[a] = int(sz)
        callees[a] = [int(c, 16) for c in cs.split(",") if c]
        for c in callees[a]:
            callers[c].append(a)
    done = set()
    for p in (ROOT / "src").rglob("*.cpp"):
        for m in re.finditer(r"RT3_REPLACE\(\s*0x([0-9a-fA-F]+)", p.read_text(encoding="utf-8")):
            done.add(int(m.group(1), 16))
    note_text = ""
    if notes:
        for p in (ROOT.parent / "full_reverse").glob("*.md"):
            note_text += p.read_text(encoding="utf-8", errors="replace")
    for arg in args:
        a = int(arg, 16)
        src = (DEC / f"{a:08x}.c").read_text()
        src = "\n".join(l for l in src.splitlines() if l.strip() and "WARNING" not in l)
        cs = " ".join(f"{c:x}{'*' if c in done else ''}" for c in callees.get(a, []))
        print(f"=== {a:08x} size={size.get(a)} purge={purge.get(a)} callers={len(callers[a])}"
              f"{' DONE' if a in done else ''}\n    callees: {cs}")
        if not brief:
            decl = re.compile(r"^\s*(undefined\d?|int|uint|float10|char|bool|byte|ushort|short)\s+\*?\w+( \[\d+\])?;$")
            print("\n".join(l for l in src.splitlines()[1:] if not decl.match(l) and l not in ("{", "}")))
        if show_asm and (DEC / f"{a:08x}.asm").exists():
            print("  --- asm")
            print("\n".join("  " + l for l in (DEC / f"{a:08x}.asm").read_text().splitlines()))
        if notes:
            hits = sorted(set(m.group(0).strip() for m in re.finditer(
                rf"[^\n]*0x0*{a:x}\b[^\n]*", note_text, re.I)))[:6]
            for h in hits:
                print("    note:", h[:220])


if __name__ == "__main__":
    main()
