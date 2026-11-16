#!/usr/bin/env python3
"""Shape index of every function in raidou-decomp (both games), smt3-decomp and dds-decomp.

Usage: similar_index.py refresh [--fetch] | stats | column --batch R09 [--batch R10 ...] [--to R22]
find_similar.py and find_body.py read this index and never disassemble; refresh it after matching.
"""
from __future__ import annotations

import argparse
import array
import collections
import hashlib
import os
import pickle
import re
import subprocess
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import body_check  # noqa: E402
from units import VERSIONS, unit_sources  # noqa: E402
from version_guard import view  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
HOME = Path.home()
INDEX = HOME / ".local/state/raidou-decomp-work/similar/index.pickle"
BATCHES = HOME / ".local/state/raidou-decomp-work/port/rest/batches.tsv"
FORMAT = 1
TIERS = ("exact", "noimm", "noreg", "mnem")
# Neighbours kept per unmatched Raidou function, and the ratio a batches.tsv similar_to entry needs.
TOP = 5
FLOOR = 0.9
COLUMN_TOP = 3
EPILOG = (
    "The index is one pickle at ~/.local/state/raidou-decomp-work/similar/index.pickle (--index). Each entry is "
    "one function keyed CORPUS:NAME (raidou1, raidou2, smt3, dds1, dds2) with the hash of its retail bytes, its "
    "exact-tier tokens (call targets wildcarded, branch targets relative), size, call targets, matched status "
    "and source path. `refresh` builds it the first time and afterwards re-tokenises only functions whose bytes "
    "or boundaries changed; status and path are re-read on every run. It then updates the top 5 neighbours with "
    "matched C of each unmatched Raidou function: a list is recomputed when its function changed or it names a "
    "changed one, and a newly matched function is merged into the lists whose size band or name it reaches. Raidou "
    "comes from this working copy's asm/ split and src/ (run configure.py first); smt3 and dds from each tree's "
    "build/ ELF and its src/ at --rev. `column` writes similar_to into batches.tsv: up to 3 neighbours at ratio "
    "0.9 or better as CORPUS:NAME:RATIO, `none`, `matched` or `short` (under 8 instructions).")

INCLUDE_ASM = re.compile(r"^INCLUDE_ASM\([^\n]*\b(\w+)\);", re.M)
FUNC_ASM = re.compile(r"^glabel (\w+)\n(.*?)^endlabel \1", re.M | re.S)
ASM_INSN = re.compile(r"^\s*/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/", re.M)
NUM = re.compile(r"^-?(?:0x[0-9a-fA-F]+|\d+)$")
SKIP = ("hand/", "data/", "matchings/", "nonmatchings/", "vu/")
UNNAMED = ("func_", "D_", "code_", "sub_")
STATUS_RANK = {"matched": 0, "nonmatching": 1, "disabled": 2}
FOREIGN = {"smt3": ("smt3",), "dds": ("dds1", "dds2")}


# --- tokens ----------------------------------------------------------------------

def operands(text: str) -> tuple[list[str], list[int]]:
    """Registers in order and numeric immediates in order."""
    regs, imms = [], []
    for tok in re.split(r"[,()\s]+", text.strip()):
        if tok.startswith("$"):
            regs.append(tok)
        elif NUM.match(tok):
            imms.append(int(tok, 16) if "0x" in tok else int(tok))
    return regs, imms


def tokens(code: bytes, vram: int) -> tuple[list[str], list[int]]:
    """Exact-tier token of each instruction ("mnem regs imms") and the target of each jal."""
    import rabbitizer
    out, calls = [], []
    for i in range(0, len(code) - len(code) % 4, 4):
        inst = rabbitizer.Instruction(int.from_bytes(code[i:i + 4], "little"), vram + i)
        mnem, _, text = inst.disassemble().partition(" ")
        rel = None
        if inst.isBranch():
            text, rel = text.rsplit(",", 1)[0] if "," in text else "", inst.getBranchOffset()
        elif inst.isJumpWithAddress():
            target, text = inst.getInstrIndexAsVram(), ""
            if mnem == "jal":
                calls.append(target)
            else:
                rel = target - (vram + i)
        regs, imms = operands(text)
        if rel is not None:
            imms.append(rel)
        out.append(f"{mnem} {','.join(regs)} {','.join(str(v) for v in imms)}")
    return out, calls


