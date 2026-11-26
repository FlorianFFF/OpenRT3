"""End-to-end determinism check of the rewrite.

Loads a save with the harness (src/harness.cpp), runs N frames, saves, and
compares the saves (via the per-write CRC trace) between runs:
  A = all replacements off (original code), B = A again (harness noise check),
  C = all replacements on.
A == B proves the setup is deterministic; A == C proves the rewrite matches.

    python tools/simcheck.py [save] [frames] [--do=<frame>:<addr>,...] [--coverage=<file>] [--mode=<n>]
"""
import os
import re
import subprocess
import sys
import time
from pathlib import Path

import stockgame
from pe import find_exe
from replacements import REPLACEMENT_RE
from runlock import shared_run

ROOT = Path(__file__).resolve().parents[1]
# The game without mods (tools/stockgame.py): the rewrite follows the unmodded
# exe, so a mod's patches would show up as differences.
GAME = stockgame.MIRROR
SAVES = GAME / "Saved Games"
# --do=<frame>:<addr>,... : commands run at those frames after loading, as if
# their hotkeys were pressed (src/harness.cpp).
ACTIONS = next((a[5:] for a in sys.argv if a.startswith("--do=")), "")
# --mode=<n>: the game's start mode (-s<n>). 4 (default) is the safe display
# setup (640x480 windowed, low detail); 9 the 1024x768 windowed defaults, with
# the detail levels the machine gets, so more of the rendering runs.
MODE = next((a[7:] for a in sys.argv if a.startswith("--mode=")), "4")


def parse_actions(text):
    """An action file: one action per line, "<frame> <action> [args] [comment]":
        <hex addr>              a key-table command (src/harness.cpp)
        move / ldown / lup / rdown / rup <x> <y>   the mouse (fractions of the window)
        click / rclick <x> <y>  button down, and up two frames later
        drag <x0> <y0> <x1> <y1> <frames>   left drag in steps of a frame
        key <vk>                a key (virtual key code, hex)
        type <text>             characters, one a frame
        settext <id> <text>     sets widget id's text (as the game sets an edit box)
        waitplayers <n>         waits until the network session has n players
        dump                    log every window and widget (harness "W" lines)
    """
    out = []
    short = {"move": "m", "ldown": "ld", "lup": "lu", "rdown": "rd", "rup": "ru"}
    for line in text.splitlines():
        t = line.split()
        if not t or t[0].startswith("#"):
            continue
        f, cmd = int(t[0]), t[1]
        if cmd in short:
            out.append(f"{f}:{short[cmd]}:{t[2]}:{t[3]}")
        elif cmd in ("click", "rclick"):
            b = "l" if cmd == "click" else "r"
            out += [f"{f}:{b}d:{t[2]}:{t[3]}", f"{f + 2}:{b}u:{t[2]}:{t[3]}"]
        elif cmd == "drag":
            x0, y0, x1, y1, n = float(t[2]), float(t[3]), float(t[4]), float(t[5]), int(t[6])
            out.append(f"{f}:ld:{x0}:{y0}")
            for i in range(1, n + 1):
                out.append(f"{f + i}:m:{x0 + (x1 - x0) * i / n:.4f}:{y0 + (y1 - y0) * i / n:.4f}")
            out.append(f"{f + n + 1}:lu:{x1}:{y1}")
        elif cmd == "key":
            out.append(f"{f}:k:{t[2]}")
        elif cmd == "type":
            out += [f"{f + i}:c:{ord(ch):x}" for i, ch in enumerate(t[2])]
        elif cmd == "waitplayers":
            out.append(f"{f}:w:{t[2]}")
        elif cmd == "settext":
            out.append(f"{f}:t:{t[2]}:{t[3]}")
        elif cmd == "dump":
            out.append(f"{f}:dump")
        else:
            out.append(f"{f}:{cmd}")
    out.sort(key=lambda a: int(a.split(":")[0]))  # (stable: same-frame actions keep their order)
    return ",".join(out)


if ACTIONS.startswith("@"):
    ACTIONS = parse_actions(Path(ACTIONS[1:]).read_text())


def names():
    out = []
    for p in sorted((ROOT / "src").rglob("*.cpp")):
        out += [m.group(2) for m in REPLACEMENT_RE.finditer(p.read_text(encoding="utf-8"))]
    return out


