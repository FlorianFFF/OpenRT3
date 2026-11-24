"""Pipeline guard tests; all build/game subprocesses are mocked."""
import contextlib
import io
import subprocess
import tempfile
import unittest
from pathlib import Path
from unittest import mock

import cycle

BUILT = "built and installed rt3re.dll rt3re_test.dll rt3re.exe\n"


def result(code, text=""):
    return subprocess.CompletedProcess([], code, text, "")


class CycleTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="rt3_cycle_test_")
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        (self.root / "out").mkdir()

    def run_cycle(self, results):
        with mock.patch.object(cycle, "root", self.root), mock.patch.object(cycle.sys, "argv", ["cycle.py"]):
            with mock.patch.object(cycle.stockgame, "build", return_value=self.root / "game") as mirror:
                with mock.patch.object(cycle.subprocess, "run", side_effect=results) as commands:
                    with contextlib.redirect_stdout(io.StringIO()):
                        rc = cycle.main()
                    return rc, commands.call_count, mirror.call_count

    def test_command_failure_cannot_test_old_dll(self):
        rc, calls, mirror = self.run_cycle([result(1, "'build.bat' is not recognized as an internal or external command")])
        self.assertEqual((rc, calls, mirror), (1, 1, 0))

    def test_compile_failure_cannot_test_old_dll(self):
        self.assertEqual(self.run_cycle([result(2, "error C1234: failed")]), (1, 1, 0))

    def test_missing_build_marker_rejects_even_passing_summary(self):
        self.assertEqual(self.run_cycle([result(0, "668 passed, 0 failed")]), (1, 1, 0))

    def test_nonzero_exit_rejects_passing_summary(self):
        self.assertEqual(self.run_cycle([result(7, BUILT + "668 passed, 0 failed")]), (1, 1, 0))

    def test_clean_run_checks_registers_and_simcheck(self):
        rc, calls, mirror = self.run_cycle([
            result(0, BUILT + "668 passed, 0 failed"), result(0, "0 unreviewed pairs"), result(0, "MATCH:")
        ])
        self.assertEqual((rc, calls, mirror), (0, 3, 0))

    def test_flaky_retry_still_checks_exit_status(self):
        self.assertEqual(self.run_cycle([
            result(1, BUILT + "667 passed, 1 failed"), result(1, "668 passed, 0 failed")
        ]), (1, 2, 1))

    def test_clean_flaky_retry_can_continue(self):
        self.assertEqual(self.run_cycle([
            result(1, BUILT + "667 passed, 1 failed"), result(0, "668 passed, 0 failed"),
            result(0, "0 unreviewed pairs"), result(0, "MATCH:")
        ]), (0, 4, 1))


if __name__ == "__main__":
    unittest.main()
