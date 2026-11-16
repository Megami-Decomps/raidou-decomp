#!/usr/bin/env python3
"""Print smt3-decomp, dds-decomp and raidou-decomp bodies for Raidou functions, by name, alias, HD name or shape.

Usage: find_body.py NAME [NAME ...] | --batch R07 [--summary] [--write DIR] [--set-column]; see --help.
"""
from __future__ import annotations

import argparse
import re
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path

import body_check
import draft_store
import find_similar

EPILOG = ("Trees are read at a git revision (default origin/main); identity uses each tree's build/ ELF. "
          "The raidou source reads the shape index (tools/similar_index.py; build it once with `similar_index.py "
          "refresh` and rerun that after matching): the other game's matched C for the function (via name) and "
          "matched functions of the same asm shape in any corpus (via similar, ratio 0.9 or better, precomputed "
          "for unmatched functions). "
          "A hit is matched, nonmatching (a NON_MATCHING draft in its tree) or disabled (#if 0). "
          "body_source is CLASS:TREE:STATUS:VIA:CONFIDENCE (a draft store hit has TREE store and "
          "its cluster as VIA: near:store:matched:C0014:1.00), CLASS one of match, near (usable), adapt, rework, "
          "other (reference only), found (not compiled), or none; then flags: size Nx (outside 0.5-2x, left out "
          "of the counts) and proto differs (parameter count or void return against include/; shown but counted, "
          "since on R08 it flagged 7 of the 11 bodies that worked: SMT3 reads a global Raidou passes in).")
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
TREE_RANK = {"raidou": 0, "smt3": 1, "dds": 2, "store": 3}
VIA_RANK = {"name": 0, "alias": 1, "hd": 2, "store": 3, "similar": 4}
# A same-shape sibling is a template, never the function itself; below this ratio it is a rewrite.
SIMILAR_FLOOR = 0.9
SIMILAR_TOP = 3


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
    version: str = ""
    decls: tuple = ()
    cluster: str = ""

    def key(self):
        # A sibling sorts after every hit that names the function itself. A draft-store entry, whatever its
        # status, sorts after matched definitions of the function and before everything else.
        status = 0 if self.via == "store" else STATUS_RANK[self.status]
        return (status, self.via == "similar", TREE_RANK[self.tree], VIA_RANK[self.via],
                STATUS_RANK[self.status], self.path)

    def label(self):
        # A draft store hit names its cluster where other hits name how they were found.
        via = (self.cluster or "store") if self.via == "store" else self.via
        return f"{self.tree}:{self.status}:{via}"

    def text(self):
        """The body with the declarations a store draft assumes, for a trial compile; a splice takes the body alone."""
        return "".join(d.rstrip("\n") + "\n" for d in self.decls) + self.body


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
    return guard_statuses(text[:start])[-1]


GUARD = re.compile(r"\s*#\s*(ifdef|ifndef|if|elif|else|endif)\b\s*(.*)")


def guard_statuses(text):
    """guard_status at the start of every line of text, and after its last line."""
    stack, out = [], []
    for line in text.splitlines():
        out.append("disabled" if "off" in stack else "nonmatching" if "nm" in stack else "matched")
        m = GUARD.match(line) if "#" in line else None
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
    out.append("disabled" if "off" in stack else "nonmatching" if "nm" in stack else "matched")
    return out


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


