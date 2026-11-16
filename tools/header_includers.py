#!/usr/bin/env python3
"""List the units that include a header, directly or through other headers, with the games that compile them.

    python3 tools/header_includers.py <headers> [--check] [--save FILE] [--baseline FILE | --baseline-tree DIR]
--check runs check_unit (default and -DNON_MATCHING) on each and compares every function with the baseline.
"""
import argparse
import json
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
INCLUDE = re.compile(r'^\s*#\s*include\s+"([^"]+)"', re.M)
MODES = ("default", "nonmatching")
RESULT = re.compile(r"^(OK|DIFF|OVER|MISSING)\s+(\S+)(.*)$")
DISTANCE = re.compile(r"(\d+) of (\d+) words differ")
CONTEXT = re.compile(r"^CONTEXT (\S+): ")
FULL_UNIT = re.compile(r"full unit vs retail: (\d+) of (\d+) words differ")
UNIT_PROBLEM = re.compile(r"^(NOASM|RODATA|DATA) ")


def versions(root: Path) -> list[str]:
    return list(json.loads((root / "config/versions.json").read_text()))


def unit_paths(root: Path) -> dict[Path, list[str]]:
    """Source path relative to root -> games that compile it (a per-game file replaces the shared one)."""
    out: dict[Path, list[str]] = {}
    for version in versions(root):
        units = {p.relative_to(root / "src/shared").with_suffix(""): p for p in (root / "src/shared").rglob("*.c")}
        units.update({p.relative_to(root / "src" / version).with_suffix(""): p
                      for p in (root / "src" / version).rglob("*.c")})
        for path in units.values():
            out.setdefault(path.relative_to(root), []).append(version)
    return dict(sorted(out.items()))


def resolve(root: Path, including: Path, name: str) -> Path | None:
    """The file an #include "name" in `including` opens: its own directory, then -Iinclude, then -Isrc."""
    for base in ((root / including).parent, root / "include", root / "src"):
        path = base / name
        if path.is_file():
            return path.resolve().relative_to(root.resolve())
    return None


def header_path(root: Path, header: str) -> Path:
    for path in (Path(header), Path("include") / header, Path("include") / f"{header}.h"):
        if (root / path).is_file():
            return path
    raise SystemExit(f"header_includers: no header {header}")


def includers(root: Path, headers: list[Path]) -> dict[Path, list[str]]:
    """Units whose include closure contains any of `headers` -> games that compile them."""
    files = [p.relative_to(root) for d in ("include", "src") for p in (root / d).rglob("*") if p.suffix in (".c", ".h")]
    users: dict[Path, set[Path]] = {}
    for path in files:
        for name in INCLUDE.findall((root / path).read_text(errors="replace")):
            if (target := resolve(root, path, name)) is not None:
                users.setdefault(target, set()).add(path)
    reached, todo = set(headers), list(headers)
    while todo:
        for user in users.get(todo.pop(), ()):
            if user not in reached:
                reached.add(user)
                todo.append(user)
    return {path: games for path, games in unit_paths(root).items() if path in reached}


def summarise(output: str) -> dict[str, str]:
    """Function -> "OK" or the result kind with its word distance; "!error" holds the first compiler error and
    "!unit" the unit-level problems. A function that compiles differently inside the full unit is CONTEXT."""
    out = {}
    errors, unit, context, last = [], [], {}, None
    for line in output.splitlines():
        if m := CONTEXT.match(line):
            last = m.group(1)
            context[last] = "CONTEXT"
        elif last and (m := FULL_UNIT.search(line)):
            context[last] = f"CONTEXT {m.group(1)}/{m.group(2)}"
            last = None
        elif UNIT_PROBLEM.match(line):
            unit.append(line.strip()[:200])
        elif m := RESULT.match(line):
            d = DISTANCE.search(m.group(3))
            out[m.group(2)] = m.group(1) if m.group(1) == "OK" or not d else f"{m.group(1)} {d.group(1)}/{d.group(2)}"
        elif re.match(r"^\S+:\d+: (?!warning:)", line) or "Error:" in line or line.startswith("compile failed"):
            errors.append(re.sub(r"^\S*:\d+(:\d+)?: ", "", line)[:200])
    out.update(context)
    if errors:
        out["!error"] = errors[0]
    if unit:
        out["!unit"] = "\n".join(sorted(unit))
    return out


