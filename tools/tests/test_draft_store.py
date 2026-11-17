#!/usr/bin/env python3
"""Tests for tools/draft_store.py (retire, plan, the store's git history), tools/batch/decls.py and place_decls.py."""
import json
import os
import pathlib
import subprocess
import sys
import tempfile
import unittest

TOOLS = pathlib.Path(__file__).resolve().parents[1]
sys.path.insert(0, str(TOOLS))
sys.path.insert(0, str(TOOLS / "batch"))
import decls  # noqa: E402
import draft_store  # noqa: E402
import place_decls  # noqa: E402

for k in ("AUTHOR", "COMMITTER"):
    os.environ.setdefault(f"GIT_{k}_NAME", "t")
    os.environ.setdefault(f"GIT_{k}_EMAIL", "t@t")


def entry(name, addr, unit="src/shared/u", status="match", cluster="C0001"):
    return draft_store.Entry(name, addr, "raidou1", 2, "h", "fresh", status, ["extern s32 gX;"], cluster, unit,
                             "2026-11-16", f"s32 {name}(void) {{\n    return gX;\n}}\n")


def put(store, e):
    c, meta = draft_store.get_paths(store, "raidou1", e.addr)
    c.parent.mkdir(parents=True, exist_ok=True)
    c.write_text(e.body)
    meta.write_text(json.dumps(e.meta()))


class DeclTests(unittest.TestCase):
    def test_kinds(self) -> None:
        got = [(d.kind, d.name) for d in decls.parse(
            '#include "a.h"\ntypedef struct T { s32 a; } T;\nstruct U;\nextern s32 gX;\nextern void (*fp)(s32);\n'
            "s32 f(void (*cb)(void), s32 n);\ns32 data = 1;\nstatic s32 g(void) {\n    return 0;\n}\n"
            'INCLUDE_ASM(const s32, "u", h);\n')]
        self.assertEqual(got, [("include", "a.h"), ("type", "T"), ("tag", "U"), ("extern", "gX"), ("extern", "fp"),
                               ("proto", "f"), ("data", "data"), ("def", "g"), ("other", "")])

    def test_a_definition_with_struct_parameters_ends_at_its_brace(self) -> None:
        got = [(d.kind, d.name, d.line) for d in decls.parse(
            '#include "a.h"\n\ns32 f(struct X *x)\n{\n    return 0;\n}\n\nINCLUDE_ASM(const s32, "u", g);\n'
            "struct Y { s32 a; } gY = { 1 };\n")]
        self.assertEqual(got, [("include", "a.h", 1), ("def", "f", 3), ("other", "", 8), ("data", "gY", 9)])

    def test_norm_ignores_parameter_names_and_extern(self) -> None:
        a, b = decls.parse_lines(["void f(s32 speed, char *name);", "void  f(s32 a0, char* p );"])
        self.assertEqual(a.norm(), b.norm())
        c, d = decls.parse_lines(["extern struct X *gX;", "struct X* gX;"])
        self.assertEqual(c.norm(), d.norm())
        self.assertNotEqual(*[x.norm() for x in decls.parse_lines(["void f(s32);", "void f(u32);"])])


