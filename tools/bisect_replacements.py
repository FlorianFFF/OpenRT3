"""Finds the replaced function that breaks the simcheck rewrite run (stall or
mismatch) by switching halves of the replacements off.
    python tools/bisect_replacements.py [simcheck args...]
    python tools/bisect_replacements.py --mp [mpcheck args...]   (a desync in tools/mpcheck.py;
                                                                  give the rewrite side, e.g. --client=rewrite)"""
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import simcheck  # noqa: E402


def good(off):
    lst = ROOT / "out" / "bisect_off.txt"
    lst.write_text(",".join(off))
    mp = "--mp" in sys.argv
    args = [a for a in sys.argv[1:] if a != "--mp"]
    tool = "mpcheck.py" if mp else "simcheck.py"
    r = subprocess.run([sys.executable, str(ROOT / "tools" / tool), f"--offfile={lst}", *args],
                       cwd=ROOT, capture_output=True, text=True)
    ok = r.returncode == 0 and ("IN SYNC" if mp else "MATCH:") in r.stdout  # (simcheck: save and drawing)
    print(f"  {len(off)} off -> {'good' if ok else 'bad'}", flush=True)
    return ok


def main():
    names = sorted(set(simcheck.names()))
    suspects = names
    # Invariant: switching off all suspects makes the run good.
    if not good(suspects):
        print("still bad with everything off: not caused by a replacement")
        return
    while len(suspects) > 1:
        half = suspects[: len(suspects) // 2]
        print(f"{len(suspects)} suspects", flush=True)
        suspects = half if good(half) else suspects[len(half):]
    print("culprit:", suspects[0])


if __name__ == "__main__":
    main()
