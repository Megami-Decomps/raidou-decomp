#!/usr/bin/env python3
"""Tests for tools/check_unit.py: a call through an smt3_symbols name is compared at its address."""
import contextlib
import io
import pathlib
import sys
import tempfile
import unittest
from unittest import mock

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[1]))
import check_unit  # noqa: E402
from retail_elf import RETAIL, ROOT  # noqa: E402

UNIT = ROOT / "src/raidou1/code_00323280.c"
CALL = "        dds3KillProcess(wk->drawPID, 0);\n"
DRAFT_CALL = "        smt3Alias(wk->drawPID, 0);\n"
WRONG = 0x00100E38  # _KillProcess, the address smt3_symbols gives KillProcess


@unittest.skipUnless((ROOT / RETAIL["raidou1"]).exists(), "no retail executable")
class Smt3AliasRelocation(unittest.TestCase):
    def check(self, aliases, call=DRAFT_CALL):
        text = UNIT.read_text()
        self.assertIn(CALL, text, f"{UNIT.name} no longer calls dds3KillProcess in Shutdown_cmpMap")
        draft = text.replace(CALL, call).replace(
            "void Shutdown_cmpMap(", "extern s32 smt3Alias(dds3ProcessID_t *, s32);\nvoid Shutdown_cmpMap(", 1)
        with tempfile.TemporaryDirectory() as tmp:
            source = pathlib.Path(tmp) / UNIT.name
            source.write_text(draft)
            argv = ["check_unit.py", str(UNIT), "--source", str(source), "--func", "Shutdown_cmpMap"]
            out = io.StringIO()
            with mock.patch.object(sys, "argv", argv), \
                    mock.patch.object(check_unit, "smt3_symbols", lambda version: aliases), \
                    contextlib.redirect_stdout(out), self.assertRaises(SystemExit) as done:
                check_unit.main()
        return done.exception.code, out.getvalue()

    def test_alias_at_retail_target_matches(self):
        target = check_unit.symbols("raidou1")["dds3KillProcess"]
        code, out = self.check({"smt3Alias": target})
        self.assertEqual(code, 0, out)
        self.assertIn("OK   Shutdown_cmpMap", out)

    def test_alias_at_other_address_differs(self):
        code, out = self.check({"smt3Alias": WRONG})
        self.assertEqual(code, 1, out)
        self.assertIn("DIFF Shutdown_cmpMap", out)
        self.assertIn("R_MIPS_26 smt3Alias (retail uses a different address)", out)
        self.assertNotIn("UNDEF smt3Alias", out)

    def test_symbol_addrs_wins_over_alias(self):
        code, out = self.check({"dds3KillProcess": WRONG}, call=CALL)
        self.assertEqual(code, 0, out)


if __name__ == "__main__":
    unittest.main()