def check(root: Path, path: Path, version: str, mode: str) -> dict[str, str]:
    if not (root / path).exists():
        return {"!absent": "absent"}
    cmd = [str(root / ".venv/bin/python"), "tools/check_unit.py", str(path), "--version", version]
    if mode == "nonmatching":
        cmd.append("--cflags=-DNON_MATCHING")
    r = subprocess.run(cmd, cwd=root, capture_output=True, text=True)
    return summarise(r.stdout + r.stderr)


JOBS = os.cpu_count() or 8


def measure(root: Path, units: dict[Path, list[str]], jobs: int = JOBS) -> dict[str, dict[str, str]]:
    """"<path> <game> <mode>" -> function summary, for every unit, game and mode."""
    keys = [(path, game, mode) for path, games in units.items() for game in games for mode in MODES]
    # Largest units first, so a 6 s unit does not start last.
    size = {p: (root / p).stat().st_size if (root / p).exists() else 0 for p in units}
    order = sorted(keys, key=lambda k: -size[k[0]])
    with ThreadPoolExecutor(jobs) as pool:
        results = dict(zip(order, pool.map(lambda k: check(root, *k), order)))
    return {f"{p.as_posix()} {g} {m}": results[(p, g, m)] for p, g, m in keys}


def rank(status: str | None) -> tuple[int, int]:
    """Lower is better: OK, then fewer differing words, then not emitted."""
    if status == "OK":
        return (0, 0)
    if status and (d := DISTANCE.search(status.replace("/", " of ") + " words differ")):
        return (1, int(d.group(1)))
    return (2, 0)


def compare(before: dict, after: dict, renames: dict[str, str] | None = None) -> tuple[list[str], list[str]]:
    """(worse, better) lines for each function whose result changed; `renames` maps baseline names to current ones."""
    worse, better = [], []
    renames = renames or {}
    for key in sorted(set(before) | set(after)):
        old = {renames.get(f, f): s for f, s in before.get(key, {}).items()}
        new = after.get(key, {})
        if "!absent" in old or "!absent" in new:
            continue
        if new.get("!error") and not old.get("!error"):
            worse.append(f"{key}: error: {new['!error']}")
            continue
        # a unit missing from the baseline has nothing to be worse than
        added = set(new.get("!unit", "").split("\n")) - set(old.get("!unit", "").split("\n")) - {""} \
            if key in before else set()
        for problem in sorted(added):
            worse.append(f"{key}: unit: {problem}")
        for fn in sorted(set(old) | set(new)):
            if fn.startswith("!") or old.get(fn) == new.get(fn):
                continue
            line = f"{key}: {fn} {old.get(fn, '-')} -> {new.get(fn, '-')}"
            (worse if fn in old and rank(new.get(fn)) > rank(old.get(fn)) else better).append(line)
    return worse, better


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("headers", nargs="+")
    ap.add_argument("--check", action="store_true", help="run check_unit on every includer and compare")
    ap.add_argument("--save", type=Path, help="write the check_unit summary here (implies --check)")
    ap.add_argument("--baseline", type=Path, help="summary written by an earlier --save")
    ap.add_argument("--baseline-tree", type=Path, help="checkout to measure the same units in")
    ap.add_argument("--tree", type=Path, default=ROOT, help="checkout to read and measure (default: this one)")
    ap.add_argument("-j", "--jobs", type=int, default=JOBS)
    args = ap.parse_args()
    root = args.tree.resolve()
    units = includers(root, [header_path(root, h) for h in args.headers])
    if not (args.check or args.save or args.baseline or args.baseline_tree):
        for path, games in units.items():
            print(path.as_posix(), " ".join(games))
        return 0
    after = measure(root, units, args.jobs)
    if args.save:
        args.save.write_text(json.dumps(after, indent=1, sort_keys=True) + "\n")
    if args.baseline:
        before = json.loads(args.baseline.read_text())
    elif args.baseline_tree:
        before = measure(args.baseline_tree.resolve(), units, args.jobs)
    else:
        print(f"{len(after)} checks over {len(units)} units; no baseline given")
        return 0
    worse, better = compare(before, after)
    for line in better:
        print("better", line)
    for line in worse:
        print("WORSE", line)
    print(f"{len(units)} units, {len(after)} checks: {len(worse)} worse, {len(better)} better")
    return 1 if worse else 0


if __name__ == "__main__":
    sys.exit(main())
