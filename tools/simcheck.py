"""End-to-end determinism check of the rewrite.

Loads a save with the harness (src/harness.cpp), runs N frames, saves, and
compares the saves (via the per-write CRC trace) between runs:
  A = all replacements off (original code), B = A again (harness noise check),
  C = all replacements on.
A == B proves the setup is deterministic; A == C proves the rewrite matches.

    python tools/simcheck.py [save] [frames]
"""
import os
import re
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
GAME = ROOT.parent / "Railroad Tycoon 3 Full Game"
SAVES = GAME / "Saved Games"


def names():
    out = []
    for p in sorted((ROOT / "src").rglob("*.cpp")):
        out += re.findall(r"RT3_REPLACE\(\s*0x[0-9a-fA-F]+\s*,\s*(\w+)", p.read_text(encoding="utf-8"))
    return out


def run(tag, enabled, save, days, timeout=240):
    ini = GAME / "rt3re.ini"
    ini.write_text("[replace]\n" + "".join(f"{n}={1 if enabled else 0}\n" for n in names()))
    ext = Path(save).suffix
    out = f"simcheck_{tag}{ext}"
    target = SAVES / out
    if target.exists():
        target.unlink()
    env = dict(os.environ, RT3RE_HARNESS=f"load={save};frames={days};out={out}", RT3RE_COVERAGE="1")
    p = subprocess.Popen([str(GAME / "rt3re.exe"), "-s4"], cwd=GAME, env=env)  # safe mode, windowed, no sound
    p.wait()
    t0 = time.time()
    while time.time() - t0 < timeout:
        r = subprocess.run(["tasklist", "/fi", "imagename eq RT3_mp.exe"], capture_output=True, text=True)
        if "RT3_mp.exe" not in r.stdout:
            break
        time.sleep(2)
    else:
        subprocess.run(["taskkill", "/f", "/im", "RT3_mp.exe"], capture_output=True)
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


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    save = args[0] if len(args) > 0 else "mature.gmc"
    frames = args[1] if len(args) > 1 else "2000"
    # The two original runs only change with the save/frame count: reuse them
    # unless --fresh is given.
    def cached(tag):
        t = GAME / f"simcheck_{tag}{Path(save).suffix}.trace"
        stamp = GAME / f"simcheck_{tag}.key"
        key = f"{save} {frames}"
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
    covfile = GAME / f"simcheck_rewrite{Path(save).suffix}.coverage"
    if covfile.exists():
        rows = [l.split() for l in covfile.read_text().splitlines() if l]
        ran = [r for r in rows if int(r[2]) > 0]
        print(f"coverage: {len(ran)}/{len(rows)} replaced functions ran during the check")
        lines = [" ".join(r) for r in sorted(rows)]
        (ROOT / "docs" / "simcheck_coverage.txt").write_text("\n".join(lines) + "\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
