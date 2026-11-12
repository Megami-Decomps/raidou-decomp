#!/usr/bin/env python3
"""Pick and apply fuzzy_match.py candidates, or measure their precision on named functions.

Usage: fuzzy_apply.py VERSION [--eval] [--apply] [--work DIR]
A candidate counts when its name is unused in the version, its size is within 20% of
the target's, its named callees agree (>= 0.5, when both sides have any), its score is
at least 0.9, and it lies between the source addresses of the target's named neighbours
in the same game (at most 12 source functions apart). --eval runs the same rule on named
functions, with and without their true name in the pool (negative control).
"""
import argparse
import bisect
import json
import re
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
GAMES = {"SLUS_209.11": "smt3", "SLUS_209.74": "dds1", "SLUS_211.52": "dds2"}
SERIAL_OF = {v: k for k, v in GAMES.items()}
MAP_DIR = Path.home() / ".local/state/smt3-port-work/raidou-map"
THRESHOLD, SIZE_MAX, AGREE_MIN, GAP_MAX = 0.9, 0.2, 0.5, 12
MARKER = "// Names from tools/xgame/fuzzy_apply.py (provenance: name_sources.txt)"


class State:
    def __init__(self, version, work):
        self.version = version
        cfg = ROOT / "config" / version
        self.used = set()
        for line in (cfg / "symbol_addrs.txt").read_text().splitlines():
            if m := re.match(r"\s*(\S+)\s*=\s*0x", line):
                self.used.add(m[1])
        self.anchors = defaultdict(list)
        for line in (cfg / "name_sources.txt").read_text().splitlines():
            p = line.split("\t")
            if len(p) >= 4 and not line.startswith("#"):
                m = re.search(r"via (SLUS_\S+) 0x([0-9A-Fa-f]+)", p[3])
                if m and m[1] in GAMES:
                    self.anchors[GAMES[m[1]]].append((int(p[0], 16), int(m[2], 16)))
        for a in self.anchors.values():
            a.sort()
        self.src_eas = {g: sorted(f["ea"] for f in json.load(open(MAP_DIR / f"{g}.json"))) for g in GAMES.values()}
        self.rows = defaultdict(list)
        for line in (Path(work) / f"{version}_all.tsv").read_text().splitlines()[1:]:
            a, cur, game, cand, caddr, ro, rf, ag, n, sz = line.split("\t")
            self.rows[int(a, 16)].append({"cur": cur, "game": game, "name": cand, "src": int(caddr, 16),
                                          "ro": float(ro), "rf": float(rf), "agree": float(ag),
                                          "n": int(n), "size": float(sz)})

    def gap(self, ea, game, src):
        """Source functions between the neighbours' sources, None when src is outside them."""
        a = self.anchors[game]
        keys = [x[0] for x in a]
        i = bisect.bisect_left(keys, ea)
        j = i + 1 if i < len(a) and a[i][0] == ea else i
        if i == 0 or j >= len(a):
            return None
        lo, hi = a[i - 1][1], a[j][1]
        if not lo <= src <= hi:
            return None
        e = self.src_eas[game]
        return bisect.bisect_left(e, hi) - bisect.bisect_left(e, lo)

    def best(self, ea, skip=None):
        """Best accepted candidate for ea, or None. skip is a name to leave out."""
        ok = []
        for c in self.rows[ea]:
            if c["name"] == skip or (c["name"] in self.used and c["name"] != c["cur"]):
                continue
            if abs(c["size"] - 1) > SIZE_MAX or (c["n"] and c["agree"] < AGREE_MIN):
                continue
            g = self.gap(ea, c["game"], c["src"])
            if g is None or g > GAP_MAX:
                continue
            ok.append((score(c), c, g))
        ok.sort(key=lambda t: -t[0])
        return ok[0] if ok and ok[0][0] >= THRESHOLD else None


def score(c):
    return 0.5 * (c["ro"] + c["rf"])


def evaluate(st):
    for label, neg in (("with true name in pool", False), ("true name removed (negative control)", True)):
        n = ok = 0
        for ea, rows in st.rows.items():
            cur = rows[0]["cur"]
            if not cur or cur.startswith("func_"):
                continue
            st.used.discard(cur)
            r = st.best(ea, skip=cur if neg else None)
            st.used.add(cur)
            if r and not (r[1]["rf"] >= 0.999 and r[1]["ro"] >= 0.999):
                n += 1
                ok += r[1]["name"] == cur
        print(f"{label}: {ok}/{n} correct" if not neg else f"{label}: {n} wrong names offered")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("version")
    ap.add_argument("--work", default=str(Path.home() / ".local/state/raidou-decomp-work/fuzzy"))
    ap.add_argument("--eval", action="store_true")
    ap.add_argument("--apply", action="store_true")
    a = ap.parse_args()
    st = State(a.version, a.work)
    if a.eval:
        evaluate(st)
        return
    picks = {}
    with open(Path(a.work) / f"{a.version}_candidates.tsv", "w") as fp:
        fp.write("addr\tcandidate\tgame\tscore\tcallee_agreement\tsize_ratio\tsource_gap\taccepted\n")
        for ea in sorted(st.rows):
            rows = st.rows[ea]
            if rows[0]["cur"]:
                continue
            r = st.best(ea)
            if r:
                s, c, g = r
            else:
                c = max((x for x in rows if x["name"] not in st.used), key=score, default=None)
                if c is None:
                    continue
                s, g = score(c), st.gap(ea, c["game"], c["src"])
            fp.write(f"{ea:08X}\t{c['name']}\t{c['game']}\t{s:.3f}\t{c['agree']:.2f}\t{c['size']:.2f}\t"
                     f"{'' if g is None else g}\t{'yes' if r else 'no'}\n")
            if r:
                picks[ea] = (s, c)
    by_name = {}
    for ea, (s, c) in picks.items():
        if c["name"] not in by_name or s > by_name[c["name"]][0]:
            by_name[c["name"]] = (s, ea, c)
    chosen = sorted((ea, name, s, c) for name, (s, ea, c) in by_name.items())
    counts = defaultdict(int)
    for _, _, _, c in chosen:
        counts[c["game"]] += 1
    print(a.version, len(chosen), "names", dict(counts))
    if a.apply and chosen:
        cfg = ROOT / "config" / a.version
        with open(cfg / "symbol_addrs.txt", "a") as sym, open(cfg / "name_sources.txt", "a") as src:
            sym.write(MARKER + "\n")
            for ea, name, s, c in chosen:
                sym.write(f"{name} = 0x{ea:08X}; // type:func\n")
                src.write(f"0x{ea:08X}\t{name}\tinferred\tfuzzy name from {SERIAL_OF[c['game']]} "
                          f"0x{c['src']:08X}, score {s:.2f}, tools/xgame/fuzzy_apply.py\n")


if __name__ == "__main__":
    main()
