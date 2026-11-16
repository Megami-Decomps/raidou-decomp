#!/usr/bin/env python3
"""Rank matched functions of Raidou 1/2, SMT3 and DDS 1/2 by asm shape similarity to a Raidou function.

Usage: find_similar.py --for NAME [--version raidou2] [--body] | [--unit DIR] | --clusters | --stats.
Read `sub` (instructions the template does not supply), not the ratio; below 0.9 it is a rewrite.
Tiers: exact (mnemonic, registers, immediates), noimm, noreg, mnem; callees compare by name.
"""
from __future__ import annotations

import argparse
import bisect
import collections
import difflib
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import similar_index  # noqa: E402
from units import VERSIONS, unit_sources  # noqa: E402
from version_guard import view  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
TIERS = similar_index.TIERS
EPILOG = ("Reads only the shape index, ~/.local/state/raidou-decomp-work/similar/index.pickle. Build it once with "
          "`python3 tools/similar_index.py refresh` and rerun that after matching: it re-tokenises only what "
          "changed and recomputes the precomputed neighbours of unmatched Raidou functions. Templates come from "
          "matched C in raidou1, raidou2, smt3, dds1 and dds2.")
# Below this an instruction sequence carries no shape: a prologue and a jr $ra.
MIN_INSNS = 8
# A template more than this much longer or shorter is not one worth reading.
SIZE_BAND = 0.15
# Sequence comparison is the expensive step; only the best of the cheap score run it.
RERANK = 40
# Shortest shared name prefix worth offering as a same-family candidate.
FAMILY_PREFIX = 8
UNNAMED = similar_index.UNNAMED
SHORT = {"raidou1": "R1", "raidou2": "R2", "smt3": "S3", "dds1": "D1", "dds2": "D2"}


class Context:
    """Per-load lookups shared by every record: tier strings of vocabulary ids and names by address."""

    def __init__(self, index: dict):
        self.index, self.vocab, self.cache, self.names = index, index["vocab"], {}, {}

    def tier(self, i: int) -> tuple[str, str, str, str]:
        got = self.cache.get(i)
        if got is None:
            got = self.cache[i] = similar_index.tiers(self.vocab[i])
        return got

    def name_at(self, corpus: str) -> dict[int, str]:
        if corpus not in self.names:
            funcs = self.index["corpora"][corpus]["funcs"]
            self.names[corpus] = {f["addr"]: n for n, f in funcs.items()}
        return self.names[corpus]


class Func(dict):
    """One index entry as a record; its four sequences and callee names are derived on first use."""

    def __init__(self, data: dict, entry: dict, ctx: Context):
        super().__init__(data)
        self.entry, self.ctx = entry, ctx

    def __missing__(self, k):
        if k == "seq":
            rows = [self.ctx.tier(i) for i in self.entry["tok"]]
            v = [[r[j] for r in rows] for j in range(len(TIERS))]
        elif k == "calls":
            names = self.ctx.name_at(self["version"])
            v = [names.get(a, hex(a)) for a in self.entry["calls"]]
        else:
            raise KeyError(k)
        self[k] = v
        return v


def records(index: dict) -> dict[str, Func]:
    """Every function of the index keyed CORPUS:NAME; an unmatched Raidou one carries its neighbours."""
    ctx, out = Context(index), {}
    for corpus, c in index["corpora"].items():
        for name, e in c["funcs"].items():
            out[f"{corpus}:{name}"] = Func(
                {"name": name, "version": corpus, "tree": c["tree"], "rev": c["rev"], "repo": c["repo"], "addr": e["addr"],
                 "n": e["n"], "unit": e["unit"], "path": e["path"], "line": e["line"], "hash": e["shape"],
                 "state": {"matched": "c"}.get(e["status"], e["status"])}, e, ctx)
    for k, near in index["neighbours"].items():
        if k in out:
            out[k]["near"] = near
    return out


def load(path: Path | None = None) -> dict[str, Func]:
    """Functions of every corpus keyed CORPUS:NAME, from the index only."""
    return records(similar_index.read(path or similar_index.INDEX))


def key(f: dict) -> str:
    return f"{f['version']}:{f['name']}"


def lookup(funcs: dict[str, dict], name: str, version: str | None) -> dict | None:
    """The Raidou target by name, on the given game first."""
    order = [version] if version else []
    order += [v for v in VERSIONS if v not in order]
    for v in order:
        if f"{v}:{name}" in funcs:
            return funcs[f"{v}:{name}"]
    return None


# --- similarity ------------------------------------------------------------------

