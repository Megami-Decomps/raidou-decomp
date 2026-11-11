#!/usr/bin/env python3
"""Render the reconciled progress reports as a static site for GitHub Pages.

    python3 tools/progress_site.py [raidou1 raidou2] [--out build/site] [--history-url URL]
                                   [--repo URL] [--commit SHA] [--date ISO-8601]

Reads build/<v>/report.json (game scope, the headline) and
build/<v>/report.all.json (whole binary, by category) for each version, both
written by `ninja report`. With --history-url, the history.json each version
published last is fetched from <URL>/<v>/history.json and the current commit is
added to it, replacing an entry for the same commit. A 404 starts a new history;
any other failure stops. Writes:

    index.html                   both versions side by side
    <v>/index.html               headline, VU microcode, history chart, categories, units
    <v>/history.json             one entry per published commit, oldest first
    <v>/report.json              the game-scope report the page was rendered from
    badges/<v>-code.json         shields.io endpoint badge: matched code bytes
    badges/<v>-functions.json    shields.io endpoint badge: matched functions

Only reconciled credit is shown, never raw objdiff: see README.md.
"""
from __future__ import annotations

import argparse
import html
import json
import os
import shutil
import subprocess
import urllib.error
import urllib.request
import sys
from datetime import datetime
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import vu_include  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
VERSIONS = json.loads((ROOT / "config/versions.json").read_text())
ORIGIN_NAMES = {"from_smt3": "found in SMT3", "from_dds": "found in DDS",
                "raidou_only": "Raidou only"}
ORIGINS = tuple(ORIGIN_NAMES)
MEASURES = ("matched_code", "total_code", "matched_functions", "total_functions")


def measure(measures: dict, key: str) -> int:
    # objdiff writes 64-bit counts as strings and omits zero-valued fields.
    return int(measures.get(key, 0))


def percent(matched: int, total: int) -> float:
    return 100.0 * matched / total if total else 0.0


def totals(measures: dict) -> dict:
    return {key: measure(measures, key) for key in MEASURES}


def merge_history(history: list[dict], entry: dict) -> list[dict]:
    """Add entry, replacing a re-run of the same commit, ordered by date."""
    merged = [item for item in history if item["commit"] != entry["commit"]]
    merged.append(entry)
    merged.sort(key=lambda item: datetime.fromisoformat(item["date"]))
    return merged


def badge(label: str, matched: int, total: int) -> dict:
    return {
        "schemaVersion": 1,
        "label": label,
        "message": f"{percent(matched, total):.2f}%",
        "color": "blue",
    }


def git(*args: str) -> str:
    return subprocess.run(["git", *args], cwd=ROOT, check=True,
                          capture_output=True, text=True).stdout.strip()


def number(value: int) -> str:
    return f"{value:,}"


def bar(matched: int, total: int) -> str:
    width = percent(matched, total)
    return f'<span class="bar"><span style="width:{width:.3f}%"></span></span>'


def state(function: dict) -> str:
    return "ok" if function.get("fuzzy_match_percent") == 100.0 else "todo"


