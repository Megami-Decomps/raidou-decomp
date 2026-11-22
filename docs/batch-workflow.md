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
A function drafted for both games has two rows, one per game's unit; the
Raidou 2 row's key is `NAME@raidou2` in `results.tsv`, `final/`, `docs.json`
and `comments.json`, and `NAME@raidou2.c` falls back to `NAME.c` when both
games take the same C.

| File | Contents |
|---|---|
| `results.tsv` | `name words result minutes`, result `match` or `nonmatch N` (N differing words) |
| `final/<name>.c` | the text to splice: plain C, or the `NON_MATCHING` block with its comment |
| `<name>.c`, `comments.json` | used when `final/<name>.c` is missing: the draft, and the comment for a nonmatch (a draft already in a `NON_MATCHING` block is rewrapped, not wrapped twice) |
| `headers.wip.patch` | `git diff` of the drafting tree's header and declaration edits, new files included |
| `renames.tsv` | `game old new reason`, applied in order (a chain `a -> b`, `c -> a` is fine); `--renames FILE` adds a table |
| `drop_symbols.tsv` | `game name`: labels to delete, such as data that a C function now defines |
| `r1only.txt` | shared-unit functions whose Raidou 2 body differs, one per line by batch or renamed name, a reason may follow; Raidou 2 keeps the `INCLUDE_ASM` |
| `units/<unit>.pre` | the includes and declarations that open a new gap unit |
| `docs.json` | `name: [unit, text, lever]` for each NON_MATCHING draft's docs/matching entry |
| `<name>.decls` | the declarations a draft store entry assumes, one per line, from `find_body.py --write` |
| `store_decls.tsv`, `includes.tsv` | where `place_decls.py` put each of them; `source header` rows of includes a unit needs |

The drafts and tables use the names after the renames (the drafting worker
renames them when it writes `renames.tsv`). Pass `--rename-drafts` when they
still use the old names. Either way a row is spliced at the `INCLUDE_ASM` of
its renamed function, and its draft's own name is renamed. A `docs.json` unit
is the unit with its folder (`event/evtMoon`); the batches.tsv unit wins.

## Draft store pick-up

`find_body.py --batch B --write D` takes a draft store entry
(`tools/draft_store.py`) like any other body; `--set-column` marks it in
`body_source` with the cluster as VIA (`match:store:matched:C0022:1.00`). The
entry's declarations go to `D/<name>.decls`, never into `D/<name>.c`. Then

```sh
.venv/bin/python tools/batch/place_decls.py D --tree T
```

puts each one in a header of T: present (include/ declares it already),
include (declared in a header the unit does not reach, which joins the unit's
includes), placed (the owning unit's `include/<unit>.h`, else the header that
declares its neighbours in its asm or data file, when that header sees every
type it uses, else the drafting unit's own `include/<unit>.h`, created when
missing), differs (include/ declares it another way and wins) or required (a
header edit for the batch handoff). Its exit status is 1 when any is required.
The header edits travel in `headers.wip.patch`; the includes a C unit needs go
in `includes.tsv`, which land_batch's splice applies, and a gap unit's go in
its `units/<unit>.pre`.

Entries in no planned batch get batches of their own:
`draft_store.py plan --append batches.tsv --write DIR` writes one batch
`S-<unit>` per unit (rows, `<name>.c`, `<name>.decls`, `results.tsv` from the
store status), skipping entries already in batches.tsv, already C or not
compiling. After a land with a passing gate, land_batch moves the batch's
entries to the store's `landed/`, so they are not offered again;
`draft_store.py check` lists entries whose function is already C.

## Pseudocode

The drafting worker reads Hex-Rays pseudocode from the shared IDBs. The tools
in `tools/ida/` make it closer to the decomp's C:

| Tool | Use |
|---|---|
| `ee32.py` | Hex-Rays pass loaded in IDA (`_install()`): compares and branches on the low 32 bits of a register, so ee-gcc's sign-extended ints stop decompiling as `__int64`. Wrong for real 64-bit compares. |
| `stylefix.py` | text filter (`stylefix.py -`): `s32`/`u8`/`f32` types, `NULL`, no `__fastcall` or `LL` suffixes, `struct TAG` where the decomp writes it |
| `propose_protos.py TSV OUT.json` | prototype candidates for undrafted functions: HD signatures and matched shape neighbours, never the function's own definition |
| `shared.py`, `land_sync.sh [--game r1\|r2]`, `sync/` | sync the shared IDBs to a tree (names, types, prototypes, globals) and apply prototype proposals; data and `changes.log` live in `~/.local/state/raidou-decomp-work/ida-pseudo` |