class Index:
    """Nearest matched functions for a target, by instruction sequence, over every corpus."""

    def __init__(self, funcs: dict[str, dict], tier: int = 3):
        self.funcs, self.tier = funcs, tier
        self.pool: dict[str, int] = {}
        self.cache: dict[str, tuple] = {}
        self.by_size: dict[int, list[dict]] = collections.defaultdict(list)
        for f in funcs.values():
            if f["state"] == "c" and f["n"] >= MIN_INSNS:
                self.by_size[f["n"]].append(f)
        self.sizes = sorted(self.by_size)

    def shape(self, f: dict) -> tuple[tuple, collections.Counter, float]:
        """(token ids, bag, bag norm) at this tier, computed once per function."""
        k = key(f)
        got = self.cache.get(k)
        if got is None:
            tok = tuple(self.pool.setdefault(s, len(self.pool)) for s in f["seq"][self.tier])
            bag = collections.Counter(tok)
            got = self.cache[k] = (tok, bag, sum(v * v for v in bag.values()) ** 0.5 or 1.0)
        return got

    def ratio(self, a: dict, b: dict) -> float:
        return difflib.SequenceMatcher(None, self.shape(a)[0], self.shape(b)[0], autojunk=False).ratio()

    def nearest(self, target: dict, top: int = 3, state: str = "c") -> list[tuple[float, dict]]:
        """The closest functions in `state`, best first, as (ratio, function)."""
        if target["n"] < MIN_INSNS:
            return []
        lo, hi = target["n"] * (1 - SIZE_BAND), target["n"] * (1 + SIZE_BAND)
        if state == "c":
            pool = [f for s in self.sizes[bisect.bisect_left(self.sizes, lo):
                                          bisect.bisect_right(self.sizes, hi)]
                    for f in self.by_size[s]]
        else:
            pool = [f for f in self.funcs.values()
                    if f["state"] == state and f["version"] in VERSIONS and lo <= f["n"] <= hi
                    and f["n"] >= MIN_INSNS]
        _, bag, norm = self.shape(target)
        me, scored = key(target), []
        for c in pool:
            if key(c) == me:
                continue
            _, cbag, cnorm = self.shape(c)
            scored.append((sum(v * cbag[k] for k, v in bag.items() if k in cbag) / (norm * cnorm), c))
        scored.sort(key=lambda x: -x[0])
        reranked = [(self.ratio(target, c), c) for _, c in scored[:RERANK]]
        return sorted(reranked, key=lambda x: -x[0])[:top]


def families(funcs: dict[str, dict]) -> dict[str, list[dict]]:
    """Matched functions by their first FAMILY_PREFIX characters."""
    out = collections.defaultdict(list)
    for g in funcs.values():
        if g["state"] == "c" and len(g["name"]) >= FAMILY_PREFIX:
            out[g["name"][:FAMILY_PREFIX]].append(g)
    return out


def family(funcs: dict[str, dict], name: str, me: str | None = None,
           fam: dict[str, list[dict]] | None = None) -> list[dict]:
    """Matched functions sharing a long name prefix with this one, in any corpus.

    A size band hides the variant that grew; a shared prefix does not care about
    size. Numbered wrappers (sbtlCallBtlGetSkilltbl8/10/16) come out here. It is only
    a candidate generator: rank what comes back by sequence ratio.
    """
    if len(name) < FAMILY_PREFIX or name.startswith(UNNAMED):
        return []
    fam = fam if fam is not None else families(funcs)
    return [g for g in fam.get(name[:FAMILY_PREFIX], ()) if key(g) != me]


def same_name(funcs: dict[str, dict], t: dict) -> list[dict]:
    """The other game's matched copy of the target, whatever its size."""
    return [g for v in VERSIONS if v != t["version"]
            if (g := funcs.get(f"{v}:{t['name']}")) and g["state"] == "c"]


def templates(funcs: dict[str, dict], index: Index, t: dict, top: int = 5,
              fam: dict[str, list[dict]] | None = None, cached: bool = True) -> list[tuple[float, dict]]:
    """Matched candidates for t, best ratio first: size-banded neighbours, name family, other game.

    An unmatched Raidou function's list is precomputed in the index at the mnem tier.
    """
    if cached and "near" in t and index.tier == 3 and top <= similar_index.TOP:
        return [(r, funcs[k]) for r, k in t["near"] if k in funcs][:top]
    seen, out = {key(t)}, []
    # The band is widened by the duplicates the per-tree pass below removes (dds1 and dds2 share source).
    cands = [c for _, c in index.nearest(t, top=2 * top)] + family(funcs, t["name"], key(t), fam) \
        + same_name(funcs, t)
    for c in cands:
        if key(c) not in seen:
            seen.add(key(c))
            out.append((index.ratio(t, c), c))
    out.sort(key=lambda x: -x[0])
    return distinct(out)[:top]