def tiers(token: str) -> tuple[str, str, str, str]:
    """One exact-tier token at each of the four tiers."""
    mnem, regs, imms = token.split(" ", 2)
    return token, f"{mnem} {regs}", f"{mnem}  {imms}", mnem


def shape_hashes(toks: list[str]) -> list[str]:
    return [hashlib.blake2b("\n".join(tiers(t)[k] for t in toks).encode(), digest_size=8).hexdigest()
            for k in range(len(TIERS))]


# --- corpora ---------------------------------------------------------------------

def asm_files(version: str) -> list[Path]:
    base = ROOT / "asm" / version
    return [p for p in sorted(base.rglob("*.s"))
            if not p.relative_to(base).as_posix().startswith(SKIP)]


def inventory(version: str) -> list[dict]:
    """Every function of every unit: name, address, instruction count, unit, state.

    The inventory tools/progress.py counts: a function is assembly when its unit
    has no source (a gap unit) or the version's view of the source holds its INCLUDE_ASM.
    """
    sources = unit_sources(version)
    base = ROOT / "asm" / version
    out = []
    for path in asm_files(version):
        unit = path.relative_to(base).as_posix()[:-2]
        src = sources.get(unit)
        asm = set(INCLUDE_ASM.findall(view(src.read_text(errors="replace"), version))) if src else None
        for fname, body_text in FUNC_ASM.findall(path.read_text()):
            addrs = ASM_INSN.findall(body_text)
            if addrs:
                state = "c" if asm is not None and fname not in asm else "asm"
                out.append({"name": fname, "addr": int(addrs[0], 16), "n": len(addrs), "unit": unit,
                            "state": state, "src": src, "asm": path})
    out.sort(key=lambda f: f["addr"])
    return out


def raidou_functions(version: str) -> tuple[dict, dict[str, dict]]:
    """(corpus header, name -> function) for one Raidou game from this working copy."""
    if not (ROOT / "asm" / version).exists():
        raise SystemExit(f"{version}: no asm/ split in {ROOT}; run configure.py first")
    image = body_check.Image(ROOT / "orig" / version / VERSIONS[version]["serial"])
    funcs = {}
    for f in inventory(version):
        matched = f["state"] == "c"
        path = (f["src"] if matched else f["asm"]).relative_to(ROOT).as_posix()
        funcs[f["name"]] = {"addr": f["addr"], "n": f["n"], "unit": f["unit"], "path": path, "line": 0,
                            "status": "matched" if matched else "asm",
                            "code": image.code(f["addr"], f["n"] * 4)}
    rev = subprocess.run(["git", "-C", str(ROOT), "rev-parse", "HEAD"], capture_output=True,
                         text=True).stdout.strip()
    return {"tree": "raidou", "repo": str(ROOT), "rev": rev}, funcs


def line_offsets(text: str) -> list[int]:
    out, pos = [0], 0
    for line in text.splitlines(True):
        pos += len(line)
        out.append(pos)
    return out


def definitions(tree, wanted: set[str]) -> dict[str, tuple[str, int, str]]:
    """name -> (path, line, status) of the best definition of each wanted function in a find_body.Tree."""
    import find_body
    offsets, guards, out = {}, {}, {}
    for name in wanted & tree.index.keys():
        best = None
        for path, line in tree.index[name]:
            text = tree.text(path)
            if path not in offsets:
                offsets[path], guards[path] = line_offsets(text), find_body.guard_statuses(text)
            if find_body.definition_at(text, offsets[path][line - 1]) is None:
                continue
            got = (path, line, guards[path][line - 1])
            if best is None or STATUS_RANK[got[2]] < STATUS_RANK[best[2]]:
                best = got
        if best:
            out[name] = best
    return out


def pick_name(names: set[str], defs: dict) -> str:
    """One name for an address: a defined one, else a real one, else the first."""
    return min(names, key=lambda n: (n not in defs, n.startswith(UNNAMED), n))


