"""End-to-end determinism check of the rewrite.

Loads a save with the harness (src/harness.cpp), runs N frames, saves, and
compares the saves (via the per-write CRC trace) between runs:
  A = all replacements off (original code), B = A again (harness noise check),
  C = all replacements on.
A == B proves the setup is deterministic; A == C proves the rewrite matches.

    python tools/simcheck.py [save] [frames] [--do=<frame>:<addr>,...] [--coverage=<file>] [--mode=<n>]
                             [--quit] [--menu] [--sound]

[save] can also be a scenario file ('..\\maps\\Britain.gmp': a new game of it).
--menu loads nothing: the --do script drives the menus from the first frame
(e.g. into the map editor), and the frames count from when a world exists;
[save] then only names the output (its extension).
--quit closes the game after saving instead of ending the process, so the
game's shutdown runs (and counts for --coverage); each run reports whether
it shut down cleanly.
--sound turns the effects on (the settings the tests start from have them
at 0) with the game's audio muted, so the sound code runs and is compared.
--probe runs only the original once and reports whether it got through the
actions to the save (PROBE OK / PROBE FAILED); --timeout=<seconds> limits each
run (default 240).
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
        hold <vk> <frames>      a key held down for that many frames
        cell <x> <y>            moves the mouse over map cell (x, y) (found with the camera's ground pick)
        cellclick <x> <y>       left click on map cell (x, y)
        celldrag <x0> <y0> <x1> <y1> <frames>   left drag from cell to cell
        look <x> <y>            centres the view on map cell (x, y) (the game's own camera call)
        objlook <coll> <i>      centres the view on the i-th object of the collection global <coll> (hex)
        objclick <coll> <i>     left click on that object's cell
        zoom <steps>            zooms the view that many steps (negative: in)
        call <addr> <cc> <ret> <args...> [# comment]   calls a game function with real
                                objects as arguments and logs its result ("R" lines,
                                compared between the runs; src/harness.cpp CallFunction)
        hex <coll> <i> <len>    logs the bytes of the i-th object (-1: the newest) of a collection
        poke <g>[.<off>..] <hex>  writes a dword at global g (each .<off> follows the pointer
                                there first: 6cec74.277 is the app's +0x277)
        autoenter 0|1         whether the harness answers dialogs with Enter (default 1)
        modal 0|1             whether actions also run inside modal dialogs of a loaded game (default 0)
        cities                  log every city's id, cell and name (harness "C" lines)
        tracks                  log every track piece's id, cell and owner (harness "T" lines)
        wheel <ticks>         mouse wheel at the mouse position (signed, notches of 120)
        type <text>             characters, one a frame
        settext <id> <text>     sets widget id's text (as the game sets an edit box)
        waitplayers <n>         waits until the network session has n players
        waitgame                waits until the game world runs (later actions move with it)
        clickid <id>            clicks the visible widget with that id (waits until there is one)
        explore <period> <count>  clicks a harness-chosen visible button every period frames
        dump                    log every window and widget (harness "W" lines)
        shot                    screenshot of the game window (<out>_<tick>.bmp; tools/shot.py)
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
        elif cmd == "explore":
            out.append(f"{f}:x:{t[2]}:{t[3]}")
        elif cmd == "waitplayers":
            out.append(f"{f}:w:{t[2]}")
        elif cmd == "call":  # call <hex addr> <cc> <ret> <args...> [# comment]
            spec = line.split("#")[0].split()[2:]
            out.append(f"{f}:fn:" + "/".join(spec))
        elif cmd == "hex":  # hex <coll> <i> <len hex>: logs that object's bytes ("H" lines)
            out.append(f"{f}:hex:{t[2]}.{t[3]}:{t[4]}")
        elif cmd == "poke":  # poke <global>[.<off>...] <hex value>: writes a dword (an option, say)
            out.append(f"{f}:poke:{t[2]}:{t[3]}")
        elif cmd == "autoenter":
            out.append(f"{f}:ae:{int(t[2])}")
        elif cmd == "modal":  # modal 1: actions run inside modal dialogs too (one opened with a key)
            out.append(f"{f}:md:{int(t[2])}")
        elif cmd == "objlook":  # objlook <coll hex> <i>: centre the view on that object
            out.append(f"{f}:ol:{t[2]}.{t[3]}")
        elif cmd == "objclick":  # objclick <coll hex> <i>: left click on that object's cell
            out += [f"{f}:ocm:{t[2]}.{t[3]}", f"{f + 1}:ocd:{t[2]}.{t[3]}", f"{f + 3}:ocu:{t[2]}.{t[3]}"]
        elif cmd == "cell":
            out.append(f"{f}:cm:{t[2]}:{t[3]}")
        elif cmd == "cellclick":
            out += [f"{f}:cd:{t[2]}:{t[3]}", f"{f + 2}:cu:{t[2]}:{t[3]}"]
        elif cmd == "celldrag":
            x0, y0, x1, y1, n = float(t[2]), float(t[3]), float(t[4]), float(t[5]), int(t[6])
            out.append(f"{f}:cd:{x0}:{y0}")
            for i in range(1, n + 1):
                out.append(f"{f + i}:cm:{x0 + (x1 - x0) * i / n:.2f}:{y0 + (y1 - y0) * i / n:.2f}")
            out.append(f"{f + n + 1}:cu:{x1}:{y1}")
        elif cmd in ("look", "zoom"):
            out.append(f"{f}:{cmd}:" + ":".join(t[2:3] if cmd == "zoom" else t[2:4]))
        elif cmd == "hold":
            out += [f"{f}:kd:{t[2]}", f"{f + int(t[3])}:ku:{t[2]}"]
        elif cmd == "wheel":
            out.append(f"{f}:wh:{int(t[2])}")
        elif cmd == "clickid":
            out.append(f"{f}:i:{t[2]}")
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
    for t in (target, GAME / "maps" / out):  # (the map editor saves into maps, private in the mirror)
        if t.exists():
            t.unlink()
    spec = f"load={save};frames={days};out={out}"
    if "--menu" in sys.argv:  # no save: the actions drive the menus (map editor, a new game)
        spec = f"menu=1;frames={days};out={out}"
    if ACTIONS:  # scripted actions (src/harness.cpp, "do="), passed in a file
        (GAME / "simcheck_actions.txt").write_text(ACTIONS)
        spec += ";do=@simcheck_actions.txt"
    if "--quit" in sys.argv:  # then close the game normally: its shutdown runs too
        spec += ";quit=1"
    if "--sound" in sys.argv:  # effects on (the game's audio muted), so the sound code runs
        spec += ";sound=1"
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
    if "--quit" in sys.argv:
        print(f"{tag}: {'shut down cleanly' if 'harness: game closed' in log else 'DID NOT SHUT DOWN CLEANLY'}")
    trace = GAME / f"{out}.trace"
    if not target.exists():
        target = GAME / "maps" / out
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


def calls(log):
    """The results of a run's scripted calls ("R" lines, src/harness.cpp CallFunction)."""
    if not log.exists():
        return []
    return [l for l in log.read_text(errors="replace").splitlines() if l.startswith("R ")]


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
        key = f"{save} {frames} {ACTIONS} {'--nocoverage' in sys.argv} {MODE} {'--quit' in sys.argv} {'--menu' in sys.argv} {'--sound' in sys.argv}"
        if "--fresh" not in sys.argv and t.exists() and stamp.exists() and stamp.read_text() == key:
            return [l.split()[1:] for l in t.read_text().splitlines() if l]
        r = run(tag, False, save, frames)
        stamp.write_text(key)
        return r
    if "--probe" in sys.argv:  # only whether the original gets through the actions (tools/autocall_run.py)
        ok = run("orig1", False, save, frames)
        (GAME / "simcheck_orig1.key").unlink(missing_ok=True)
        print("PROBE OK" if ok else "PROBE FAILED")
        return 0 if ok else 1
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
    # Scripted calls ("call" actions): their results, in order. A result that
    # differs between the two original runs (a heap pointer) is noise.
    ra, rb, rc = (calls(GAME / f"simcheck_{t}.log") for t in ("orig1", "orig2", "rewrite"))
    if ra or rc:
        if len(ra) != len(rc):
            print(f"CALLS DIFFER: {len(ra)} results in the original run, {len(rc)} in the rewrite run")
            return 1
        bad = [i for i in range(len(ra)) if ra[i] != rc[i] and (i >= len(rb) or ra[i] == rb[i])]
        print(f"{len(ra)} scripted calls, {sum(1 for i in range(min(len(ra), len(rb))) if ra[i] != rb[i])} vary between original runs")
        if bad:
            for i in bad[:5]:
                print(f"  CALL DIFFERS: original {ra[i]} / rewrite {rc[i]}")
            return 1
        print("MATCH: every scripted call returns the same")
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
