#!/usr/bin/env python3
"""Tests for tools/nm_behaviour.py: fact reduction on hand-made code, and on the built tree when present."""
import json
import os
import pathlib
import subprocess
import sys
import tempfile
import unittest

ROOT = pathlib.Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import nm_behaviour as nmb  # noqa: E402

SYM = 0x400010
FUNC = (0x1000, 0x1100)

# V5's drafts that did the wrong thing at 82c3c22, and the draft names it reviewed.
V5_TREE = "82c3c22"
V5_WRONG = """nbCallGetIntPara3 nbCallGetIntPara5 nbCallChipAllocDirect nbCallGetIntPara11 sbtlCallGetIntPara44
sbtlDeleteEffectData_SerialID2 sbtlForceDelete_ID sbtlForceDelete_SerialID sbtlDrawSprite_RGBA2 sbtlGetMotionNumSub
sbtlCallMemFreeDelayedDirect sbtlCallExistProcess2 nbLeaf69 sdfVec4Add Shutdown_cmpStatus ProcessStart_cmpStatus
ProcessEnd_cmpStatus cmpProcessStartEx cmpProcessEnd fclCallExistProcess Shutdown_nwdNews nwdProcessEnd
sbtlCheckAreaCount sbtlCallRegistScript2 sbtlCallGetIntPara31 sbtlCallGetIntPara35 sbtlCallGetIntPara36
sbtlCallGetIntPara3 sbtlCallGetIntPara4 dds3EffectFileListWindowCreate sbtlGetSkillNameFont""".split()
V5_CODEGEN = """SetChkskl cmpGetFlagCallDebug PartsCalc sbtlDraw3DSphere effBTL effSimpleBEDDelete
effSelectCameraFileJobFromRows effPCPShockWave02Init effPCPZiodin03Init""".split()


def make_image() -> nmb.Image:
    syms = [(SYM, "gSym", 8), (0x200000, "callee", 0), (0x200100, "other", 0)]
    return nmb.Image("raidou1", b"", [], 0x480000, (0x100000, 0x300000), (0x300000, 0x380000),
                     {"callee": 0x200000, "other": 0x200100}, {0x200000: "callee", 0x200100: "other"},
                     [0x200000, 0x200100], sorted(syms), {n: a for a, n, _ in syms}, {})


def insns(text: str, relocs: dict | None = None) -> list:
    """One instruction per line, 'op a, b'; relocs maps a line index to (type, symbol, addend)."""
    out = []
    for k, line in enumerate(x.strip() for x in text.strip().splitlines()):
        op, _, rest = line.partition(" ")
        out.append(nmb.Insn(FUNC[0] + 4 * k, op, [a.strip() for a in rest.split(",") if a.strip()],
                            (relocs or {}).get(k)))
    return out


