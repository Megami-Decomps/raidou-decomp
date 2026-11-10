#!/usr/bin/env python3
"""`splat split` that reads src/<unit>.c (a TU both games share) when
src/<version>/<unit>.c is missing.  Usage: tools/splat_split.py <yaml>"""
import sys
from pathlib import Path

from splat.__main__ import splat_main
from splat.segtypes.common.c import CommonSegC
from splat.util import options

_out_path = CommonSegC.out_path


def out_path(self):
    path = _out_path(self)
    src = Path(options.opts.src_path)
    shared = src.parent / path.relative_to(src)
    return shared if not path.exists() and shared.exists() else path


CommonSegC.out_path = out_path

if __name__ == "__main__":
    sys.argv = ["splat", "split", *sys.argv[1:]]
    splat_main()
