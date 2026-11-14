#!/usr/bin/env python3
"""Tests for tools/raidou_c.py: the scope of port --retry and the candidate pairs."""

from __future__ import annotations

import sys
import unittest
from unittest import mock
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import raidou_c  # noqa: E402


class RetryScopeTests(unittest.TestCase):
    def test_only_asm_rows_count_as_retried(self) -> None:
        carry = {"a": "x.c", "b": "y.c", "c": "z.c"}
        done = {"a": ("asm", "no member named ref"), "b": ("c", "")}
        self.assertEqual(raidou_c.retried_functions(carry, done), {"a"})

    def test_units_holding_matches_include_asm_names_exactly(self) -> None:
        texts = {
            "u1": 'INCLUDE_ASM(const s32, "u1", fooBar);\n',
            "u2": 'INCLUDE_ASM(const s32, "u2", foo);\n',
            "u3": "void foo(void) {}\n",
        }
        self.assertEqual(raidou_c.units_holding(texts, {"foo"}), {"u2"})

    def test_no_retried_function_selects_no_unit(self) -> None:
        texts = {"u1": 'INCLUDE_ASM(const s32, "u1", foo);\n'}
        self.assertEqual(raidou_c.units_holding(texts, set()), set())


class CandidateTests(unittest.TestCase):
    def test_differing_pair_counts_only_when_marked(self) -> None:
        def pair(fn, status):
            return {"version": "raidou1", "function": fn, "smt3_name": fn,
                    "smt3_src": "src/smt3/mc/mcManager.c", "status": status}
        pairs = [pair("same", "identical"), pair("marked", "differs"), pair("plain", "differs")]
        done = {"marked": ("asm", raidou_c.COMPILES), "plain": ("asm", "1 words differ")}
        with mock.patch.object(raidou_c, "read_pairs", side_effect=[pairs, []]), \
                mock.patch.object(raidou_c, "outcomes", return_value=done):
            self.assertEqual(set(raidou_c.candidates("raidou1")), {"same", "marked"})


if __name__ == "__main__":
    unittest.main()
