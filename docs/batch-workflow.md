# Batch workflow

A batch is a group of functions drafted together outside the repository and
landed in one commit. Drafting happens in a batch directory next to a working
tree; `tools/batch/land_batch.py` then packages the batch into a clean checkout
of `origin/main`. It never commits.

## The batch directory

The directory name is the batch id. Its rows come from a `batches.tsv` (by
default two levels up: `<dir>/../../batches.tsv`) with the columns `batch`,
`name`, `addr`, `words`, `unit`, where `unit` is `src/<folder>/<unit>` for an
existing C unit or `asm/<game>/<unit>` for a gap unit that becomes C.

| File | Contents |
|---|---|
| `results.tsv` | `name words result minutes`, result `match` or `nonmatch N` (N differing words) |
| `final/<name>.c` | the text to splice: plain C, or the `NON_MATCHING` block with its comment |
| `<name>.c`, `comments.json` | used when `final/<name>.c` is missing: the draft, and the comment for a nonmatch |
| `headers.wip.patch` | `git diff` of the drafting tree's header and declaration edits, new files included |
| `renames.tsv` | `game old new reason`, applied in order (a chain `a -> b`, `c -> a` is fine); `--renames FILE` adds a table |
| `drop_symbols.tsv` | `game name`: labels to delete, such as data that a C function now defines |
| `r1only.txt` | shared-unit functions whose Raidou 2 body differs; Raidou 2 keeps the `INCLUDE_ASM` |
| `units/<unit>.pre` | the includes and declarations that open a new gap unit |
| `docs.json` | `name: [unit, text, lever]` for each NON_MATCHING draft's docs/matching entry |

The drafts and tables use the names after the renames (the drafting worker
renames them when it writes `renames.tsv`). Pass `--rename-drafts` when they
still use the old names. Either way a row is spliced at the `INCLUDE_ASM` of
its renamed function, and its draft's own name is renamed. A `docs.json` unit
is the unit with its folder (`event/evtMoon`); the batches.tsv unit wins.

## Pseudocode

The drafting worker reads Hex-Rays pseudocode from the shared IDBs. Three
tools in `tools/ida/` make it closer to the decomp's C:

| Tool | Use |
|---|---|
| `ee32.py` | Hex-Rays pass loaded in IDA (`_install()`): compares and branches on the low 32 bits of a register, so ee-gcc's sign-extended ints stop decompiling as `__int64`. Wrong for real 64-bit compares. |
| `stylefix.py` | text filter (`stylefix.py -`): `s32`/`u8`/`f32` types, `NULL`, no `__fastcall` or `LL` suffixes, `struct TAG` where the decomp writes it |
| `propose_protos.py TSV OUT.json` | prototype candidates for undrafted functions: HD signatures and matched shape neighbours, never the function's own definition |

## Landing a batch

```sh
git -C T fetch origin
.venv/bin/python tools/batch/land_batch.py --batch DIR --tree T --base origin/main
```

T is a clean checkout with `.venv`, `orig/` and the compilers linked in. Steps,
in order (skip any with `--skip step,...`):

1. Check out `--base` detached and configure. Measure every unit that includes
   a header the patch edits, or a header that holds a renamed name, plus the
   batch's units: `check_unit.py` on each game, default build and
   `-DNON_MATCHING`, into `<batch>/land/before.json`, with the base revision in
   `base.txt`. A measurement of the same base is kept and extended, never
   replaced, so a resumed run cannot measure the edited tree as its base.
   Measurements are also kept in
   `~/.local/state/raidou-decomp-work/cache/land_before/<base>.<games>.json`
   (`--cache DIR`, `--cache ''` for none), so a rerun or another batch on the
   same base measures only the units it has not seen.
2. `patch`: `git apply` the header patch, with `--3way` when it does not apply
   cleanly.
3. `renames`: symbol_addrs, smt3 aliases at the same address, name_sources rows
   (tier from the reason), the carried config tables and the pair tables of
   both games, then `src`, `include` and `docs`; then `drop_symbols.tsv` and
   `revisit_names.py`. It lists derived data labels used only by a batch
   function as candidates for `drop_symbols.tsv`.