def foreign_functions(tree_name: str, repo: Path, rev: str) -> dict[str, tuple[dict, dict[str, dict]]]:
    """corpus -> (header, name -> function) for each built version of smt3-decomp or dds-decomp."""
    import find_body
    tree = find_body.Tree(tree_name, repo, rev)
    full = find_body.git(repo, "rev-parse", tree.rev).strip()
    images = body_check.tree_images(tree_name, repo, tree.rev)
    defs = definitions(tree, {n for image in images.values() for n in image.funcs})
    out = {}
    for version, image in images.items():
        by_addr = collections.defaultdict(set)
        for name, (addr, _) in image.funcs.items():
            by_addr[addr].add(name)
        funcs = {}
        for addr, names in by_addr.items():
            name = pick_name(names, defs)
            size = image.size_of(name)
            code = image.code(addr, size) if size else b""
            if not code or size % 4 or len(code) != size:
                continue
            path, line, status = defs.get(name, ("", 0, "asm"))
            funcs[name] = {"addr": addr, "n": size // 4, "unit": path, "path": path, "line": line,
                           "status": status, "code": code}
        out[version] = ({"tree": tree_name, "repo": str(repo), "rev": full}, funcs)
    if not out:
        raise SystemExit(f"{tree_name}: no built ELF under {repo}/build (body_check.TREE_IMAGES)")
    return out


# --- the index -------------------------------------------------------------------

def empty() -> dict:
    return {"format": FORMAT, "vocab": [], "corpora": {}, "neighbours": {}}


def read(path: Path = INDEX, missing_ok: bool = False) -> dict:
    if not path.exists():
        if missing_ok:
            return empty()
        raise SystemExit(f"no similarity index at {path}: run `python3 tools/similar_index.py refresh`")
    with path.open("rb") as f:
        got = pickle.load(f)
    if got.get("format") != FORMAT:
        if missing_ok:
            return empty()
        raise SystemExit(f"{path}: index format {got.get('format')}, want {FORMAT}; rerun refresh")
    return got


def write(index: dict, path: Path = INDEX) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_suffix(f".tmp{os.getpid()}")
    with tmp.open("wb") as f:
        pickle.dump(index, f, protocol=pickle.HIGHEST_PROTOCOL)
    tmp.replace(path)


def update_corpus(index: dict, name: str, header: dict, funcs: dict[str, dict]) -> tuple[int, set[str]]:
    """Merge one corpus into the index; (functions tokenised, keys whose bytes or status changed)."""
    vocab = index["vocab"]
    ids = index.setdefault("_ids", {t: i for i, t in enumerate(vocab)})
    old = index["corpora"].get(name, {}).get("funcs", {})
    new, done, changed = {}, 0, set()
    for fname, f in funcs.items():
        digest = hashlib.blake2b(f["code"], digest_size=8).hexdigest()
        prev = old.get(fname)
        entry = {k: f[k] for k in ("addr", "n", "unit", "path", "line", "status")}
        entry["hash"] = digest
        if prev and prev["hash"] == digest and prev["addr"] == f["addr"]:
            entry.update(tok=prev["tok"], calls=prev["calls"], shape=prev["shape"])
        else:
            toks, calls = tokens(f["code"], f["addr"])
            tok = array.array("I")
            for t in toks:
                if t not in ids:
                    ids[t] = len(vocab)
                    vocab.append(t)
                tok.append(ids[t])
            entry.update(tok=tok, calls=calls, shape=shape_hashes(toks))
            done += 1
        if not prev or (prev["hash"], prev["status"]) != (digest, entry["status"]):
            changed.add(f"{name}:{fname}")
        new[fname] = entry
    changed |= {f"{name}:{n}" for n in old if n not in new}
    index["corpora"][name] = dict(header, funcs=new, refreshed=time.time())
    return done, changed


_WORK: tuple = ()


def _neighbours_of(keys: list[str]) -> list[tuple[str, list]]:
    import find_similar as fs
    funcs, nidx, fam = _WORK
    return [(k, [(round(r, 4), fs.key(c)) for r, c in fs.templates(funcs, nidx, funcs[k], TOP, fam, cached=False)])
            for k in keys]


