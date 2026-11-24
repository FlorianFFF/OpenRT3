"""Finds which replaced function makes the game crash at startup.

Runs the game (via rt3re.exe) with subsets of replacements disabled through
rt3re.ini and checks rt3re.log for an exception. Prints the culprit(s).

    python tools/find_crash.py [seconds_per_run]
"""
import subprocess
import sys
import time
from pathlib import Path
from replacements import REPLACEMENT_RE
from pe import find_exe

ROOT = Path(__file__).resolve().parents[1]
GAME = ROOT.parent / "Railroad Tycoon 3 Full Game"
INI = GAME / "rt3re.ini"
LOG = GAME / "rt3re.log"


def names():
    out = []
    for p in sorted((ROOT / "src").rglob("*.cpp")):
        out += [m.group(2) for m in REPLACEMENT_RE.finditer(p.read_text(encoding="utf-8"))]
    return out


def crashes(enabled, all_names, secs):
    INI.write_text("[replace]\n" + "".join(f"{n}={1 if n in enabled else 0}\n" for n in all_names))
    if LOG.exists():
        LOG.unlink()
    subprocess.Popen([str(GAME / "rt3re.exe")], cwd=GAME)
    time.sleep(secs)
    subprocess.run(["taskkill", "/f", "/im", find_exe().name], capture_output=True)
    time.sleep(1)
    text = LOG.read_text() if LOG.exists() else ""
    return "exception" in text


def main():
    secs = int(sys.argv[1]) if len(sys.argv) > 1 else 15
    all_names = names()
    suspects = list(all_names)
    if not crashes(set(suspects), all_names, secs):
        print("no crash with everything enabled")
        return
    # Keep halving the suspect set: enable only one half, see if it still crashes.
    while len(suspects) > 1:
        half = suspects[: len(suspects) // 2]
        rest = suspects[len(suspects) // 2:]
        if crashes(set(half), all_names, secs):
            suspects = half
        elif crashes(set(rest), all_names, secs):
            suspects = rest
        else:
            print("crash needs functions from both halves:", suspects)
            break
        print("suspects:", len(suspects), suspects[:8])
    print("culprit:", suspects)
    INI.unlink()


if __name__ == "__main__":
    main()
