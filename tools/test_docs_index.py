#!/usr/bin/env python3
"""Tests for tools/docs_index.py, the index of the NON_MATCHING draft notes."""
import contextlib
import io
import pathlib
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
import docs_index  # noqa: E402

ROOT = pathlib.Path(__file__).resolve().parent.parent


class DraftScanTests(unittest.TestCase):
    def test_fallback_name_is_the_draft(self) -> None:
        text = ("#ifdef NON_MATCHING\nvoid f(void) {\n#if 0\n#endif\n}\n#else\n"
                'INCLUDE_ASM(const s32, "unit", f);\n#endif\n'
                'INCLUDE_ASM(const s32, "unit", g);\n')
        self.assertEqual(docs_index.draft_functions(text), ["f"])

    def test_other_guards_are_not_drafts(self) -> None:
        text = '#ifdef FIX_BUGS\n#else\nINCLUDE_ASM(const s32, "unit", f);\n#endif\n'
        self.assertEqual(docs_index.draft_functions(text), [])

    def test_sibling_names_split(self) -> None:
        self.assertEqual(docs_index.entry_names("a, b and c"), ["a", "b", "c"])


class MatchingRowTests(unittest.TestCase):
    def test_every_entry_is_indexed(self) -> None:
        pages = [p for p in (ROOT / "docs/matching").rglob("*.md") if p.name != "README.md"]
        headings = sum(
            sum(1 for l in p.read_text().split("\n") if l.startswith("## ")) for p in pages)
        self.assertEqual(len(docs_index.matching_rows()), headings)

    def test_unit_and_line_point_at_the_entry(self) -> None:
        for fn, unit, _size, _d, page, line, _fragile in docs_index.matching_rows():
            text = (ROOT / page).read_text().split("\n")[int(line) - 1]
            self.assertTrue(text.startswith("## "), page)
            self.assertIn(fn, text)
            self.assertEqual(page, f"docs/matching/{unit}.md")

    def test_wrapped_distance_stays_in_one_tsv_cell(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            root = pathlib.Path(directory)
            drafts = root / "docs/matching"
            drafts.mkdir(parents=True)
            page = drafts / "example.md"
            for separator in ("\n", "\n\t"):
                with self.subTest(separator=separator):
                    page.write_text(
                        "## example (example, raidou1 0x28C)\n\n"
                        f"Raidou 1: 51 of 155 words{separator}differ; 32 bytes uncovered.\n")
                    with patch.object(docs_index, "ROOT", root), \
                            patch.object(docs_index, "MATCHING", drafts):
                        rows = docs_index.matching_rows()
                    self.assertEqual(len(rows), 1)
                    self.assertEqual(rows[0][3], "51 of 155 words differ")
                    serialized = "\t".join(rows[0])
                    self.assertEqual(len(serialized.splitlines()), 1)
                    self.assertEqual(len(serialized.split("\t")), 7)

    def test_fragile_line_is_carried_into_the_row(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            root = pathlib.Path(directory)
            drafts = root / "docs/matching"
            drafts.mkdir(parents=True)
            (drafts / "example.md").write_text(
                "## a (example, raidou1 0x28C)\n\nRaidou 1: 0 of 155 words differ.\n\n"
                "Fragile: matches only with the current declaration\n order in a.h.\n\n"
                "## b (example, raidou1 0x10)\n\nRaidou 1: 1 of 4 words differ.\n")
            with patch.object(docs_index, "ROOT", root), patch.object(docs_index, "MATCHING", drafts):
                rows = docs_index.matching_rows()
            self.assertEqual(rows[0][6], "matches only with the current declaration order in a.h.")
            self.assertEqual(rows[1][6], "")
            self.assertEqual(docs_index.fragile_names(rows), {"a"})

    def test_entries_carry_their_games(self) -> None:
        rows = [("a and b", "u", "raidou1 0x10, raidou2 0x14", "", "docs/matching/u.md", "3", "")]
        self.assertEqual(docs_index.entries(rows), {
            ("a", "u", "raidou1"), ("a", "u", "raidou2"),
            ("b", "u", "raidou1"), ("b", "u", "raidou2")})

    def test_entry_without_a_game_is_reported(self) -> None:
        rows = [("a", "u", "0x10", "", "docs/matching/u.md", "3", "")]
        self.assertEqual(len(docs_index.misplaced(rows)), 1)


class IndexFreshnessTests(unittest.TestCase):
    def test_a_draft_without_an_entry_fails_the_check(self) -> None:
        extra = {("no_such_draft", "no_such_unit", "raidou1")}
        with patch.object(docs_index, "drafts", lambda: extra | docs_index.entries(
                docs_index.matching_rows())), contextlib.redirect_stdout(io.StringIO()) as out:
            self.assertEqual(docs_index.main(["--check"]), 1)
        self.assertIn("no_such_draft has no entry", out.getvalue())

    def test_the_committed_index_is_current(self) -> None:
        """CI runs this; every draft needs an entry and the tables must be regenerated."""
        with contextlib.redirect_stdout(io.StringIO()) as out:
            self.assertEqual(docs_index.main(["--check"]), 0,
                             out.getvalue() + "run python3 tools/docs_index.py")


if __name__ == "__main__":
    unittest.main()