def run(tag, enabled, save, days, timeout=None):
    if timeout is None:  # --timeout=<seconds> for long runs
        timeout = next((int(a[10:]) for a in sys.argv if a.startswith("--timeout=")), 240)
    ini = GAME / "rt3re.ini"
    off = {n for a in sys.argv if a.startswith("--off=") for n in a[6:].split(",")}  # bisecting: keep these original
    off |= {n for a in sys.argv if a.startswith("--offfile=") for n in Path(a[10:]).read_text().split(",") if n}  # long lists
    ini.write_text("[replace]\n" + "".join(f"{n}={1 if enabled and n not in off else 0}\n" for n in names()))
    ext = Path(save).suffix
    out = f"simcheck_{tag}{ext}"
    target = SAVES / out
    if target.exists():
        target.unlink()
    spec = f"load={save};frames={days};out={out}"
    if ACTIONS:  # scripted actions (src/harness.cpp, "do="), passed in a file
        (GAME / "simcheck_actions.txt").write_text(ACTIONS)
        spec += ";do=@simcheck_actions.txt"
    env = dict(os.environ, RT3RE_HARNESS=spec)
    if "--nocoverage" not in sys.argv:  # coverage: counting stubs / one-shot probes (src/hook/hook.cpp)
        env["RT3RE_COVERAGE"] = "1"
    # --standalone: the rewrite runs in OpenRT3.exe (our own process) instead
    # of being loaded into the game's exe; the original runs are unchanged.
    image = "OpenRT3.exe" if enabled and "--standalone" in sys.argv else find_exe(GAME).name
    exe = "OpenRT3.exe" if image == "OpenRT3.exe" else "rt3re.exe"
    stockgame.reset_settings()  # the same settings for every run
    p = subprocess.Popen([str(GAME / exe), "-s" + MODE], cwd=GAME, env=env)
    try:
        p.wait(timeout=timeout)
    except subprocess.TimeoutExpired:
        pass
    t0 = time.time()
    while time.time() - t0 < timeout:
        r = subprocess.run(["tasklist", "/fi", f"imagename eq {image}"], capture_output=True, text=True)
        if image not in r.stdout:
            break
        time.sleep(2)
    else:
        subprocess.run(["taskkill", "/f", "/im", image], capture_output=True)
        print(f"{tag}: timeout")
    ini.unlink()
    log = (GAME / "rt3re.log").read_text(errors="replace")
    (GAME / f"simcheck_{tag}.log").write_text(log)
    if "exception" in log:
        print(f"{tag}: exception in log:\n" + "\n".join(l for l in log.splitlines() if "exception" in l))
    trace = GAME / f"{out}.trace"
    if not target.exists() or not trace.exists():
        return None
    return [l.split()[1:] for l in trace.read_text().splitlines() if l]  # (size, crc) per write


def draws(log):
    """Per main-loop frame, the draw CRCs of a run's log ("D <frame> <crc> <draws>
    <crc per kind>", src/drawtrace.cpp). Frames that ran a modal loop are logged
    with a negative number and left out: the loop draws as often as the wall
    clock allows."""
    if not log.exists():
        return {}
    out = {}
    for l in log.read_text(errors="replace").splitlines():
        if l.startswith("D ") and not l.startswith("D -"):
            f = l.split()
            out[int(f[1])] = f[2:]
    return out


def main():
    stockgame.build()
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    save = args[0] if len(args) > 0 else "mature.gmc"
    frames = args[1] if len(args) > 1 else "2000"
    # The two original runs only change with the save/frame count: reuse them
    # unless --fresh is given.
    def cached(tag):
        t = GAME / f"simcheck_{tag}{Path(save).suffix}.trace"
        stamp = GAME / f"simcheck_{tag}.key"
        key = f"{save} {frames} {ACTIONS} {'--nocoverage' in sys.argv} {MODE}"
        if "--fresh" not in sys.argv and t.exists() and stamp.exists() and stamp.read_text() == key:
            return [l.split()[1:] for l in t.read_text().splitlines() if l]
        r = run(tag, False, save, frames)
        stamp.write_text(key)
        return r
    a = cached("orig1")
    b = cached("orig2")
    c = run("rewrite", True, save, frames)
    if not (a and b and c):
        print("FAILED: a run produced no save/trace")
        return 1
    if len(a) != len(b) or len(a) != len(c):
        print(f"DIFFER: different number of save writes {len(a)}/{len(b)}/{len(c)}")
        return 1
    # Writes that differ between two original runs are noise (heap pointers
    # stored in the save); everything else must match exactly.
    noise = {i for i in range(len(a)) if a[i] != b[i]}
    real = [i for i in range(len(a)) if a[i] != c[i] and i not in noise]
    print(f"{len(a)} save writes, {len(noise)} vary between original runs (heap pointers)")
    if real:
        print(f"REWRITE DIFFERS in {len(real)} writes; first: write #{real[0]}")
        return 1
    print("MATCH: rewrite produces the same game state as the original")
    # Drawing (src/drawtrace.cpp): one CRC per presented frame of everything
    # handed to Direct3D. Frames that differ between the two original runs
    # are noise; any other difference is a drawing difference.
    da, db, dc = (draws(GAME / f"simcheck_{t}.log") for t in ("orig1", "orig2", "rewrite"))
    if da and dc:
        frames = sorted(set(da) & set(db) & set(dc))
        noisy = {f for f in frames if da[f] != db[f]}
        bad = [f for f in frames if da[f] != dc[f] and f not in noisy]
        print(f"{len(frames)} frames drawn, {len(noisy)} vary between original runs")
        if bad:
            kinds = [k for k in range(6) if da[bad[0]][2 + k] != dc[bad[0]][2 + k]]
            print(f"DRAWING DIFFERS in {len(bad)} frames; first: frame {bad[0]} (kinds {kinds}, see src/drawtrace.cpp)")
            return 1
        print("MATCH: rewrite draws the same frames as the original")
    # Measured in the original run (src/hook/hook.cpp probes each function's
    # first byte): in the rewrite run, rewritten code calls rewrites directly,
    # past the counters at the exe addresses.
    covfile = GAME / f"simcheck_orig1{Path(save).suffix}.coverage"
    if covfile.exists():
        rows = [l.split() for l in covfile.read_text().splitlines() if l]
        ran = [r for r in rows if int(r[2]) > 0]
        print(f"coverage: {len(ran)}/{len(rows)} replaced functions ran during the check")
        out = next((a[11:] for a in sys.argv if a.startswith("--coverage=")), None)
        if out:  # one file per scenario; tools/coverage.py merges them
            Path(out).write_text("\n".join(" ".join(r) for r in sorted(rows)) + "\n")
    return 0


if __name__ == "__main__":
    with shared_run("simcheck"):
        sys.exit(main())
