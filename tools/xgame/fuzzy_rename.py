#!/usr/bin/env python3
"""Rename func_<addr> to its name in src/<v> for names appended by fuzzy_apply.py.

Usage: fuzzy_rename.py VERSION SINCE_NAME_SOURCES_LINE_COUNT
"""
import re
import subprocess
import sys
from pathlib import Path

v = sys.argv[1]
lines = Path(f"config/{v}/name_sources.txt").read_text().splitlines()[int(sys.argv[2]):]
ren = {f"func_{p[0][2:]}": p[1] for p in (l.split("\t") for l in lines) if len(p) > 3 and p[3].startswith("fuzzy name")}
pat = re.compile(r"\b(" + "|".join(ren) + r")\b") if ren else None
n = 0
for f in subprocess.check_output(["grep", "-rlE", "func_[0-9A-F]{8}", f"src/{v}", "include"], text=True).split():
    t = Path(f).read_text()
    u = pat.sub(lambda m: ren[m[1]], t)
    if u != t:
        Path(f).write_text(u)
        n += 1
print(len(ren), "names,", n, "files")
