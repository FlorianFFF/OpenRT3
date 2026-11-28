"""Multiplayer check: a host and a client on this machine (127.0.0.1).

Both run in the test mirror (tools/stockgame.py) under the harness
(src/harness.cpp, "mp=1"): the host starts with -h, the client joins with
-c 127.0.0.1, each plays its own action script (the lobby is driven by
mouse and keys) and, once the game runs, saves after N frames and exits.

    python tools/mpcheck.py <host.txt> <client.txt> [frames] [--host=orig|rewrite] [--client=orig|rewrite]
                            [--client-delay=<seconds>] [--timeout=<seconds>] [--off=Name,...] [--offfile=<file>] [--quit]
                            [--client-nomaps]

--client-nomaps runs the client from a second mirror without scenarios, so the
host sends it the map before the game starts (the map transfer messages).
The game in that mirror is a program of its own to Windows Firewall: it must
be allowed to accept connections, as the one in the main mirror is, or the
client cannot join.

tools/mp_host.txt / mp_client.txt only start a game; tools/mp_host_play.txt /
mp_client_play.txt also play it (companies, stations, track, a train).

orig keeps every function original, rewrite installs every replacement.
Coverage (which originals ran) is written next to each save
(mp_<role>.gmc.coverage in the game folder, for tools/coverage.py). With
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


def game_of(role):
    """The folder a role runs in: --client-nomaps puts the client in the mirror
    without scenarios (tools/stockgame.py), so the host sends it the map."""
    return stockgame.MIRROR_NOMAPS if role == "client" and "--client-nomaps" in sys.argv else GAME


def launch(role, side, script, frames, timeout):
    GAME = game_of(role)
    ini = GAME / f"mp_{role}.ini"
    on = side == "rewrite"
    # --off=Name,...: keep these original on the rewrite side (bisecting a difference)
    off = {n for a in sys.argv if a.startswith("--off=") for n in a[6:].split(",")}
    off |= {n for a in sys.argv if a.startswith("--offfile=") for n in Path(a[10:]).read_text().split(",") if n}
    ini.write_text("[replace]\n" + "".join(f"{n}={1 if on and n not in off else 0}\n" for n in names()))
    acts = GAME / f"mp_{role}_actions.txt"
    acts.write_text(parse_actions(Path(script).read_text()))
    out = f"mp_{role}.gmc"
    for f in (GAME / "Saved Games" / out, GAME / f"mp_{role}.log"):
        if f.exists():
            f.unlink()
    spec = f"mp=1;frames={frames};out={out};do=@{acts.name}"
    if role == "client":
        spec += ";mpport=9001"  # (both on port 9000 cannot be on one machine; src/harness.cpp SetPort)
    if "--quit" in sys.argv:  # after saving, close the game normally: the network shutdown runs too
        spec += ";quit=1"
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
    if "--client-nomaps" in sys.argv:
        stockgame.build_nomaps()
        stockgame.reset_settings(stockgame.MIRROR_NOMAPS)
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
        log = game_of(role) / f"mp_{role}.log"
        text = log.read_text(errors="replace") if log.exists() else ""
        save = game_of(role) / "Saved Games" / f"mp_{role}.gmc"
        print(f"{role}: {'saved' if save.exists() else 'no save'}, log {len(text.splitlines())} lines")
        # What a playing script built (its "tracks" actions), and what it could not reach.
        for l in text.splitlines():
            if l.endswith(" track pieces") or "not found" in l or "not in view" in l:
                print("  " + l)
    # The client's weekly state checks against the host (src/harness.cpp LogSyncChecks).
    log = game_of("client") / "mp_client.log"
    checks = [l for l in log.read_text(errors="replace").splitlines() if l.startswith("S ")] if log.exists() else []
    bad = [l for l in checks if not l.endswith("mismatch")]
    # Scripted calls ("call" lines in both scripts, src/harness.cpp CallFunction)
    # that must return the same on both PCs: with the original on one side and
    # the rewrite on the other, this compares the two.
    # Net_Send (0x469d30) calls are the scripts' messages to the other PC (chat,
    # requests): each side sends its own, and their effect shows in the sync
    # checks, so they are not compared.
    calls = {r: [l for l in (game_of(r) / f"mp_{r}.log").read_text(errors="replace").splitlines()
                 if l.startswith("R ") and not l.startswith("R 469d30")]
             if (game_of(r) / f"mp_{r}.log").exists() else [] for r in ("host", "client")}
    if calls["host"] or calls["client"]:
        same = calls["host"] == calls["client"]
        print(f"scripted calls: {len(calls['host'])} / {len(calls['client'])}, {'the same on both' if same else 'DIFFERENT'}")
        if not same:
            for a, b in zip(calls["host"], calls["client"]):
                if a != b:
                    print(f"  host {a} / client {b}")
            return 1
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
