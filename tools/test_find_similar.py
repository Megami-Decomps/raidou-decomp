#!/usr/bin/env python3
"""Tests for tools/find_similar.py, the matched-function template search over both games."""
import pathlib
import sys
import unittest

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
import find_similar as fs  # noqa: E402

ROOT = pathlib.Path(__file__).resolve().parent.parent


def seq(*words):
    """A function record carrying just the sequences the comparisons read."""
    return {"seq": [list(words), [], [], [w.split()[0] for w in words]],
            "n": len(words), "calls": [], "state": "asm", "name": "x", "unit": "u",
            "addr": 0, "version": "raidou1"}


class OperandTests(unittest.TestCase):
    def test_registers_and_immediates_are_separated(self) -> None:
        self.assertEqual(fs.operands("v0,0x10(a0)"), (["v0", "a0"], [0x10]))

    def test_float_and_vector_registers_count_as_registers(self) -> None:
        regs, imms = fs.operands("$f20,$vf10xyzw")
        self.assertEqual(regs, ["$f20", "$vf10xyzw"])
        self.assertEqual(imms, [])

    def test_a_negative_immediate_keeps_its_sign(self) -> None:
        self.assertEqual(fs.operands("sp,sp,-96"), (["sp", "sp"], [-96]))

    def test_the_symbol_objdump_appends_is_not_an_operand(self) -> None:
        self.assertEqual(fs.operands("0x285d60 <ChkAiHojo>"), ([], [0x285D60]))


class TokenTests(unittest.TestCase):
    """The tiers must coarsen in one direction and nothing else."""

    def test_the_four_tiers_drop_what_they_say_they_drop(self) -> None:
        got = fs.tokens(0x1000, "lw", "v0,0x10(a0)", 0x100000, 0x2E6E50)
        self.assertEqual(got, ("lw v0,a0 16", "lw v0,a0", "lw  16", "lw"))

    def test_a_call_target_is_wildcarded_at_every_tier(self) -> None:
        a = fs.tokens(0x100000, "jal", "0x200000 <foo>", 0x100000, 0x2E6E50)
        b = fs.tokens(0x100000, "jal", "0x210000 <bar>", 0x100000, 0x2E6E50)
        self.assertEqual(a, b)

    def test_a_branch_target_becomes_an_offset_so_two_copies_agree(self) -> None:
        a = fs.tokens(0x100000, "beqz", "a0,0x100020", 0x100000, 0x2E6E50)
        b = fs.tokens(0x200000, "beqz", "a0,0x200020", 0x100000, 0x2E6E50)
        self.assertEqual(a, b)
        self.assertIn("32", a[0])

    def test_a_target_outside_text_is_left_alone(self) -> None:
        """A %lo immediate that happens to sit last is data, not a branch."""
        got = fs.tokens(0x100000, "bltz", "a0,0x900000", 0x100000, 0x2E6E50)
        self.assertEqual(got[0], "bltz a0 9437184")


class DifferenceTests(unittest.TestCase):
    def test_equal_sequences_have_no_differences(self) -> None:
        a, b = seq("lw v0,a0 16", "jr ra "), seq("lw v0,a0 16", "jr ra ")
        self.assertEqual(fs.differences(a, b), [])

    def test_a_changed_immediate_is_reported_once(self) -> None:
        a, b = seq("lw v0,a0 16"), seq("lw v0,a0 32")
        got = fs.differences(a, b)
        self.assertEqual(len(got), 1)
        self.assertIn("16", got[0])
        self.assertIn("32", got[0])

    def test_a_differing_callee_is_reported(self) -> None:
        a, b = seq("jal  "), seq("jal  ")
        a["calls"], b["calls"] = ["ChkAiHojoMax"], ["ChkAiHojo"]
        got = fs.differences(a, b)
        self.assertEqual(len(got), 1)
        self.assertEqual(got[0].split(), ["call", "ChkAiHojoMax", "template", "ChkAiHojo"])

    def test_an_extra_instruction_shows_against_a_dash(self) -> None:
        a, b = seq("lw v0,a0 16", "nop "), seq("lw v0,a0 16")
        self.assertEqual(len(fs.differences(a, b)), 1)
        self.assertIn("template -", fs.differences(a, b)[0])


