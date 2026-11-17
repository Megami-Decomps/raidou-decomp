#!/usr/bin/env python3
"""Print the C literal that gives a float its exact bits under ee-gcc.

ee-gcc truncates a decimal float literal toward zero, so the shortest
round-to-nearest decimal can land one ulp low. The literal printed is the
shortest decimal in [|f|, |f| + ulp/2), which gives the same bits under
truncation and under rounding to nearest.

    python3 tools/float_literal.py 0x3DCCCCCD 0.1 -2.5
"""
import argparse
import struct
from decimal import Decimal, localcontext


def get_float_bits(f: float) -> int:
    return struct.unpack("<I", struct.pack("<f", f))[0]


def get_bits_float(bits: int) -> float:
    return struct.unpack("<f", struct.pack("<I", bits))[0]


def get_float_literal(f: float) -> str:
    """Shortest decimal for the single-precision value f that ee-gcc reads back as f."""
    f = get_bits_float(get_float_bits(f))
    if f == int(f) and abs(f) < 1e8:
        return "%d.0" % f
    sign = "-" if f < 0 else ""
    bits = get_float_bits(abs(f))
    with localcontext() as ctx:
        ctx.prec = 60
        a = Decimal(abs(f))
        half_ulp = (Decimal(get_bits_float(bits + 1)) - a) / 2
        for digits in range(1, 20):
            q = Decimal(1).scaleb(a.adjusted() - digits + 1)
            d = (a / q).to_integral_value(rounding="ROUND_CEILING") * q
            if a <= d < a + half_ulp:
                return sign + format(d.normalize(), "f")
    return repr(f)


def parse_value(text: str) -> float:
    if text.lower().startswith("0x"):
        return get_bits_float(int(text, 16))
    return float(text)


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    p.add_argument("values", nargs="+", help="float bits as 0xHEX, or a decimal value")
    args = p.parse_args()
    for v in args.values:
        f = parse_value(v)
        print(f"0x{get_float_bits(f):08X}\t{get_float_literal(f)}f")


if __name__ == "__main__":
    main()
