#!/usr/bin/env python3
"""Fuzzy-match Raidou functions to SMT3 and DDS functions (opcode n-gram index, difflib).

Usage: fuzzy_match.py VERSION [--sources LIST] [--map-dir DIR] [--out DIR] [--jobs N]
Writes <out>/<v>_all.tsv: top candidates for every function; fuzzy_apply.py scores them.
"""
import argparse
import difflib
import json
import multiprocessing as mp
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from elf_export_funcs import sections, trim_padding  # noqa: E402
import struct  # noqa: E402

ROOT = Path(__file__).resolve().parents[2]
SERIAL = {"raidou1": "SLUS_214.31", "raidou2": "SLUS_218.45"}
SOURCES = ("smt3", "dds1", "dds2")
MIN_WORDS = 10
GRAM = 4
MAX_POSTING = 400
TOP_PREFILTER = 25
KEEP = 8
PLACEHOLDER = re.compile(r"^(sub|nullsub|func|loc|locret|def|unk|j_sub|jpt|code|D)_[0-9A-Za-z]+$")
ADDRESS_TAG = re.compile(r"_[0-9A-Fa-f]{5,}$")
IDENT = re.compile(r"^[A-Za-z_]\w*$")
ROW = re.compile(r"^\s*(\S+)\s*=\s*0x([0-9A-Fa-f]+)\s*;(.*)$")


def good_name(n):
    return bool(IDENT.match(n)) and not PLACEHOLDER.match(n) and not ADDRESS_TAG.search(n)


