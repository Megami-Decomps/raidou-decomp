#!/usr/bin/env python3
"""Tests for tools/find_body.py: definitions, their guard status, names to search and the batches column."""
import pathlib
import sys
import tempfile
import unittest

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
import find_body  # noqa: E402

UNIT = """\
#include "common.h"

s32 proto(s32 a);

s32 matched(s32 a) {
    char c = '{';
    /* } */
    return a + c;
}

#ifdef NON_MATCHING
void draft(void) {
    proto(1);
}
#else
INCLUDE_ASM(const s32, "unit", draft);
#endif

#ifndef NON_MATCHING
INCLUDE_ASM(const s32, "unit", other);
#else
void other(void) {
}
#endif

#if 0
void off(void) {
}
#endif
"""


def at(text, head):
    return text.index(head)


class DefinitionTests(unittest.TestCase):
    def test_braces_in_literals_and_comments_do_not_count(self) -> None:
        start = at(UNIT, "s32 matched(")
        end = find_body.definition_at(UNIT, start)
        self.assertTrue(UNIT[start:end].endswith("return a + c;\n}\n"))

    def test_a_prototype_is_not_a_definition(self) -> None:
        self.assertIsNone(find_body.definition_at(UNIT, at(UNIT, "s32 proto(")))

    def test_guard_status(self) -> None:
        self.assertEqual(find_body.guard_status(UNIT, at(UNIT, "s32 matched(")), "matched")
        self.assertEqual(find_body.guard_status(UNIT, at(UNIT, "void draft(")), "nonmatching")
        self.assertEqual(find_body.guard_status(UNIT, at(UNIT, "void other(")), "nonmatching")
        self.assertEqual(find_body.guard_status(UNIT, at(UNIT, "void off(")), "disabled")

    def test_def_name_skips_attributes(self) -> None:
        self.assertEqual(find_body.def_name("static __attribute__((x)) s32 f(void) {"), "f")


class NameTests(unittest.TestCase):
    def names(self) -> find_body.Names:
        n = find_body.Names.__new__(find_body.Names)
        n.addr = {"rdName": 0x100}
        n.alias = {0x100: ["smtName", "rdName"]}
        n.hd = {0x100: "hdName"}
        return n

    def test_lookups_in_order_without_repeats(self) -> None:
        self.assertEqual(self.names().lookups("rdName"),
                         [("rdName", "name"), ("smtName", "alias"), ("hdName", "hd")])

    def test_an_unknown_name_is_searched_alone(self) -> None:
        self.assertEqual(self.names().lookups("nope"), [("nope", "name")])

    def test_rename_whole_words_only(self) -> None:
        body = "void smtName(void) {\n    smtName2();\n    smtName();\n}\n"
        self.assertEqual(find_body.renamed(body, "smtName", "rdName"),
                         "void rdName(void) {\n    smtName2();\n    rdName();\n}\n")

    def test_best_hit_is_matched_then_smt3_then_raidou_name(self) -> None:
        def hit(tree, via, status):
            return find_body.Hit(tree, "r", "p", 1, "f", via, status, "")
        hits = [hit("dds", "name", "matched"), hit("smt3", "name", "nonmatching"), hit("smt3", "hd", "matched")]
        self.assertEqual(sorted(hits, key=find_body.Hit.key)[0].label(), "smt3:matched:hd")


class ColumnTests(unittest.TestCase):
    def test_column_added_and_only_named_rows_set(self) -> None:
        with tempfile.TemporaryDirectory() as d:
            p = pathlib.Path(d) / "batches.tsv"
            p.write_text("batch\tname\taddr\nR07-a\tf\t1\nR08-b\tg\t2\n")
            find_body.set_column(p, {("R07-a", "f"): "smt3:matched:name"})
            self.assertEqual(p.read_text(), "batch\tname\taddr\tbody_source\n"
                             "R07-a\tf\t1\tsmt3:matched:name\nR08-b\tg\t2\t\n")
            find_body.set_column(p, {("R08-b", "g"): "none"})
            self.assertIn("R07-a\tf\t1\tsmt3:matched:name\nR08-b\tg\t2\tnone\n", p.read_text())

    def test_batch_by_prefix(self) -> None:
        with tempfile.TemporaryDirectory() as d:
            p = pathlib.Path(d) / "batches.tsv"
            p.write_text("batch\tname\taddr\nR07-a\tf\t1\nR08-b\tg\t2\n")
            self.assertEqual([r["name"] for r in find_body.batch_rows(p, "R07")], ["f"])


if __name__ == "__main__":
    unittest.main()
