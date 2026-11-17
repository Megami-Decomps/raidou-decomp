#!/usr/bin/env python3
"""Tests for tools/similar_index.py, the cached shape index find_similar.py and find_body.py read."""
import pathlib
import struct
import sys
import tempfile
import unittest
from unittest import mock

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[1]))
import find_similar as fs  # noqa: E402
import similar_index as si  # noqa: E402

ROOT = pathlib.Path(__file__).resolve().parents[2]
LW = 0x8C820010      # lw $v0, 0x10($a0)
LW32 = 0x8C820020    # lw $v0, 0x20($a0)
JAL = 0x0C080000     # jal 0x200000
BEQZ = 0x10800007    # beqz $a0, . + 0x20
JR = 0x03E00008      # jr $ra


def code(*words):
    return struct.pack(f"<{len(words)}I", *words)


def func(words, addr=0x100000, status="asm", n=None):
    return {"addr": addr, "n": n or len(words), "unit": "u", "path": "src/u.c", "line": 0, "status": status,
            "code": code(*words)}


class OperandTests(unittest.TestCase):
    def test_registers_and_immediates_are_separated(self) -> None:
        self.assertEqual(si.operands("$v0, 0x10($a0)"), (["$v0", "$a0"], [0x10]))

    def test_a_negative_immediate_keeps_its_sign(self) -> None:
        self.assertEqual(si.operands("$sp, $sp, -0x60"), (["$sp", "$sp"], [-0x60]))

    def test_a_zero_padded_number_is_decimal(self) -> None:
        self.assertEqual(si.operands("00001000"), ([], [1000]))


class TokenTests(unittest.TestCase):
    def test_the_four_tiers_drop_what_they_say_they_drop(self) -> None:
        toks, _ = si.tokens(code(LW), 0x1000)
        self.assertEqual(si.tiers(toks[0]), ("lw $v0,$a0 16", "lw $v0,$a0", "lw  16", "lw"))

    def test_a_call_target_is_wildcarded_and_recorded(self) -> None:
        a, ca = si.tokens(code(JAL), 0x100000)
        b, cb = si.tokens(code(0x0C084000), 0x100000)
        self.assertEqual(a, b)
        self.assertEqual((ca, cb), ([0x200000], [0x210000]))

    def test_a_branch_target_is_an_offset_so_two_copies_agree(self) -> None:
        a, _ = si.tokens(code(BEQZ), 0x100000)
        b, _ = si.tokens(code(BEQZ), 0x200000)
        self.assertEqual(a, b)
        self.assertEqual(a[0], "beqz $a0 32")


class RefreshTests(unittest.TestCase):
    """Refresh re-tokenises only changed bytes; a status change alone updates the entry."""

    def setUp(self) -> None:
        self.index = si.empty()
        self.calls = 0
        real = si.tokens

        def counting(*args):
            self.calls += 1
            return real(*args)
        patcher = mock.patch.object(si, "tokens", counting)
        patcher.start()
        self.addCleanup(patcher.stop)

    def update(self, funcs):
        return si.update_corpus(self.index, "raidou1", {"tree": "raidou", "repo": "r", "rev": "x"}, funcs)

    def test_unchanged_functions_are_not_tokenised_again(self) -> None:
        funcs = {"a": func([LW, JR, 0]), "b": func([JAL, 0, JR, 0], addr=0x100010)}
        self.assertEqual(self.update(funcs), (2, {"raidou1:a", "raidou1:b"}))
        self.calls = 0
        funcs["b"] = func([JAL, LW, JR, 0], addr=0x100010)
        self.assertEqual(self.update(funcs), (1, {"raidou1:b"}))
        self.assertEqual(self.calls, 1)

    def test_a_status_change_updates_the_entry_without_tokenising(self) -> None:
        self.update({"a": func([LW, JR, 0])})
        tok = self.index["corpora"]["raidou1"]["funcs"]["a"]["tok"]
        self.calls = 0
        got = self.update({"a": dict(func([LW, JR, 0]), status="matched", path="src/v.c")})
        self.assertEqual(got, (0, {"raidou1:a"}))
        entry = self.index["corpora"]["raidou1"]["funcs"]["a"]
        self.assertEqual((entry["status"], entry["path"], entry["tok"]), ("matched", "src/v.c", tok))

    def test_a_moved_boundary_is_tokenised_again(self) -> None:
        self.update({"a": func([LW, JR, 0])})
        self.calls = 0
        self.assertEqual(self.update({"a": func([LW, JR, 0, 0])})[0], 1)

    def test_a_removed_function_counts_as_changed(self) -> None:
        self.update({"a": func([LW, JR, 0]), "b": func([JR, 0], addr=0x100010)})
        self.assertEqual(self.update({"a": func([LW, JR, 0])}), (0, {"raidou1:b"}))


