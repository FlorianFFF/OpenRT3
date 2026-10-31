"""Build + difftests, then (only if both are clean) the simcheck save comparison.
    python tools/cycle.py [simcheck args...]"""
import subprocess, sys, re
from pathlib import Path

root = Path(__file__).resolve().parent.parent
b = subprocess.run('cmd /c ".\\build.bat test"', cwd=root, shell=True, capture_output=True, text=True,
                   encoding="utf-8", errors="replace")
out = b.stdout + b.stderr
lines = [l for l in out.splitlines() if re.search(r"error|warning C|passed|FAIL", l)]
print("\n".join(lines[-15:]))
if re.search(r"\berror\b", out) or not re.search(r"\b0 failed", out):
    print("build/test not clean: simcheck skipped")
    sys.exit(1)
r = subprocess.run([sys.executable, str(root / "tools" / "regpreserve.py")], cwd=root, capture_output=True, text=True)
print("\n".join(r.stdout.splitlines()[-8:]))
if r.returncode:
    print("callers rely on ECX/EDX of a replaced function: wrap it (hook/keepregs.h) or review; simcheck skipped")
    sys.exit(1)
m = subprocess.run([sys.executable, str(root / "tools" / "modsites.py")], cwd=root, capture_output=True, text=True)
print("\n".join(m.stdout.splitlines()[-6:]))
if m.returncode:
    print("call the hooked callee via hook::CallTarget(site); simcheck skipped")
    sys.exit(1)
s = subprocess.run([sys.executable, str(root / "tools" / "simcheck.py"), *sys.argv[1:]], cwd=root,
                   capture_output=True, text=True, encoding="utf-8", errors="replace")
print("\n".join((s.stdout + s.stderr).splitlines()[-3:]))
sys.exit(s.returncode)
