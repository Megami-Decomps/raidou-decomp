#!/usr/bin/env python3
"""Print smt3-decomp and dds-decomp bodies for Raidou functions, found by Raidou, SMT3 alias or HD name.

Usage: find_body.py NAME [NAME ...] | --batch R07 [--summary] [--write DIR] [--set-column]; see --help.
"""
from __future__ import annotations

import argparse
import re
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path

EPILOG = ("Trees are read at a git revision (default origin/main), so their working copies do not matter. "
          "A hit is matched, nonmatching (a NON_MATCHING draft in its tree) or disabled (#if 0).")
ROOT = Path(__file__).resolve().parents[1]
HOME = Path.home()
BATCHES = HOME / ".local/state/raidou-decomp-work/port/rest/batches.tsv"
SYMROW = re.compile(r"^\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+)\s*;")
HD_NOTE = re.compile(r"Raidou HD (\w+)\.(\w+)")
DEF_LINE = r"^[A-Za-z_][^;=]*\b[A-Za-z_]\w*[[:space:]]*\("
NOT_FUNCTIONS = {"if", "while", "for", "switch", "return", "sizeof", "__attribute__", "INCLUDE_ASM",
                 "INCLUDE_RODATA", "INCLUDE_SDATA", "typedef"}
# Lower sorts first: a matched body before a draft, smt3 before dds, the Raidou name first.
STATUS_RANK = {"matched": 0, "nonmatching": 1, "disabled": 2}
TREE_RANK = {"smt3": 0, "dds": 1}
VIA_RANK = {"name": 0, "alias": 1, "hd": 2}


@dataclass
class Hit:
    tree: str
    rev: str
    path: str
    line: int
    found: str
    via: str
    status: str
    body: str

    def key(self):
        return STATUS_RANK[self.status], TREE_RANK[self.tree], VIA_RANK[self.via], self.path

    def label(self):
        return f"{self.tree}:{self.status}:{self.via}"


def git(repo, *args):
    return subprocess.run(["git", "-C", str(repo), *args], capture_output=True, text=True, check=True).stdout


def skip_literal(t, i):
    q, i = t[i], i + 1
    while i < len(t) and t[i] != q:
        i += 2 if t[i] == "\\" else 1
    return i + 1


def definition_at(text, start):
    """End of the function definition whose head starts at offset start, or None for a prototype."""
    i, depth, opened = start, 0, False
    while i < len(text):
        c = text[i]
        if text.startswith("//", i):
            i = text.find("\n", i)
            i = len(text) if i < 0 else i
            continue
        if text.startswith("/*", i):
            i = text.index("*/", i) + 2
            continue
        if c in "\"'":
            i = skip_literal(text, i)
            continue
        if c == ";" and not opened and depth == 0:
            return None
        if c == "(":
            depth += 1
        elif c == ")":
            depth -= 1
        elif c == "{":
            depth += 1
            opened = True
        elif c == "}":
            depth -= 1
            if opened and depth == 0:
                end = i + 1
                return end + 1 if text[end:end + 1] == "\n" else end
        i += 1
    return None


def guard_status(text, start):
    """matched, nonmatching (inside #ifdef NON_MATCHING) or disabled (inside #if 0) at offset start."""
    stack = []
    for line in text[:start].splitlines():
        m = re.match(r"\s*#\s*(ifdef|ifndef|if|elif|else|endif)\b\s*(.*)", line)
        if not m:
            continue
        kind, cond = m.group(1), m.group(2).strip()
        if kind in ("ifdef", "ifndef", "if"):
            nm = cond == "NON_MATCHING" or re.fullmatch(r"defined\s*\(?\s*NON_MATCHING\s*\)?", cond)
            if nm:
                stack.append("nm" if kind != "ifndef" else "match")
            elif kind == "if" and cond == "0":
                stack.append("off")
            else:
                stack.append("other")
        elif kind in ("else", "elif") and stack:
            stack[-1] = {"nm": "match", "match": "nm", "off": "other"}.get(stack[-1], stack[-1])
        elif kind == "endif" and stack:
            stack.pop()
    if "off" in stack:
        return "disabled"
    return "nonmatching" if "nm" in stack else "matched"


