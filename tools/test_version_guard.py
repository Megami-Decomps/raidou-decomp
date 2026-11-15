#!/usr/bin/env python3
"""Tests for tools/version_guard.py: per-game views of #ifdef VERSION_<GAME> and the merge back."""
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import version_guard as g  # noqa: E402

R1, R2 = g.VERSIONS

GUARDED = """\
#include "common.h"

INCLUDE_ASM(const s32, "u", a);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "u", b);
#else
s32 b(void) {
#ifdef NON_MATCHING
    return 1;
#else
    return 2;
#endif
}
#endif

#if defined(VERSION_RAIDOU2)
INCLUDE_SDATA(const s32, "u", D_004E0000);
#endif

#ifndef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "u", c2);
#elif defined(VERSION_RAIDOU1) && !defined(VERSION_RAIDOU2)
INCLUDE_RODATA(const s32, "u", c1);
#endif
"""


class View(unittest.TestCase):
    def test_raidou1(self):
        v = g.view(GUARDED, R1)
        self.assertIn('INCLUDE_ASM(const s32, "u", b);', v)
        self.assertNotIn("s32 b(void)", v)
        self.assertNotIn("D_004E0000", v)
        self.assertIn("c1", v)
        self.assertNotIn("c2", v)
        self.assertNotIn("VERSION_", v)

    def test_raidou2_keeps_other_conditionals(self):
        v = g.view(GUARDED, R2)
        self.assertIn("s32 b(void) {\n#ifdef NON_MATCHING\n    return 1;\n#else\n    return 2;\n#endif\n}\n", v)
        self.assertIn("D_004E0000", v)
        self.assertIn("c2", v)
        self.assertNotIn("c1", v)
        self.assertNotIn('INCLUDE_ASM(const s32, "u", b);', v)

    def test_blank_keeps_line_numbers(self):
        for v in g.VERSIONS:
            self.assertEqual(g.view(GUARDED, v, blank=True).count("\n"), GUARDED.count("\n"))

    def test_other_version_macros_are_plain_conditions(self):
        text = "#ifndef VERSION_DDS1\n#define VERSION_DDS1 1\n#endif\n"
        self.assertEqual(g.view(text, R1), text)

    def test_unsupported(self):
        for bad in ("#if VERSION_RAIDOU1 > 0\n#endif\n",
                    "#if defined(VERSION_RAIDOU1) && defined(NON_MATCHING)\n#endif\n",
                    "#ifdef NON_MATCHING\n#elif defined(VERSION_RAIDOU1)\n#endif\n",
                    "#ifdef VERSION_RAIDOU1\n",
                    "#endif\n"):
            with self.assertRaises(ValueError, msg=bad):
                g.check(bad)


class Merge(unittest.TestCase):
    def test_roundtrip(self):
        views = {v: g.view(GUARDED, v) for v in g.VERSIONS}
        merged = g.merge(views)
        for v in g.VERSIONS:
            self.assertEqual(g.view(merged, v), views[v])
        self.assertEqual(g.merge({v: g.view(merged, v) for v in g.VERSIONS}), merged)

    def test_shape(self):
        a = 'x;\n\nINCLUDE_SDATA(const s32, "u", D_0046F328);\n\ny;\n'
        b = 'x;\n\nINCLUDE_SDATA(const s32, "u", D_004E4340);\n\ny;\n'
        self.assertEqual(g.merge({R1: a, R2: b}),
                         'x;\n\n#ifdef VERSION_RAIDOU1\nINCLUDE_SDATA(const s32, "u", D_0046F328);\n'
                         '#else\nINCLUDE_SDATA(const s32, "u", D_004E4340);\n#endif\n\ny;\n')
        self.assertEqual(g.merge({R1: "x;\ny;\n", R2: "x;\nz;\ny;\n"}),
                         "x;\n#ifdef VERSION_RAIDOU2\nz;\n#endif\ny;\n")

    def test_identical(self):
        self.assertEqual(g.merge({R1: "x;\n", R2: "x;\n"}), "x;\n")


if __name__ == "__main__":
    unittest.main()
