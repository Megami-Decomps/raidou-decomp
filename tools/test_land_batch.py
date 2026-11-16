#!/usr/bin/env python3
"""Tests for tools/batch (land_batch.py's helpers) and tools/header_includers.py."""
import pathlib
import sys
import tempfile
import unittest

TOOLS = pathlib.Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))
sys.path.insert(0, str(TOOLS / "batch"))
import header_includers  # noqa: E402
import matching_docs  # noqa: E402
import renames  # noqa: E402
import segments  # noqa: E402
import unit_text  # noqa: E402

ASM = 'INCLUDE_ASM(const s32, "u", f);'


class SpliceTests(unittest.TestCase):
    def test_replaces_the_include_asm(self) -> None:
        text = f'#include "common.h"\n\n{ASM}\n\nINCLUDE_ASM(const s32, "u", g);\n'
        out = unit_text.splice(text, "f", "void f(void) {\n}\n")
        self.assertEqual(out, '#include "common.h"\n\nvoid f(void) {\n}\n\nINCLUDE_ASM(const s32, "u", g);\n')

    def test_r1only_keeps_the_asm_for_raidou2(self) -> None:
        out = unit_text.splice(ASM + "\n", "f", "void f(void) {}\n", r1only=True)
        self.assertEqual(out, f"#ifdef VERSION_RAIDOU1\nvoid f(void) {{}}\n#else\n{ASM}\n#endif\n")

    def test_an_old_draft_is_replaced_whole(self) -> None:
        old = (f"/* NON_MATCHING: 1 of 2 words match. x */\n#ifdef NON_MATCHING\n#ifdef VERSION_RAIDOU1\n#define A 1\n"
               f"#else\n#define A 2\n#endif\nvoid f(void) {{}}\n#else\n{ASM}\n#endif\n")
        out = unit_text.splice("int a;\n" + old + "int b;\n", "f", "void f(void) { A; }\n")
        self.assertEqual(out, "int a;\nvoid f(void) { A; }\nint b;\n")

    def test_version_guarded_asm_pair(self) -> None:
        text = f"#ifdef VERSION_RAIDOU1\n{ASM}\n#else\n{ASM}\n#endif\n"
        self.assertEqual(unit_text.splice(text, "f", "void f(void) {}\n", r1only=True),
                         f"#ifdef VERSION_RAIDOU1\nvoid f(void) {{}}\n#else\n{ASM}\n#endif\n")
        self.assertEqual(unit_text.splice(text, "f", "void f(void) {}\n"), "void f(void) {}\n")

    def test_a_missing_function_is_an_error(self) -> None:
        with self.assertRaises(ValueError):
            unit_text.splice(ASM, "g", "void g(void) {}")

    def test_gap_unit_keeps_address_order(self) -> None:
        out = unit_text.gap_unit('#include "common.h"\n', "u", ["f", "g"], {"g": "void g(void) {}\n"})
        self.assertEqual(out, f'#include "common.h"\n\n{ASM}\n\nvoid g(void) {{}}\n')

    def test_unwrap_round_trips(self) -> None:
        final = unit_text.nonmatching_final("void f(void) {}\n", "u", "f", 10, 3, "Why.")
        self.assertTrue(final.startswith("/* NON_MATCHING: 7 of 10 words match. Why. */\n#ifdef NON_MATCHING\n"))
        self.assertEqual(unit_text.unwrap(final), "void f(void) {}\n")
        self.assertIsNone(unit_text.unwrap("void f(void) {}\n"))

    def test_substitute_does_not_compound_chains(self) -> None:
        names = renames.name_map([("raidou1", "UserInit", "MainProc", ""), ("raidou1", "UserInit_dat", "UserInit", "")])
        self.assertEqual(unit_text.substitute("UserInit(); UserInit_dat();", names), "MainProc(); UserInit();")


class RenameTests(unittest.TestCase):
    def test_name_map_follows_a_chain(self) -> None:
        rows = [("raidou1", "a", "b", ""), ("raidou1", "b", "c", ""), ("raidou2", "x", "y", "")]
        self.assertEqual(renames.name_map(rows), {"a": "c", "b": "c"})

    def test_tier_from_reason(self) -> None:
        self.assertEqual(renames.tier("SMT3 body"), ("A", "body"))
        self.assertEqual(renames.tier("HD dat.c"), ("B", "role"))
        self.assertEqual(renames.tier("callee of x"), ("C", "derived-role"))