class RaidouTree:
    """Matched functions of the shape index: the other Raidou game's copy, and same-shape functions of any corpus."""

    name = "raidou"

    def __init__(self):
        self.repo = ROOT
        self.rev = git(ROOT, "rev-parse", "--short", "HEAD").strip()
        self.funcs = find_similar.load()
        self.index = find_similar.Index(self.funcs)

    def hit(self, f, via):
        got = find_similar.definition(f)
        if got is None:
            return None
        path, line, text = got
        if path.is_absolute():
            return Hit(self.name, self.rev, path.relative_to(ROOT).as_posix(), line, f["name"], via, "matched",
                       text, f["version"])
        return Hit(f["tree"], f["rev"][:9], path.as_posix(), line, f["name"], via, "matched", text, f["version"])

    def find(self, name, version):
        """The other game's matched copy, then same-shape matched functions of either game."""
        t = find_similar.lookup(self.funcs, name, version)
        if t is None or t["version"] != version:
            return []
        out = [self.hit(g, "name") for g in find_similar.same_name(self.funcs, t)]
        out += [self.hit(g, "similar") for r, g in find_similar.templates(self.funcs, self.index, t, SIMILAR_TOP)
                if r >= SIMILAR_FLOOR and g["name"] != name]
        return [h for h in out if h]


class StoreTree:
    """Drafts in the draft store (tools/draft_store.py), keyed by address and checked against the build."""

    name = "store"

    def __init__(self, store, version):
        self.store, self.version = Path(store), version
        self.entries = draft_store.load(self.store, version)
        self.by_name = {e.name: e for e in self.entries.values()}
        self.image = draft_store.load_image(version) if self.entries else None
        self.rejected = {}

    def find(self, name, addr, words):
        """The entry for the function at addr (else under name), when its address, size and bytes still agree."""
        e = self.entries.get(addr) if addr is not None else self.by_name.get(name)
        if e is None:
            return []
        why = draft_store.find_problem(e, self.image, words, name)
        if why:
            self.rejected[name] = why
            return []
        path = draft_store.get_paths(self.store, self.version, e.addr)[0]
        return [Hit(self.name, e.date, str(path), 1, e.name, "store", e.guard(), e.body, self.version,
                    tuple(e.decls), e.cluster)]


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


def set_column(path, sources, column="body_source"):
    """Write column for the rows named in sources; other rows keep theirs (or get an empty one)."""
    lines = path.read_text().splitlines()
    head = lines[0].split("\t")
    if column not in head:
        head.append(column)
    col, name_col = head.index(column), head.index("name")
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


class Checker:
    """Identity of each hit against the Raidou function, and an optional compile of the best one in its unit."""

    def __init__(self, names, trees, version, compile_bodies):
        self.names, self.version, self.compile = names, version, compile_bodies
        rd = ROOT / "build" / version / f"{VERSIONS_SERIAL[version]}.elf"
        self.raidou = body_check.Image(rd) if rd.exists() else None
        self.images = {t.name: body_check.tree_images(t.name, t.repo, t.rev) for t in trees}
        self.protos = body_check.prototypes(ROOT)
        self.alias_at = {}
        for a in set(names.alias) | set(names.hd):
            self.alias_at[a] = set(names.alias.get(a, ())) | ({names.hd[a]} if a in names.hd else set())

    def raidou_names(self, addr):
        out = set(self.raidou.names.get(addr, ())) if self.raidou else set()
        return out | self.alias_at.get(addr, set())

    def raidou_profile(self, name, addr, words):
        if self.raidou is None or addr is None:
            return None
        size = words * 4 if words else (self.raidou.size_of(name) if name in self.raidou.funcs else 0)
        if not size:
            return None
        return body_check.profile(self.raidou.code(addr, size), addr, self.raidou_names)

    def identity(self, mine, hit):
        if mine is None:
            return None
        images = self.images.get(hit.tree, {})
        # The hit's own version, else the version its path names, then the others.
        order = sorted(images, key=lambda v: (v != hit.version, f"/{v}/" not in f"/{hit.path}"))
        for v in order:
            theirs = images[v].profile(hit.found)
            if theirs is not None:
                return body_check.identity(mine, theirs)
        return None

    def flags(self, name, hit, ident):
        out = []
        if not body_check.size_ok(ident):
            out.append(f"size {body_check.size_ratio(ident):.1f}x")
        if body_check.proto_differs(self.protos.get(name), body_check.signature(hit.body, hit.found)):
            out.append("proto differs")
        return out

    def check(self, name, addr, words, unit, hits):
        """(class, best hit, identity, compile result, flags); class is one of CLASSES."""
        cls, hit, ident, tried = self.classify(name, addr, words, unit, hits)
        return cls, hit, ident, tried, (self.flags(name, hit, ident) if hit else [])

    def classify(self, name, addr, words, unit, hits):
        mine = self.raidou_profile(name, addr, words)
        scored = [(h, self.identity(mine, h)) for h in hits]
        # An alias or HD name is only a lead: its hit needs a size within 2x to be listed.
        scored = [(h, i) for h, i in scored if h.via == "name" or body_check.size_ok(i)]
        if not scored:
            return "none", None, None, None
        # A hit that is the same function beats a better ranked one that is not. A sibling's
        # callees differ by definition; its asm shape already passed find_similar's floor.
        same = [(h, i) for h, i in scored
                if i is None or h.via == "similar" or i.confidence >= body_check.SAME_FUNCTION]
        hit, ident = (same or scored)[0]
        if hit.via != "similar" and ident is not None and ident.confidence < body_check.SAME_FUNCTION:
            return "other", hit, ident, None
        if not self.compile or not unit:
            return "found", hit, ident, None
        tried = body_check.try_compile(ROOT, sys.executable, unit, name, renamed(hit.text(), hit.found, name),
                                       self.version)
        cls = {"ok": "match", "fail": "adapt" if tried.light() else "rework", "no-unit": "found",
               "no-asm-line": "found"}.get(tried.result, "near")
        return cls, hit, ident, tried


