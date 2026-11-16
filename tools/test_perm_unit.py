#!/usr/bin/env python3
"""Tests for tools/perm_unit.py: what a candidate is spliced into and what is dumped."""

from __future__ import annotations

import sys
import unittest
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))

import perm_unit  # noqa: E402

UNIT = """\
#include "common.h"

extern s32 helper(s32);

static inline s32 clamp(s32 a) {
    return a < 0 ? 0 : a;
}

void func_001001D8(void) {
    helper(1);
}

INCLUDE_ASM(const s32, "code_001001D8", dds3SoftReset);

void func_00100200(void) {
    helper(2);
}
"""

DRAFT = """\
extern s32 g_flag;

void dds3SoftReset(void) {
    g_flag = clamp(helper(0));
}
"""


class ScanTests(unittest.TestCase):
    BASE = "s32 f(s32 *p) {\n    s32 a;\n\n    a = p[1];\n    return a + p[0];\n}\n"

    def cand(self, decl, use):
        return f"s32 f(s32 *p) {{\n    s32 a;\n    {decl}\n\n    a = p[1];\n    {use}\n    return a + p[0];\n}}\n"

    def test_a_constant_only_local_is_rejected(self) -> None:
        got = perm_unit.fake_locals(self.BASE, self.cand("s32 temp;", "temp = 0;"), "f")
        self.assertEqual(got, ["temp (constant only: 0)"])

    def test_a_copy_only_local_is_rejected(self) -> None:
        got = perm_unit.fake_locals(self.BASE, self.cand("s32 *q = p;", "(void)q;"), "f")
        self.assertEqual(got, ["q (copy only: p)"])

    def test_a_local_holding_an_expression_is_kept(self) -> None:
        self.assertEqual(perm_unit.fake_locals(self.BASE, self.cand("s32 b;", "b = p[2] + 1; a += b;"), "f"), [])

    def test_a_reordered_existing_local_is_kept(self) -> None:
        self.assertEqual(perm_unit.fake_locals(self.BASE, self.BASE.replace("a = p[1]", "a = 0"), "f"), [])


class BatchTests(unittest.TestCase):
    def test_nonmatches_under_a_distance(self) -> None:
        import tempfile
        with tempfile.TemporaryDirectory() as tmp:
            batch = Path(tmp) / "batches" / "R09-x"
            batch.mkdir(parents=True)
            (Path(tmp) / "batches.tsv").write_text("batch\tname\taddr\twords\tunit\n"
                                                   "R09-x\tf\t0\t9\tsrc/shared/u\nR09-x\tg\t0\t9\tsrc/shared/u\n"
                                                   "R09-x\th\t0\t9\tasm/raidou1/v\n")
            (batch / "results.tsv").write_text("name\twords\tresult\nf\t9\tnonmatch 3 words\n"
                                               "g\t9\tnonmatch 30 words\nh\t9\tnonmatch 2 words\n")
            got = perm_unit.batch_nonmatches(batch, 10)
            self.assertEqual([(u, f, d.name) for u, f, d in got],
                             [("src/shared/u.c", "f", "f.c"), ("src/raidou1/v.c", "h", "h.c")])


class SpliceTests(unittest.TestCase):
    def test_candidate_replaces_the_include_asm_line(self) -> None:
        template, full, include_asm = perm_unit.splice(UNIT, DRAFT, "dds3SoftReset")
        self.assertEqual(include_asm, 'INCLUDE_ASM(const s32, "code_001001D8", dds3SoftReset);')
        # The draft's declarations stay; only its definition becomes the hole.
        self.assertIn("extern s32 g_flag;\n\n@@PERM@@\n", template)
        self.assertNotIn("INCLUDE_ASM", template)
        self.assertNotIn("@@PERM@@", full)
        self.assertIn("void dds3SoftReset(void) {", full)
        # Neighbours keep their order around the spliced function.
        self.assertLess(full.index("func_001001D8"), full.index("dds3SoftReset"))
        self.assertLess(full.index("dds3SoftReset"), full.index("func_00100200"))

    def test_a_function_that_is_already_c_is_refused(self) -> None:
        with self.assertRaises(SystemExit):
            perm_unit.splice(UNIT, DRAFT, "func_00100200")

    def test_a_non_matching_block_is_replaced_whole(self) -> None:
        unit = ("void a(void) {\n}\n\n#ifdef NON_MATCHING\nvoid f(void) {\n    a();\n}\n#else\n"
                'INCLUDE_ASM(const s32, "u", f);\n#endif\n\nvoid b(void) {\n}\n')
        template, full, include_asm = perm_unit.splice(unit, None, "f")
        self.assertEqual(include_asm, 'INCLUDE_ASM(const s32, "u", f);')
        self.assertEqual(template, "void a(void) {\n}\n\n@@PERM@@\n\nvoid b(void) {\n}\n")
        self.assertNotIn("NON_MATCHING", full)
        self.assertIn("void f(void) {\n    a();\n}\n\nvoid b", full)
        # A draft file replaces the unit's own draft.
        _, full, _ = perm_unit.splice(unit, "void f(void) {\n    b();\n}\n", "f")
        self.assertIn("    b();", full)
        self.assertNotIn("    a();\n}\n\nvoid b", full)

    def test_no_draft_is_refused(self) -> None:
        with self.assertRaises(SystemExit):
            perm_unit.splice(UNIT, None, "dds3SoftReset")

    def test_unit_versions(self) -> None:
        self.assertEqual(perm_unit.unit_versions("src/shared/effect/billManager.c"), ("raidou1", "raidou2"))
        self.assertEqual(perm_unit.unit_versions("src/raidou2/code.c"), ("raidou2",))
        with self.assertRaises(SystemExit):
            perm_unit.unit_versions("include/common.h")

    def test_find_def_skips_a_prototype(self) -> None:
        text = "void f(void);\n\nvoid f(void) {\n    return;\n}\n"
        start, end = perm_unit.find_def(text, "f")
        self.assertEqual(text[start:end], "void f(void) {\n    return;\n}\n")