class ReduceTests(unittest.TestCase):
    def setUp(self) -> None:
        self.img = make_image()
        self.res = nmb.Resolver(self.img)
        obj = nmb.ObjInfo({}, {}, {}, {"f": (0, 0x100)})
        self.draft_res = nmb.DraftResolver(self.img, obj, "unit")

    def retail(self, text: str) -> nmb.Facts:
        return nmb.reduce_function(insns(text), self.res, False, FUNC)

    def draft(self, text: str, relocs: dict) -> nmb.Facts:
        return nmb.reduce_function(insns(text, relocs), self.draft_res, True, (0, 0x100))

    def test_regalloc_and_relocation_give_the_same_facts(self) -> None:
        retail = self.retail("""
            lui v0,0x40
            lw v1,16(v0)
            addiu a0,zero,5
            sw a0,8(v1)
            jr ra
            nop""")
        draft = self.draft("""
            lui a1,0x0
            lw a2,0(a1)
            addiu t0,zero,5
            jr ra
            sw t0,8(a2)""", {0: ("R_MIPS_HI16", "gSym", 0), 1: ("R_MIPS_LO16", "gSym", 0)})
        self.assertEqual(nmb.diff_facts(retail, draft), {})
        self.assertIn(("gSym", 4, "L"), retail.c["mem"])

    def test_wrong_store_field_differs(self) -> None:
        d = nmb.diff_facts(self.retail("sw zero,8(a0)\njr ra\nnop"), self.retail("sw zero,12(a0)\njr ra\nnop"))
        self.assertEqual(set(d), {"mem"})

    def test_wrong_callee_differs(self) -> None:
        d = nmb.diff_facts(self.retail("jal 200000\nnop"), self.retail("jal 200100\nnop"))
        self.assertEqual(d["call"][0], {"callee": 1})
        self.assertEqual(d["call"][1], {"other": 1})

    def test_wrong_constant_differs(self) -> None:
        d = nmb.diff_facts(self.retail("addiu v0,zero,5\nsw v0,0(a0)"), self.retail("addiu v0,zero,6\nsw v0,0(a0)"))
        self.assertEqual(set(d), {"imm"})

    def test_sign_test_written_either_way_is_the_same(self) -> None:
        self.assertEqual(nmb.diff_facts(self.retail("slti v0,a0,0\njr ra\nnop"),
                                        self.retail("srl v0,a0,31\njr ra\nnop")), {})

    def test_likely_slot_only_reaches_the_taken_path(self) -> None:
        facts = self.retail("""
            beql a0,zero,1010
            addiu a1,zero,32
            sll v0,a1,2
            sw v0,0(a2)
            jr ra
            nop""")
        self.assertNotIn(128, facts.c["imm"])

    def test_alignment_test_of_an_address_is_not_a_constant(self) -> None:
        facts = self.retail("lui v1,0x40\naddiu v1,v1,16\nandi v0,v1,0x7\nsw v0,0(a0)")
        self.assertNotIn(7, facts.c["imm"])

    def test_inlined_helper_replaces_its_call(self) -> None:
        helper = self.retail("sw zero,4(a0)\njr ra\nnop")
        caller = self.retail("jal 200000\nnop")
        caller.c["call"] = nmb.Counter({"helper": 1})
        nmb.inline_helpers(caller, {"helper": helper})
        self.assertEqual(caller.c["call"], nmb.Counter())
        self.assertEqual(caller.c["mem"], nmb.Counter({("arg0+0x4", 4, "S"): 1}))


def built(version: str) -> pathlib.Path | None:
    serial = json.loads((ROOT / "config/versions.json").read_text())[version]["serial"]
    elf = ROOT / "build" / version / f"{serial}.elf"
    return elf if elf.exists() else None


VERSIONS = ("raidou1", "raidou2")
JOBS = os.cpu_count() or 4


@unittest.skipUnless(all(built(v) for v in VERSIONS), "needs the matching build of both versions")
class TreeTests(unittest.TestCase):
    def test_matched_functions_are_clean(self) -> None:
        for v in VERSIONS:
            results = nmb.run_all(ROOT, [v], [], None, None, JOBS, matched=True)
            self.assertGreater(len(results), 5000)
            self.assertEqual([r.error for r in results if r.error], [])
            self.assertEqual([(r.func, r.diffs) for r in results if not r.draft and r.diffs], [])

    def test_reviewed_drafts_are_clean(self) -> None:
        results = nmb.run_all(ROOT, list(VERSIONS), [], set(V5_WRONG + V5_CODEGEN), None, JOBS)
        self.assertIn("effSelectCameraFileJobFromRows", {r.func for r in results})
        self.assertEqual([(r.version, r.func, r.diffs) for r in results if r.diffs or r.error], [])

    def test_wrong_drafts_flag(self) -> None:
        if subprocess.run(["git", "cat-file", "-e", f"{V5_TREE}^{{commit}}"], cwd=ROOT,
                          capture_output=True).returncode:
            self.skipTest(f"{V5_TREE} is not in this clone")
        with tempfile.TemporaryDirectory() as tmp:
            old = pathlib.Path(tmp) / "old"
            subprocess.run(["git", "worktree", "add", "-q", "--detach", str(old), V5_TREE], cwd=ROOT, check=True)
            try:
                for d in ("bin", "compilers", "glibc32"):
                    (old / "tools" / d).symlink_to(ROOT / "tools" / d)
                flagged = set()
                for v in VERSIONS:
                    results = nmb.run_all(old, [v], [], set(V5_WRONG), built(v), JOBS)
                    flagged |= {r.func for r in results if r.diffs}
            finally:
                subprocess.run(["git", "worktree", "remove", "--force", str(old)], cwd=ROOT, check=True)
        self.assertEqual(sorted(set(V5_WRONG) - flagged), [])


if __name__ == "__main__":
    unittest.main()
