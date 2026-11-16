#!/usr/bin/env python3
"""Tests for tools/find_body.py: definitions, their guard status, names to search and the batches column."""
import json
import pathlib
import sys
import tempfile
import unittest

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
import body_check  # noqa: E402
import draft_store  # noqa: E402
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

    def test_the_other_games_copy_leads_and_a_sibling_trails(self) -> None:
        def hit(tree, via):
            return find_body.Hit(tree, "r", "p", 1, "f", via, "matched", "")
        hits = [hit("raidou", "similar"), hit("dds", "hd"), hit("raidou", "name"), hit("smt3", "name")]
        got = [h.label() for h in sorted(hits, key=find_body.Hit.key)]
        self.assertEqual(got, ["raidou:matched:name", "smt3:matched:name", "dds:matched:hd",
                               "raidou:matched:similar"])

    def test_a_store_draft_trails_matched_names_and_leads_siblings(self) -> None:
        def hit(tree, via, status):
            return find_body.Hit(tree, "r", "p", 1, "f", via, status, "")
        hits = [hit("raidou", "similar", "matched"), hit("smt3", "name", "nonmatching"),
                hit("store", "store", "nonmatching"), hit("dds", "alias", "matched")]
        got = [h.label() for h in sorted(hits, key=find_body.Hit.key)]
        self.assertEqual(got, ["dds:matched:alias", "store:nonmatching:store", "raidou:matched:similar",
                               "smt3:nonmatching:name"])


class StoreTests(unittest.TestCase):
    def tree(self, d, body="s32 f(void) {\n    return 1;\n}\n", words=4):
        code = bytes(range(16))
        e = draft_store.Entry("f", 0x100, "raidou1", words, draft_store.hash_code(code[:words * 4]), "smt3:g",
                              "words 2", ["s32 g(void);"], "C0001", "", "2026-11-16", body)
        c, meta = draft_store.get_paths(pathlib.Path(d), "raidou1", 0x100)
        c.parent.mkdir(parents=True)
        c.write_text(body)
        meta.write_text(json.dumps(e.meta()))
        tree = find_body.StoreTree.__new__(find_body.StoreTree)
        tree.store, tree.version, tree.rejected = pathlib.Path(d), "raidou1", {}
        tree.entries = draft_store.load(pathlib.Path(d), "raidou1")
        tree.by_name = {x.name: x for x in tree.entries.values()}
        tree.image = FakeImage({0x100: {"f"}, 0x200: {"h"}}, {"f": (0x100, 16), "h": (0x200, 16)}, code)
        return tree

    def test_found_by_address_with_its_declarations(self) -> None:
        with tempfile.TemporaryDirectory() as d:
            hits = self.tree(d).find("f", 0x100, 4)
            self.assertEqual([h.label() for h in hits], ["store:nonmatching:C0001"])
            self.assertTrue(hits[0].body.startswith("s32 f(void) {"))
            self.assertTrue(hits[0].text().startswith("s32 g(void);\ns32 f(void) {"))

    def test_pick_up_is_marked_and_writes_declarations_apart(self) -> None:
        with tempfile.TemporaryDirectory() as d:
            hit = self.tree(d).find("f", 0x100, 4)[0]
            self.assertEqual(find_body.source_label("near", hit, None), "near:store:nonmatching:C0001:")
            out = pathlib.Path(d) / "out"
            out.mkdir()
            find_body.write_hit(out, "f", hit, "/* body */\n")
            self.assertEqual((out / "f.c").read_text(), "/* body */\ns32 f(void) {\n    return 1;\n}\n")
            self.assertEqual((out / "f.decls").read_text(), "s32 g(void);\n")

    def test_found_after_a_rename(self) -> None:
        with tempfile.TemporaryDirectory() as d:
            tree = self.tree(d)
            tree.image.names[0x100].add("fNew")
            self.assertEqual(len(tree.find("fNew", 0x100, 4)), 1)

    def test_wrong_address_size_bytes_or_definition_is_left_out(self) -> None:
        with tempfile.TemporaryDirectory() as d:
            tree = self.tree(d)
            self.assertEqual(tree.find("h", 0x100, 4), [])
            self.assertIn("not at", tree.rejected["h"])
            self.assertEqual(tree.find("f", 0x100, 5), [])
            self.assertIn("row has 5", tree.rejected["f"])
            self.assertEqual(tree.find("h", 0x200, 4), [])
        with tempfile.TemporaryDirectory() as d:
            tree = self.tree(d, words=3)
            self.assertEqual(tree.find("f", 0x100, 0), [])
            self.assertIn("size 4 words", tree.rejected["f"])
        with tempfile.TemporaryDirectory() as d:
            tree = self.tree(d)
            tree.image.data = bytes(16)
            self.assertEqual(tree.find("f", None, 0), [])
            self.assertIn("stale", tree.rejected["f"])
        with tempfile.TemporaryDirectory() as d:
            tree = self.tree(d, body="s32 g(void) {\n}\n")
            self.assertEqual(tree.find("f", 0x100, 4), [])
            self.assertIn("defines g", tree.rejected["f"])


class FakeImage:
    def __init__(self, names, funcs, data):
        self.names, self.funcs, self.data = names, funcs, data

    def size_of(self, name):
        return self.funcs[name][1]

    def code(self, addr, size):
        return self.data[:size] if addr == 0x100 else b""


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


