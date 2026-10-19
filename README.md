# OpenRT3 — Railroad Tycoon 3 rebuilt in C++

> **Work in progress.** Most of the game still runs as original code; functions are replaced one at a time.
>
> You need your own copy of Railroad Tycoon 3. This repo has no game data, binaries or decompiled game code.

Goal: a complete C++ reimplementation of `RT3_mp.exe` (the 2004-09-08 build, imagebase
`0x400000`) that runs on the original game data.

## How: replace the game one function at a time

Rewriting ~5,100 game functions from scratch and only testing at the end does not work; nothing
runs until everything runs. So this project uses the method that rebuilt GTA III (re3) and
RollerCoaster Tycoon 2 (OpenRCT2):

1. `rt3re.exe` (launcher) starts `RT3_mp.exe` suspended and loads `rt3re.dll` into it.
2. Every rewritten function is tagged `RT3_REPLACE(0xADDRESS, Function)`. At startup the DLL
   writes a `JMP` over the original, so the game calls our code instead.
3. The game stays fully playable the whole time. Anything not yet rewritten is still the
   original code; a rewritten function calls not-yet-rewritten ones through their address.
4. Each replacement can be switched off in `rt3re.ini` (`[replace] Name=0`) to compare
   behaviour with the original.
5. When every game function is replaced, the original exe is no longer needed: the same
   sources link into a standalone `rt3.exe`.

Calling conventions: `__thiscall` methods are written as `__fastcall(this, int /*edx*/, ...)`,
which matches the register layout. `__cdecl`/`__stdcall`/`__fastcall` are kept as-is.
Globals that still live in the exe are reached with `hook::Global<T>(0xADDR)`.

## Verifying a rewrite

`build.bat test` runs the **differential tests**: `rt3re_test.dll` is injected into the real
`RT3_mp.exe`, replaces `WinMain` with a test runner (so the exe's own CRT is initialised but no
game code has run), and calls the original function and the rewrite on the same inputs.
Results: `difftest.log` in the game folder.

```
DIFFTEST(text_ToUpper) {
    auto orig = (int(__fastcall*)(char))0x51d660;
    for (int c = 0; c < 256; ++c)
        EXPECT_EQ(orig((char)c), rt3::ToUpper((char)c), "c=%d", c);
}
```

A function only counts as done when it has a passing difftest, or, where the function only makes
sense inside a running game (rendering, UI, networking), after a recorded in-game check.

## Layout

| Path | What |
|---|---|
| `src/hook/` | replacement registry + JMP patching |
| `src/core/` | logging, shared helpers |
| `src/assets/` | file-format readers (PK4, …) — see `docs/formats.md` |
| `src/game/` | the rewritten game, one file per subsystem |
| `src/launcher/` | `rt3re.exe` |
| `tests/` | differential tests |
| `tools/` | Python format tools, progress report, `vc.bat` (run anything in the MSVC x86 env) |
| `decomp/` | full Ghidra decompile, one `.c` per function + `index.tsv` call graph (regenerable, not committed) |
| `docs/` | formats, progress |

## Build

Needs Visual Studio 2019 C++ build tools (x86). The game folder is expected next to this repo as
`..\Railroad Tycoon 3 Full Game\`.

First build only: `src/game/builtin_strings.inc` holds text from the original exe and is not
committed. Generate it by running the `dump_tables` difftest (writes `builtin_strings.tsv` to the
game folder) and then `python tools/gen_builtin_strings.py`.

`build.bat` builds and copies `rt3re.dll`,
`rt3re_test.dll` and `rt3re.exe` into `Railroad Tycoon 3 Full Game\`. `build.bat test` also
runs the tests. Play with the rewrite by starting `rt3re.exe` from the game folder.

Regenerate the decompile (Ghidra 11.3.2 headless, ~10 min):
```
analyzeHeadless <tmp-proj-dir> RT3H -import RT3_mp.exe -scriptPath reversing_notes/tools ^
    -postScript DecompAll.java rebuild/decomp
```

## Progress

`python tools/progress.py` counts replaced functions against the game's function list
(CRT, D3DX and GameSpy SDK code excluded) and writes `docs/progress.md`.
