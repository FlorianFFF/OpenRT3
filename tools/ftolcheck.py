"""Checks that the rewrite converts floats to integers as the original does.

The game converts every float to an integer with its C runtime's _ftol
(0x5a10d0): truncation through a 64-bit FISTP, so a value beyond the 32-bit
range gives the low 32 bits of that integer, and NaN gives 0. Our compiler's
own helpers give 0x80000000 there, so src/core/ftol.cpp defines the helpers
the compiler calls for a cast (_ftol2, _ftol2_sse, _ftoui2) as jumps to the
exe's _ftol.

This reads the link maps of the last build (out/rt3re.map,
out/rt3re_test.map) and fails if any float-to-integer helper there comes
from the C runtime instead of ftol.obj (for example a new kind of conversion
the compiler started using, which would then need an entry in ftol.cpp).

    python tools/ftolcheck.py
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
HELPER = re.compile(r"^\s*\S+\s+(__(?:ftol|ftoi|ftoui|ftoul|dtol|dtoi|dtoui|dtoul|ltod|ultod)\w*)\s+\S+\s+\S*\s*(\S+)$")


def main():
    bad = 0
    for name in ("rt3re.map", "rt3re_test.map"):
        path = ROOT / "out" / name
        if not path.exists():
            print(f"{name}: not built")
            bad += 1
            continue
        found = {}
        for line in path.read_text(errors="replace").splitlines():
            m = HELPER.match(line)
            if m:
                found[m.group(1)] = m.group(2)
        wrong = {s: o for s, o in found.items() if o != "ftol.obj"}
        for s, o in sorted(wrong.items()):
            print(f"{name}: {s} comes from {o}, not src/core/ftol.cpp")
        bad += len(wrong)
        if not wrong:
            print(f"{name}: every float-to-integer helper is the exe's _ftol ({', '.join(sorted(found))})")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
