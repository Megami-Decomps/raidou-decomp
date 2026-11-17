#!/usr/bin/env python3
"""Tests for tools/float_literal.py, the float literal choice for ee-gcc's truncating parse."""
import pathlib
import random
import sys
import unittest
from decimal import Decimal

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
import float_literal as fl  # noqa: E402


def get_truncated_bits(literal: str) -> int:
    """Bits of the largest float not above |literal|: ee-gcc's truncating parse."""
    d = abs(Decimal(literal))
    bits = fl.get_float_bits(float(d))
    if Decimal(fl.get_bits_float(bits)) > d:
        bits -= 1
    if literal.startswith("-"):
        bits |= 0x80000000
    return bits


class FloatLiteralTests(unittest.TestCase):
    def test_round_to_nearest_shortest_is_not_used_when_it_is_low(self) -> None:
        # 0.1f is 0.100000001490116...; "0.1" truncates to the float below it.
        self.assertNotEqual(get_truncated_bits("0.1"), 0x3DCCCCCD)
        lit = fl.get_float_literal(fl.get_bits_float(0x3DCCCCCD))
        self.assertEqual(lit, "0.100000002")
        self.assertEqual(get_truncated_bits(lit), 0x3DCCCCCD)

    def test_exact_values_stay_short(self) -> None:
        self.assertEqual(fl.get_float_literal(0.5), "0.5")
        self.assertEqual(fl.get_float_literal(-2.0), "-2.0")
        self.assertEqual(fl.get_float_literal(4096.0), "4096.0")

    def test_negative_values_keep_their_magnitude_digits(self) -> None:
        lit = fl.get_float_literal(fl.get_bits_float(0xBDCCCCCD))
        self.assertEqual(lit, "-0.100000002")
        self.assertEqual(get_truncated_bits(lit), 0xBDCCCCCD)

    def test_every_literal_gives_its_bits_under_truncation_and_nearest(self) -> None:
        rng = random.Random(1)
        for _ in range(3000):
            bits = rng.randrange(0x00800000, 0x7F000000) | rng.choice((0, 0x80000000))
            lit = fl.get_float_literal(fl.get_bits_float(bits))
            self.assertEqual(get_truncated_bits(lit), bits, lit)
            self.assertEqual(fl.get_float_bits(float(lit)), bits, lit)


if __name__ == "__main__":
    unittest.main()
