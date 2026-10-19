"""Suggests what to rewrite next: game functions not yet replaced whose callees
are all replaced or library code (so they can be difftested on their own),
ranked by how many functions call them.

    python tools/next.py [count] [--block 0x51]
"""
import re
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
GAME_LO, GAME_HI = 0x401000, 0x56c850


def load():
    funcs, callees = {}, {}
    for line in (ROOT / "decomp" / "index.tsv").read_text().splitlines():
        addr, name, size, cs = (line.split("\t") + [""])[:4]
        a = int(addr, 16)
        funcs[a] = (name, int(size))
        callees[a] = [int(c, 16) for c in cs.split(",") if c]
    done = set()
    for p in (ROOT / "src").rglob("*.cpp"):
        for m in re.finditer(r"RT3_REPLACE\(\s*0x([0-9a-fA-F]+)", p.read_text(encoding="utf-8")):
            done.add(int(m.group(1), 16))
    callers = defaultdict(set)
    for a, cs in callees.items():
        for c in cs:
            callers[c].add(a)
    return funcs, callees, callers, done


def main():
    args = sys.argv[1:]
    count = int(args[0]) if args and args[0].isdigit() else 40
    block = None
    if "--block" in args:
        block = int(args[args.index("--block") + 1], 16)
    funcs, callees, callers, done = load()
    game = lambda a: GAME_LO <= a < GAME_HI
    ready = []
    for a in funcs:
        if not game(a) or a in done or not re.match(r"(thunk_)?FUN_", funcs[a][0]):
            continue
        if block is not None and a >> 16 != block:
            continue
        if all(c in done or not game(c) or not re.match(r"(thunk_)?FUN_", funcs.get(c, ("FUN_",))[0]) for c in callees[a]):
            ready.append((len(callers[a]), a))
    ready.sort(key=lambda t: (-t[0], t[1]))
    print(f"{len(ready)} ready (all callees done or library)")
    for n, a in ready[:count]:
        print(f"{a:08x} size={funcs[a][1]:5} callers={n}")


if __name__ == "__main__":
    main()
