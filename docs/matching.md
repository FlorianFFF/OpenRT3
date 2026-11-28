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

## With the original compiler

The toolchains are next to this repository in `reversing_notes/tools/` (or where `RT3_VC7`
points): Visual C++ .NET 2002 (`msvc700-master`, cl 13.00.9466, the exe's compiler), the
DirectX 8.1 SDK (`dx81sdk-main`) and IGN's 2011 BSD release of the GameSpy SDK
(`GameSpy-master`, newer than the code in the exe).

Matched functions are written in `match/` in that compiler's C++ (C++98: no lambdas,
`constexpr`, `auto`), as real classes with `__thiscall` members (`match/types.h` holds the
layouts). Each function has a marker comment with its exe address(es) and qualified name
(`// 0x4010b0 Collection::GetAt`). `python tools/matchbuild.py [--diff]` compiles every file
with the original compiler (`/O2`, as the exe; `/Gy` so each function is scored alone) and
scores each marked function against the exe.

The original options are plain `/O2`: the first functions match with nothing else. What
decides the code is the source's shape, e.g. the order of two stores in each branch of an
`if` chain picks the registers; the rewrite's version (`src/`) gives the behaviour, the diff
shows the shape.

A `__thiscall` member is call-compatible with the rewrite's `__fastcall(this, int edx, ...)`
stand-in (`this` in ECX, arguments on the stack, callee pops), so matched objects can later
replace the rewrite's functions one by one, their calls to other members bound to the
rewrite's functions by the linker.

Progress: `match/AI.cpp`, 5 functions (0x401000, 0x401070, 0x4010b0, 0x4010d0 / 0x406bf0),
all at score 1.0 (identical apart from relocated operands).