class PrototypeOnlyTests(unittest.TestCase):
    def test_unused_bodies_go_but_inline_helpers_and_callees_stay(self) -> None:
        template, full, _ = perm_unit.splice(UNIT, DRAFT, "dds3SoftReset")
        body = full[slice(*perm_unit.find_def(full, "dds3SoftReset"))]
        fast = perm_unit.prototypes_only(template, body)
        self.assertIn("void func_001001D8(void);", fast)
        self.assertIn("void func_00100200(void);", fast)
        self.assertNotIn("helper(1)", fast)
        # An inline helper, and anything the body calls, must keep its body.
        self.assertIn("return a < 0 ? 0 : a;", fast)
        self.assertIn("@@PERM@@", fast)


class HideAsmTests(unittest.TestCase):
    def test_inline_asm_becomes_a_b64literal_pragma(self) -> None:
        src = 'void f(void) {\n    __asm__ volatile ("qmtc2 $0, $vf1" : : "r" (0));\n}\n'
        out = perm_unit.hide_asm(src)
        self.assertNotIn("__asm__", out)
        self.assertIn("#pragma _permuter b64literal ", out)
        import base64
        blob = out.split("b64literal ")[1].split("\n")[0]
        self.assertIn("qmtc2 $0, $vf1", base64.b64decode(blob).decode())


class StripOtherCodeTests(unittest.TestCase):
    ASM = """\
\t.extern g_flag, 4
\t.ent other
other:
\tjr\t$31
\t.end other
\t.include "build/eeasm/asm/raidou1/nonmatchings/code_001001D8/func_00100200.s"
\t.include "build/eeasm/asm/raidou1/nonmatchings/code_001001D8/jtbl_00200000.s"
\t.ent target
target:
\tjr\t$31
\t.end target
"""

    def test_only_the_target_function_and_shared_data_survive(self) -> None:
        out = perm_unit.strip_other_code(self.ASM, "target")
        self.assertIn(".ent target", out)
        self.assertIn(".extern g_flag, 4", out)
        self.assertNotIn("other", out)
        self.assertNotIn("func_00100200.s", out)
        # Jump tables and data the function itself needs are still included.
        self.assertIn("jtbl_00200000.s", out)


class DumpGamesTests(unittest.TestCase):
    def test_other_games_append_and_candidate_side_files_go(self) -> None:
        import tempfile
        with tempfile.TemporaryDirectory() as d:
            cand, target = Path(d) / "c.o", Path(d) / "target.o"
            for p in (cand, target, Path(f"{cand}.raidou2"), Path(f"{target}.raidou2")):
                p.write_text("")
            with patch("perm_unit.dump", side_effect=lambda o: [o.name]):
                self.assertEqual(perm_unit.dump_games(cand), ["c.o", "# raidou2", "c.o.raidou2"])
                self.assertEqual(perm_unit.dump_games(target), ["target.o", "# raidou2", "target.o.raidou2"])
            self.assertFalse(Path(f"{cand}.raidou2").exists())
            self.assertTrue(Path(f"{target}.raidou2").exists())


class DumpTests(unittest.TestCase):
    OBJDUMP = """\

f.o:     file format elf32-tradlittlemips

Disassembly of section .text:

00000000 <target>:
   0:\t0c000000 \tjal\t0 <target>
\t\t\t0: R_MIPS_26\thelper
   4:\t3c020000 \tlui\t$2,0x0
\t\t\t4: R_MIPS_HI16\tg_flag
   8:\t8f838018 \tlw\t$3,-32744(gp)
   c:\t8fa40010 \tlw\t$4,16(sp)
"""

    def dump(self):
        with patch("perm_unit.subprocess.run") as run:
            run.return_value.stdout = self.OBJDUMP
            return perm_unit.dump(Path("f.o"))

    def test_relocations_are_folded_into_their_instruction(self) -> None:
        out = self.dump()
        self.assertIn("   0:\t0c000000 \tjal\thelper", out)
        self.assertIn("   4:\t3c020000 \tlui\t$2,<sym>", out)
        self.assertFalse([line for line in out if "R_MIPS" in line])

    def test_gp_offsets_normalize_but_stack_offsets_stay_visible(self) -> None:
        out = self.dump()
        self.assertIn("   8:\t8f838018 \tlw\t$3,<sym>(gp)", out)
        # SP is upper-cased so the scorer's stack wildcards cannot hide a
        # frame or local-offset difference.
        self.assertIn("   c:\t8fa40010 \tlw\t$4,16(SP)", out)


if __name__ == "__main__":
    unittest.main()