4. `splice`: each final draft replaces its `INCLUDE_ASM` (and any older
   NON_MATCHING draft around it). A gap unit is written from its `.pre` and its
   functions in address order, its subsegment and tu_evidence row become `c`,
   and it takes the `.rodata` and `.lit4` labels that only its asm uses, when
   they form one run in one unowned chunk. Anything else (a label another unit
   uses, labels in several chunks) is reported for a hand decision.
5. `promote`: a NON_MATCHING draft whose C leaves `check_unit.py` passing for
   the whole unit, on every game that builds it, becomes plain C. A draft that
   matches alone but changes another function of the unit stays a draft. A
   `match` row that is not OK in its unit is reported.
6. `configure`: places the INCLUDE_RODATA and INCLUDE_SDATA lines.
7. `docs`: drops the docs/matching entries of every batch function and writes
   one for each remaining NON_MATCHING draft, measured with `-DNON_MATCHING`.
   Then every entry in docs/matching is checked against the tree, and an entry
   whose function is no longer a draft (promoted outside `final/`, or by a
   header change) is dropped. Then `docs_index.py`.
8. `regress`: every unit that includes a header changed since the base
   (`git diff` plus untracked files, through `header_includers.py`), every
   changed unit and the batch's units, against the base. Units step 1 did not
   measure are measured at the base in a worktree, `<batch>/land/base-tree`.
   It lists each function that got worse or better (old names mapped through
   the renames), counting a function that compiles differently inside the full
   unit (`CONTEXT`) and new unit-level problems. A worse function of the batch
   itself is marked as such; any other is a regression to fix.
9. `gate`: `configure.py && ninja`, then `check_config.py`,
   `check_nonmatching.py`, `test_raidou_c.py`, `test_docs_index.py`,
   `docs_index.py --check` and `revisit_names.py --check`. With
   `--clean-gate` it runs `configure.py --clean` first and counts every
   compiler warning; without it the count covers only the objects rebuilt.

The report goes to stdout and `<batch>/land/report.txt`; lines starting
with `DECIDE` need a hand decision, and the exit status is 1 when there are
any. Fix them in T, then rerun only the gate on the edited tree (skipping
`patch` skips the clean-tree check and the checkout, and takes the base from
`base.txt` or `--base`):

```sh
.venv/bin/python tools/batch/land_batch.py --batch DIR --tree T \
    --skip patch,renames,splice,configure,promote,docs,regress
```

Every gate while drafting and packaging is incremental: `configure.py` (3 s
when splat's inputs are unchanged) and `ninja` prove the same image as a clean
build, since ninja relinks and re-checksums whatever an edit reaches. The
gate before `git push` is the one clean build, `--clean-gate` here or
`configure.py --clean && ninja` by hand, because a stale `asm/` once hid
missing names. After the push, print the CI run id and stop; the orchestrator
checks CI.

`check_unit.py` runs on both games at once in `promote`, and the base and
regress measurements run `-j` (default: the CPU count) check_units at a time.

A run that stops partway leaves T edited; reset it (`git checkout -- .` and
delete the new units by name) before starting again, or resume with the steps
already done skipped. `git worktree remove land/base-tree` cleans up the base
worktree.

Reproducing R06 from its parent (`--base 3243932^`) gives commit 3243932 except
seven hand edits: two type renames, one SIZE_CHECK, and three include fixes,
two of which the regress step reports as compile failures.

## Header edits during drafting

```sh
.venv/bin/python tools/header_includers.py include/sbtlWork.h
.venv/bin/python tools/header_includers.py sbtlWork.h --save build/sbtlWork.before.json --check
# edit the header
.venv/bin/python tools/header_includers.py sbtlWork.h --baseline build/sbtlWork.before.json
```

The first form lists every unit that includes the header directly or through
another header, with the games that compile it. `--check` runs `check_unit.py`
on each in the default build and with `-DNON_MATCHING`; with `--baseline FILE`
or `--baseline-tree DIR` it prints every function that got worse or better and
exits 1 on a regression. Run it after each header edit rather than at
packaging.
