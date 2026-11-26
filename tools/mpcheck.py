"""Multiplayer check: a host and a client on this machine (127.0.0.1).

Both run in the test mirror (tools/stockgame.py) under the harness
(src/harness.cpp, "mp=1"): the host starts with -h, the client joins with
-c 127.0.0.1, each plays its own action script (the lobby is driven by
mouse and keys) and, once the game runs, saves after N frames and exits.

    python tools/mpcheck.py <host.txt> <client.txt> [frames] [--host=orig|rewrite] [--client=orig|rewrite]
                            [--client-delay=<seconds>] [--timeout=<seconds>]

orig keeps every function original, rewrite installs every replacement.
Coverage (which originals ran) is written next to each save
(Saved Games/mp_<role>.gmc.coverage, for tools/coverage.py). With
the original as host and the rewrite as client, the game's own lockstep
checks compare the two simulations as they run.
"""
import os
import subprocess
import sys
import time
from pathlib import Path

import stockgame
from pe import find_exe
from runlock import shared_run
from simcheck import names, parse_actions

ROOT = Path(__file__).resolve().parents[1]
GAME = stockgame.MIRROR


def launch(role, side, script, frames, timeout):
    ini = GAME / f"mp_{role}.ini"
    on = side == "rewrite"
    ini.write_text("[replace]\n" + "".join(f"{n}={1 if on else 0}\n" for n in names()))
    acts = GAME / f"mp_{role}_actions.txt"
    acts.write_text(parse_actions(Path(script).read_text()))
    out = f"mp_{role}.gmc"
    for f in (GAME / "Saved Games" / out, GAME / f"mp_{role}.log"):
        if f.exists():
            f.unlink()
    spec = f"mp=1;frames={frames};out={out};do=@{acts.name}"
    if role == "client":
        spec += ";mpport=9001"  # (both on port 9000 cannot be on one machine; src/harness.cpp SetPort)
    env = dict(os.environ, RT3RE_HARNESS=spec,
               RT3RE_INI=str(ini), RT3RE_LOG=f"mp_{role}.log", RT3RE_COVERAGE="1")
    # --lobby=gamespy: the command-line way (-h / -c 127.0.0.1); else the
    # script drives the menus (LAN).
    if any(a == "--lobby=gamespy" for a in sys.argv):
        args = [str(GAME / "rt3re.exe"), "-s4"] + (["-h", f"-n{role}"] if role == "host" else ["-c", "127.0.0.1", f"-n{role}"])
    else:
        args = [str(GAME / "rt3re.exe"), "-s4"]
    return subprocess.Popen(args, cwd=GAME, env=env)


def main():
    stockgame.build()
    stockgame.reset_settings()
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    host_script, client_script = args[0], args[1]
    frames = int(args[2]) if len(args) > 2 else 1000
    timeout = next((int(a[10:]) for a in sys.argv if a.startswith("--timeout=")), 600)
    side = lambda r: next((a.split("=")[1] for a in sys.argv if a.startswith(f"--{r}=")), "orig")
    host = launch("host", side("host"), host_script, frames, timeout)
    # The client joins at startup: only once the host is hosting.
    time.sleep(next((float(a[15:]) for a in sys.argv if a.startswith("--client-delay=")), 5))
    client = launch("client", side("client"), client_script, frames, timeout)
    # The launcher exits once the game runs: wait for the games themselves.
    image = find_exe(GAME).name
    t0 = time.time()
    while time.time() - t0 < timeout:
        r = subprocess.run(["tasklist", "/fi", f"imagename eq {image}"], capture_output=True, text=True)
        if image not in r.stdout:
            break
        time.sleep(2)
    else:
        print("timeout")
    subprocess.run(["taskkill", "/f", "/im", image], capture_output=True)
    for role in ("host", "client"):
        log = GAME / f"mp_{role}.log"
        text = log.read_text(errors="replace") if log.exists() else ""
        save = GAME / "Saved Games" / f"mp_{role}.gmc"
        print(f"{role}: {'saved' if save.exists() else 'no save'}, log {len(text.splitlines())} lines")
    # The client's weekly state checks against the host (src/harness.cpp LogSyncChecks).
    log = GAME / "mp_client.log"
    checks = [l for l in log.read_text(errors="replace").splitlines() if l.startswith("S ")] if log.exists() else []
    bad = [l for l in checks if not l.endswith("mismatch")]
    print(f"sync checks: {len(checks)}, with a mismatch: {len(bad)}")
    for l in bad[:5]:
        print("  " + l)
    if not checks:
        print("NO SYNC CHECKS (the game did not run long enough, or never started)")
        return 1
    if bad:
        print("DESYNC: the client's state differs from the host's")
        return 1
    print("IN SYNC: every weekly check matched")
    return 0


if __name__ == "__main__":
    with shared_run("mpcheck"):
        sys.exit(main())