class PlaceTests(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        root = pathlib.Path(self.tmp.name)
        self.tree, self.d = root / "tree", root / "batches" / "S-u"
        for p in ("include", "src/shared", "asm/raidou1/data"):
            (self.tree / p).mkdir(parents=True)
        self.d.mkdir(parents=True)
        (root / "batches.tsv").write_text("batch\tname\taddr\twords\tunit\nS-u\tf\t00000100\t2\tsrc/shared/u\n")
        (self.tree / "include/common.h").write_text("typedef signed int s32;\n")
        (self.tree / "include/nb.h").write_text('#ifndef NB_H\n#define NB_H\n\n#include "common.h"\n\n'
                                                "void nbFunc(s32 a);\n\n#endif\n")
        (self.tree / "include/t.h").write_text('#include "common.h"\ntypedef struct T { s32 a; } T;\n')
        (self.tree / "asm/raidou1/nbunit.s").write_text("glabel nbFunc\nendlabel nbFunc\nglabel newFunc\n")
        (self.tree / "asm/raidou1/data/t.data.s").write_text("dlabel gT\n")
        self.unit = self.tree / "src/shared/u.c"
        self.unit.write_text('#include "common.h"\n\nINCLUDE_ASM(const s32, "u", f);\n')

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def place(self, lines):
        (self.d / "f.decls").write_text("\n".join(lines) + "\n")
        sys.argv = ["place_decls.py", str(self.d), "--tree", str(self.tree)]
        rc = place_decls.main()
        rows = [r.split("\t") for r in (self.d / "store_decls.tsv").read_text().splitlines()[1:]]
        return rc, [(r[1], r[2]) for r in rows]

    def test_each_declaration_lands_in_a_header(self) -> None:
        rc, got = self.place(['#include "t.h"', "void nbFunc(s32 speed);", "void newFunc(s32 n);",
                              "T *otherFunc(void);", "typedef struct V { s32 b; } V;", "void nbFunc(s32 a, s32 b);",
                              "extern s32 gT;"])
        self.assertEqual(rc, 0)
        self.assertEqual(got, [("include", "t.h"), ("include", "nb.h"), ("placed", "nb.h"), ("placed", "u.h"),
                               ("placed", "u.h"), ("differs", "nb.h"), ("placed", "t.h")])
        self.assertIn("void newFunc(s32 n);\n\n#endif", (self.tree / "include/nb.h").read_text())
        own = (self.tree / "include/u.h").read_text()
        self.assertTrue(own.startswith('#ifndef U_H\n#define U_H\n\n#include "common.h"\n#include "t.h"\n'), own)
        self.assertIn("T *otherFunc(void);\ntypedef struct V { s32 b; } V;\n\n#endif", own)
        self.assertEqual(self.unit.read_text().split("\n\n")[0],
                         '#include "common.h"\n#include "t.h"\n#include "nb.h"\n#include "u.h"')
        self.assertEqual((self.d / "includes.tsv").read_text(),
                         "src/shared/u.c\tt.h\nsrc/shared/u.c\tnb.h\nsrc/shared/u.c\tu.h\n")
        self.assertNotIn("extern", self.unit.read_text())

    def test_unplaceable_declarations_are_required_edits(self) -> None:
        rc, got = self.place(['#include "missing.h"', "static void sf(void);"])
        self.assertEqual((rc, got), (1, [("required", "no header missing.h"),
                                         ("required", "static forward declaration")]))


class StoreTests(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        self.store = pathlib.Path(self.tmp.name) / "store"

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def git(self, *args) -> str:
        return subprocess.run(["git", "-C", str(self.store), *args], capture_output=True, text=True).stdout

    def test_retire_moves_the_entry_and_commits(self) -> None:
        put(self.store, entry("f", 0x100))
        put(self.store, entry("g", 0x200))
        draft_store.commit(self.store, "save f g")
        done = draft_store.retire(self.store, "raidou1", ["f", 0x200, "nope"], "S-u")
        self.assertEqual([e.name for e in done], ["f", "g"])
        self.assertEqual(draft_store.load(self.store, "raidou1"), {})
        self.assertTrue((self.store / "raidou1/landed/00000100.c").exists())
        self.assertEqual(self.git("log", "--format=%s").splitlines(), ["retire 2 landed in S-u: f g", "save f g"])
        self.assertEqual(self.git("status", "--porcelain"), "")

    def test_plan_groups_unbatched_entries_by_unit(self) -> None:
        es = {e.addr: e for e in (entry("f", 0x100), entry("g", 0x200), entry("h", 0x300, "asm/raidou1/gap"),
                                  entry("i", 0x400), entry("j", 0x500), entry("k", 0x600, status="nocompile"),
                                  entry("m", 0x700, status="words 3"))}
        rows, skipped = draft_store.plan_rows(es, {0x400}, {0x500: "j"}, "S")
        self.assertEqual([(r["batch"], r["name"], r["shared"]) for r in rows],
                         [("S-gap", "h", "R1"), ("S-u", "f", "R1/R2"), ("S-u", "g", "R1/R2"), ("S-u", "m", "R1/R2")])
        self.assertEqual(rows[1]["body_source"], "found:store:matched:C0001:")
        self.assertEqual(skipped, ["i\talready in batches.tsv", "j\talready C", "k\tdoes not compile"])
        root = pathlib.Path(self.tmp.name) / "batches"
        draft_store.write_batch_dirs(root, rows, es)
        self.assertEqual((root / "S-u/results.tsv").read_text(),
                         "name\twords\tresult\tminutes\nf\t2\tmatch\t0\ng\t2\tmatch\t0\nm\t2\tnonmatch 3\t0\n")
        self.assertEqual((root / "S-u/f.decls").read_text(), "extern s32 gX;\n")
        self.assertTrue((root / "S-u/f.c").read_text().startswith("s32 f(void)"))


if __name__ == "__main__":
    unittest.main()