def def_name(line):
    for m in re.finditer(r"([A-Za-z_]\w*)\s*\(", line):
        if m.group(1) not in NOT_FUNCTIONS:
            return m.group(1)
    return None


class Tree:
    """Function definitions in one decomp repository's src/ at a revision."""

    def __init__(self, name, repo, rev):
        self.name, self.repo = name, Path(repo)
        self.rev = git(self.repo, "rev-parse", "--short", rev).strip()
        self.files = {}
        self.index = {}
        out = git(self.repo, "grep", "-nE", DEF_LINE, self.rev, "--", "src/*.c", "src/*.h")
        for row in out.splitlines():
            _, path, line, text = row.split(":", 3)
            if text.rstrip().endswith(";") or text.startswith(("typedef", "INCLUDE_")):
                continue
            name = def_name(text)
            if name:
                self.index.setdefault(name, []).append((path, int(line)))

    def text(self, path):
        if path not in self.files:
            self.files[path] = git(self.repo, "show", f"{self.rev}:{path}")
        return self.files[path]

    def find(self, name, via):
        for path, line in self.index.get(name, ()):
            text = self.text(path)
            start = sum(len(s) for s in text.splitlines(True)[:line - 1])
            end = definition_at(text, start)
            if end is None:
                continue
            yield Hit(self.name, self.rev, path, line, name, via, guard_status(text, start), text[start:end])


def read_symbols(path):
    out = {}
    if path.exists():
        for line in path.read_text().splitlines():
            if m := SYMROW.match(line):
                out.setdefault(m.group(1), int(m.group(2), 16))
    return out


def hd_symbols(hd):
    """Raidou HD 'Class$$Method' -> the C symbol the reconstructed sources use."""
    out = {}
    path = Path(hd) / "symbols.txt"
    if path.exists():
        for line in path.read_text().splitlines():
            f = line.split()
            if len(f) >= 4 and not line.startswith("#") and "$$" in f[3]:
                cls, method = f[3].rsplit("$$", 1)
                out[(cls.rsplit(".", 1)[-1], method)] = f[1]
    return out


class Names:
    """The Raidou address of each name and the alias and HD names at each address, for one version."""

    def __init__(self, version, hd):
        cfg = ROOT / "config" / version
        self.addr = read_symbols(cfg / "symbol_addrs.txt")
        self.alias = {}
        for name, a in read_symbols(cfg / "smt3_symbols.txt").items():
            self.alias.setdefault(a, []).append(name)
        hdsym = hd_symbols(hd)
        self.hd = {}
        src = cfg / "name_sources.txt"
        for line in src.read_text().splitlines() if src.exists() else ():
            f = line.split("\t")
            if len(f) >= 4 and not line.startswith("#") and (m := HD_NOTE.search(f[3])):
                self.hd[int(f[0], 16)] = hdsym.get(m.groups(), m.group(2))

    def lookups(self, name, addr=None):
        """(name, via) to search for, Raidou name first, without repeats."""
        a = addr if addr is not None else self.addr.get(name)
        out = [(name, "name")]
        if a is not None:
            out += [(n, "alias") for n in self.alias.get(a, ())]
            if a in self.hd:
                out.append((self.hd[a], "hd"))
        seen, uniq = set(), []
        for n, via in out:
            if n not in seen:
                seen.add(n)
                uniq.append((n, via))
        return uniq


def find_bodies(trees, lookups):
    hits = [h for tree in trees for n, via in lookups for h in tree.find(n, via)]
    return sorted(hits, key=Hit.key)


def renamed(body, found, name):
    return body if found == name else re.sub(rf"\b{re.escape(found)}\b", name, body)


def batch_rows(path, batch):
    lines = path.read_text().splitlines()
    head = lines[0].split("\t")
    rows = [dict(zip(head, line.split("\t"))) for line in lines[1:] if line]
    want = [r for r in rows if r["batch"] == batch or r["batch"].split("-")[0] == batch]
    if not want:
        sys.exit(f"no batch {batch} in {path}")
    return want