def reaches(fs, t: dict, c: dict) -> bool:
    """Whether templates() would consider c for t: inside t's size band, same name family or same name."""
    if t["n"] * (1 - fs.SIZE_BAND) <= c["n"] <= t["n"] * (1 + fs.SIZE_BAND):
        return True
    if c["version"] in VERSIONS and c["name"] == t["name"]:
        return True
    return (len(t["name"]) >= fs.FAMILY_PREFIX and not t["name"].startswith(UNNAMED)
            and c["name"][:fs.FAMILY_PREFIX] == t["name"][:fs.FAMILY_PREFIX])


def refresh_neighbours(index: dict, changed: set[str], force: bool) -> int:
    """Bring the neighbour lists of unmatched Raidou functions up to date; returns how many changed.

    A list is recomputed when its target is new or changed or it names a changed function. Otherwise each
    changed function now matched is scored against the targets it reaches and merged in by ratio.
    """
    global _WORK
    import find_similar as fs
    funcs = fs.records(index)
    nidx = fs.Index(funcs)
    fam = fs.families(funcs)
    near = index["neighbours"]
    targets = {fs.key(t): t for t in funcs.values() if t["version"] in VERSIONS and t["state"] == "asm"
               and t["n"] >= fs.MIN_INSNS}
    for k in [k for k in near if k not in targets]:
        del near[k]
    full = [k for k in targets if force or k not in near or k in changed or any(c in changed for _, c in near[k])]
    if len(full) > 400:
        import multiprocessing
        _WORK = (funcs, nidx, fam)
        chunks = [full[i:i + 100] for i in range(0, len(full), 100)]
        with multiprocessing.get_context("fork").Pool(os.cpu_count()) as pool:
            for got in pool.imap_unordered(_neighbours_of, chunks):
                near.update(got)
        _WORK = ()
    else:
        _WORK = (funcs, nidx, fam)
        near.update(_neighbours_of(full))
        _WORK = ()
    added = [funcs[k] for k in changed if k in funcs and funcs[k]["state"] == "c" and funcs[k]["n"] >= fs.MIN_INSNS]
    redone, merged = set(full), 0
    for k, t in targets.items():
        if k in redone:
            continue
        got = [(round(nidx.ratio(t, c), 4), fs.key(c)) for c in added if reaches(fs, t, c)]
        if got:
            near[k] = sorted(near[k] + got, key=lambda x: -x[0])[:TOP]
            merged += 1
    return len(full) + merged


def refresh(path: Path, smt3: Path, dds: Path, rev: str, fetch: bool, force: bool) -> None:
    t0 = time.time()
    index = empty() if force else read(path, missing_ok=True)
    if fetch:
        for repo in (smt3, dds):
            subprocess.run(["git", "-C", str(repo), "fetch", "-q", "origin"], check=True)
    sources = [(v, *raidou_functions(v)) for v in VERSIONS]
    for tree, repo in (("smt3", smt3), ("dds", dds)):
        sources += [(v, h, f) for v, (h, f) in foreign_functions(tree, repo, rev).items()]
    t1 = time.time()
    total, changed = 0, set()
    for name, header, funcs in sources:
        done, ch = update_corpus(index, name, header, funcs)
        total += done
        changed |= ch
        print(f"{name}: {len(funcs)} functions, {done} tokenised, {len(ch)} changed ({header['rev'][:10]})")
    for name in list(index["corpora"]):
        if name not in {s[0] for s in sources}:
            changed |= {f"{name}:{n}" for n in index["corpora"].pop(name)["funcs"]}
    t2 = time.time()
    n = refresh_neighbours(index, changed, force)
    index.pop("_ids", None)
    write(index, path)
    print(f"{total} tokenised, {len(changed)} changed, {n} neighbour lists recomputed; read {t1 - t0:.1f}s, "
          f"tokenise {t2 - t1:.1f}s, neighbours {time.time() - t2:.1f}s; "
          f"{path} {path.stat().st_size // 1024} KiB")


