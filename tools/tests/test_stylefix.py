#!/usr/bin/env python3
"""Tests for tools/ida/stylefix.py: Hex-Rays spellings rewritten to the decomp's."""
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "ida"))
import stylefix  # noqa: E402


class StyleFix(unittest.TestCase):
    def setUp(self):
        stylefix.TAGS = {"sdfTag": True, "Plain": False}

    def test_types(self):
        self.assertEqual(stylefix.fix("unsigned __int8 a; __int64 b; unsigned int c; float d;"),
                         "u8 a; s64 b; u32 c; f32 d;")

    def test_literals_and_keywords(self):
        self.assertEqual(stylefix.fix("int __fastcall f(void *p) { return p == nullptr ? 0x10LL : 3i64; }"),
                         "s32 f(void *p) { return p == NULL ? 0x10 : 3; }")

    def test_comment_untouched(self):
        self.assertEqual(stylefix.fix("int v; // int $v0"), "s32 v; // int $v0")

    def test_struct_tags(self):
        self.assertEqual(stylefix.fix("sdfTag *a; Plain *b; struct sdfTag *c;"),
                         "struct sdfTag *a; Plain *b; struct sdfTag *c;")


if __name__ == "__main__":
    unittest.main()
