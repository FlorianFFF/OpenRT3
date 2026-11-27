# Byte matching (phase 2)

The rewrite behaves like the original; phase 2 makes it compile to the same machine code.
The original was built with Visual C++ .NET 2002 (cl 13.00.9466, linker 7.0; Rich header).

## Scoring a function

`python tools/matchdiff.py <addr> [--diff]` compiles the function's source file, takes the
function from the object file's COFF symbols and compares it instruction by instruction
(capstone) with the original's bytes in the exe. Operands the linker fills in (relocations,
addresses of the game image) compare as symbols, branch targets as offsets in the function.
The score is matching instructions over the longer side's count; 1.0 means identical apart from
relocated operands, and `BYTE MATCH` means identical bytes.
`python tools/matchdiff.py --all src/game/Cars.cpp` scores every function of a file;
`out/matching_scores.txt` holds a run over all files.

## Baseline (Visual Studio 2019 as a stand-in compiler)

All 4,276 registered functions: mean score 0.429, 383 at 1.0. With the original compiler the
numbers will differ; `--cl=<path to cl.exe>` and `--flags=...` select it once it is installed.

## What the first diffs show

- `__thiscall` methods are written as `__fastcall(this, int edx, ...)`, so the compiler loads
  EDX (`xor edx, edx`) before each such call; the original never does. Matching needs real
  member functions (classes with the reconstructed layouts) instead of the stand-in convention.
- Field access through `F<T>(p, off)` keeps the offsets but not the types the original
  compiler saw; struct layouts with real types will be needed for its register allocation.
- The original duplicates short function tails (one epilogue per return path); VC2019 merges
  them.

## Needed

- Visual C++ .NET 2002 (cl 13.00.9466) with its static C runtime: the compiler and the
  library the exe was linked with. It is not on this machine.
- For linking the libraries instead of running them from the exe image: the DirectX 8.1 SDK
  (D3DX 8) and the GameSpy SDK of that time.
