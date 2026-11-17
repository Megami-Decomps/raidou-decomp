#!/usr/bin/env python3
"""Package a drafted batch into a tree: header patch, renames, units, config, docs, regression run and gate.

    python3 tools/batch/land_batch.py --batch DIR --tree T [--base REV] [--skip STEP,...]
Steps run in order; see docs/batch-workflow.md for the batch directory's files. It never commits.
"""
import argparse
import json
import os
import re
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(HERE.parent))
import batch_files  # noqa: E402
import decls  # noqa: E402
import docs_index  # noqa: E402
import draft_store  # noqa: E402
import header_includers  # noqa: E402
import matching_docs  # noqa: E402
import renames  # noqa: E402
import segments  # noqa: E402
import unit_text  # noqa: E402

STEPS = ("patch", "renames", "splice", "r1only", "configure", "promote", "docs", "regress", "gate", "retire")
DRY_STEPS = ("patch", "renames", "splice", "r1only")
GATE = (["tools/check_config.py"], ["tools/check_nonmatching.py"], ["tools/tests/test_raidou_c.py"],
        ["tools/tests/test_docs_index.py"], ["tools/docs_index.py", "--check"], ["tools/revisit_names.py", "--check"])
NONMATCHING_NAME = re.compile(r"^nonmatching (\w+), 0x", re.M)
CACHE = Path.home() / ".local/state/raidou-decomp-work/cache"


