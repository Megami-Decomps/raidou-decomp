#!/usr/bin/env python3
"""Tests for tools/shape_clusters.py: exemplar choice and drafted names."""
import pathlib
import sys
import tempfile
import unittest

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[1]))
import shape_clusters  # noqa: E402


class GreedyTests(unittest.TestCase):
    def test_exemplar_covers_most_words_and_members_leave_the_pool(self) -> None:
        funcs = {k: {"n": n, "addr": i} for i, (k, n) in enumerate([("a", 10), ("b", 10), ("c", 50), ("d", 10)])}
        edges = {"a": {"b": 0.95, "c": 0.9}, "b": {"a": 0.95}, "c": {"a": 0.9}, "d": {}}
        got = shape_clusters.greedy(funcs, list(funcs), edges)
        self.assertEqual(got[0], ("a", [("b", 0.95), ("c", 0.9)]))
        self.assertEqual(got[1:], [("d", [])])

    def test_template_needs_the_floor(self) -> None:
        funcs = {"a": {"near": [(0.85, "smt3:x")]}, "b": {"near": [(0.93, "dds1:y")]}, "c": {}}
        self.assertEqual(shape_clusters.template(funcs, ["a", "b", "c"], 0.9), (0.93, "b", "dds1:y"))
        self.assertIsNone(shape_clusters.template(funcs, ["a", "c"], 0.9))


class DraftedTests(unittest.TestCase):
    def test_names_with_a_result_row(self) -> None:
        with tempfile.TemporaryDirectory() as d:
            p = pathlib.Path(d) / "R10-x"
            p.mkdir()
            (p / "results.tsv").write_text("name\twords\tresult\nf\t3\tmatch\ng\t4\tnonmatch 2\n")
            self.assertEqual(shape_clusters.drafted_names(pathlib.Path(d)), {"f": "R10-x", "g": "R10-x"})


if __name__ == "__main__":
    unittest.main()
