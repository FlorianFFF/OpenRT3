"""Serialize shared build/game runs, including subprocesses within one cycle.

    python tools/runlock.py -- command [args...]
    python tools/runlock.py --status

Custom Python runners should hold shared_run() across build, stockgame.build(),
all game processes, and their output collection.
"""
import contextlib
import ctypes
import json
import os
import subprocess
import sys
import time
import uuid
from ctypes import wintypes
from datetime import datetime, timezone
from pathlib import Path

if os.name == "nt":
    import msvcrt
else:
    import fcntl

ROOT = Path(__file__).resolve().parents[1]
TOKEN_ENV = "RT3_RUN_LOCK_TOKEN"
PATH_ENV = "RT3_RUN_LOCK_PATH"


def _owner_path(path):
    return path.with_suffix(".owner.json")


def read_owner(path=None):
    path = Path(path or os.environ.get(PATH_ENV, ROOT / "out" / "shared-run.lock")).resolve()
    try:
        return json.loads(_owner_path(path).read_text(encoding="utf-8"))
    except (OSError, ValueError):
        return {}


def _alive(pid):
    if os.name == "nt":
        kernel = ctypes.WinDLL("kernel32", use_last_error=True)
        kernel.OpenProcess.argtypes = [wintypes.DWORD, wintypes.BOOL, wintypes.DWORD]
        kernel.OpenProcess.restype = wintypes.HANDLE
        kernel.WaitForSingleObject.argtypes = [wintypes.HANDLE, wintypes.DWORD]
        kernel.CloseHandle.argtypes = [wintypes.HANDLE]
        handle = kernel.OpenProcess(0x100000, False, pid)
        if not handle:
            return False
        try:
            return kernel.WaitForSingleObject(handle, 0) == 258
        finally:
            kernel.CloseHandle(handle)
    try:
        os.kill(pid, 0)
        return True
    except ProcessLookupError:
        return False
    except PermissionError:
        return True


def _try_lock(file):
    file.seek(0)
    try:
        if os.name == "nt":
            msvcrt.locking(file.fileno(), msvcrt.LK_NBLCK, 1)
        else:
            fcntl.flock(file.fileno(), fcntl.LOCK_EX | fcntl.LOCK_NB)
        return True
    except (BlockingIOError, PermissionError):
        return False
    except OSError as exc:
        if exc.errno in (11, 13, 36):
            return False
        raise


def _unlock(file):
    file.seek(0)
    if os.name == "nt":
        msvcrt.locking(file.fileno(), msvcrt.LK_UNLCK, 1)
    else:
        fcntl.flock(file.fileno(), fcntl.LOCK_UN)


@contextlib.contextmanager
def shared_run(label, lock_path=None):
    path = Path(lock_path or os.environ.get(PATH_ENV, ROOT / "out" / "shared-run.lock")).resolve()
    owner = read_owner(path)
    token = os.environ.get(TOKEN_ENV)
    # Cycle children inherit the token; do not deadlock on their parent's lock.
    if (token and owner.get("token") == token and owner.get("state") == "held"
            and _alive(owner.get("pid", 0))):
        yield
        return
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("a+b", buffering=0) as file:
        if file.seek(0, os.SEEK_END) == 0:
            file.write(b"\0")
        announced = False
        while not _try_lock(file):
            if not announced:
                owner = read_owner(path)
                print(f"shared run: waiting for {owner.get('label', 'another run')} "
                      f"(pid {owner.get('pid', '?')})", file=sys.stderr, flush=True)
                announced = True
            time.sleep(0.2)
        previous = {name: os.environ.get(name) for name in (TOKEN_ENV, PATH_ENV)}
        owner = {"pid": os.getpid(), "label": label, "state": "held",
                 "token": uuid.uuid4().hex, "started": datetime.now(timezone.utc).isoformat()}
        try:
            _owner_path(path).write_text(json.dumps(owner, indent=2) + "\n", encoding="utf-8")
            os.environ[TOKEN_ENV] = owner["token"]
            os.environ[PATH_ENV] = str(path)
            yield
        finally:
            try:
                owner.update(state="released", finished=datetime.now(timezone.utc).isoformat())
                _owner_path(path).write_text(json.dumps(owner, indent=2) + "\n", encoding="utf-8")
            finally:
                for name, value in previous.items():
                    if value is None:
                        os.environ.pop(name, None)
                    else:
                        os.environ[name] = value
                _unlock(file)


def main(args=None):
    args = list(sys.argv[1:] if args is None else args)
    if args == ["--status"]:
        print(json.dumps(read_owner(), indent=2))
        return 0
    build = args[:1] == ["--build"]
    if build:
        # Full path: with NoDefaultCurrentDirectoryInExePath set, cmd does not
        # look in the working directory for a bare "build.bat".
        command = ["cmd", "/c", str(ROOT / "build.bat"), *args[1:]]
        label = "build.bat " + " ".join(args[1:])
    else:
        command = args[1:] if args[:1] == ["--"] else args
        if not command:
            print(__doc__)
            return 2
        label = Path(command[0]).name
    with shared_run(label):
        env = os.environ.copy()
        if build:
            env["RT3_BUILD_LOCKED"] = "1"
        return subprocess.call(command, cwd=ROOT, env=env)


if __name__ == "__main__":
    sys.exit(main())
