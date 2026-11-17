#!/usr/bin/env python3
"""Tests for tools/batch (land_batch.py's helpers) and tools/header_includers.py."""
import json
import os
import pathlib
import subprocess
import sys
import tempfile
import unittest

TOOLS = pathlib.Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))
sys.path.insert(0, str(TOOLS / "batch"))
import batch_files  # noqa: E402
import draft_store  # noqa: E402
import header_includers  # noqa: E402
import land_batch  # noqa: E402
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

    def test_the_copy_in_the_raidou1_block_is_spliced(self) -> None:
        text = f"#ifdef VERSION_RAIDOU1\nr1;\n#else\n{ASM}\n#endif\nx;\n#ifdef VERSION_RAIDOU1\n{ASM}\n#endif\n"
        want = f"#ifdef VERSION_RAIDOU1\nr1;\n#else\n{ASM}\n#endif\nx;\n#ifdef VERSION_RAIDOU1\nvoid f(void) {{}}\n#endif\n"
        self.assertEqual(unit_text.splice(text, "f", "void f(void) {}\n"), want)
        self.assertEqual(unit_text.splice(text, "f", "void f(void) {}\n", r1only=True), want)
        with self.assertRaises(ValueError):
            unit_text.splice(f"{ASM}\n{ASM}\n", "f", "void f(void) {}\n")

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

    def test_a_wrapped_draft_is_not_wrapped_again(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            d = pathlib.Path(tmp)
            row = batch_files.Row("f", "src/shared/u", words=28, result="nonmatch 2 words")
            b = batch_files.Batch(d, "R", {"f": row})
            (d / "f.c").write_text(f"#ifdef NON_MATCHING\nvoid f(void) {{}}\n#else\n{ASM}\n#endif\n")
            self.assertEqual(b.final("f"), unit_text.nonmatching_final("void f(void) {}\n", "u", "f", 28, 2, ""))
            (d / "f.c").write_text(f"/* NON_MATCHING: 1 of 9 words match. Why. */\n#ifdef NON_MATCHING\nvoid f(void) {{}}\n"
                                   f"#else\n{ASM}\n#endif\n")
            self.assertEqual(b.final("f").count("#ifdef NON_MATCHING"), 1)
            self.assertTrue(b.final("f").startswith("/* NON_MATCHING: 26 of 28 words match. Why. */\n"))

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


    def symbols(self, root: pathlib.Path, r1: str, r2: str) -> None:
        for game, names in (("raidou1", r1), ("raidou2", r2)):
            (root / "config" / game).mkdir(parents=True)
            (root / "config" / game / "symbol_addrs.txt").write_text(
                "".join(f"{n} = 0x{0x100 + 4 * i:X}; // type:func\n" for i, n in enumerate(names.split())))

    def test_a_raidou1_row_of_a_shared_name_gets_its_raidou2_row(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = pathlib.Path(tmp)
            self.symbols(root, "a b only1", "a b c")
            rows = [("raidou1", "a", "x", "HD"), ("raidou1", "only1", "y", "HD"), ("raidou1", "b", "z", "HD"),
                    ("raidou2", "b", "w", "HD")]
            out, added = renames.complete(root, rows)
            self.assertEqual(added, [("raidou2", "a", "x", "HD")])
            self.assertEqual(out, rows[:1] + added + rows[1:])

    def test_a_raidou1_row_onto_a_raidou2_name_is_refused(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = pathlib.Path(tmp)
            self.symbols(root, "a", "a x")
            with self.assertRaises(ValueError):
                renames.complete(root, [("raidou1", "a", "x", "HD")])

    def test_a_raidou1_only_function_is_refused(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = pathlib.Path(tmp)
            self.symbols(root, "a", "a")
            with self.assertRaises(ValueError):
                renames.complete(root, [("raidou1", "a", "x", "HD")], {"a"})
            self.assertEqual(len(renames.complete(root, [("raidou1", "a", "x", ""), ("raidou2", "a", "a2", "")],
                                                  {"a"})[0]), 2)

    def test_a_chain_is_completed_in_order(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = pathlib.Path(tmp)
            self.symbols(root, "a b", "a b")
            rows = [("raidou1", "a", "t", ""), ("raidou1", "b", "a", ""), ("raidou1", "t", "b", "")]
            out, added = renames.complete(root, rows)
            self.assertEqual([r[1:3] for r in added], [("a", "t"), ("b", "a"), ("t", "b")])
            self.assertEqual(len(out), 6)

    def test_sources_rename_reaches_untracked_headers(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = pathlib.Path(tmp)
            (root / "include").mkdir()
            (root / "include/old.h").write_text("void fldFunc17(void);\n")
            (root / "include/new.h").write_text("void fldFunc17(void);\n")
            git = lambda *a: subprocess.run(["git", *a], cwd=root, check=True, capture_output=True)
            git("init", "-q")
            git("add", "include/old.h")
            renames.rename_sources(root, "fldFunc17", "mapAutoMapInit")
            for name in ("old.h", "new.h"):
                self.assertEqual((root / "include" / name).read_text(), "void mapAutoMapInit(void);\n", name)


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


class RenamedRowTests(unittest.TestCase):
    def run_with(self, rows) -> land_batch.Run:
        run = land_batch.Run.__new__(land_batch.Run)
        run.maps = {g: renames.name_map(rows, g) for g in ("raidou1", "raidou2")}
        run.batch = batch_files.Batch(pathlib.Path("."), "R", {"old": batch_files.Row("old", "src/shared/u")})
        run.notes, run.lines = [], []
        return run

    def test_splice_finds_the_renamed_include_asm(self) -> None:
        run = self.run_with([("raidou1", "old", "new", ""), ("raidou2", "old", "new", "")])
        text = 'INCLUDE_ASM(const s32, "u", new);\n'
        out = run.splice_row("u.c", text, run.batch.rows["old"], "void new(void) {}\n")
        self.assertEqual((out, run.notes), ("void new(void) {}\n", []))

    def test_splice_reports_a_missing_line(self) -> None:
        run = self.run_with([])
        run.say = lambda line: None
        run.splice_row("u.c", "", run.batch.rows["old"], "")
        self.assertEqual(run.notes, ["u.c: old: 0 INCLUDE_ASM lines"])


class LateR1onlyTests(unittest.TestCase):
    def test_a_spliced_function_named_in_r1only_later_is_guarded(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = pathlib.Path(tmp)
            (root / "final").mkdir()
            (root / "final/old.c").write_text("void old(void) {}\n")
            (root / "src/shared").mkdir(parents=True)
            unit = root / "src/shared/u.c"
            unit.write_text("int a;\nvoid new(void) {}\nint b;\n")
            run = land_batch.Run.__new__(land_batch.Run)
            run.tree = root
            run.maps = {g: {"old": "new"} for g in ("raidou1", "raidou2")}
            run.draft_names = {}
            run.batch = batch_files.Batch(root, "R", {"old": batch_files.Row("old", "src/shared/u")}, r1only={"new"})
            run.notes, run.lines = [], []
            run.say = run.lines.append
            want = ('int a;\n#ifdef VERSION_RAIDOU1\nvoid new(void) {}\n#else\nINCLUDE_ASM(const s32, "u", new);\n'
                    "#endif\nint b;\n")
            run.r1only()
            self.assertEqual((unit.read_text(), run.notes), (want, []))
            run.r1only()
            self.assertEqual(unit.read_text(), want)

class BeforeCacheTests(unittest.TestCase):
    def run_in(self, root: pathlib.Path, work: str, base: str) -> land_batch.Run:
        run = land_batch.Run.__new__(land_batch.Run)
        run.tree, run.work, run.cache, run.base = root, root / work, root / "cache", base
        run.work.mkdir(exist_ok=True)
        return run

    def test_another_work_dir_reuses_the_base_measurement(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = pathlib.Path(tmp)
            (root / "config").mkdir()
            (root / "config/versions.json").write_text('{"raidou1": {}, "raidou2": {}}')
            first = self.run_in(root, "a", "b1")
            first.save_before({"u raidou1 default": {"f": "OK"}, "v raidou1 default": {"!error": "x"}})
            self.assertTrue((root / "cache/land_before/b1.raidou1+raidou2.json").exists())
            again = self.run_in(root, "b", "b1")
            self.assertEqual(again.load_before(), {"!base": "b1", "u raidou1 default": {"f": "OK"}})
            self.assertEqual(self.run_in(root, "c", "b2").load_before(), {})


class StaleDocsTests(unittest.TestCase):
    def test_drops_entries_of_functions_that_are_no_longer_drafts(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            tree = pathlib.Path(tmp)
            page = tree / "docs/matching/event/u.md"
            page.parent.mkdir(parents=True)
            page.write_text("# Draft notes: event/u\n\n## f (event/u, raidou1 0x8, raidou2 0x8)\n\nA.\n\n"
                            "## g (event/u, raidou1 0x8, raidou2 0x8)\n\nB.\n\n## h (event/u, raidou1 0x8)\n\nC.\n")
            stale = {("event/u", "f"): {"raidou1", "raidou2"}, ("event/u", "g"): {"raidou2"},
                     ("event/u", "h"): {"raidou1"}}
            old = matching_docs.stale_entries
            matching_docs.stale_entries = lambda _tree: stale
            try:
                dropped, left = matching_docs.drop_stale(tree)
            finally:
                matching_docs.stale_entries = old
            self.assertEqual(dropped, ["docs/matching/event/u.md: f", "docs/matching/event/u.md: h"])
            self.assertEqual(left, ["docs/matching/event/u.md: g is no longer a raidou2 draft only"])
            self.assertEqual(page.read_text(), "# Draft notes: event/u\n\n## g (event/u, raidou1 0x8, raidou2 0x8)\n\nB.\n")

    def test_not_a_draft_lines(self) -> None:
        out = "docs/matching/event/u.md: f is not a raidou1 draft; drop or fix the entry\n"
        self.assertEqual(matching_docs.NOT_A_DRAFT.findall(out), [("event/u", "f", "raidou1")])


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

    def test_a_jump_table_moves_the_start_to_a_16_byte_label(self) -> None:
        labels = [(0x447288, "s1"), (0x4472A0, "s2"), (0x4472C0, "jtbl_004472C0"), (0x4472E8, "s3")]
        users = {n: {"u"} for _, n in labels}
        p, note = segments.plan("r", "rodata", labels, users, "u")
        self.assertEqual((p.start, p.left, note), (0x4472A0, ["s1"], ""))
        p, _ = segments.plan("r", "rodata", labels[:2] + labels[3:], users, "u")
        self.assertEqual((p.start, p.left), (0x447288, []))
        p, _ = segments.plan("r", "lit4", labels, users, "u")
        self.assertEqual(p.start, 0x447288)

    def test_no_aligned_label_is_left_for_a_decision(self) -> None:
        labels = [(0x444A58, "s"), (0x444A68, "jtbl_00444A68")]
        p, note = segments.plan("r", "rodata", labels, {n: {"u"} for _, n in labels}, "u")
        self.assertIsNone(p)
        self.assertIn("section alignment", note)

    def test_c_literals_take_their_run_when_the_range_is_refused(self) -> None:
        """code_00323280: its C strings lead the chunk, its asm functions' labels lie past other units' labels."""
        labels = [(0x462880, "sMain"), (0x462898, "sDraw"), (0x4628B0, "cmpStrValuables"), (0x4628C0, "sAsm")]
        users = {"sMain": {"u"}, "sDraw": {"u"}, "cmpStrValuables": {"v"}, "sAsm": {"u"}}
        p, note = segments.plan("rodata_00462880", "rodata", labels, users, "u", {"sMain", "sDraw"})
        self.assertEqual((p.start, p.rest, p.c_only, note), (0x462880, 0x4628B0, True, ""))
        p, note = segments.plan("rodata_00462880", "rodata", labels, users, "u")
        self.assertIsNone(p)
        self.assertIn("inside its range", note)

    def test_c_literals_that_cannot_be_owned_name_the_address(self) -> None:
        labels = [(0x462880, "sMain"), (0x4628B0, "cmpStrValuables"), (0x4628C0, "sDraw")]
        users = {"sMain": {"u"}, "cmpStrValuables": {"v"}, "sDraw": {"u"}}
        p, note = segments.plan("r", "rodata", labels, users, "u", {"sMain", "sDraw"})
        self.assertIsNone(p)
        self.assertIn("u: rodata r", note)
        self.assertIn("its C uses sMain at 0x00462880", note)

    def test_plan_unit_reads_the_c_functions_labels(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = pathlib.Path(tmp)
            labels = [(0x462880, "sMain"), (0x4628B0, "sOther"), (0x4628C0, "sAsm")]
            files = {
                "config/raidou1/x.yaml": ("  - name: main\n    start: 0x1000\n    vram: 0x100000\n"
                                          "    subsegments:\n      - [0x363880, rodata, rodata_00462880]\n"
                                          "      - [0x3647B0, .rodata, k]\n"),
                "asm/raidou1/data/rodata_00462880.rodata.s": "".join(
                    f"dlabel {n}\n    /* {a - 0xFF000:06X} {a:08X} */ .asciz \"x\"\n" for a, n in labels),
                "asm/raidou1/u.s": ("nonmatching start, 0x8\nglabel start\n addiu $a0, %lo(sMain)\n"
                                    "nonmatching later, 0x8\nglabel later\n addiu $a0, %lo(sAsm)\n"),
                "asm/raidou1/other.s": "addiu $a0, %lo(sOther)\n",
            }
            for name, text in files.items():
                (root / name).parent.mkdir(parents=True, exist_ok=True)
                (root / name).write_text(text)
            (p,), notes = segments.plan_unit(root, "raidou1", "u", {"start"})
            self.assertEqual((p.start, p.rest, p.c_only, notes), (0x462880, 0x4628B0, True, []))
            plans, notes = segments.plan_unit(root, "raidou1", "u", {"start", "later"})
            self.assertEqual(plans, [])
            self.assertIn("its C uses sMain at 0x00462880", notes[0])

    def test_apply_splits_the_chunk_at_rom_offsets(self) -> None:
        yaml = ("  - name: main\n    start: 0x1000\n    vram: 0x100000\n    subsegments:\n"
                "      - [0x345680, rodata, rodata_00444680]\n      - [0x3456A0, .rodata, k]\n")
        out = segments.apply_plan(yaml, segments.Plan("u", "rodata", "rodata_00444680", 0x444690, 0x444698))
        self.assertIn("      - [0x345680, rodata, rodata_00444680]\n      - [0x345690, .rodata, u]\n"
                      "      - [0x345698, rodata, rodata_00444698]\n      - [0x3456A0, .rodata, k]\n", out)


    def test_two_units_in_one_chunk_keep_address_order(self) -> None:
        yaml = ("  - name: main\n    start: 0x1000\n    vram: 0x100000\n    subsegments:\n"
                "      - [0x345680, rodata, rodata_00444680]\n      - [0x3456A0, .rodata, k]\n")
        a = segments.Plan("a", "rodata", "rodata_00444680", 0x444680, 0x444688)
        b = segments.Plan("b", "rodata", "rodata_00444680", 0x444690, 0x444698)
        want = ("      - [0x345680, .rodata, a]\n      - [0x345688, rodata, rodata_00444688]\n"
                "      - [0x345690, .rodata, b]\n      - [0x345698, rodata, rodata_00444698]\n"
                "      - [0x3456A0, .rodata, k]\n")
        for first, second in ((a, b), (b, a)):
            out = segments.apply_plan(segments.apply_plan(yaml, first), second)
            self.assertTrue(out.endswith(want), out)
        c = segments.Plan("c", "rodata", "rodata_00444680", 0x444688, 0x444690)
        out = segments.apply_plan(segments.apply_plan(yaml, b), c)
        self.assertTrue(out.endswith("      - [0x345680, rodata, rodata_00444680]\n      - [0x345688, .rodata, c]\n"
                                     "      - [0x345690, .rodata, b]\n      - [0x345698, rodata, rodata_00444698]\n"
                                     "      - [0x3456A0, .rodata, k]\n"), out)
    def test_second_unit_in_one_run_finds_its_lit4(self) -> None:
        """The first plan splits the chunk; the second unit's piece has no file until splat reruns."""
        with tempfile.TemporaryDirectory() as tmp:
            root = pathlib.Path(tmp)
            labels = [(0x469C00, "fa"), (0x469C08, "fb"), (0x469C24, "fc"), (0x469C38, "fd")]
            files = {
                "config/raidou1/x.yaml": ("  - name: main\n    start: 0x1000\n    vram: 0x100000\n"
                                          "    subsegments:\n      - [0x36AC00, lit4, lit4_00469C00]\n"
                                          "      - [0x36AC40, .lit4, k]\n"),
                "asm/raidou1/data/lit4_00469C00.lit4.s": "".join(
                    f"dlabel {n}\n    /* {a - 0xFF000:06X} {a:08X} 00000000 */ .float 0\n" for a, n in labels),
                "asm/raidou1/u1.s": "lwc1 $f0, %lo(fa)($at)\n",
                "asm/raidou1/u2.s": "lwc1 $f0, %lo(fc)($at)\n",
                "asm/raidou1/other.s": "lwc1 $f0, %lo(fb)($at)\nlwc1 $f0, %lo(fd)($at)\n",
            }
            for name, text in files.items():
                (root / name).parent.mkdir(parents=True, exist_ok=True)
                (root / name).write_text(text)
            yaml = root / "config/raidou1/x.yaml"
            (p1,), notes = segments.plan_unit(root, "raidou1", "u1")
            self.assertEqual((p1.start, p1.rest, notes), (0x469C00, 0x469C08, []))
            yaml.write_text(segments.apply_plan(yaml.read_text(), p1))
            (p2,), notes = segments.plan_unit(root, "raidou1", "u2")
            self.assertEqual((p2.chunk, p2.start, p2.rest, notes), ("lit4_00469C08", 0x469C24, 0x469C38, []))
            yaml.write_text(segments.apply_plan(yaml.read_text(), p2))
            self.assertIn("      - [0x36AC24, .lit4, u2]\n      - [0x36AC38, lit4, lit4_00469C38]\n",
                          yaml.read_text())
            (root / "asm/raidou1/data/lit4_00469C00.lit4.s").unlink()
            _, notes = segments.plan_unit(root, "raidou1", "u2")
            self.assertTrue(any("no asm/data file holds its labels" in n for n in notes), notes)


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

    def test_fragile_worse_lines_are_information(self) -> None:
        worse = ["k: f OK -> DIFF 1/2", "k: g OK -> DIFF 3/9", "k: unit: RODATA x"]
        hard, info = header_includers.split_fragile(worse, {"f"})
        self.assertEqual(hard, ["k: g OK -> DIFF 3/9", "k: unit: RODATA x"])
        self.assertEqual(info, ["k: f OK -> DIFF 1/2"])

    def test_context_counts_against_an_ok(self) -> None:
        out = ("CONTEXT f: compiles differently inside the full unit (88 vs 84 bytes alone); ...\n"
               "       full unit vs retail: 2 of 21 words differ (relocated fields masked)\n"
               "OK   f @ 0x1 (84 bytes)\nRODATA the full unit's .rodata differs from retail at 0x10\n")
        s = header_includers.summarise(out)
        self.assertEqual(s["f"], "CONTEXT 2/21")
        worse, _ = header_includers.compare({"k": {"f": "OK"}}, {"k": s})
        self.assertEqual(worse, ["k: unit: RODATA the full unit's .rodata differs from retail at 0x10",
                                 "k: f OK -> CONTEXT 2/21"])

    def test_an_unbuilt_tree_is_refused(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = pathlib.Path(tmp)
            (root / "src/shared").mkdir(parents=True)
            (root / "src/shared/x.c").write_text("")
            units = {pathlib.Path("src/shared/x.c"): ["raidou1"]}
            with self.assertRaises(SystemExit) as cm:
                header_includers.measure(root, units)
            self.assertIn("is not built", str(cm.exception))
            self.assertIn("build/eeasm/asm/raidou1/nonmatchings", str(cm.exception))
            (root / "asm/raidou1/nonmatchings").mkdir(parents=True)
            self.assertEqual(header_includers.missing_build(root, {"raidou1"}),
                             ["build/eeasm/asm/raidou1/nonmatchings"])
            (root / "build/eeasm/asm/raidou1/nonmatchings").mkdir(parents=True)
            self.assertEqual(header_includers.missing_build(root, {"raidou1"}), [])


class DeclarationTests(unittest.TestCase):
    """land_batch refuses a unit whose .c gained a store declaration, a local prototype or a late #include."""

    def setUp(self) -> None:
        for k in ("AUTHOR", "COMMITTER"):
            os.environ.setdefault(f"GIT_{k}_NAME", "t")
            os.environ.setdefault(f"GIT_{k}_EMAIL", "t@t")
        self.tmp = tempfile.TemporaryDirectory()
        self.root = pathlib.Path(self.tmp.name)
        tree = self.root / "tree"
        (tree / "src/shared").mkdir(parents=True)
        self.unit = tree / "src/shared/u.c"
        self.unit.write_text('#include "common.h"\n\nvoid old(void);\n\nINCLUDE_ASM(const s32, "u", f);\n')
        subprocess.run(["git", "init", "-q", str(tree)], check=True)
        subprocess.run(["git", "-C", str(tree), "add", "-A"], check=True)
        subprocess.run(["git", "-C", str(tree), "commit", "-qm", "base"], check=True)
        store = self.root / "store"
        e = draft_store.Entry("f", 0x100, "raidou1", 2, "h", "fresh", "match", ["#include \"x.h\"", "extern s32 gX;"],
                              "C0001", "src/shared/u", "2026-11-16", "s32 f(void) {\n    return gX;\n}\n")
        c, meta = draft_store.get_paths(store, "raidou1", 0x100)
        c.parent.mkdir(parents=True)
        c.write_text(e.body)
        meta.write_text(json.dumps(e.meta()))
        run = land_batch.Run.__new__(land_batch.Run)
        run.tree, run.store, run.base = tree, store, "HEAD"
        run.maps = {g: {} for g in ("raidou1", "raidou2")}
        run.batch = batch_files.Batch(self.root, "S-u", {"f": batch_files.Row("f", "src/shared/u", addr="00000100")})
        run.notes, run.lines, run.gate_passed = [], [], None
        run.say = run.lines.append
        self.run_ = run

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def splice(self, final: str) -> list[str]:
        self.unit.write_text(unit_text.splice(self.unit.read_text(), "f", final))
        return self.run_.declaration_problems()

    def test_a_clean_draft_passes_and_old_prototypes_stay(self) -> None:
        self.assertEqual(self.splice("s32 f(void) {\n    return gX;\n}\n"), [])

    def test_store_declaration_prototype_and_include_are_refused(self) -> None:
        got = self.splice('#include "x.h"\nextern s32 gX;\ns32 g(void);\nvoid f(void);\nextern s32 other;\n'
                          "s32 f(void) {\n    return gX + g() + other;\n}\n")
        self.assertEqual(len(got), 3, got)
        self.assertIn("#include below", got[0])
        self.assertIn("draft store declaration left in the .c: extern s32 gX;", got[1])
        self.assertIn("local prototype: s32 g(void);", got[2])

    def test_includes_tsv_goes_after_the_last_include(self) -> None:
        (self.root / "includes.tsv").write_text("src/shared/u.c\tx.h\nsrc/shared/u.c\tcommon.h\n")
        self.run_.add_includes()
        self.assertTrue(self.unit.read_text().startswith('#include "common.h"\n#include "x.h"\n\nvoid old'))
        self.assertEqual(self.splice("s32 f(void) {\n    return gX;\n}\n"), [])

    def test_retire_needs_a_passing_gate(self) -> None:
        self.unit.write_text(unit_text.splice(self.unit.read_text(), "f", "s32 f(void) {\n}\n"))
        (self.root / "final").mkdir()
        (self.root / "final/f.c").write_text("s32 f(void) {\n}\n")
        self.run_.draft_names = {}
        self.run_.retire()
        self.assertIn("retire: skipped", self.run_.lines[-1])
        self.run_.gate_passed = True
        self.run_.retire()
        self.assertEqual(self.run_.lines[-1], "retire: 1 draft store entries landed: f")
        self.assertEqual(draft_store.load(self.run_.store, "raidou1"), {})
        landed = json.loads((self.run_.store / "raidou1/landed/00000100.json").read_text())
        self.assertEqual(landed["landed"]["batch"], "S-u")


if __name__ == "__main__":
    unittest.main()