ee-gcc truncates a decimal float literal toward zero, so the shortest
round-to-nearest decimal (Hex-Rays' `0.1`) can give the float one ulp below.
`tools/float_literal.py 0x3DCCCCCD` prints the literal that keeps the bits
(`0.100000002f`).

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
   both games, then `src`, `include` and `docs`, untracked files included; then `drop_symbols.tsv` and
   `revisit_names.py`. It lists derived data labels used only by a batch
   function as candidates for `drop_symbols.tsv`. A row for one game whose old
   name the other game also has gets the other game's row added, unless the
   other game's symbol is another function: the other game already has the new
   name, or the table has its keep row (`raidou2 old old reason`). Then each
   game's lines (by `src/<game>/`, `VERSION_` blocks and docs sections) take
   that game's name, a shared C line both games call gets a `VERSION_` guard,
   and a macro line that cannot take one is refused with its file and line, with
   nothing written. A shared name of a function in `r1only.txt` needs the other
   game's row. `tools/batch/renames.py TABLE` applies a table to a tree by hand.
4. `splice`: the includes in `includes.tsv` go after each unit's last
   `#include`; each final draft replaces its `INCLUDE_ASM` (and any older
   NON_MATCHING draft around it). When a shared unit holds the line twice, in
   a `VERSION_RAIDOU1` block and its `#else`, the Raidou 1 copy is replaced. A gap unit is written from its `.pre` and its
   functions in address order, its subsegment and tu_evidence row become `c`,
   and it takes the `.rodata` and `.lit4` labels that only its asm uses, when
   they form one run in one unowned chunk; several new units can split one
   chunk, in any order (a piece split off earlier in the run has no asm/data
   file yet, so its labels come from the file that covers its range). The `.rodata` starts at the first of those labels that
   the section's alignment allows (16 bytes when it holds a jump table, else 8);
   labels before it stay in the chunk. When that run is refused, the unit
   takes only the labels its plain C functions use (string literals and
   constants the compiler emits into its own `.rodata`), if those form a run;
   its asm functions' labels stay in the chunk. Anything else (a label another
   unit uses, labels in several chunks) is reported for a hand decision, with
   the first C label's address when the C needs it. The run
   stops (refused) when a unit's `.c` gained, against the base, a draft store
   entry's declaration, a function prototype for a function the unit does not
   define, or an `#include` below its first declaration: those belong in
   headers (`place_decls.py`). The gate repeats the check.
5. `r1only`: a shared-unit function in `r1only.txt` that is spliced as plain C
   goes under `#ifdef VERSION_RAIDOU1` with the `INCLUDE_ASM` in its `#else`,
   so a line added after the splice applies on a resumed run.
6. `promote`: a NON_MATCHING draft whose C leaves `check_unit.py` passing for
   the whole unit, on every game that builds it, becomes plain C. A draft that
   matches alone but changes another function of the unit stays a draft. A
   `match` row that is not OK in its unit is reported.
7. `configure`: places the INCLUDE_RODATA and INCLUDE_SDATA lines.
8. `docs`: drops the docs/matching entries of every batch function and writes
   one for each remaining NON_MATCHING draft, measured with `-DNON_MATCHING`.
   Then every entry in docs/matching is checked against the tree, and an entry
   whose function is no longer a draft (promoted outside `final/`, or by a
   header change) is dropped. Then `docs_index.py`.
9. `regress`: every unit that includes a header changed since the base
   (`git diff` plus untracked files, through `header_includers.py`), every
   changed unit and the batch's units, against the base. Units step 1 did not
   measure are measured at the base in a worktree, `<batch>/land/base-tree`.
   It lists each function that got worse or better (old names mapped through
   the renames), counting a function that compiles differently inside the full
   unit (`CONTEXT`) and new unit-level problems. A worse function of the batch
   itself is marked as such; any other is a regression to fix. A function whose
   docs/matching entry has a `Fragile:` line (it shifts with header declarations alone, so it is
   recorded NON_MATCHING) is listed as `info fragile`, not as a regression.
10. `gate`: `configure.py && ninja`, then `check_config.py`,
   `check_nonmatching.py`, `test_raidou_c.py`, `test_docs_index.py`,
   `docs_index.py --check` and `revisit_names.py --check`. With
   `--clean-gate` it runs `configure.py --clean` first and counts every
   compiler warning; without it the count covers only the objects rebuilt.
11. `retire`: when the gate passed in this run with nothing to decide, the
   draft store entries of the spliced functions move to `landed/` with the
   batch id (`--store DIR`, `--store ''` for none).

`--dry-run` runs only `patch`, `renames`, `splice` and `r1only` (with the declaration
check), without configuring, and leaves T edited.

The report goes to stdout and `<batch>/land/report.txt`; lines starting
with `DECIDE` need a hand decision, and the exit status is 1 when there are
any. Fix them in T, then rerun only the gate on the edited tree (skipping
`patch` skips the clean-tree check and the checkout, and takes the base from
`base.txt` or `--base`):

```sh
.venv/bin/python tools/batch/land_batch.py --batch DIR --tree T \
    --skip patch,renames,splice,r1only,configure,promote,docs,regress
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
exits 1 on a regression. It refuses a tree without `asm/<game>/nonmatchings`
and `build/eeasm/asm/<game>/nonmatchings` (configure.py and ninja not run
there), whose every full-unit check would fail. Run it after each header edit rather than at
packaging.