class Run:
    def __init__(self, args: argparse.Namespace):
        self.tree = args.tree.resolve()
        self.batch = batch_files.load(args.batch, args.batches, args.batch_id, tuple(args.renames))
        self.work = (args.work or self.batch.dir / "land").resolve()
        self.work.mkdir(parents=True, exist_ok=True)
        self.jobs = args.jobs
        self.clean_gate = args.clean_gate
        self.cache = Path(args.cache).resolve() if args.cache else None
        self.python = str(self.tree / ".venv/bin/python")
        self.maps = {g: renames.name_map(self.batch.renames, g) for g in ("raidou1", "raidou2")}
        self.draft_names = self.maps["raidou1"] if args.rename_drafts else {}
        self.notes: list[str] = []
        self.promoted: set[str] = set()
        self.lines: list[str] = []
        self.base = ""
        self.store = Path(args.store) if args.store else None
        self.gate_passed: bool | None = None

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

    def current(self, name: str, game: str = "raidou1") -> str:
        """The name a batch row has in the tree once renames.tsv is applied."""
        return self.maps[game].get(name, name)

    def is_r1only(self, row: batch_files.Row) -> bool:
        """r1only.txt names the row by its batch name or by a name renames.tsv gives it."""
        return bool({row.name, self.current(row.name), self.current(row.name, "raidou2")} & self.batch.r1only)

    def finals(self) -> dict[str, str]:
        """Row name -> final draft, with the function's own name as renamed (drafts may predate renames.tsv)."""
        out = {}
        for name in self.batch.rows:
            names = self.draft_names or ({name: self.current(name)} if self.current(name) != name else {})
            if (text := self.batch.final(name, names)) is not None:
                out[name] = text
        return out

    # steps

    def prepare(self, base: str | None, resume: bool, configure: bool = True) -> None:
        """Check out the base in a clean tree; a resumed run (patch skipped) keeps the tree as it is.
        The base revision is kept in base.txt: every before measurement is of that revision."""
        saved = self.work / "base.txt"
        if resume:
            rev = base or (saved.read_text().strip() if saved.exists() else "")
            if not rev:
                raise SystemExit("land_batch: a resumed run needs --base or the first run's base.txt")
            self.base = self.git("rev-parse", rev).strip()
            saved.write_text(self.base + "\n")
            self.say(f"tree {self.tree}: resuming on the edited tree, base {self.base[:9]}")
            return
        if self.git("status", "--porcelain", "--untracked-files=no").strip():
            raise SystemExit(f"land_batch: {self.tree} has uncommitted changes")
        if base:
            self.git("checkout", "-q", "--detach", base)
        self.base = self.git("rev-parse", "HEAD").strip()
        saved.write_text(self.base + "\n")
        self.say(f"tree {self.tree} at {self.base[:9]}")
        if configure and not self.configure("configure.base.log"):
            raise SystemExit("land_batch: configure failed at the base, see configure.base.log")

    def cache_path(self) -> Path | None:
        """Base measurements shared by every batch and rerun, keyed by the base and the games measured."""
        if not self.cache:
            return None
        games = "+".join(header_includers.versions(self.tree))
        return self.cache / "land_before" / f"{self.base}.{games}.json"

    def load_before(self) -> dict:
        """The base measurement, if it is of this base, from the work dir and the cache; it is extended, never
        replaced."""
        path = self.work / "before.json"
        before = json.loads(path.read_text()) if path.exists() else {}
        before = before if before.get("!base") == self.base else {}
        cached = self.cache_path()
        if cached and cached.exists():
            before = {**json.loads(cached.read_text()), **before}
        return before

    def save_before(self, before: dict) -> None:
        before["!base"] = self.base
        (self.work / "before.json").write_text(json.dumps(before, indent=1, sort_keys=True) + "\n")
        if not (cached := self.cache_path()):
            return
        # A failed compile is not cached: it may be the run's environment rather than the base.
        merged = json.loads(cached.read_text()) if cached.exists() else {}
        merged.update({k: v for k, v in before.items() if k == "!base" or "!error" not in v})
        cached.parent.mkdir(parents=True, exist_ok=True)
        tmp = cached.with_name(f"{cached.name}.{os.getpid()}.tmp")
        tmp.write_text(json.dumps(merged, indent=1, sort_keys=True) + "\n")
        os.replace(tmp, cached)

    def tree_at_base(self) -> bool:
        return self.git("rev-parse", "HEAD").strip() == self.base and not self.git(
            "status", "--porcelain", "--", "src", "include", "config").strip()

    def base_tree(self) -> Path:
        """A configured worktree of the base, for units the first measurement did not cover."""
        path = self.work / "base-tree"
        if not path.exists():
            self.git("worktree", "add", "--detach", str(path), self.base)
        elif subprocess.run(["git", "rev-parse", "HEAD"], cwd=path, capture_output=True,
                            text=True).stdout.strip() != self.base:
            subprocess.run(["git", "checkout", "-q", "--detach", self.base], cwd=path, check=True)
        inputs = [Path("orig") / p.name for p in (self.tree / "orig").iterdir() if not p.name.startswith(".")]
        for link in [Path(".venv"), *inputs, Path("tools/bin"), Path("tools/compilers"), Path("tools/glibc32")]:
            if (self.tree / link).exists() and not (path / link).exists():
                (path / link).symlink_to((self.tree / link).resolve())
        stamp = path / "build" / "land_base.txt"
        if not stamp.exists() or stamp.read_text().strip() != self.base:
            if not self.configure("configure.base-tree.log", path):
                raise SystemExit("land_batch: configure failed in the base worktree, see configure.base-tree.log")
            stamp.write_text(self.base + "\n")
        return path

    def measure_base(self, units: dict[Path, list[str]], before: dict) -> dict:
        """Add the base measurement of the units `before` does not hold, from this tree while it is still the
        base, else from the base worktree."""
        missing = {p: g for p, g in units.items()
                   if not all(f"{p.as_posix()} {x} {m}" in before for x in g for m in header_includers.MODES)}
        if not missing:
            self.say(f"regress: all {len(units)} units measured at base {self.base[:9]} already")
            return before
        root = self.tree if self.tree_at_base() else self.base_tree()
        self.say(f"regress: measuring {len(missing)} units at base {self.base[:9]} in {root}")
        before = {**before, **header_includers.measure(root, missing, self.jobs)}
        self.save_before(before)
        return before

    def changed_files(self) -> tuple[list[Path], list[Path]]:
        """(headers, units) that differ from the base in the tree, untracked ones included."""
        names = self.git("diff", "--name-only", self.base, "--", "include", "src").split()
        names += self.git("ls-files", "--others", "--exclude-standard", "--", "include", "src").split()
        paths = sorted({Path(n) for n in names if (self.tree / n).exists()})
        return [p for p in paths if p.suffix == ".h"], [p for p in paths if p.suffix == ".c"]

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
        """Measure the units the patch and renames will reach while the tree is the base; regress adds the rest."""
        headers = self.edited_headers()
        units = self.regress_units(headers)
        before = self.load_before()
        self.say(f"regress: {len(headers)} headers to edit, {len(units)} units, {len(before)} checks kept")
        self.measure_base(units, before)

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
        one_game = {n for r in self.batch.rows.values() if self.is_r1only(r)
                    for n in (r.name, self.current(r.name), self.current(r.name, "raidou2"))}
        try:
            rows, added = renames.complete(self.tree, self.batch.renames, one_game)
        except ValueError as e:
            raise SystemExit(f"land_batch: refused: {e}")
        for game, old, new, _ in added:
            self.say(f"renames: added {game} {old} -> {new}, a name both games share")
        if added:
            self.batch.renames = rows
            self.maps = {g: renames.name_map(rows, g) for g in ("raidou1", "raidou2")}
            self.draft_names = self.maps["raidou1"] if self.draft_names else {}
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

    def store_entries(self) -> list[draft_store.Entry]:
        """Draft store entries of the batch's rows, by address, else by name."""
        if not self.store:
            return []
        entries = draft_store.load(self.store, "raidou1")
        by_name = {e.name: e for e in entries.values()}
        out = []
        for row in self.batch.rows.values():
            e = entries.get(int(row.addr, 16)) if re.fullmatch(r"[0-9A-Fa-f]{8}", row.addr) else None
            e = e or by_name.get(row.name) or by_name.get(self.current(row.name))
            if e:
                out.append(e)
        return out

    def add_includes(self) -> None:
        """D/includes.tsv (source, header): includes the units need for declarations moved into headers."""
        for source, header in (r[:2] for r in self.batch.tsv("includes.tsv") if len(r) >= 2):
            path = self.tree / source
            if not path.exists():
                self.note(f"includes.tsv: no {source}")
                continue
            text = path.read_text()
            if header in [d.name for d in decls.parse(text) if d.kind == "include"]:
                continue
            lines = text.split("\n")
            last = max((i for i, l in enumerate(lines) if decls.INCLUDE.match(l)), default=-1)
            lines.insert(last + 1, f'#include "{header}"')
            path.write_text("\n".join(lines))
            self.say(f"splice: {source}: #include \"{header}\"")

    def declaration_problems(self) -> list[str]:
        """Declarations the batch adds to its units' .c files: a draft store entry's declarations, a function
        prototype whose function the unit does not define, an #include below the unit's first declaration."""
        store: set[str] = set()
        for e in self.store_entries():
            store |= {d.norm() for d in decls.parse_lines(e.decls) if d.kind != "include"}
        out = []
        for source in sorted({r.source for r in self.batch.rows.values()}):
            path = self.tree / source
            if not path.exists():
                continue
            old = self.git("show", f"{self.base}:{source}", check=False) if self.base else ""
            before: dict[str, int] = {}
            for d in decls.parse(unit_text.substitute(old, self.maps["raidou1"])):
                before[d.norm()] = before.get(d.norm(), 0) + 1
            after = decls.parse(path.read_text())
            defined = {d.name for d in after if d.kind == "def"}
            head = True
            for d in after:
                head = head and d.text.startswith("#")
                key = d.norm()
                if before.get(key):
                    before[key] -= 1
                    continue
                where = f"{source}:{d.line}"
                if d.kind == "include" and not head:
                    out.append(f"{where}: #include below the unit's first declaration: {d.text}")
                elif d.kind != "include" and key in store:
                    out.append(f"{where}: draft store declaration left in the .c: {' '.join(d.text.split())}")
                elif d.kind == "proto" and d.name not in defined:
                    out.append(f"{where}: local prototype: {' '.join(d.text.split())}")
        return out

    def splice(self) -> None:
        self.add_includes()
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
                text = self.splice_row(source, text, row, finals[row.name])
            path.write_text(text)
            self.say(f"splice: {source}: {len(rows)} functions")
        missing = sorted(set(self.batch.rows) - set(finals))
        if missing:
            self.say(f"splice: {len(missing)} rows without a draft: {' '.join(missing)}")
        problems = self.declaration_problems()
        for line in problems:
            self.say(f"refused: {line}")
        if problems:
            (self.work / "report.txt").write_text("\n".join(self.lines) + "\n")
            raise SystemExit(f"land_batch: refused, {len(problems)} declarations in .c files belong in headers "
                             "(tools/batch/place_decls.py)")
        self.say("splice: no local declarations added")

    def splice_row(self, source: str, text: str, row: batch_files.Row, final: str) -> str:
        """Splice at the INCLUDE_ASM of the renamed function, else of its batch name."""
        names = dict.fromkeys([self.current(row.name), self.current(row.name, "raidou2"), row.name])
        error = None
        for name in names:
            try:
                return unit_text.splice(text, name, final, self.is_r1only(row))
            except ValueError as e:
                error = error or e
        self.note(f"{source}: {error}")
        return text

    def r1only(self) -> None:
        """Guard shared-unit drafts named in r1only.txt after they were spliced, so a line added on a resumed run
        applies; Raidou 2 gets the INCLUDE_ASM back."""
        finals = self.finals()
        known = {n for r in self.batch.rows.values() for n in (r.name, self.current(r.name), self.current(r.name, "raidou2"))}
        for name in sorted(self.batch.r1only - known):
            self.note(f"r1only: {name} is not a row of the batch")
        for row in self.batch.rows.values():
            if not (self.is_r1only(row) and row.name in finals and row.source.startswith("src/shared/")):
                continue
            path = self.tree / row.source
            text = path.read_text() if path.exists() else ""
            bodies = [b.rstrip("\n") for b in (finals[row.name], unit_text.unwrap(finals[row.name])) if b]
            at = next((text.find(b) for b in bodies if b in text), -1)
            if at < 0:
                if not unit_text.asm_line(self.current(row.name)).search(text):
                    self.note(f"r1only: {row.name}: draft not found in {row.source}")
                continue
            if unit_text.only_raidou1(text, text.count("\n", 0, at)):
                continue
            body = next(b for b in bodies if text.startswith(b, at))
            asm = unit_text.asm_call(row.unit, self.current(row.name, "raidou2"))
            path.write_text(f"{text[:at]}#ifdef VERSION_RAIDOU1\n{body}\n#else\n{asm}\n#endif{text[at + len(body):]}")
            self.say(f"r1only: {row.source}: {row.name} guarded, Raidou 2 keeps the INCLUDE_ASM")

    def gap_unit(self, row: batch_files.Row, finals: dict[str, str]) -> None:
        asm = self.tree / "asm" / row.game / f"{row.unit}.s"
        asm_names = NONMATCHING_NAME.findall(asm.read_text())
        names = [self.maps[row.game].get(n, n) for n in asm_names]
        pre = self.batch.dir / "units" / f"{row.unit}.pre"
        if not pre.exists():
            self.note(f"{row.unit}: no units/{row.unit}.pre, wrote #include \"common.h\" only")
        by_name = {self.current(n, row.game): f for n, f in finals.items()}
        text = unit_text.gap_unit(pre.read_text() if pre.exists() else '#include "common.h"\n', row.unit, names,
                                  by_name)
        (self.tree / row.source).parent.mkdir(parents=True, exist_ok=True)
        (self.tree / row.source).write_text(text)
        # a NON_MATCHING draft is still its INCLUDE_ASM in the matching build
        c_funcs = {a for a, n in zip(asm_names, names) if n in by_name and unit_text.unwrap(by_name[n]) is None}
        plans, notes = segments.plan_unit(self.tree, row.game, row.unit, c_funcs)
        if not segments.make_c(self.tree, row.game, row.unit):
            self.note(f"{row.unit}: no asm subsegment in the {row.game} yaml")
        yaml = segments.yaml_path(self.tree, row.game)
        for p in plans:
            try:
                yaml.write_text(segments.apply_plan(yaml.read_text(), p))
            except ValueError as e:
                self.note(str(e))
                continue
            rest = f", {p.kind} resumes at 0x{p.rest:08X}" if p.rest is not None else ""
            left = f", {', '.join(p.left)} left in the chunk for alignment" if p.left else ""
            only = ", only the labels its C uses" if p.c_only else ""
            self.say(f"segments: {row.unit} owns {p.kind} of {p.chunk} from 0x{p.start:08X}{rest}{left}{only}")
        for n in notes:
            self.note(n)
        drafted = sum(n in by_name for n in names)
        self.say(f"splice: {row.source}: new unit, {len(names)} functions, {drafted} from drafts")

    def check(self, source: str, game: str, alt: Path | None = None) -> tuple[int, dict[str, str]]:
        cmd = [self.python, "tools/check_unit.py", source, "--version", game] + (["--source", str(alt)] if alt else [])
        r = subprocess.run(cmd, cwd=self.tree, capture_output=True, text=True)
        return r.returncode, header_includers.summarise(r.stdout + r.stderr)

    def check_games(self, source: str, games: list[str], alt: Path | None = None) -> dict:
        """check() on each game at once, in the order of `games`."""
        with ThreadPoolExecutor(max(len(games), 1)) as pool:
            return dict(zip(games, pool.map(lambda g: self.check(source, g, alt), games)))

    def check_matches(self, source: str, summaries: dict[str, dict[str, str]], finals: dict[str, str]) -> None:
        """Report rows spliced as plain C that are not OK in a game that builds them."""
        for game, summary in summaries.items():
            for row in self.batch.rows.values():
                if row.source != source or row.name not in finals or unit_text.unwrap(finals[row.name]) is not None:
                    continue
                if game != "raidou1" and self.is_r1only(row):
                    continue
                # absent from a unit that compiled: this game does not build the function
                name = self.current(row.name, game)
                status = summary.get(name, "OK" if "!error" not in summary else summary["!error"])
                if status != "OK":
                    self.note(f"{source} {game}: {row.name} recorded as a match but is {status}")

    def promote(self) -> None:
        """A NON_MATCHING draft that matches and leaves no other function of its unit worse becomes plain C."""
        finals = self.finals()
        games = header_includers.unit_paths(self.tree)
        for source in sorted({r.source for r in self.batch.rows.values() if r.name in finals}):
            unit_games = games.get(Path(source), [])
            base = self.check_games(source, unit_games)
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
                    results = {g: r[1] for g, r in self.check_games(source, unit_games, alt).items()}
                # the draft matches and no other function of the unit gets worse
                name = self.current(row.name)
                matches = [s.get(name) for s in results.values() if name in s]
                worse, _ = header_includers.compare({g: base[g][1] for g in unit_games}, results)
                worse = [w for w in worse if f": {name} " not in w]
                if matches and all(m == "OK" for m in matches) and not worse:
                    text = trial
                    self.promoted.add(row.name)
                    self.say(f"promote: {row.name} matches in {source}")
                elif matches and all(m == "OK" for m in matches):
                    self.say(f"promote: {row.name} matches alone but not in {source}: {'; '.join(worse)[:300]}")
            (self.tree / source).write_text(text)
        if self.promoted:
            self.configure()

    def configure(self, log: str = "configure.log", tree: Path | None = None) -> bool:
        """configure.py, then the eeasm copies of the split asm that check_unit includes."""
        tree = tree or self.tree
        with open(self.work / log, "w") as out:
            rc = subprocess.run([self.python, "configure.py"], cwd=tree, stdout=out, stderr=subprocess.STDOUT).returncode
        if rc:
            self.note(f"configure failed, see {log}")
            return False
        targets = subprocess.run(["ninja", "-t", "targets", "rule", "eeasm"], cwd=tree,
                                 capture_output=True, text=True).stdout.split()
        with open(self.work / log, "a") as out:
            rc = subprocess.run(["ninja", *targets], cwd=tree, stdout=out, stderr=subprocess.STDOUT).returncode
        if rc:
            self.note(f"ninja of the eeasm files failed, see {log}")
            return False
        self.say("configure: ok")
        return True

    def docs(self) -> None:
        finals = self.finals()
        drafted = {n for n in finals if n not in self.promoted and unit_text.unwrap(finals[n]) is not None}
        for line in matching_docs.drop_entries(self.tree, {self.current(n) for n in finals}):
            self.say(f"docs: dropped {line}")
        pages: dict[str, list[str]] = {}
        for name in sorted(drafted, key=list(self.batch.rows).index):
            row = self.batch.rows[name]
            if name not in self.batch.docs:
                self.note(f"docs: {name} is NON_MATCHING and has no docs.json entry")
                continue
            unit, text, lever = (self.batch.docs[name] + [""] * 3)[:3]
            games = ["raidou1"] if row.gap or self.is_r1only(row) or row.source.startswith("src/raidou1/") \
                else ["raidou1", "raidou2"]
            fn = self.current(name)
            measured = [matching_docs.measure(self.tree, row.source, g, fn) for g in games]
            # The page is the unit with its folder (event/evtMoon), as docs_index.py files it.
            if unit and unit != row.unit and unit != Path(row.unit).name:
                self.note(f"docs: {name}: docs.json unit {unit} is not {row.unit}; filed under {row.unit}")
            unit = row.unit
            pages.setdefault(unit, []).append(matching_docs.entry(fn, unit, measured, text, lever))
            self.say(f"docs: {fn}: {' '.join(l for _, l in measured)}")
        for unit, entries in pages.items():
            matching_docs.append(self.tree, unit, entries)
        # Every entry is checked against the tree, so drafts promoted outside final/ lose theirs too.
        dropped, left = matching_docs.drop_stale(self.tree)
        for line in dropped:
            self.say(f"docs: dropped {line} (no longer a draft)")
        for line in left:
            self.note(f"docs: {line}")
        if self.py("tools/docs_index.py", log="docs_index.log"):
            self.note("docs_index.py failed, see docs_index.log")

    def regress(self) -> None:
        """Every unit that includes a header edited since the base, every edited unit and the batch's units,
        against the base."""
        headers, edited = self.changed_files()
        units = self.regress_units(headers)
        all_units = header_includers.unit_paths(self.tree)
        units.update({p: all_units[p] for p in edited if p in all_units})
        units = dict(sorted(units.items()))
        self.say(f"regress: {len(headers)} headers edited since the base: {' '.join(map(str, headers))}"[:400])
        (self.work / "units.json").write_text(json.dumps({p.as_posix(): g for p, g in units.items()}, indent=1) + "\n")
        before = self.measure_base(units, self.load_before())
        keys = {f"{p.as_posix()} {g} {m}" for p, gs in units.items() for g in gs for m in header_includers.MODES}
        before = {k: {self.maps[k.split()[1]].get(f, f): s for f, s in v.items()} for k, v in before.items()
                  if k in keys}
        after = header_includers.measure(self.tree, units, self.jobs)
        (self.work / "after.json").write_text(json.dumps(after, indent=1, sort_keys=True) + "\n")
        worse, better = header_includers.compare(before, after)
        worse, fragile = header_includers.split_fragile(worse, docs_index.fragile_names())
        batch = set(self.batch.rows) | {self.current(n, g) for n in self.batch.rows for g in self.maps}
        for line in better:
            self.say(f"regress: better {line}")
        for line in fragile:
            self.say(f"regress: info fragile {line}")
        for line in worse:
            fn = line.split(": ")[1].split()[0] if ": " in line else ""
            if fn in batch:
                self.say(f"regress: worse (batch) {line}")
            else:
                self.note(f"regress: worse {line}")
        self.say(f"regress: {len(units)} units, {len(worse)} worse, {len(better)} better, {len(fragile)} fragile")

    def gate(self) -> None:
        """configure and ninja, with --clean only when asked: the incremental build proves the same image, and the
        clean one runs once, before the push."""
        configure = "configure.py --clean" if self.clean_gate else "configure.py"
        rc = subprocess.run(f"{self.python} {configure} > {self.work}/gate.configure.log 2>&1 && "
                            f"ninja > {self.work}/gate.ninja.log 2>&1", shell=True, cwd=self.tree).returncode
        log = (self.work / "gate.ninja.log").read_text() if (self.work / "gate.ninja.log").exists() else ""
        sums = len(re.findall(r"check config/\w+/checksum.sha1", log))
        self.say(f"gate: {configure} && ninja rc {rc}, {sums} checksum steps")
        failed = rc != 0
        for args in GATE:
            if not (self.tree / args[0]).exists():
                continue
            r = self.py(*args, log=f"gate.{Path(args[0]).stem}.log")
            self.say(f"gate: {' '.join(args)} rc {r}")
            failed |= r != 0
        problems = self.declaration_problems()
        for line in problems:
            self.say(f"gate: refused: {line}")
        failed |= bool(problems)
        warnings = len(re.findall(r"warning:", log))
        scope = "the build log" if self.clean_gate else "the objects rebuilt (the total needs --clean-gate)"
        self.say(f"gate: {warnings} compiler warnings in {scope}; {'FAILED' if failed else 'passed'}")
        self.gate_passed = not failed

    def retire(self) -> None:
        """Mark the draft store entries of the spliced functions landed, once the gate passed with nothing to decide."""
        if not self.store:
            return
        if not self.gate_passed or self.notes:
            self.say("retire: skipped, the gate did not pass in this run" if not self.gate_passed
                     else f"retire: skipped, {len(self.notes)} cases for a hand decision")
            return
        finals = self.finals()
        landed = {e.addr for e in self.store_entries()
                  if any(r.name in finals and (r.name == e.name or self.current(r.name) == e.name
                                               or r.addr.upper() == f"{e.addr:08X}") for r in self.batch.rows.values())}
        done = draft_store.retire(self.store, "raidou1", sorted(landed), self.batch.id)
        self.say(f"retire: {len(done)} draft store entries landed" + (f": {' '.join(e.name for e in done)}" if done else ""))


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
                    help="skip the first base measurement; regress still measures, at the base, units it lacks")
    ap.add_argument("--cache", default=str(CACHE),
                    help=f"base measurements shared across batches and reruns (default {CACHE}; '' for none)")
    ap.add_argument("--clean-gate", action="store_true",
                    help="gate with configure.py --clean, as the last gate before a push does")
    ap.add_argument("--store", default=str(draft_store.STORE),
                    help="draft store whose entries the batch lands (tools/draft_store.py; '' for none)")
    ap.add_argument("--dry-run", action="store_true",
                    help=f"run only {', '.join(DRY_STEPS)} and the declaration check, without configure; leaves T edited")
    ap.add_argument("-j", "--jobs", type=int, default=header_includers.JOBS)
    args = ap.parse_args()
    skip = set(filter(None, args.skip.split(",")))
    if skip - set(STEPS):
        ap.error(f"unknown steps: {', '.join(sorted(skip - set(STEPS)))}")
    run = Run(args)
    if args.dry_run:
        run.prepare(args.base, "patch" in skip, configure=False)
        for step in DRY_STEPS:
            if step not in skip:
                getattr(run, step)()
        run.say("dry run: stopped after splice; T is edited")
        (run.work / "report.txt").write_text("\n".join(run.lines) + "\n")
        return 1 if run.notes else 0
    run.prepare(args.base, "patch" in skip)
    if "regress" not in skip and not args.reuse_before and run.tree_at_base():
        run.measure_before()
    for step in STEPS:
        if step not in skip:
            getattr(run, step)()
    (run.work / "report.txt").write_text("\n".join(run.lines) + "\n")
    print(f"{len(run.notes)} cases for a hand decision; report in {run.work / 'report.txt'}")
    return 1 if run.notes else 0


if __name__ == "__main__":
    sys.exit(main())
