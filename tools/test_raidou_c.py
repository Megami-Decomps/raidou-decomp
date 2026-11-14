#!/usr/bin/env python3
"""Tests for tools/raidou_c.py: the scope of port --retry, the candidate pairs, the pinned dds-decomp
revision and the Raidou spelling of placeholder aliases."""

from __future__ import annotations

import subprocess
import sys
import tempfile
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


class MemberRenameTests(unittest.TestCase):
    def test_renames_follow_the_access_type(self) -> None:
        text = (
            "typedef struct Blur { u32 color; s32 position[2]; } Blur;\n"
            "typedef struct Slot { s32 delay; Blur quad; } Slot;\n"
            "typedef struct Other { s32 position[2]; } Other;\n"
            "void f(Slot *slot, Other *other) {\n"
            "    Blur *b = &slot->quad;\n"
            "    b->color = slot->quad.position[0] + other->position[1];\n"
            "}\n")
        renames = {"Blur": {"color": "abgr", "position": "center"}}
        with mock.patch.object(raidou_c, "dds_member_renames", return_value=renames), \
                mock.patch.object(raidou_c, "dds_structs", return_value=({}, {})):
            out = raidou_c.dds_members(text)
        self.assertIn("b->abgr = slot->quad.center[0] + other->position[1];", out)
        self.assertIn("u32 color; s32 position[2]; } Blur;", out)


class DdsPinTests(unittest.TestCase):
    def git(self, root, *args):
        subprocess.run(["git", "-C", str(root), *args], check=True, capture_output=True)

    def checkout(self, root):
        self.git(root, "init", "-q")
        (root / "src").mkdir()
        (root / "src" / "a.c").write_text("int a;\n")
        self.git(root, "add", ".")
        self.git(root, "-c", "user.name=t", "-c", "user.email=t@t", "commit", "-qm", "a")
        return subprocess.run(["git", "-C", str(root), "rev-parse", "HEAD"], capture_output=True,
                              text=True, check=True).stdout.strip()

    def test_checkout_at_the_pin_passes(self) -> None:
        with tempfile.TemporaryDirectory() as d:
            head = self.checkout(Path(d))
            with mock.patch.object(raidou_c, "dds_pin", return_value=head):
                self.assertEqual(raidou_c.dds_checkout(Path(d)), Path(d).resolve())

    def test_other_revision_or_edited_source_is_refused(self) -> None:
        with tempfile.TemporaryDirectory() as d:
            head = self.checkout(Path(d))
            with mock.patch.object(raidou_c, "dds_pin", return_value="0" * 40), self.assertRaises(SystemExit):
                raidou_c.dds_checkout(Path(d))
            (Path(d) / "src" / "a.c").write_text("int b;\n")
            with mock.patch.object(raidou_c, "dds_pin", return_value=head), self.assertRaises(SystemExit):
                raidou_c.dds_checkout(Path(d))

    def test_pin_file_names_a_full_revision(self) -> None:
        self.assertRegex(raidou_c.dds_pin(), r"^[0-9a-f]{40}$")

    def test_pairs_missing_a_game_are_refused(self) -> None:
        rows = [{"version": "raidou1"}]
        with tempfile.TemporaryDirectory() as d, mock.patch.object(raidou_c, "OUT", Path(d)), \
                mock.patch.object(raidou_c, "read_pairs", return_value=rows), self.assertRaises(SystemExit):
            for name in ("pairs.tsv", "pairs_dds.tsv"):
                (Path(d) / name).write_text("")
            (Path(d) / "pairs_dds.rev").write_text(raidou_c.dds_pin() + "\n")
            raidou_c.ensure_pairs(None)


class AliasRowTests(unittest.TestCase):
    rows = {"carried": {"address": 0x100}, "paired": {"address": 0x200}, "back": {"address": 0x300}}

    def port(self, aliases, other):
        return sorted(raidou_c.port_alias_rows(aliases, self.rows, {"carried", "paired"}, [("back", 0x300)], other))

    def test_carried_data_drops_its_row(self) -> None:
        self.assertEqual(self.port([("carried", 0x100), ("plain", 0x400)], set()), [("back", 0x300), ("plain", 0x400)])

    def test_carried_data_the_other_game_aliases_keeps_its_row(self) -> None:
        self.assertEqual(self.port([("paired", 0x200)], {"paired"}), [("back", 0x300), ("paired", 0x200)])
        self.assertEqual(self.port([], {"paired"}), [("back", 0x300), ("paired", 0x200)])


class RaidouTypeTests(unittest.TestCase):
    def test_overridden_definition_takes_the_raidou_layout(self) -> None:
        own = {("tag", "Font"): "/* Raidou's */\nstruct Font {\n    s32 a;\n};"}
        with mock.patch.object(raidou_c, "raidou_types", return_value=own):
            self.assertEqual(raidou_c.raidou_layout("struct Font {\n    s32 a;\n    s32 data;\n};"), own[("tag", "Font")])
            for decl in ("typedef struct Font Font;", "struct Other {\n    s32 a;\n};", "extern struct Font gFont;"):
                self.assertEqual(raidou_c.raidou_layout(decl), decl)

    def test_frfont_data_has_eight_words(self) -> None:
        text = raidou_c.raidou_types()[("tag", "frFontData_t")]
        self.assertEqual(len(raidou_c.struct_members(raidou_c.struct_body(text, "tag", "frFontData_t")[1])), 8)


class SpellingTests(unittest.TestCase):
    names = {"D_00300000": "D_r1_00400000", "D_00300010": "D_r1_00400010", "D_00300020": "D_r1_00400010"}

    def spell(self, text):
        with mock.patch.object(raidou_c, "placeholder_names", return_value=self.names):
            return raidou_c.raidou_spelling(text)

    def test_alias_takes_the_raidou_name(self) -> None:
        self.assertEqual(self.spell("extern u32 D_00300000;\nu32 f(void) { return D_00300000; }\n"),
                         "extern u32 D_r1_00400000;\nu32 f(void) { return D_r1_00400000; }\n")

    def test_alias_stays_where_the_tu_names_the_symbol_or_two_aliases_meet(self) -> None:
        text = "extern u32 D_r1_00400000;\nextern f32 D_00300000;\nextern u8 D_00300010;\nextern s8 D_00300020;\n"
        self.assertEqual(self.spell(text), text)

    def test_carried_body_is_found_in_either_spelling(self) -> None:
        body = "void f(void) { D_00300000 = 1; }"
        text = "void f(void) { D_r1_00400000 = 1; }\n"
        with mock.patch.object(raidou_c, "placeholder_names", return_value=self.names), \
                mock.patch.object(raidou_c, "body_text", return_value=body):
            self.assertEqual(raidou_c.tu_bodies(None, text, {"f": "sdf/x"}), {"f": ("sdf/x", 0, len(text) - 1)})


if __name__ == "__main__":
    unittest.main()