def tokens(words):
    """(op_tokens, full_tokens): opcode only, and opcode plus register fields."""
    ops, full = [], []
    for w in words:
        o = w >> 26
        rs, rt, rd = (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
        if o == 0:
            k = (0, w & 0x3F)
            f = (0, w & 0x3F, rs, rt, rd, (w >> 6) & 31)
        elif o == 1:
            k = (1, rt)
            f = (1, rt, rs)
        elif o in (2, 3):
            k = f = (o,)
        elif o in (0x10, 0x11, 0x12):
            k = (o, rs, w & 0x3F if rs >= 16 else 0)
            f = (o, rs, w & 0x3F if rs >= 16 else 0, rt, rd)
        else:
            k = (o,)
            f = (o, rs, rt)
        ops.append(hash(k) & 0xFFFFFFF)
        full.append(hash(f) & 0xFFFFFFF)
    return ops, full


def load_target(version):
    elf = (ROOT / "orig" / version / SERIAL[version]).read_bytes()
    text_addr, text_off, text_size, _ = sections(elf)[".text"]
    text_end = text_addr + text_size
    names = {}
    for line in (ROOT / "config" / version / "symbol_addrs.txt").read_text().splitlines():
        m = ROW.match(line)
        if m and "type:func" in m[3]:
            names.setdefault(int(m[2], 16), m[1])
    starts = {ea for ea in names if text_addr <= ea < text_end}
    unnamed = set()
    for p in (ROOT / "asm" / version).glob("code_*.s"):
        for line in p.read_text().splitlines():
            m = re.match(r"glabel func_([0-9A-Fa-f]{8})\b", line)
            if m:
                ea = int(m[1], 16)
                unnamed.add(ea)
                starts.add(ea)
    order = sorted(starts)
    funcs = []
    for i, ea in enumerate(order):
        end = order[i + 1] if i + 1 < len(order) else text_end
        words = trim_padding(list(struct.unpack_from(f"<{(end - ea) // 4}I", elf, text_off + ea - text_addr)))
        calls = []
        for k, w in enumerate(words):
            if w >> 26 == 3:
                t = ((ea + 4 * k) & 0xF0000000) | ((w & 0x03FFFFFF) << 2)
                if t in starts and t != ea:
                    calls.append(t)
        funcs.append({"ea": ea, "name": names.get(ea), "words": words, "calls": calls,
                      "unnamed": ea in unnamed and ea not in names})
    return funcs, names


def load_sources(map_dir, games, out_dir):
    out = []
    for g in games:
        path = Path(out_dir) / "src" / f"{g}.json"
        d = json.load(open(path if path.exists() else Path(map_dir) / f"{g}.json"))
        by = {f["ea"]: f["name"] for f in d}
        for f in d:
            if len(f["words"]) >= MIN_WORDS and good_name(f["name"]):
                f["game"] = g
                f["callee_names"] = [by[c] for c in f["calls"] if good_name(by.get(c, ""))]
                out.append(f)
    return out


S = {}


def init(sources):
    S["src"] = sources
    S["ops"], S["full"] = [], []
    index = defaultdict(list)
    for i, f in enumerate(sources):
        o, fl = tokens(f["words"])
        S["ops"].append(o)
        S["full"].append(fl)
        for g in {tuple(o[k:k + GRAM]) for k in range(len(o) - GRAM + 1)}:
            index[g].append(i)
    S["index"] = {g: p for g, p in index.items() if len(p) <= MAX_POSTING}


def ratio(a, b):
    sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
    if sm.real_quick_ratio() < 0.5 or sm.quick_ratio() < 0.5:
        return sm.quick_ratio() * 0.5
    return sm.ratio()


def match_one(t):
    ops, full = tokens(t["words"])
    n = len(ops)
    grams = {tuple(ops[k:k + GRAM]) for k in range(n - GRAM + 1)}
    hits = Counter()
    for g in grams:
        for i in S["index"].get(g, ()):
            hits[i] += 1
    src = S["src"]
    scored = []
    for i, c in hits.most_common(TOP_PREFILTER * 3):
        m = len(src[i]["words"])
        if min(n, m) / max(n, m) < 0.5:
            continue
        scored.append((c / (len(grams) + len(S["ops"][i]) - GRAM + 1 - c + 1), i))
        if len(scored) >= TOP_PREFILTER:
            break
    scored.sort(reverse=True)
    res = []
    for _, i in scored:
        r_op = ratio(ops, S["ops"][i])
        r_full = ratio(full, S["full"][i])
        res.append((i, r_op, r_full))
    return t["ea"], res


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("version")
    ap.add_argument("--map-dir", default=str(Path.home() / ".local/state/smt3-port-work/raidou-map"))
    ap.add_argument("--out", default=str(Path.home() / ".local/state/raidou-decomp-work/fuzzy"))
    ap.add_argument("--sources", default=",".join(SOURCES), help="comma list: smt3 dds1 dds2 raidou1 raidou2")
    ap.add_argument("--jobs", type=int, default=12)
    a = ap.parse_args()
    out = Path(a.out)
    out.mkdir(parents=True, exist_ok=True)
    (out / "src").mkdir(exist_ok=True)
    funcs, names = load_target(a.version)
    sources = load_sources(a.map_dir, a.sources.split(","), out)
    targets = [f for f in funcs if len(f["words"]) >= MIN_WORDS]
    print(f"{len(funcs)} functions, {len(targets)} targets, {sum(f['unnamed'] for f in targets)} unnamed, "
          f"{len(sources)} sources", flush=True)
    by_ea = {f["ea"]: f for f in funcs}
    init(sources)
    with mp.get_context("fork").Pool(a.jobs) as pool:
        results = []
        for k, r in enumerate(pool.imap_unordered(match_one, targets, chunksize=8)):
            results.append(r)
            if k % 500 == 0:
                print(k, flush=True)
    results.sort()
    with open(out / f"{a.version}_all.tsv", "w") as fp:
        fp.write("addr\tcur_name\tsource\tcand\tcand_addr\tr_op\tr_full\tagree\tcallee_n\tsize_ratio\n")
        for ea, res in results:
            t = by_ea[ea]
            tn = Counter(names[c] for c in t["calls"] if c in names and not names[c].startswith("func_"))
            ranked = []
            for i, r_op, r_full in res:
                s = sources[i]
                sn = Counter(s["callee_names"])
                denom = max(sum(tn.values()), sum(sn.values()))
                agree = sum((tn & sn).values()) / denom if denom else -1
                ranked.append((0.5 * (r_op + r_full), i, r_op, r_full, agree, denom))
            ranked.sort(reverse=True)
            for _, i, r_op, r_full, agree, denom in ranked[:KEEP]:
                s = sources[i]
                fp.write(f"{ea:08X}\t{t['name'] or ''}\t{s['game']}\t{s['name']}\t{s['ea']:08X}\t"
                         f"{r_op:.4f}\t{r_full:.4f}\t{agree:.3f}\t{denom}\t"
                         f"{len(t['words']) / len(s['words']):.3f}\n")


if __name__ == "__main__":
    main()
