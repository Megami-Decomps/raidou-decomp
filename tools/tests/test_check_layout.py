#!/usr/bin/env python3
"""Tests for tools/check_layout.py: strays and unused scripts are refused, the repository passes."""
import pathlib
import sys
import tempfile
import unittest

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[1]))
import check_layout  # noqa: E402


class LayoutTests(unittest.TestCase):
    def test_stray_files_are_refused(self) -> None:
        files = ["README.md", "src/raidou1/a.c", "notes.txt", "tools/x/y.py", "tools/test_a.py"]
        self.assertEqual(check_layout.stray(files), ["notes.txt", "tools/x/y.py", "tools/test_a.py"])

    def test_unused_script_is_refused(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = pathlib.Path(tmp)
            (root / "tools/tests").mkdir(parents=True)
            files = {"tools/used.py": "", "tools/caller.py": "import used\n", "tools/orphan.py": "",
                     "tools/tested.py": "", "tools/tests/test_tested.py": "import tested\n",
                     "configure.py": "run('tools/caller.py')\n"}
            for name, text in files.items():
                (root / name).write_text(text)
            self.assertEqual(check_layout.unused(root, sorted(files)), ["tools/orphan.py"])

    def test_repository_passes(self) -> None:
        self.assertEqual(check_layout.main(), 0)


if __name__ == "__main__":
    unittest.main()