class DocsTests(unittest.TestCase):
    PAGE = "# Draft notes: u\n\n## f (u, raidou1 0x8)\n\nA.\n\n## g (u, raidou1 0x8)\n\nB.\n"

    def test_drop_keeps_the_other_entries(self) -> None:
        self.assertEqual(matching_docs.drop(self.PAGE, "f"), "# Draft notes: u\n\n## g (u, raidou1 0x8)\n\nB.\n")
        self.assertEqual(matching_docs.drop(self.PAGE, "g"), "# Draft notes: u\n\n## f (u, raidou1 0x8)\n\nA.\n")
        self.assertEqual(matching_docs.drop(self.PAGE, "h"), self.PAGE)

    def test_entry_heading_is_what_docs_index_reads(self) -> None:
        e = matching_docs.entry("f", "u", [("raidou1 0x8", "Raidou 1: 1 of 2 words differ, first at +0x4.")], "T.", "control")
        self.assertTrue(e.startswith("## f (u, raidou1 0x8)\n\nRaidou 1: 1 of 2"))
        self.assertIn("control-flow.md", e)


class RodataTests(unittest.TestCase):
    LABELS = [(0x1000, "a"), (0x1010, "b"), (0x1020, "c"), (0x1030, "d")]

    def test_unit_takes_its_labels_and_the_chunk_resumes(self) -> None:
        p, note = segments.plan("rodata_1000", "rodata", self.LABELS, {"a": {"x"}, "b": {"u"}, "c": {"u"}, "d": {"y"}}, "u")
        self.assertEqual((p.start, p.rest, note), (0x1010, 0x1030, ""))

    def test_shared_label_is_left_for_a_decision(self) -> None:
        p, note = segments.plan("r", "rodata", self.LABELS, {"b": {"u", "v"}}, "u")
        self.assertIsNone(p)
        self.assertIn("also used by v", note)

    def test_foreign_label_inside_the_range_is_left(self) -> None:
        p, note = segments.plan("r", "rodata", self.LABELS, {"a": {"u"}, "b": {"v"}, "c": {"u"}}, "u")
        self.assertIsNone(p)
        self.assertIn("inside its range", note)

    def test_apply_splits_the_chunk_at_rom_offsets(self) -> None:
        yaml = ("  - name: main\n    start: 0x1000\n    vram: 0x100000\n    subsegments:\n"
                "      - [0x345680, rodata, rodata_00444680]\n      - [0x3456A0, .rodata, k]\n")
        out = segments.apply_plan(yaml, segments.Plan("u", "rodata", "rodata_00444680", 0x444690, 0x444698))
        self.assertIn("      - [0x345680, rodata, rodata_00444680]\n      - [0x345690, .rodata, u]\n"
                      "      - [0x345698, rodata, rodata_00444698]\n      - [0x3456A0, .rodata, k]\n", out)


class IncluderTests(unittest.TestCase):
    def test_closure_through_headers_and_games(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = pathlib.Path(tmp)
            for name, text in {
                "config/versions.json": '{"raidou1": {}, "raidou2": {}}',
                "include/a.h": "", "include/b.h": '#include "a.h"\n', "include/c.h": "",
                "src/shared/x.c": '#include "b.h"\n', "src/shared/y.c": '#include "c.h"\n',
                "src/raidou1/z.c": '#ifdef X\n#include "a.h"\n#endif\n', "src/raidou2/x.c": "",
            }.items():
                (root / name).parent.mkdir(parents=True, exist_ok=True)
                (root / name).write_text(text)
            units = header_includers.includers(root, [pathlib.Path("include/a.h")])
            self.assertEqual({p.as_posix(): g for p, g in units.items()},
                             {"src/shared/x.c": ["raidou1"], "src/raidou1/z.c": ["raidou1"]})

    def test_summary_and_compare(self) -> None:
        out = ("OK   f @ 0x1 (8 bytes)\nDIFF g @ 0x2: 3 of 9 words differ (first at +0x4)\n"
               "src/x.c:3: error: bad\n")
        s = header_includers.summarise(out)
        self.assertEqual(s, {"f": "OK", "g": "DIFF 3/9", "!error": "error: bad"})
        worse, better = header_includers.compare({"k": {"f": "OK", "g": "DIFF 3/9"}},
                                                 {"k": {"f": "DIFF 1/2", "g": "DIFF 2/9"}})
        self.assertEqual(worse, ["k: f OK -> DIFF 1/2"])
        self.assertEqual(better, ["k: g DIFF 3/9 -> DIFF 2/9"])


if __name__ == "__main__":
    unittest.main()