class NeighbourTests(unittest.TestCase):
    """Neighbour lists, the similar_to cell, and a round trip through the index file."""

    def setUp(self) -> None:
        body = [LW, LW32, JAL, 0, LW, LW32, BEQZ, 0, JR, 0]
        self.index = si.empty()
        hdr = {"tree": "raidou", "repo": str(ROOT), "rev": "x"}
        si.update_corpus(self.index, "raidou1", hdr, {"evtTarget": func(body)})
        si.update_corpus(self.index, "raidou2", hdr, {"evtOther": func(body, status="matched")})
        si.update_corpus(self.index, "smt3", dict(hdr, tree="smt3"),
                         {"fldThing": func(body[:-2] + [LW, JR, 0], status="matched")})

    def test_every_unmatched_raidou_function_gets_matched_neighbours(self) -> None:
        self.assertEqual(si.refresh_neighbours(self.index, set(), force=False), 1)
        near = self.index["neighbours"]["raidou1:evtTarget"]
        self.assertEqual(near[0], (1.0, "raidou2:evtOther"))
        self.assertEqual([k for _, k in near], ["raidou2:evtOther", "smt3:fldThing"])

    def test_an_untouched_list_is_not_recomputed(self) -> None:
        si.refresh_neighbours(self.index, set(), force=False)
        self.assertEqual(si.refresh_neighbours(self.index, set(), force=False), 0)
        self.assertEqual(si.refresh_neighbours(self.index, {"smt3:fldThing"}, force=False), 1)

    def test_a_newly_matched_function_is_merged_into_the_lists_it_reaches(self) -> None:
        si.refresh_neighbours(self.index, set(), force=False)
        hdr = {"tree": "raidou", "repo": str(ROOT), "rev": "x"}
        body = [LW, LW32, JAL, 0, LW, LW32, BEQZ, 0, JR, 0]
        _, changed = si.update_corpus(self.index, "raidou2", hdr, {"evtOther": func(body, status="matched"),
                                                                   "evtNew": func(body, 0x100100, "matched")})
        self.assertEqual(changed, {"raidou2:evtNew"})
        self.assertEqual(si.refresh_neighbours(self.index, changed, force=False), 1)
        keys = [k for _, k in self.index["neighbours"]["raidou1:evtTarget"]]
        self.assertEqual(sorted(keys[:2]), ["raidou2:evtNew", "raidou2:evtOther"])

    def test_the_column_lists_hits_at_the_floor(self) -> None:
        si.refresh_neighbours(self.index, set(), force=False)
        self.assertEqual(si.similar_to(self.index, "evtTarget", None, floor=1.0), "raidou2:evtOther:1.00")
        self.assertEqual(si.similar_to(self.index, "nothere", 0x100000), si.similar_to(self.index, "evtTarget", None))
        self.assertEqual(si.similar_to(self.index, "evtOther", None, version="raidou2"), "matched")

    def test_find_similar_reads_the_precomputed_list(self) -> None:
        si.refresh_neighbours(self.index, set(), force=False)
        with tempfile.TemporaryDirectory() as d:
            path = pathlib.Path(d) / "index.pickle"
            si.write(self.index, path)
            funcs = fs.load(path)
        t = funcs["raidou1:evtTarget"]
        got = fs.templates(funcs, fs.Index(funcs), t, top=1)
        self.assertEqual([(r, fs.key(c)) for r, c in got], [(1.0, "raidou2:evtOther")])
        self.assertEqual(t["seq"][3][:3], ["lw", "lw", "jal"])
        self.assertEqual(fs.differences(t, got[0][1]), [])


@unittest.skipUnless((ROOT / "asm" / "raidou1").exists(), "needs configure.py")
class InventoryTests(unittest.TestCase):
    def test_every_split_function_has_its_bytes(self) -> None:
        for version in si.VERSIONS:
            _, funcs = si.raidou_functions(version)
            short = [n for n, f in funcs.items() if len(f["code"]) != f["n"] * 4]
            self.assertEqual(short, [], version)


if __name__ == "__main__":
    unittest.main()
