#!/usr/bin/env python3
"""Tests for land_sync.sh, with python3 replaced by a stub that records its arguments."""
import os
import pathlib
import subprocess
import tempfile
import unittest

SCRIPT = pathlib.Path(__file__).resolve().parents[1] / "ida" / "land_sync.sh"


class LandSyncTests(unittest.TestCase):
    def run_sync(self, *args: str) -> tuple[subprocess.CompletedProcess, list[str]]:
        with tempfile.TemporaryDirectory() as tmp:
            log = pathlib.Path(tmp) / "calls"
            stub = pathlib.Path(tmp) / "python3"
            stub.write_text(f'#!/bin/sh\necho "$*" >> {log}\n')
            stub.chmod(0o755)
            env = {**os.environ, "PATH": f"{tmp}:{os.environ['PATH']}"}
            r = subprocess.run(["sh", str(SCRIPT), *args], env=env, capture_output=True, text=True)
            return r, log.read_text().splitlines() if log.exists() else []

    def test_help_prints_usage_and_runs_nothing(self) -> None:
        for flag in ("--help", "-h"):
            r, calls = self.run_sync(flag)
            self.assertEqual((r.returncode, calls), (0, []))
            self.assertIn("usage:", r.stdout)

    def test_an_unknown_argument_is_refused(self) -> None:
        for args in (("--bogus",), ("--game", "r3"), ("--game",)):
            r, calls = self.run_sync(*args)
            self.assertEqual((r.returncode, calls), (2, []), args)
            self.assertIn("usage:", r.stderr)

    def test_one_game(self) -> None:
        r, calls = self.run_sync("--game", "r1")
        self.assertEqual(r.returncode, 0, r.stderr)
        shared = [c for c in calls if c.startswith("shared.py")]
        self.assertEqual(shared, ["shared.py sync 13337 SLUS_214.31 r1 --defs", "shared.py plugin 13337 SLUS_214.31",
                                  "shared.py save 13337 SLUS_214.31"])
        self.assertIn("prep_stage1.py", calls)

    def test_both_games_by_default(self) -> None:
        r, calls = self.run_sync()
        self.assertEqual(r.returncode, 0, r.stderr)
        self.assertEqual(sum(c.startswith("shared.py sync") for c in calls), 2)


if __name__ == "__main__":
    unittest.main()