def chart(history: list[dict]) -> str:
    """Inline SVG of both percentages over time; no script, no external assets."""
    if len(history) < 2:
        return '<p class="muted">The chart starts once a second commit is published.</p>'
    width, height, left, right, top, bottom = 900, 260, 44, 12, 12, 28
    times = [datetime.fromisoformat(item["date"]).timestamp() for item in history]
    series = {
        "code": [percent(i["matched_code"], i["total_code"]) for i in history],
        "functions": [percent(i["matched_functions"], i["total_functions"]) for i in history],
    }
    ceiling = max(10, -(-max(max(v) for v in series.values()) // 10) * 10)
    span = (times[-1] - times[0]) or 1.0

    def x(t: float) -> float:
        return left + (t - times[0]) / span * (width - left - right)

    def y(v: float) -> float:
        return top + (1 - v / ceiling) * (height - top - bottom)

    parts = [f'<svg class="chart" viewBox="0 0 {width} {height}" role="img" '
             f'aria-label="Progress over time">']
    for step in range(0, int(ceiling) + 1, int(ceiling) // 5):
        parts.append(f'<line class="grid" x1="{left}" x2="{width - right}" '
                     f'y1="{y(step):.1f}" y2="{y(step):.1f}"/>'
                     f'<text x="{left - 6}" y="{y(step) + 4:.1f}" text-anchor="end">{step}%</text>')
    for label, when in ((history[0]["date"], times[0]), (history[-1]["date"], times[-1])):
        anchor = "start" if when == times[0] else "end"
        parts.append(f'<text x="{x(when):.1f}" y="{height - 8}" text-anchor="{anchor}">'
                     f'{html.escape(label[:10])}</text>')
    for name, values in series.items():
        points = " ".join(f"{x(t):.1f},{y(v):.1f}" for t, v in zip(times, values))
        parts.append(f'<polyline class="{name}" points="{points}"/>')
    parts.append("</svg>")
    return "".join(parts)


def source_link(repo: str | None, commit: str, path: str, text: str) -> str:
    if not repo:
        return html.escape(text)
    return f'<a href="{html.escape(repo)}/blob/{commit}/{html.escape(path)}">{html.escape(text)}</a>'


def vu_coverage(version: str) -> dict | None:
    """The microcode counters, or None when the retail image is not at hand."""
    try:
        return vu_include.coverage(version, ROOT / "orig" / version / VERSIONS[version]["serial"])
    except FileNotFoundError:
        return None


def unit_source(version: str, name: str) -> str | None:
    for path in (f"src/{version}/{name}.c", f"src/shared/{name}.c"):
        if (ROOT / path).is_file():
            return path
    return None


def render(version: str, report: dict, audit: dict, history: list[dict],
           repo: str | None, commit: str, date: str,
           vu: dict | None = None) -> str:
    info = VERSIONS[version]
    game = totals(report["measures"])
    commit_text = html.escape(commit[:12])
    if repo:
        commit_text = f'<a href="{html.escape(repo)}/commit/{commit}">{commit_text}</a>'
    docs = source_link(repo, commit, "README.md", "how progress is measured")
    vu_note = ("The microcode row counts the bytes the VU listings assemble against all of "
               f"<code>.vutext</code>; its other {number(vu['section_bytes'] - vu['microcode_bytes'])} "
               "bytes are VIF headers, unpack data and the debug font texture, never instructions."
               ) if vu else ""

    cards = []
    for title, matched, total, unit in (
            ("Code bytes", game["matched_code"], game["total_code"], "bytes"),
            ("Functions", game["matched_functions"], game["total_functions"], "functions")):
        cards.append(
            f'<div class="card"><h2>{title}</h2>'
            f'<p class="big">{percent(matched, total):.2f}%</p>{bar(matched, total)}'
            f'<p class="muted">{number(matched)} / {number(total)} {unit}</p></div>')
    # The microcode is not EE code, so it stays out of the headline; objdiff
    # cannot measure it either, since splat gives .vutext a single label.
    if vu:
        cards.append(
            f'<div class="card"><h2>VU microcode</h2>'
            f'<p class="big">{percent(vu["source_bytes"], vu["microcode_bytes"]):.2f}%</p>'
            f'{bar(vu["source_bytes"], vu["microcode_bytes"])}'
            f'<p class="muted">{number(vu["instructions"])} instructions assembled from '
            f'<code>src/{html.escape(version)}/sdf/vu</code>, outside the code denominator</p></div>')

    categories = []
    for category in audit["categories"]:
        m = totals(category["measures"])
        units = measure(category["measures"], "total_units")
        if category["id"] == "vu1" and vu:
            # objdiff scores .vutext as one unmatched function spanning the whole
            # section. Count the microcode the build assembles instead.
            m = {"matched_code": vu["source_bytes"], "total_code": vu["section_bytes"],
                 "matched_functions": 0, "total_functions": 0}
        if m["total_code"]:
            function_cell = (f'{number(m["matched_functions"])} / {number(m["total_functions"])}'
                             if m["total_functions"] else "&ndash;")
            code = (f'{bar(m["matched_code"], m["total_code"])} '
                    f'{percent(m["matched_code"], m["total_code"]):.2f}%</td>'
                    f'<td class="num">{number(m["matched_code"])} / {number(m["total_code"])}</td>'
                    f'<td class="num">{function_cell}')
        else:
            code = 'not compared</td><td class="num">&ndash;</td><td class="num">&ndash;'
        categories.append(f'<tr><td>{html.escape(category["name"])}</td>'
                          f'<td class="num">{units}</td><td>{code}</td></tr>')
    whole = totals(audit["measures"])
    categories.append(
        f'<tr class="total"><td>Whole binary</td>'
        f'<td class="num">{measure(audit["measures"], "total_units")}</td>'
        f'<td>{bar(whole["matched_code"], whole["total_code"])} '
        f'{percent(whole["matched_code"], whole["total_code"]):.2f}%</td>'
        f'<td class="num">{number(whole["matched_code"])} / {number(whole["total_code"])}</td>'
        f'<td class="num">{number(whole["matched_functions"])} / {number(whole["total_functions"])}</td></tr>')

    units = []
    for unit in sorted(report["units"], key=lambda u: u["name"]):
        m = totals(unit["measures"])
        share = percent(m["matched_code"], m["total_code"])
        functions = sorted(unit.get("functions", []), key=lambda f: int(f.get("address", 0)))
        rows = "".join(
            f'<li class="{state(f)}">'
            f'<span>{html.escape(f["name"])}</span><span class="num">{number(int(f.get("size", 0)))}</span></li>'
            for f in functions)
        path = unit_source(version, unit["name"])
        origin = ", ".join(ORIGIN_NAMES[c] for c in unit.get("metadata", {}).get("progress_categories", [])
                           if c in ORIGINS)
        link = (source_link(repo, commit, path, path) if path
                else "Still assembly: no C source yet")
        units.append(
            f'<details data-name="{html.escape(unit["name"].lower())}" data-code="{share:.6f}" '
            f'data-size="{m["total_code"]}"><summary>'
            f'<span class="name">{html.escape(unit["name"])}</span>'
            f'<span>{bar(m["matched_code"], m["total_code"])} {share:.2f}%</span>'
            f'<span class="num">{number(m["matched_code"])} / {number(m["total_code"])}</span>'
            f'<span class="num">{number(m["matched_functions"])} / {number(m["total_functions"])}</span>'
            f'</summary><p>{link}{origin and " &middot; " + html.escape(origin)}</p>'
            f'<ol class="functions">{rows}</ol></details>')

    return TEMPLATE.format(
        title=html.escape(info["title"]),
        serial=html.escape(info["serial"]),
        commit=commit_text,
        date=html.escape(date),
        docs=docs,
        unit_count=len(report["units"]),
        cards="".join(cards),
        chart=chart(history),
        categories="".join(categories),
        vu_note=vu_note,
        units="".join(units),
    )


TEMPLATE = """<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>{title} decompilation progress</title>
<style>
:root {{ color-scheme: light dark; --fg: #1f2328; --muted: #656d76; --border: #d0d7de;
  --surface: #f6f8fa; --accent: #0969da; --ok: #1a7f37; --code: #0969da; --functions: #bf8700; }}
@media (prefers-color-scheme: dark) {{ :root {{ --fg: #e6edf3; --muted: #8d96a0; --border: #30363d;
  --surface: #161b22; --accent: #4493f8; --ok: #3fb950; --code: #4493f8; --functions: #d29922; }} }}
body {{ font: 15px/1.5 system-ui, sans-serif; color: var(--fg); max-width: 1100px; margin: 0 auto; padding: 24px; }}
a {{ color: var(--accent); }}
h1 {{ margin: 0 0 4px; font-size: 1.6em; }}
.muted {{ color: var(--muted); }}
.cards {{ display: grid; grid-template-columns: repeat(auto-fit, minmax(260px, 1fr)); gap: 16px; margin: 24px 0; }}
.card {{ border: 1px solid var(--border); border-radius: 8px; padding: 16px; background: var(--surface); }}
.card h2 {{ margin: 0; font-size: 1em; color: var(--muted); }}
.big {{ font-size: 2.2em; font-weight: 600; margin: 4px 0 8px; }}
.card .bar {{ display: block; width: 100%; height: 10px; }}
.bar {{ display: inline-block; width: 120px; height: 8px; background: var(--border); border-radius: 4px;
  overflow: hidden; vertical-align: middle; }}
.bar span {{ display: block; height: 100%; background: var(--ok); }}
.chart {{ width: 100%; height: auto; }}
.chart text {{ fill: var(--muted); font-size: 12px; }}
.chart .grid {{ stroke: var(--border); }}
.chart polyline {{ fill: none; stroke-width: 2; }}
.chart .code {{ stroke: var(--code); }}
.chart .functions {{ stroke: var(--functions); }}
.key-code {{ color: var(--code); }} .key-functions {{ color: var(--functions); }}
table {{ border-collapse: collapse; width: 100%; }}
th, td {{ text-align: left; padding: 6px 8px; border-bottom: 1px solid var(--border); }}
.total td {{ font-weight: 600; }}
.num {{ text-align: right; font-variant-numeric: tabular-nums; white-space: nowrap; }}
.controls {{ display: flex; gap: 8px; margin: 8px 0; flex-wrap: wrap; }}
input, select {{ font: inherit; color: inherit; background: var(--surface); border: 1px solid var(--border);
  border-radius: 6px; padding: 4px 8px; }}
.units summary, .units .head {{ display: grid; grid-template-columns: 1fr 200px 170px 110px; gap: 8px;
  padding: 6px 8px; border-bottom: 1px solid var(--border); cursor: pointer; }}
.units .head {{ font-weight: 600; cursor: default; }}
.units .name {{ overflow: hidden; text-overflow: ellipsis; white-space: nowrap; }}
.units details > p {{ margin: 8px 16px; }}
.functions {{ columns: 3 280px; margin: 0 16px 16px; padding: 0; list-style: none;
  font: 13px/1.6 ui-monospace, monospace; }}
.functions li {{ display: flex; justify-content: space-between; gap: 8px; break-inside: avoid; padding-right: 16px; }}
.functions .ok {{ color: var(--ok); }}
.functions .todo {{ color: var(--muted); }}
</style>
</head>
<body>
<h1>{title}</h1>
<p class="muted"><a href="../">Both versions</a> &middot; Matching decompilation of <code>{serial}</code> &middot; commit {commit} &middot; {date}</p>
<p>Game and engine code, {unit_count} units. A function counts only when the linked build proves its
retail bytes come from C; assembly fallbacks earn nothing. See {docs}.</p>
<div class="cards">{cards}</div>
<h2>History</h2>
<p class="muted"><span class="key-code">&#9632; code bytes</span> &nbsp; <span class="key-functions">&#9632; functions</span></p>
{chart}
<h2>Categories</h2>
<p class="muted">{vu_note}</p>
<table>
<thead><tr><th>Category</th><th class="num">Units</th><th>Code bytes</th><th class="num">Matched / total bytes</th>
<th class="num">Functions</th></tr></thead>
<tbody>{categories}</tbody>
</table>
<h2>Units</h2>
<div class="controls">
<input id="filter" type="search" placeholder="Filter units" aria-label="Filter units">
<select id="sort" aria-label="Sort units">
<option value="name">Sort by name</option>
<option value="code">Sort by matched share</option>
<option value="size">Sort by size</option>
</select>
</div>
<div class="units">
<div class="head"><span>Unit</span><span>Code bytes</span><span class="num">Matched / total</span>
<span class="num">Functions</span></div>
<div id="units">{units}</div>
</div>
<script>
const list = document.getElementById("units");
document.getElementById("filter").addEventListener("input", (event) => {{
  const text = event.target.value.toLowerCase();
  for (const unit of list.children) unit.hidden = !unit.dataset.name.includes(text);
}});
document.getElementById("sort").addEventListener("change", (event) => {{
  const key = event.target.value;
  const units = [...list.children];
  units.sort(key === "name"
    ? (a, b) => a.dataset.name.localeCompare(b.dataset.name)
    : (a, b) => Number(b.dataset[key]) - Number(a.dataset[key]));
  list.append(...units);
}});
</script>
</body>
</html>
"""


def fetch_history(base_url: str | None, version: str) -> list[dict]:
    """The history the last deployment served; a 404 means none was published yet."""
    if not base_url:
        return []
    url = f"{base_url.rstrip('/')}/{version}/history.json"
    try:
        with urllib.request.urlopen(url, timeout=60) as response:
            return json.loads(response.read())
    except urllib.error.HTTPError as error:
        if error.code == 404:
            return []
        raise SystemExit(f"fetching {url} failed: HTTP {error.code}")


def landing(rows: list[dict], commit: str, date: str) -> str:
    cards = []
    for row in rows:
        game = row["game"]
        cards.append(
            f'<div class="card"><h2><a href="{row["version"]}/">{html.escape(row["title"])}</a></h2>'
            f'<p class="big">{percent(game["matched_code"], game["total_code"]):.2f}%</p>'
            f'{bar(game["matched_code"], game["total_code"])}'
            f'<p class="muted">code bytes, {number(game["matched_code"])} / {number(game["total_code"])}</p>'
            f'<p class="big">{percent(game["matched_functions"], game["total_functions"]):.2f}%</p>'
            f'{bar(game["matched_functions"], game["total_functions"])}'
            f'<p class="muted">functions, {number(game["matched_functions"])} / '
            f'{number(game["total_functions"])}</p></div>')
    head = TEMPLATE.split("<h1>")[0].replace("{title} decompilation progress", "Raidou decompilation progress")
    page = head.replace("{{", "{").replace("}}", "}")
    return (page + "<h1>Devil Summoner: Raidou Kuzunoha</h1>\n"
            f'<p class="muted">Matching decompilation progress &middot; commit {html.escape(commit[:12])} '
            f'&middot; {html.escape(date)}</p>\n<div class="cards">{"".join(cards)}</div>\n</body>\n</html>\n')


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("versions", nargs="*", choices=list(VERSIONS))
    parser.add_argument("--out", type=Path, default=ROOT / "build" / "site")
    parser.add_argument("--history-url", help="base URL of the published site, for history.json")
    parser.add_argument("--repo", default=os.environ.get("GITHUB_REPOSITORY") and
                        f"{os.environ.get('GITHUB_SERVER_URL', 'https://github.com')}/"
                        f"{os.environ['GITHUB_REPOSITORY']}",
                        help="repository URL for source links (default: from GitHub Actions)")
    parser.add_argument("--commit", help="commit the reports were built from (default: HEAD)")
    parser.add_argument("--date", help="commit date, ISO 8601 (default: the commit's date)")
    args = parser.parse_args()

    commit = args.commit or git("rev-parse", "HEAD")
    date = args.date or git("show", "-s", "--format=%cI", commit)
    out = args.out
    (out / "badges").mkdir(parents=True, exist_ok=True)
    rows = []
    for version in args.versions or VERSIONS:
        build = ROOT / "build" / version
        report_path = build / "report.json"
        try:
            report = json.loads(report_path.read_text())
            audit = json.loads((build / "report.all.json").read_text())
        except FileNotFoundError as error:
            sys.exit(f"{error.filename} is missing: run `ninja report` first")
        game = totals(report["measures"])
        history = merge_history(fetch_history(args.history_url, version),
                                {"commit": commit, "date": date, **game})
        (out / version).mkdir(exist_ok=True)
        (out / "badges" / f"{version}-code.json").write_text(json.dumps(
            badge("code bytes", game["matched_code"], game["total_code"])) + "\n")
        (out / "badges" / f"{version}-functions.json").write_text(json.dumps(
            badge("functions", game["matched_functions"], game["total_functions"])) + "\n")
        (out / version / "history.json").write_text(json.dumps(history, indent=1) + "\n")
        shutil.copyfile(report_path, out / version / "report.json")
        (out / version / "index.html").write_text(render(
            version, report, audit, history, args.repo, commit, date, vu_coverage(version)))
        rows.append({"version": version, "title": VERSIONS[version]["title"], "game": game})
        print(f"{version}: {percent(game['matched_code'], game['total_code']):.3f}% code, "
              f"{percent(game['matched_functions'], game['total_functions']):.3f}% functions, "
              f"{len(history)} history entries")
    (out / "index.html").write_text(landing(rows, commit, date))


if __name__ == "__main__":
    main()