def set_column(path, sources):
    """Write body_source for the rows named in sources; other rows keep theirs (or get an empty one)."""
    lines = path.read_text().splitlines()
    head = lines[0].split("\t")
    if "body_source" not in head:
        head.append("body_source")
    col, name_col = head.index("body_source"), head.index("name")
    out = ["\t".join(head)]
    for line in lines[1:]:
        if not line:
            continue
        f = line.split("\t")
        f += [""] * (len(head) - len(f))
        key = (f[0], f[name_col])
        if key in sources:
            f[col] = sources[key]
        out.append("\t".join(f))
    path.write_text("\n".join(out) + "\n")


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0], epilog=EPILOG)
    ap.add_argument("names", nargs="*")
    ap.add_argument("--batch", help="every function of this batch (name or Rnn prefix) in --batches")
    ap.add_argument("--batches", type=Path, default=BATCHES)
    ap.add_argument("--version", default="raidou1", choices=("raidou1", "raidou2"))
    ap.add_argument("--smt3", type=Path, default=HOME / "projects/smt3-decomp")
    ap.add_argument("--dds", type=Path, default=HOME / "projects/dds-decomp")
    ap.add_argument("--hd", type=Path, default=HOME / "projects/raidouhd-reconstructed-sources")
    ap.add_argument("--rev", default="origin/main", help="revision read in both trees")
    ap.add_argument("--fetch", action="store_true", help="git fetch origin in both trees first")
    ap.add_argument("--all", action="store_true", help="print every hit's body, not only the best")
    ap.add_argument("--summary", action="store_true", help="one tab-separated line per function, no bodies")
    ap.add_argument("--write", type=Path, metavar="DIR", help="write each best body to DIR/NAME.c")
    ap.add_argument("--set-column", action="store_true", help="write body_source into --batches (needs --batch)")
    a = ap.parse_args()
    if a.set_column and not a.batch:
        ap.error("--set-column needs --batch")

    todo = [(None, n, None) for n in a.names]
    if a.batch:
        todo += [(r["batch"], r["name"], int(r["addr"], 16)) for r in batch_rows(a.batches, a.batch)]
    if not todo:
        ap.error("no function names")
    if a.fetch:
        for repo in (a.smt3, a.dds):
            git(repo, "fetch", "-q", "origin")
    trees = [Tree("smt3", a.smt3, a.rev), Tree("dds", a.dds, a.rev)]
    names = Names(a.version, a.hd)

    sources, missing = {}, 0
    for batch, name, addr in todo:
        lookups = names.lookups(name, addr)
        hits = find_bodies(trees, lookups)
        best = hits[0].label() if hits else "none"
        if batch:
            sources[(batch, name)] = best
        if a.write and hits:
            h = hits[0]
            a.write.mkdir(parents=True, exist_ok=True)
            (a.write / f"{name}.c").write_text(f"/* body: {h.status} in {h.tree} @ {h.rev}: {h.path}:{h.line} */\n"
                                               + renamed(h.body, h.found, name))
        if a.summary:
            where = f"{hits[0].tree}@{hits[0].rev}:{hits[0].path}:{hits[0].line}" if hits else ""
            print(f"{name}\t{best}\t{where}")
            continue
        others = ", ".join(n for n, via in lookups[1:])
        print(f"== {name}" + (f" (also {others})" if others else ""))
        if not hits:
            print("-- no body\n")
            missing += 1
        for i, h in enumerate(hits):
            print(f"-- {h.status} in {h.tree} @ {h.rev}: {h.path}:{h.line}, as {h.found} ({h.via})")
            if i == 0 or a.all:
                print(renamed(h.body, h.found, name))
    if a.set_column:
        set_column(a.batches, sources)
    if a.batch:
        counts = {}
        for v in sources.values():
            k = v.rsplit(":", 1)[0] if v != "none" else v
            counts[k] = counts.get(k, 0) + 1
        print(f"# {a.batch}: " + ", ".join(f"{k} {n}" for k, n in sorted(counts.items())), file=sys.stderr)


if __name__ == "__main__":
    main()
