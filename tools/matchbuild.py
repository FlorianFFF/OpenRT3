"""Byte matching (phase 2): compile match/*.cpp with the original's compiler and score them.

    python tools/matchbuild.py [match/<file>.cpp ...] [--diff]

The sources in match/ are the functions written for Visual C++ .NET 2002
(cl 13.00.9466, the compiler the exe was built with: C++98, real classes and
__thiscall members). Each function carries a marker comment with its exe
address(es) and its qualified name:

    // 0x4010b0 Collection::GetAt
    // 0x4010d0 0x406bf0 Collection::RecordAt     (an exe duplicate: one source, both scored)

The tool compiles each file with that compiler (/O2, as the original;
out/match/<file>.obj), finds each marked function's symbol in the object
(by its demangled name) and compares it with the exe's bytes
(tools/matchdiff.py compare: relocated operands as symbols). The compiler is
taken from reversing_notes/tools/msvc700-master next to this repository, or
from the RT3_VC7 environment variable (the folder holding Common7 and Vc7).
"""
import os
import re
import subprocess
import sys
from pathlib import Path

from matchdiff import compare

ROOT = Path(__file__).resolve().parents[1]
VC7 = Path(os.environ.get("RT3_VC7", ROOT.parent / "reversing_notes" / "tools" / "msvc700-master"))
# /Gy: each function in a section of its own, so its relocations and size are its own.
FLAGS = "/nologo /c /O2 /Gy"
MARK = re.compile(r"^//\s*((?:0x[0-9a-fA-F]+\s+)+)([\w:~]+)\s*$", re.M)


def vc7(cmd, cwd):
    bat = VC7 / "Common7" / "Tools" / "vsvars32.bat"
    if not bat.exists():
        sys.exit(f"Visual C++ .NET 2002 not found at {VC7} (set RT3_VC7)")
    return subprocess.run(f'call "{bat}" >nul && {cmd}', shell=True, cwd=cwd, capture_output=True, text=True,
                          errors="replace")


def symbols(obj):
    """Demangled name (without the argument list) -> mangled name, for the functions defined in obj."""
    out = vc7(f'dumpbin /symbols "{obj}"', obj.parent).stdout
    syms = {}
    for line in out.splitlines():
        m = re.search(r"SECT\w+\s+notype \(\)\s+External\s+\|\s+(\S+) \((.*)\)$", line)
        if m:
            q = re.search(r"([\w:~]+)\(", m.group(2))
            if q:
                syms.setdefault(q.group(1), m.group(1))
    return syms


def build(src, show):
    out_dir = ROOT / "out" / "match"
    out_dir.mkdir(parents=True, exist_ok=True)
    obj = out_dir / (src.stem + ".obj")
    r = vc7(f'cl {FLAGS} "{src.resolve()}" /Fo"{obj}"', out_dir)
    if r.returncode:
        print(r.stdout + r.stderr)
        return []
    syms = symbols(obj)
    scores = []
    for m in MARK.finditer(src.read_text(encoding="utf-8")):
        name = m.group(2)
        sym = syms.get(name)
        for a in m.group(1).split():
            if not sym:
                print(f"{int(a, 16):06x} {name}: not in {obj.name}")
                scores.append(0.0)
                continue
            scores.append(compare(int(a, 16), obj, sym, show))
    return scores


def main():
    show = "--diff" in sys.argv
    files = [Path(a) for a in sys.argv[1:] if not a.startswith("--")] or sorted((ROOT / "match").glob("*.cpp"))
    scores = []
    for f in files:
        scores += build(f, show)
    if scores:
        print(f"{len(scores)} functions, mean score {sum(scores) / len(scores):.3f}, "
              f"at 1.0: {sum(1 for s in scores if s == 1.0)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
