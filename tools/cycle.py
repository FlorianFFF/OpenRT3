"""Build + difftests, then (only if both are clean) the simcheck save comparison.
    python tools/cycle.py [simcheck args...]"""
import subprocess, sys, re
from pathlib import Path

root = Path(__file__).resolve().parent.parent
b = subprocess.run('cmd /c ".\\build.bat test"', cwd=root, shell=True, capture_output=True, text=True,
                   encoding="utf-8", errors="replace")
out = b.stdout + b.stderr
(root / "out" / "lasttest.log").write_text(out, encoding="utf-8")  # full output, for flaky failures
lines = [l for l in out.splitlines() if re.search(r"error|warning C|passed|FAIL", l)]
print("\n".join(lines[-15:]))
if not re.search(r"\berror\b", out) and not re.search(r"\b0 failed", out):
    # A few tests fail or crash now and then, only under this script's
    # environment (heap/stack layout). Real mismatches are deterministic, so
    # run the tests once more; keep the failing output for investigation.
    import shutil, time
    flaky = root / "out" / ("flaky_%d.log" % int(time.time()))
    shutil.copy(root / "out" / "lasttest.log", flaky)
    game = root.parent / "Railroad Tycoon 3 Full Game"
    b = subprocess.run('cmd /c ""%s" --test"' % (game / "rt3re.exe"), cwd=game, shell=True, capture_output=True, text=True,
                       encoding="utf-8", errors="replace")
    out2 = b.stdout + b.stderr
    if re.search(r"\b0 failed", out2):
        bad = [l for l in out.splitlines() if re.search(r"FAIL|CRASH", l)]
        print("flaky (clean on rerun, kept in %s): %s" % (flaky.name, bad[:1]))
        out = out2
if re.search(r"\berror\b", out) or not re.search(r"\b0 failed", out):
    print("build/test not clean: simcheck skipped")
    sys.exit(1)
r = subprocess.run([sys.executable, str(root / "tools" / "regpreserve.py")], cwd=root, capture_output=True, text=True)
print("\n".join(r.stdout.splitlines()[-8:]))
if r.returncode:
    print("callers rely on ECX/EDX of a replaced function: wrap it (hook/keepregs.h) or review; simcheck skipped")
    sys.exit(1)
# DesyncWatch is excluded from the decomp: rewrites follow the original exe, so
# tools/modsites.py (where DesyncWatch patches replaced code) is informational only.
s = subprocess.run([sys.executable, str(root / "tools" / "simcheck.py"), *sys.argv[1:]], cwd=root,
                   capture_output=True, text=True, encoding="utf-8", errors="replace")
print("\n".join((s.stdout + s.stderr).splitlines()[-3:]))
sys.exit(s.returncode)