def distinct(scored: list[tuple[float, dict]]) -> list[tuple[float, dict]]:
    """The first of each (tree, name): one game's copy of a shared function stands for the others."""
    seen, out = set(), []
    for r, c in scored:
        if (c["tree"], c["name"]) not in seen:
            seen.add((c["tree"], c["name"]))
            out.append((r, c))
    return out


def clusters(funcs: dict[str, dict], tier: int = 3) -> list[list[dict]]:
    """Groups of two or more functions with the same normalised sequence."""
    by = collections.defaultdict(list)
    for f in funcs.values():
        by[f["hash"][tier]].append(f)
    out = [sorted(v, key=lambda f: (f["version"], f["addr"])) for v in by.values() if len(v) > 1]
    return sorted(out, key=lambda v: -sum(f["n"] * 4 for f in v if f["state"] == "asm"))


def braced(text: str, name: str, start: int = 0) -> tuple[int, str] | None:
    """(offset, text) of the first definition of name at or after start."""
    m = re.compile(rf"^[A-Za-z_][^;#\n]*\b{re.escape(name)}\s*\([^;{{]*\)\s*\{{", re.M).search(text, start)
    if m is None:
        return None
    i, depth = m.end() - 1, 0
    while i < len(text):
        if text[i] == "{":
            depth += 1
        elif text[i] == "}":
            depth -= 1
            if depth == 0:
                break
        i += 1
    return m.start(), text[m.start():i + 1]


def definition(f: dict) -> tuple[Path, int, str] | None:
    """(source path, line, text) of a matched function's C definition.

    A Raidou function is read from this working copy as its game compiles it; an smt3 or dds one from its
    tree at the revision the index was refreshed from, with the path relative to that tree.
    """
    if f is None or f["state"] != "c":
        return None
    if f["version"] in VERSIONS:
        path = unit_sources(f["version"]).get(f["unit"])
        if path is None:
            return None
        text = view(path.read_text(errors="replace"), f["version"])
        start = 0
    else:
        repo = Path(f["repo"])
        r = subprocess.run(["git", "-C", str(repo), "show", f"{f['rev']}:{f['path']}"],
                           capture_output=True, text=True)
        if r.returncode:
            return None
        text, path = r.stdout, Path(f["path"])
        start = sum(len(s) for s in text.splitlines(True)[:max(f["line"] - 1, 0)])
    got = braced(text, f["name"], start)
    if got is None:
        return None
    return path, text[:got[0]].count("\n") + 1, got[1]


def body(f: dict) -> str | None:
    """The C definition of a matched function, with its file and line."""
    got = definition(f)
    if got is None:
        return None
    path, line, text = got
    shown = path.relative_to(ROOT) if path.is_absolute() else f"{f['tree']}:{path}"
    return f"// {shown}:{line} ({f['version']} view)\n{text}"


def differences(a: dict, b: dict) -> list[str]:
    """What substituting b's C for a would have to change, at the exact tier."""
    out = []
    for op, i0, i1, j0, j1 in difflib.SequenceMatcher(
            None, a["seq"][0], b["seq"][0], autojunk=False).get_opcodes():
        if op == "equal":
            continue
        for k in range(max(i1 - i0, j1 - j0)):
            mine = a["seq"][0][i0 + k] if i0 + k < i1 else "-"
            theirs = b["seq"][0][j0 + k] if j0 + k < j1 else "-"
            out.append(f"    {mine:<40} template {theirs}")
    for x, y in zip(a["calls"], b["calls"]):
        if x != y:
            out.append(f"    call {x:<35} template {y}")
    if len(a["calls"]) != len(b["calls"]):
        out.append(f"    {len(a['calls'])} calls, template has {len(b['calls'])}")
    return out


# --- reporting -------------------------------------------------------------------

def label(f: dict) -> str:
    return f"{f['name']} [{SHORT.get(f['version'], f['version'])}]"


def row(ratio: float, t: dict, c: dict, diffs: int | None = None) -> str:
    left = f"{diffs:>4} sub" if diffs is not None else f"{ratio:.3f}   "
    return (f"{left} {label(t):<40} {t['n'] * 4:>5}B {t['unit']:<28} "
            f"<- {label(c):<40} {c['n'] * 4:>5}B {c['unit']}")


