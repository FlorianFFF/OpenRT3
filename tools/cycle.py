"""Build + difftests, then (only if both are clean) the simcheck save comparison.
    python tools/cycle.py [simcheck args...]"""
import re
import shutil
import subprocess
import sys
import time
from pathlib import Path

import stockgame
from runlock import shared_run

root = Path(__file__).resolve().parent.parent


def main():
    b = subprocess.run('cmd /c ".\\build.bat test"', cwd=root, shell=True, capture_output=True, text=True,
                       encoding="utf-8", errors="replace")
    out = b.stdout + b.stderr
    (root / "out" / "lasttest.log").write_text(out, encoding="utf-8")  # full output, for flaky failures
    lines = [l for l in out.splitlines() if re.search(r"error|warning C|passed|FAIL", l)]
    print("\n".join(lines[-15:]))
    built = "built and installed rt3re.dll rt3re_test.dll rt3re.exe" in out
    fatal = re.search(r"\berror\b|not recognized as an internal or external command|Traceback", out)
    if not built or fatal:
        print("build did not complete cleanly: retry/simcheck skipped (do not test a stale DLL)")
        return 1
    if b.returncode and re.search(r"\b0 failed", out):
        print("build/test returned a failure status despite a passing summary: simcheck skipped")
        return 1
    if not re.search(r"\berror\b", out) and not re.search(r"\b0 failed", out):
        # Retry flaky layouts once, retaining the first failure for investigation.
        flaky = root / "out" / ("flaky_%d.log" % int(time.time()))
        shutil.copy(root / "out" / "lasttest.log", flaky)
        game = stockgame.build()
        b = subprocess.run('cmd /c ""%s" --test"' % (game / "rt3re.exe"), cwd=game, shell=True, capture_output=True, text=True,
                           encoding="utf-8", errors="replace")
        out2 = b.stdout + b.stderr
        if b.returncode == 0 and re.search(r"\b0 failed", out2):
            bad = [l for l in out.splitlines() if re.search(r"FAIL|CRASH", l)]
            print("flaky (clean on rerun, kept in %s): %s" % (flaky.name, bad[:1]))
            out = out2
    if b.returncode or re.search(r"\berror\b", out) or not re.search(r"\b0 failed", out):
        print("build/test not clean: simcheck skipped")
        return 1
    r = subprocess.run([sys.executable, str(root / "tools" / "regpreserve.py")], cwd=root, capture_output=True, text=True)
    print("\n".join(r.stdout.splitlines()[-8:]))
    if r.returncode:
        print("callers rely on ECX/EDX of a replaced function: wrap it (hook/keepregs.h) or review; simcheck skipped")
        return 1
    s = subprocess.run([sys.executable, str(root / "tools" / "simcheck.py"), *sys.argv[1:]], cwd=root,
                       capture_output=True, text=True, encoding="utf-8", errors="replace")
    print("\n".join((s.stdout + s.stderr).splitlines()[-3:]))
    return s.returncode


if __name__ == "__main__":
    with shared_run("cycle"):
        sys.exit(main())