# --- reports ---------------------------------------------------------------------

def stats(path: Path) -> None:
    index = read(path)
    print(f"{path}: {len(index['vocab'])} distinct exact tokens, "
          f"{len(index['neighbours'])} unmatched Raidou functions with neighbour lists")
    for name, c in index["corpora"].items():
        by = collections.Counter(f["status"] for f in c["funcs"].values())
        when = time.strftime("%Y-%m-%d %H:%M", time.localtime(c["refreshed"]))
        print(f"  {name:8} {len(c['funcs']):6} functions ({', '.join(f'{s} {n}' for s, n in by.most_common())}) "
              f"{c['tree']}@{c['rev'][:10]} refreshed {when}")


def similar_to(index: dict, name: str, addr: int | None, version: str = "raidou1",
               floor: float = FLOOR, top: int = COLUMN_TOP) -> str:
    """The similar_to cell for one Raidou function."""
    import find_similar as fs
    funcs = index["corpora"].get(version, {}).get("funcs", {})
    f = funcs.get(name)
    if f is None and addr is not None:
        f = next((g for g in funcs.values() if g["addr"] == addr), None)
        name = next((n for n, g in funcs.items() if g is f), name)
    if f is None:
        return "missing"
    if f["status"] == "matched":
        return "matched"
    if f["n"] < fs.MIN_INSNS:
        return "short"
    got = [(r, k) for r, k in index["neighbours"].get(f"{version}:{name}", []) if r >= floor][:top]
    return ",".join(f"{k}:{r:.2f}" for r, k in got) or "none"


def batch_in(batch: str, lo: str, hi: str) -> bool:
    b = batch.split("-")[0]
    return lo <= b <= hi and len(b) == len(lo)


def column(path: Path, batches: Path, first: list[str], last: str | None, floor: float) -> None:
    import find_body
    index = read(path)
    lines = batches.read_text().splitlines()
    head = lines[0].split("\t")
    lo, hi = min(first), last or max(first)
    want = set(first)
    sources, counts = {}, collections.Counter()
    for line in lines[1:]:
        r = dict(zip(head, line.split("\t")))
        if not r.get("batch") or not (r["batch"] in want or r["batch"].split("-")[0] in want
                                      or (last and batch_in(r["batch"], lo, hi))):
            continue
        cell = similar_to(index, r["name"], int(r["addr"], 16) if r.get("addr") else None, floor=floor)
        sources[(r["batch"], r["name"])] = cell
        counts[cell if cell in ("none", "matched", "short", "missing") else "hit"] += 1
    find_body.set_column(batches, sources, "similar_to")
    print(f"similar_to on {len(sources)} rows: " + ", ".join(f"{k} {n}" for k, n in counts.most_common()))


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0], epilog=EPILOG)
    ap.add_argument("command", choices=("refresh", "stats", "column"))
    ap.add_argument("--index", type=Path, default=INDEX)
    ap.add_argument("--smt3", type=Path, default=HOME / "projects/smt3-decomp")
    ap.add_argument("--dds", type=Path, default=HOME / "projects/dds-decomp")
    ap.add_argument("--rev", default="origin/main", help="revision of smt3-decomp and dds-decomp read")
    ap.add_argument("--fetch", action="store_true", help="git fetch origin in smt3-decomp and dds-decomp first")
    ap.add_argument("--force", action="store_true", help="rebuild from nothing")
    ap.add_argument("--batches", type=Path, default=BATCHES)
    ap.add_argument("--batch", action="append", default=[], help="column: batch name or Rnn prefix")
    ap.add_argument("--to", help="column: every batch from the first --batch through this Rnn")
    ap.add_argument("--floor", type=float, default=FLOOR, help="column: lowest ratio written")
    a = ap.parse_args()
    if a.command == "refresh":
        refresh(a.index, a.smt3, a.dds, a.rev, a.fetch, a.force)
    elif a.command == "stats":
        stats(a.index)
    else:
        if not a.batch:
            ap.error("column needs --batch")
        column(a.index, a.batches, a.batch, a.to, a.floor)


if __name__ == "__main__":
    main()