def worklist(funcs, tier, version, unit, count, floor) -> None:
    """Unmatched functions of a game ranked by retail bytes per substitution left to make."""
    index = Index(funcs, tier)
    targets = [f for f in funcs.values() if f["state"] == "asm" and f["n"] >= MIN_INSNS
               and f["version"] in VERSIONS and (version is None or f["version"] == version)
               and (unit is None or f["unit"].startswith(unit))]
    rows = []
    for t in targets:
        best = templates(funcs, index, t, top=1)
        if not best or best[0][0] < floor:
            continue
        diffs = len(differences(t, best[0][1]))
        rows.append((t["n"] * 4 / (diffs + 1), diffs, best[0][0], t, best[0][1]))
    rows.sort(key=lambda r: -r[0])
    print(f"{len(rows)} of {len(targets)} unmatched functions have a matched {TIERS[tier]} "
          f"template at ratio {floor:.2f} or better ({sum(r[3]['n'] * 4 for r in rows)} retail bytes). "
          f"'sub' is how many instructions the template does not already supply.")
    for _, diffs, ratio, t, c in rows[:count]:
        print(row(ratio, t, c, diffs))


def stats(funcs: dict[str, dict]) -> None:
    asm = [f for f in funcs.values() if f["state"] == "asm" and f["version"] in VERSIONS]
    print(f"{len(funcs)} functions in the index, {len(asm)} Raidou functions still assembly")
    for k, tier in enumerate(TIERS):
        by = collections.defaultdict(list)
        for f in funcs.values():
            by[f["hash"][k]].append(f)
        twin = [f for f in asm if any(g["state"] == "c" for g in by[f["hash"][k]])]
        print(f"  {tier:6} {len(twin):5} unmatched with an identical matched twin "
              f"({sum(f['n'] * 4 for f in twin)}B)")


def show_clusters(funcs, tier, version, unit, count) -> None:
    out = []
    for group in clusters(funcs, tier):
        rest = [f for f in group if f["state"] == "asm" and f["version"] in VERSIONS
                and (version is None or f["version"] == version)]
        if len(rest) < 2 or group[0]["n"] < MIN_INSNS:
            continue
        if unit and not any(f["unit"].startswith(unit) for f in group):
            continue
        out.append(group)
    print(f"{len(out)} clusters with two or more unmatched members")
    for group in out[:count]:
        shown = ", ".join(f"{label(f)}{'' if f['state'] == 'asm' else ' C'}" for f in group[:8])
        more = f", +{len(group) - 8} more" if len(group) > 8 else ""
        print(f"  {group[0]['n'] * 4}B x{len(group)} {shown}{more}")


def show_one(funcs, tier, name, version, want_body, top) -> None:
    t = lookup(funcs, name, version)
    if t is None:
        raise SystemExit(f"{name}: not a function of any unit")
    index = Index(funcs, tier)
    print(f"{label(t)} {t['n'] * 4}B {t['unit']} ({t['state']})")
    near = templates(funcs, index, t, top)
    print("  matched templates:" if near else "  no matched templates")
    for ratio, c in near:
        print("  " + row(ratio, t, c))
    sib = index.nearest(t, top=3, state="asm")
    if sib:
        print("  unmatched siblings:")
        for ratio, c in sib:
            print("  " + row(ratio, t, c))
    if not near:
        return
    best = near[0][1]
    diff = differences(t, best)
    print(f"  against {label(best)}: {len(diff)} exact-tier differences" if diff
          else f"  against {label(best)}: identical beyond the callees")
    for line in diff[:40]:
        print(line)
    if len(diff) > 40:
        print(f"    ... {len(diff) - 40} more")
    if want_body:
        print()
        print(body(best) or f"// no C definition found for {label(best)}")


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, epilog=EPILOG,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--for", dest="target", help="one function to find templates for")
    ap.add_argument("--version", choices=list(VERSIONS),
                    help="game of the target or worklist (templates come from every corpus)")
    ap.add_argument("--body", action="store_true", help="print the best template's C")
    ap.add_argument("--clusters", action="store_true", help="groups of unmatched functions with the same shape")
    ap.add_argument("--unit", help="restrict to units whose path starts with this")
    ap.add_argument("--tier", choices=TIERS, default="mnem")
    ap.add_argument("--floor", type=float, default=0.9, help="lowest sequence ratio in the worklist")
    ap.add_argument("-n", "--count", type=int, default=40)
    ap.add_argument("--top", type=int, default=5, help="templates shown with --for")
    ap.add_argument("--index", type=Path, default=similar_index.INDEX, help="the shape index")
    ap.add_argument("--stats", action="store_true", help="per-tier identical-twin counts")
    args = ap.parse_args()

    funcs = load(args.index)
    tier = TIERS.index(args.tier)
    if args.stats:
        stats(funcs)
    elif args.clusters:
        show_clusters(funcs, tier, args.version, args.unit, args.count)
    elif args.target:
        show_one(funcs, tier, args.target, args.version, args.body, args.top)
    else:
        worklist(funcs, tier, args.version, args.unit, args.count, args.floor)


if __name__ == "__main__":
    main()