class IdentityTests(unittest.TestCase):
    def test_same_code_and_calls_is_certain(self) -> None:
        p = body_check.Profile(["addiu", "jal", "nop", "jr"], [{"a", "smtA"}])
        q = body_check.Profile(["addiu", "jal", "nop", "jr"], [{"smtA"}])
        self.assertEqual(body_check.identity(p, q).confidence, 1.0)

    def test_other_calls_and_size_lower_it(self) -> None:
        p = body_check.Profile(["addiu", "jal", "nop", "jr"], [{"a"}])
        q = body_check.Profile(["lw", "lw", "jal", "sw", "sw", "jal", "nop", "jr"], [{"b"}, {"c"}])
        self.assertLess(body_check.identity(p, q).confidence, body_check.SAME_FUNCTION)

    def test_no_calls_on_either_side(self) -> None:
        self.assertEqual(body_check.call_score([], []), 1.0)
        self.assertEqual(body_check.call_score([{"a"}], []), 0.0)


class CompileTests(unittest.TestCase):
    ERRORS = ("u.c: In function `f':\n"
              "u.c:3: conflicting types for `f'\n"
              "include/h.h:9: previous declaration of `f'\n"
              "u.c:4: `viewerinfo' undeclared (first use in this function)\n"
              "u.c:4: (Each undeclared identifier is reported only once\n"
              "u.c:9: structure has no member named `effect'\n"
              "u.c:12: case label not within a switch statement\n"
              "u.c:13: too few arguments to function `g'\n"
              "u.c:14: warning: unused variable `x'\n")

    def test_errors_by_cause(self) -> None:
        self.assertEqual(body_check.classify_errors(self.ERRORS), {
            "signature": ["conflicting types for `f'"],
            "undeclared": ["`viewerinfo' undeclared (first use in this function)"],
            "member": ["structure has no member named `effect'"],
            "arity": ["too few arguments to function `g'"]})

    def test_light_adaptation(self) -> None:
        tried = body_check.Tried("fail", body_check.classify_errors(self.ERRORS))
        self.assertEqual(tried.missing(), {"viewerinfo", "effect"})
        self.assertFalse(tried.light())
        del tried.errors["member"]
        self.assertTrue(tried.light())

    def test_splice_replaces_the_include_asm(self) -> None:
        unit = 'a;\nINCLUDE_ASM(const s32, "u", f);\nb;\n'
        self.assertEqual(body_check.splice(unit, "f", "void f(void) {}\n"), "a;\nvoid f(void) {}\nb;\n")
        self.assertIsNone(body_check.splice(unit, "g", ""))

    def test_splice_takes_the_version_block(self) -> None:
        asm = 'INCLUDE_ASM(const s32, "u", f);\n'
        unit = f"#ifdef VERSION_RAIDOU1\nr1;\n#else\n{asm}#endif\n#ifdef VERSION_RAIDOU1\n{asm}#endif\n"
        body = "void f(void) {}\n"
        self.assertEqual(body_check.splice(unit, "f", body, "raidou1"),
                         f"#ifdef VERSION_RAIDOU1\nr1;\n#else\n{asm}#endif\n#ifdef VERSION_RAIDOU1\n{body}#endif\n")
        self.assertEqual(body_check.splice(unit, "f", body, "raidou2"),
                         f"#ifdef VERSION_RAIDOU1\nr1;\n#else\n{body}#endif\n#ifdef VERSION_RAIDOU1\n{asm}#endif\n")
        self.assertIsNone(body_check.splice("#ifdef VERSION_RAIDOU1\n" + asm + "#endif\n", "f", body, "raidou2"))

    def test_label(self) -> None:
        hit = find_body.Hit("smt3", "r", "p", 1, "f", "alias", "matched", "")
        ident = body_check.Identity((10, 12), 1.0, 0.9)
        self.assertEqual(find_body.source_label("near", hit, ident), "near:smt3:matched:alias:0.91")
        self.assertEqual(find_body.source_label("none", None, None), "none")
        self.assertEqual(find_body.source_label("other", hit, ident, ["size 0.3x", "proto differs"]),
                         "other:smt3:matched:alias:0.91:size 0.3x, proto differs")

    def test_size_and_prototype_flags(self) -> None:
        self.assertFalse(body_check.size_ok(body_check.Identity((100, 30), 0, 0)))
        self.assertTrue(body_check.size_ok(body_check.Identity((100, 150), 0, 0)))
        self.assertEqual(body_check.split_params("void (*cb)(s32, s32), s32 n"), ["void (*cb)(s32, s32)", "s32 n"])
        self.assertIsNone(body_check.split_params("char *fmt, ..."))
        sig = body_check.signature("s32 f(s32 a) {\n    return a;\n}\n", "f")
        self.assertEqual(sig, ("s32 ", "s32 a"))
        self.assertTrue(body_check.proto_differs(("s32 ", "s32 a, void *info"), sig))
        self.assertTrue(body_check.proto_differs(("void ", "s32 a"), sig))
        self.assertFalse(body_check.proto_differs(("extern s32 ", "s32"), sig))


if __name__ == "__main__":
    unittest.main()