class FamilyTests(unittest.TestCase):
    def setUp(self) -> None:
        def f(name, state, version="raidou1"):
            return {"name": name, "state": state, "unit": "u", "n": 8, "addr": 0, "version": version}
        rows = (("sbtlCallBtlGetSkilltbl8", "c", "raidou1"), ("sbtlCallBtlGetSkilltbl16", "c", "raidou2"),
                ("sbtlCallBtlGetSkilltbl10", "asm", "raidou1"), ("effBTLModelHide", "c", "raidou1"),
                ("nbPnl_rsetTex1", "c", "raidou1"), ("func_00206308", "asm", "raidou1"))
        self.funcs = {f"{v}:{n}": f(n, s, v) for n, s, v in rows}

    def names(self, name):
        return sorted(g["name"] for g in fs.family(self.funcs, name, f"raidou1:{name}"))

    def test_numbered_wrappers_are_family_on_both_games(self) -> None:
        self.assertEqual(self.names("sbtlCallBtlGetSkilltbl10"),
                         ["sbtlCallBtlGetSkilltbl16", "sbtlCallBtlGetSkilltbl8"])

    def test_only_matched_functions_are_offered(self) -> None:
        self.funcs["raidou1:sbtlCallBtlGetSkilltbl8"]["state"] = "asm"
        self.assertEqual(self.names("sbtlCallBtlGetSkilltbl10"), ["sbtlCallBtlGetSkilltbl16"])

    def test_a_short_shared_prefix_is_not_family(self) -> None:
        self.assertNotIn("nbPnl_rsetTex1", self.names("effBTLModelLook2"))

    def test_a_splat_placeholder_name_has_no_family(self) -> None:
        self.assertEqual(fs.family(self.funcs, "func_00206308"), [])


class SameNameTests(unittest.TestCase):
    def test_the_other_games_matched_copy_is_offered(self) -> None:
        t = {"name": "evtFoo", "version": "raidou1", "state": "asm"}
        funcs = {"raidou2:evtFoo": {"name": "evtFoo", "version": "raidou2", "state": "c"}}
        self.assertEqual([g["version"] for g in fs.same_name(funcs, t)], ["raidou2"])
        funcs["raidou2:evtFoo"]["state"] = "asm"
        self.assertEqual(fs.same_name(funcs, t), [])


class ClusterTests(unittest.TestCase):
    def test_functions_sharing_a_tier_hash_group_together(self) -> None:
        funcs = {n: {"name": n, "hash": h, "state": "asm", "n": 8, "addr": a, "unit": "u",
                     "version": "raidou1"}
                 for n, h, a in (("a", ["1", "1", "1", "1"], 0),
                                 ("b", ["2", "1", "1", "1"], 4),
                                 ("c", ["3", "3", "3", "3"], 8))}
        self.assertEqual([[f["name"] for f in g] for g in fs.clusters(funcs, 0)], [])
        self.assertEqual([[f["name"] for f in g] for g in fs.clusters(funcs, 1)], [["a", "b"]])


@unittest.skipUnless((ROOT / "asm" / "raidou1").exists(), "needs configure.py")
class CorpusTests(unittest.TestCase):
    """The corpus covers the split asm exactly; objdump without -z would silently drop nops."""

    def test_every_function_of_every_unit_is_covered(self) -> None:
        for version in fs.VERSIONS:
            funcs = fs.corpus(version)
            for f in fs.inventory(version):
                got = funcs[f["name"]]
                self.assertEqual(len(got["seq"][0]), f["n"], f["name"])
                self.assertEqual(got["addr"], f["addr"], f["name"])


if __name__ == "__main__":
    unittest.main()