# body_source classes. usable: match, near (compiles in the unit as found). adapt: same function, fails only on
# its signature, callee arity or one global. Reference only: rework (same function, the unit lacks several of
# its names), other (a different function under the same name or alias). found: not compiled (no unit .c yet,
# or no --compile).
CLASSES = ("match", "near", "adapt", "rework", "other", "found", "none")
GROUPS = {"match": "usable", "near": "usable", "adapt": "adapt", "rework": "reference", "other": "reference",
          "found": "untried", "none": "none"}
VERSIONS_SERIAL = {"raidou1": "SLUS_214.31", "raidou2": "SLUS_218.45"}


def write_hit(folder, name, hit, head):
    """folder/NAME.c with the body; a store draft's declarations go to NAME.decls for tools/batch/place_decls.py,
    never into the .c."""
    (folder / f"{name}.c").write_text(head + renamed(hit.body, hit.found, name))
    decls = folder / f"{name}.decls"
    if hit.decls:
        decls.write_text("".join(d.rstrip("\n") + "\n" for d in hit.decls))
    elif decls.exists():
        decls.unlink()


def source_label(cls, hit, ident, flags=()):
    if hit is None:
        return "none"
    conf = f":{ident.confidence:.2f}" if ident else ":"
    return f"{cls}:{hit.label()}{conf}" + (f":{', '.join(flags)}" if flags else "")


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
    ap.add_argument("--compile", action="store_true",
                    help="splice each same-function body into its unit and run check_unit.py on it")
    ap.add_argument("--unit", help="unit for --compile with plain NAMEs (src/shared/...); --batch uses its column")
    ap.add_argument("--set-column", action="store_true", help="write body_source into --batches (needs --batch)")
    ap.add_argument("--no-raidou", action="store_true", help="leave out this repository's own matched functions")
    ap.add_argument("--store", type=Path, default=draft_store.STORE, help="draft store (tools/draft_store.py)")
    ap.add_argument("--no-store", action="store_true", help="leave out the draft store")
    a = ap.parse_args()
    if a.set_column and not a.batch:
        ap.error("--set-column needs --batch")

    todo = [(None, n, None, 0, a.unit) for n in a.names]
    if a.batch:
        todo += [(r["batch"], r["name"], int(r["addr"], 16), int(r.get("words") or 0), r.get("unit"))
                 for r in batch_rows(a.batches, a.batch)]
    if not todo:
        ap.error("no function names")
    if a.fetch:
        for repo in (a.smt3, a.dds):
            git(repo, "fetch", "-q", "origin")
    trees = [Tree("smt3", a.smt3, a.rev), Tree("dds", a.dds, a.rev)]
    own = None if a.no_raidou else RaidouTree()
    names = Names(a.version, a.hd)
    store = None if a.no_store else StoreTree(a.store, a.version)
    checker = Checker(names, trees, a.version, a.compile)
    if own:
        checker.images["raidou"] = {v: body_check.Image(p) for v, s in VERSIONS_SERIAL.items()
                                    if (p := ROOT / "build" / v / f"{s}.elf").exists()}

    sources, counts, errors, groups = {}, {}, {}, {}
    for batch, name, addr, words, unit in todo:
        lookups = names.lookups(name, addr)
        hits = find_bodies(trees, lookups)
        if own:
            hits = sorted(hits + own.find(name, a.version), key=Hit.key)
        addr = addr if addr is not None else names.addr.get(name)
        if store:
            hits = sorted(hits + store.find(name, addr, words), key=Hit.key)
        cls, best, ident, tried, flags = checker.check(name, addr, words, unit, hits)
        counts[cls] = counts.get(cls, 0) + 1
        # A hit off by more than 2x in size is left out of the usable and reference counts.
        group = "flagged" if any(f.startswith("size") for f in flags) else GROUPS[cls]
        groups[group] = groups.get(group, 0) + 1
        for cause, msgs in (tried.errors if tried else {}).items():
            errors.setdefault(cause, []).extend(f"{name}: {m}" for m in msgs)
        label = source_label(cls, best, ident, flags)
        if batch:
            sources[(batch, name)] = label
        if a.write and best:
            a.write.mkdir(parents=True, exist_ok=True)
            note = (f", same function {ident.text()}" if ident else "") + "".join(f", {f}" for f in flags)
            write_hit(a.write, name, best,
                      f"/* body: {best.status} in {best.tree} @ {best.rev}: {best.path}:{best.line}, {cls}{note} */\n")
        if a.summary:
            where = f"{best.tree}@{best.rev}:{best.path}:{best.line}" if best else ""
            extra = "\t".join(x for x in (ident.text() if ident else "", tried.text() if tried else "",
                                         ", ".join(flags)) if x)
            print(f"{name}\t{label}\t{where}" + (f"\t{extra}" if extra else ""))
            continue
        others = ", ".join(n for n, via in lookups[1:])
        print(f"== {name}" + (f" (also {others})" if others else ""))
        if store and name in store.rejected:
            print(f"-- draft store entry left out: {store.rejected[name]}")
        if not hits:
            print("-- no body\n")
        print(f"-- class {cls}" + (f", same function {ident.text()}" if ident else "")
              + (f", compile {tried.text()}" if tried else "") + "".join(f", {f}" for f in flags))
        for msgs in (tried.errors if tried else {}).values():
            for m in msgs[:5]:
                print(f"   {m}")
        for i, h in enumerate(hits):
            print(f"-- {h.status} in {h.tree} @ {h.rev}: {h.path}:{h.line}, as {h.found} ({h.via})")
            if h is best or a.all:
                print(renamed(h.text(), h.found, name))
    if a.set_column:
        set_column(a.batches, sources)
    print("# " + (f"{a.batch}: " if a.batch else "") + ", ".join(f"{c} {counts[c]}" for c in CLASSES if c in counts)
          + " (" + ", ".join(f"{g} {n}" for g, n in sorted(groups.items())) + ")", file=sys.stderr)
    if errors:
        print("# compile errors by cause: " + ", ".join(f"{c} {len(v)}" for c, v in
                                                       sorted(errors.items(), key=lambda kv: -len(kv[1]))),
              file=sys.stderr)


if __name__ == "__main__":
    main()
