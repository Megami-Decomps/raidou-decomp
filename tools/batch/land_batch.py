#!/usr/bin/env python3
"""Package a drafted batch into a tree: header patch, renames, units, config, docs, regression run and gate.

    python3 tools/batch/land_batch.py --batch DIR --tree T [--base REV] [--skip STEP,...]
Steps run in order; see docs/batch-workflow.md for the batch directory's files. It never commits.
"""
import argparse
import json
import re
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(HERE.parent))
import batch_files  # noqa: E402
import header_includers  # noqa: E402
import matching_docs  # noqa: E402
import renames  # noqa: E402
import segments  # noqa: E402
import unit_text  # noqa: E402

STEPS = ("patch", "renames", "splice", "configure", "promote", "docs", "regress", "gate")
GATE = (["tools/check_config.py"], ["tools/check_nonmatching.py"], ["tools/test_raidou_c.py"],
        ["tools/test_docs_index.py"], ["tools/docs_index.py", "--check"], ["tools/revisit_names.py", "--check"])
NONMATCHING_NAME = re.compile(r"^nonmatching (\w+), 0x", re.M)


class Run:
    def __init__(self, args: argparse.Namespace):
        self.tree = args.tree.resolve()
        self.batch = batch_files.load(args.batch, args.batches, args.batch_id, tuple(args.renames))
        self.work = (args.work or self.batch.dir / "land").resolve()
        self.work.mkdir(parents=True, exist_ok=True)
        self.jobs = args.jobs
        self.python = str(self.tree / ".venv/bin/python")
        self.maps = {g: renames.name_map(self.batch.renames, g) for g in ("raidou1", "raidou2")}
        self.draft_names = self.maps["raidou1"] if args.rename_drafts else {}
        self.notes: list[str] = []
        self.promoted: set[str] = set()
        self.lines: list[str] = []

    def say(self, line: str) -> None:
        print(line, flush=True)
        self.lines.append(line)

    def note(self, line: str) -> None:
        """A case the tool left for a hand decision."""
        self.notes.append(line)
        self.say(f"DECIDE {line}")

    def git(self, *args: str, check: bool = True) -> str:
        return subprocess.run(["git", *args], cwd=self.tree, capture_output=True, text=True, check=check).stdout

    def py(self, *args: str, log: str | None = None) -> int:
        out = open(self.work / log, "w") if log else subprocess.DEVNULL
        return subprocess.run([self.python, *args], cwd=self.tree, stdout=out, stderr=subprocess.STDOUT).returncode

    def finals(self) -> dict[str, str]:
        out = {}
        for name in self.batch.rows:
            if (text := self.batch.final(name, self.draft_names)) is not None:
                out[name] = text
        return out

    # steps

    def prepare(self, base: str | None, resume: bool) -> None:
        """Check out the base in a clean tree; a resumed run (patch skipped) keeps the tree as it is."""
        if resume:
            self.say(f"tree {self.tree}: resuming on the edited tree")
            return
        if self.git("status", "--porcelain", "--untracked-files=no").strip():
            raise SystemExit(f"land_batch: {self.tree} has uncommitted changes")
        if base:
            self.git("checkout", "-q", "--detach", base)
        self.say(f"tree {self.tree} at {self.git('rev-parse', '--short', 'HEAD').strip()}")
        if not self.configure("configure.base.log"):
            raise SystemExit("land_batch: configure failed at the base, see configure.base.log")

    def edited_headers(self) -> list[Path]:
        """Headers the patch edits plus headers that hold a name the renames change, measured at the base."""
        patch = self.batch.dir / "headers.wip.patch"
        heads = set()
        if patch.exists():
            heads |= {p for p in re.findall(r"^diff --git a/(\S+)", patch.read_text(), re.M) if p.endswith(".h")}
        for old in {r[1] for r in self.batch.renames}:
            heads |= {p for p in self.git("grep", "-l", "-w", old, "--", "include", check=False).split()}
        return sorted(Path(h) for h in heads if (self.tree / h).exists())

    def regress_units(self, headers: list[Path]) -> dict[Path, list[str]]:
        units = header_includers.includers(self.tree, headers)
        all_units = header_includers.unit_paths(self.tree)
        for row in self.batch.rows.values():
            path = Path(row.source)
            if path in all_units:
                units[path] = all_units[path]
            elif row.gap:
                units[path] = [row.game]
        return dict(sorted(units.items()))

    def measure_before(self) -> None:
        headers = self.edited_headers()
        units = self.regress_units(headers)
        self.say(f"regress: {len(headers)} edited headers, {len(units)} units")
        before = header_includers.measure(self.tree, units, self.jobs)
        (self.work / "before.json").write_text(json.dumps(before, indent=1, sort_keys=True) + "\n")
        (self.work / "units.json").write_text(json.dumps({p.as_posix(): g for p, g in units.items()}, indent=1) + "\n")

    def patch(self) -> None:
        patch = self.batch.dir / "headers.wip.patch"
        if not patch.exists():
            self.say("patch: none")
            return
        r = subprocess.run(["git", "apply", "--whitespace=nowarn", str(patch)], cwd=self.tree,
                           capture_output=True, text=True)
        if r.returncode:
            r = subprocess.run(["git", "apply", "--3way", "--whitespace=nowarn", str(patch)], cwd=self.tree,
                               capture_output=True, text=True)
            if r.returncode:
                raise SystemExit(f"land_batch: headers.wip.patch does not apply:\n{r.stderr}")
            self.say("patch: applied with --3way")
        self.say(f"patch: {len(re.findall(r'^diff --git', patch.read_text(), re.M))} files")

    def renames(self) -> None:
        renames.apply(self.tree, self.batch.renames)
        renames.drop(self.tree, self.batch.drop)
        self.say(f"renames: {len(self.batch.renames)} rows, {len(self.batch.drop)} labels dropped")
        if self.py("tools/revisit_names.py"):
            self.note("revisit_names.py failed")
        for fn in self.batch.rows:
            for game in ("raidou1", "raidou2"):
                sources = (self.tree / "config" / game / "name_sources.txt").read_text()
                for m in re.finditer(r"^0x[0-9A-F]+\t(\w+)\t.*derived-sole.*evidence=used by %s$" % re.escape(fn),
                                     sources, re.M):
                    self.say(f"review: {game} label {m.group(1)} is used only by {fn} (drop_symbols.tsv if C owns it)")

    def splice(self) -> None:
        finals = self.finals()
        by_source: dict[str, list[batch_files.Row]] = {}
        for row in self.batch.rows.values():
            if row.name in finals:
                by_source.setdefault(row.source, []).append(row)
        for source, rows in sorted(by_source.items()):
            path = self.tree / source
            if rows[0].gap:
                self.gap_unit(rows[0], finals)
                continue
            text = path.read_text()
            for row in rows:
                try:
                    text = unit_text.splice(text, row.name, finals[row.name], row.name in self.batch.r1only)
                except ValueError as e:
                    self.note(f"{source}: {e}")
            path.write_text(text)
            self.say(f"splice: {source}: {len(rows)} functions")
        missing = sorted(set(self.batch.rows) - set(finals))
        if missing:
            self.say(f"splice: {len(missing)} rows without a draft: {' '.join(missing)}")

    def gap_unit(self, row: batch_files.Row, finals: dict[str, str]) -> None:
        asm = self.tree / "asm" / row.game / f"{row.unit}.s"
        names = [self.maps[row.game].get(n, n) for n in NONMATCHING_NAME.findall(asm.read_text())]
        pre = self.batch.dir / "units" / f"{row.unit}.pre"
        if not pre.exists():
            self.note(f"{row.unit}: no units/{row.unit}.pre, wrote #include \"common.h\" only")
        text = unit_text.gap_unit(pre.read_text() if pre.exists() else '#include "common.h"\n', row.unit, names, finals)
        (self.tree / row.source).parent.mkdir(parents=True, exist_ok=True)
        (self.tree / row.source).write_text(text)
        plans, notes = segments.plan_unit(self.tree, row.game, row.unit)
        if not segments.make_c(self.tree, row.game, row.unit):
            self.note(f"{row.unit}: no asm subsegment in the {row.game} yaml")
        yaml = segments.yaml_path(self.tree, row.game)
        for p in plans:
            yaml.write_text(segments.apply_plan(yaml.read_text(), p))
            rest = f", {p.kind} resumes at 0x{p.rest:08X}" if p.rest is not None else ""
            self.say(f"segments: {row.unit} owns {p.kind} of {p.chunk} from 0x{p.start:08X}{rest}")
        for n in notes:
            self.note(n)
        drafted = sum(n in finals for n in names)
        self.say(f"splice: {row.source}: new unit, {len(names)} functions, {drafted} from drafts")

    def check(self, source: str, game: str, alt: Path | None = None) -> tuple[int, dict[str, str]]:
        cmd = [self.python, "tools/check_unit.py", source, "--version", game] + (["--source", str(alt)] if alt else [])
        r = subprocess.run(cmd, cwd=self.tree, capture_output=True, text=True)
        return r.returncode, header_includers.summarise(r.stdout + r.stderr)

    def check_matches(self, source: str, summaries: dict[str, dict[str, str]], finals: dict[str, str]) -> None:
        """Report rows spliced as plain C that are not OK in a game that builds them."""
        for game, summary in summaries.items():
            for row in self.batch.rows.values():
                if row.source != source or row.name not in finals or unit_text.unwrap(finals[row.name]) is not None:
                    continue
                if game != "raidou1" and row.name in self.batch.r1only:
                    continue
                # absent from a unit that compiled: this game does not build the function
                status = summary.get(row.name, "OK" if "!error" not in summary else summary["!error"])
                if status != "OK":
                    self.note(f"{source} {game}: {row.name} recorded as a match but is {status}")

    def promote(self) -> None:
        """A NON_MATCHING draft that matches and leaves no other function of its unit worse becomes plain C."""
        finals = self.finals()
        games = header_includers.unit_paths(self.tree)
        for source in sorted({r.source for r in self.batch.rows.values() if r.name in finals}):
            unit_games = games.get(Path(source), [])
            base = {g: self.check(source, g) for g in unit_games}
            self.check_matches(source, {g: s for g, (_, s) in base.items()}, finals)
            text = (self.tree / source).read_text()
            for row in self.batch.rows.values():
                if row.source != source or row.name not in finals:
                    continue
                if (body := unit_text.unwrap(finals[row.name])) is None:
                    continue
                final = finals[row.name].rstrip("\n")
                if final not in text:
                    continue
                trial = text.replace(final, body.rstrip("\n"), 1)
                with tempfile.TemporaryDirectory() as tmp:
                    alt = Path(tmp) / Path(source).name
                    alt.write_text(trial)
                    results = {g: self.check(source, g, alt)[1] for g in unit_games}
                # the draft matches and no other function of the unit gets worse
                matches = [s.get(row.name) for s in results.values() if row.name in s]
                worse, _ = header_includers.compare({g: base[g][1] for g in unit_games}, results)
                worse = [w for w in worse if f": {row.name} " not in w]
                if matches and all(m == "OK" for m in matches) and not worse:
                    text = trial
                    self.promoted.add(row.name)
                    self.say(f"promote: {row.name} matches in {source}")
                elif matches and all(m == "OK" for m in matches):
                    self.say(f"promote: {row.name} matches alone but not in {source}: {'; '.join(worse)[:300]}")
            (self.tree / source).write_text(text)
        if self.promoted:
            self.configure()

    def configure(self, log: str = "configure.log") -> bool:
        """configure.py, then the eeasm copies of the split asm that check_unit includes."""
        if self.py("configure.py", log=log):
            self.note(f"configure failed, see {log}")
            return False
        targets = subprocess.run(["ninja", "-t", "targets", "rule", "eeasm"], cwd=self.tree,
                                 capture_output=True, text=True).stdout.split()
        with open(self.work / log, "a") as out:
            rc = subprocess.run(["ninja", *targets], cwd=self.tree, stdout=out, stderr=subprocess.STDOUT).returncode
        if rc:
            self.note(f"ninja of the eeasm files failed, see {log}")
            return False
        self.say("configure: ok")
        return True

    def docs(self) -> None:
        finals = self.finals()
        drafted = {n for n in finals if n not in self.promoted and unit_text.unwrap(finals[n]) is not None}
        for line in matching_docs.drop_entries(self.tree, set(finals)):
            self.say(f"docs: dropped {line}")
        pages: dict[str, list[str]] = {}
        for name in sorted(drafted, key=list(self.batch.rows).index):
            row = self.batch.rows[name]
            if name not in self.batch.docs:
                self.note(f"docs: {name} is NON_MATCHING and has no docs.json entry")
                continue
            unit, text, lever = (self.batch.docs[name] + [""] * 3)[:3]
            games = ["raidou1"] if row.gap or name in self.batch.r1only or row.source.startswith("src/raidou1/") \
                else ["raidou1", "raidou2"]
            measured = [matching_docs.measure(self.tree, row.source, g, name) for g in games]
            unit = unit or row.unit
            pages.setdefault(unit, []).append(matching_docs.entry(name, unit, measured, text, lever))
            self.say(f"docs: {name}: {' '.join(l for _, l in measured)}")
        for unit, entries in pages.items():
            matching_docs.append(self.tree, unit, entries)
        if self.py("tools/docs_index.py", log="docs_index.log"):
            self.note("docs_index.py failed, see docs_index.log")

    def regress(self) -> None:
        units = {Path(p): g for p, g in json.loads((self.work / "units.json").read_text()).items()}
        before = json.loads((self.work / "before.json").read_text())
        before = {k: {self.maps[k.split()[1]].get(f, f): s for f, s in v.items()} for k, v in before.items()}
        after = header_includers.measure(self.tree, units, self.jobs)
        (self.work / "after.json").write_text(json.dumps(after, indent=1, sort_keys=True) + "\n")
        worse, better = header_includers.compare(before, after)
        batch = set(self.batch.rows)
        for line in better:
            self.say(f"regress: better {line}")
        for line in worse:
            fn = line.split(": ")[1].split()[0] if ": " in line else ""
            if fn in batch:
                self.say(f"regress: worse (batch) {line}")
            else:
                self.note(f"regress: worse {line}")
        self.say(f"regress: {len(units)} units, {len(worse)} worse, {len(better)} better")

    def gate(self) -> None:
        rc = subprocess.run(f"{self.python} configure.py --clean > {self.work}/gate.configure.log 2>&1 && "
                            f"ninja > {self.work}/gate.ninja.log 2>&1", shell=True, cwd=self.tree).returncode
        log = (self.work / "gate.ninja.log").read_text() if (self.work / "gate.ninja.log").exists() else ""
        sums = len(re.findall(r"check config/\w+/checksum.sha1", log))
        self.say(f"gate: configure --clean && ninja rc {rc}, {sums} checksum steps")
        failed = rc != 0
        for args in GATE:
            if not (self.tree / args[0]).exists():
                continue
            r = self.py(*args, log=f"gate.{Path(args[0]).stem}.log")
            self.say(f"gate: {' '.join(args)} rc {r}")
            failed |= r != 0
        warnings = len(re.findall(r"warning:", log))
        self.say(f"gate: {warnings} compiler warnings in the build log; {'FAILED' if failed else 'passed'}")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--batch", type=Path, required=True, help="batch directory")
    ap.add_argument("--tree", type=Path, required=True, help="clean, configured checkout to package into")
    ap.add_argument("--base", help="revision to check out first (detached), e.g. origin/main")
    ap.add_argument("--batches", type=Path, help="batches.tsv (default: <batch>/../../batches.tsv)")
    ap.add_argument("--batch-id", help="batch column value (default: the directory name)")
    ap.add_argument("--renames", action="append", default=["renames.tsv"],
                    help="another rename table in the batch directory, applied after renames.tsv")
    ap.add_argument("--rename-drafts", action="store_true",
                    help="drafts use the names before renames.tsv; rename them while splicing")
    ap.add_argument("--skip", default="", help=f"comma-separated steps to skip: {','.join(STEPS)}")
    ap.add_argument("--work", type=Path, help="logs and measurements (default: <batch>/land; configure --clean empties build/)")
    ap.add_argument("--reuse-before", action="store_true",
                    help="keep the work directory's base measurement from an earlier run at the same base")
    ap.add_argument("-j", "--jobs", type=int, default=8)
    args = ap.parse_args()
    skip = set(filter(None, args.skip.split(",")))
    if skip - set(STEPS):
        ap.error(f"unknown steps: {', '.join(sorted(skip - set(STEPS)))}")
    run = Run(args)
    run.prepare(args.base, "patch" in skip)
    if "regress" not in skip and not (args.reuse_before and (run.work / "before.json").exists()):
        run.measure_before()
    for step in STEPS:
        if step not in skip:
            getattr(run, step)()
    (run.work / "report.txt").write_text("\n".join(run.lines) + "\n")
    print(f"{len(run.notes)} cases for a hand decision; report in {run.work / 'report.txt'}")
    return 1 if run.notes else 0


if __name__ == "__main__":
    sys.exit(main())
