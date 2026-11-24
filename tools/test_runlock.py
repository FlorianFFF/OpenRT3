"""Subprocess tests for shared-run locking; never builds or launches the game."""
import json
import os
import subprocess
import sys
import tempfile
import time
import unittest
from pathlib import Path
from unittest import mock

import runlock

TOOLS = Path(__file__).resolve().parent
WORKER = """
import os, sys
from pathlib import Path
from runlock import shared_run, read_owner
path = Path(sys.argv[1])
action = sys.argv[2]
print("starting", flush=True)
with shared_run(action, lock_path=path):
    print("acquired", flush=True)
    if action == "hold":
        sys.stdin.readline()
    if action == "mark":
        Path(sys.argv[3]).write_text("entered")
    if action == "fresh":
        assert read_owner(path)["pid"] == os.getpid()
"""


class SharedRunTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="rt3_runlock_test_")
        self.addCleanup(self.temp.cleanup)
        self.path = Path(self.temp.name) / "shared.lock"

    def spawn(self, action, *args, inherit=False, extra_env=None):
        env = os.environ.copy()
        env["PYTHONPATH"] = str(TOOLS) + os.pathsep + env.get("PYTHONPATH", "")
        if not inherit:
            env.pop(runlock.TOKEN_ENV, None)
            env.pop(runlock.PATH_ENV, None)
        env.update(extra_env or {})
        p = subprocess.Popen([sys.executable, "-u", "-c", WORKER, str(self.path), action, *map(str, args)],
                             stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                             env=env, text=True)
        self.addCleanup(self.stop, p)
        self.assertEqual(p.stdout.readline().strip(), "starting")
        return p

    @staticmethod
    def stop(p):
        if p.poll() is None:
            p.terminate()
        p.communicate(timeout=10)
        for stream in (p.stdin, p.stdout, p.stderr):
            if stream and not stream.closed:
                stream.close()

    def test_independent_process_waits(self):
        owner = self.spawn("hold")
        self.assertEqual(owner.stdout.readline().strip(), "acquired")
        marker = Path(self.temp.name) / "entered.txt"
        waiter = self.spawn("mark", marker)
        time.sleep(0.3)
        self.assertFalse(marker.exists())
        self.assertIsNone(waiter.poll())
        owner.stdin.write("release\n")
        owner.stdin.flush()
        owner.communicate(timeout=10)
        out, err = waiter.communicate(timeout=10)
        self.assertEqual(waiter.returncode, 0, err)
        self.assertIn("acquired", out)
        self.assertTrue(marker.exists())

    def test_nested_child_inherits_cycle(self):
        with runlock.shared_run("parent", lock_path=self.path):
            parent = runlock.read_owner(self.path)
            child = self.spawn("child", inherit=True)
            out, err = child.communicate(timeout=10)
            self.assertEqual(child.returncode, 0, err)
            self.assertIn("acquired", out)
            self.assertEqual(runlock.read_owner(self.path), parent)
        self.assertEqual(runlock.read_owner(self.path)["state"], "released")

    def test_nested_context_keeps_owner(self):
        with runlock.shared_run("outer", lock_path=self.path):
            first = runlock.read_owner(self.path)
            with runlock.shared_run("inner", lock_path=self.path):
                self.assertEqual(runlock.read_owner(self.path), first)
            self.assertEqual(runlock.read_owner(self.path), first)

    def test_crashed_owner_releases_lock_and_stale_token(self):
        owner = self.spawn("hold")
        self.assertEqual(owner.stdout.readline().strip(), "acquired")
        stale = runlock.read_owner(self.path)
        owner.terminate()
        owner.communicate(timeout=10)
        child = self.spawn("fresh", extra_env={
            runlock.TOKEN_ENV: stale["token"], runlock.PATH_ENV: str(self.path)
        })
        out, err = child.communicate(timeout=10)
        self.assertEqual(child.returncode, 0, err)
        self.assertIn("acquired", out)
        self.assertEqual(runlock.read_owner(self.path)["state"], "released")

    def test_build_wrapper_uses_child_guard_and_returns_status(self):
        def command(args, cwd, env):
            self.assertEqual(args, ["cmd", "/c", str(runlock.ROOT / "build.bat"), "test", "--filter=fixture"])
            self.assertEqual(cwd, runlock.ROOT)
            self.assertEqual(env["RT3_BUILD_LOCKED"], "1")
            self.assertEqual(env[runlock.TOKEN_ENV], runlock.read_owner(self.path)["token"])
            return 7
        with mock.patch.dict(os.environ, {runlock.PATH_ENV: str(self.path)}):
            with mock.patch.object(runlock.subprocess, "call", side_effect=command):
                self.assertEqual(runlock.main(["--build", "test", "--filter=fixture"]), 7)
        self.assertEqual(runlock.read_owner(self.path)["state"], "released")

    def test_environment_restored_on_exception(self):
        before = {name: os.environ.get(name) for name in (runlock.TOKEN_ENV, runlock.PATH_ENV)}
        with self.assertRaisesRegex(RuntimeError, "fixture"):
            with runlock.shared_run("failure", lock_path=self.path):
                raise RuntimeError("fixture")
        self.assertEqual(before, {name: os.environ.get(name) for name in before})
        self.assertEqual(runlock.read_owner(self.path)["state"], "released")


if __name__ == "__main__":
    unittest.main()
