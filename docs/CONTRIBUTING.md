# Contributing

This guide covers the loop for one function: pick it, decompile it, verify it.
The repository holds two games, `raidou1` and `raidou2`, built from one tree.

## Matching rules

The [dds-decomp matching rules](https://github.com/Megami-Decomps/dds-decomp/blob/main/docs/CONTRIBUTING.md#matching-rules)
are the rules here too, and so is
[smt3-decomp's contributing guide](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/CONTRIBUTING.md#matching-rules).
Accepted source preserves behavior and compiles to the exact retail bytes.

A fakematch is C that reaches the bytes by something other than a plausible
reconstruction: a raw pointer offset where a struct field exists, a dummy use
or an artificial dependency, a pinned register, a false type. A close
reconstruction stays `INCLUDE_ASM` with its best draft preserved (below).

Raw offsets usually mean a struct is wrong, not that the function is hard.
`include/` carries headers taken from smt3-decomp, and the Raidou layouts can
differ (`effBTLCut_t` is eight bytes longer here). Fix the type for Raidou,
with a per-version definition if the two games differ, and write the field
access. Never cast to `u8 *` and add the offset.

## 1. Pick a function

A function not done yet is an `INCLUDE_ASM(const s32, "<unit>", NAME);` line,
and its assembly is in `asm/<version>/nonmatchings/<unit>/NAME.s`. A unit that
is the same in both games is `src/shared/<unit>.c` and must match in both.

```sh
python3 tools/progress.py
```

Many functions exist in both games with the same code. `config/xgame/r1_r2_pairs.tsv`
lists them by address (`r1_addr`, `r2_addr`, name, whether the bytes are
identical or identical after masking relocations, and the unit in each version).
Match a function once, then put the same C in the other version's unit and check
both, so no function is matched twice. `config/xgame/r1_r2_data_pairs.tsv` pairs
the data symbols the same way.

### Names in both games

A function or data symbol both games have carries one name in both, so C moves
between versions unchanged:

- A real name in either version is the name in both.
- Otherwise the name is `func_r1_<addr>` or `D_r1_<addr>`, with the Raidou 1
  address. Splat's `func_<addr>` and `D_<addr>` are left only for symbols one
  game has, so an address name never means two things.
- The `r1_` names keep the `func_`/`D_` prefix the tools read as "not named yet".
  They name no unit, because unit names change while the symbol stays.

`tools/xgame/common_names.py pairs --from PAIRS` rebuilds both tables from the
matched build, `apply` writes the names into `config/<v>/` and renames `src/` and
`include/`, and `tools/check_config.py` fails when a pair has two names. A new
name for a paired symbol goes into both versions' `symbol_addrs.txt`.

Every name's source is in `config/<v>/name_sources.txt`. Inferred names carry
`method=..; p=..; tier=..` in the note: tier A has measured precision of at least
0.90, B at least 0.75, C below that or measured on fewer than 20 names, and D is
unmeasured (names derived from a user function, a string, a unit or a section).
`config/<v>/revisit_names.tsv` lists every tier B, C and D name, functions and
data, worst tier first; `tools/revisit_names.py` writes it from `name_sources.txt`
and `tools/check_config.py` fails when it is stale.

`config/<v>/ida_protos.txt` holds the prototypes the IDB analysis marked strong
(`tools/xgame/sig_leads.py`), as leads for whoever decompiles the function. Nothing
reads it.

### Type names

A struct, union or enum takes its SMT3 name (smt3-decomp), else its Raidou HD name
(raidouhd-reconstructed-sources), else a DDS name dds-decomp did not invent. Match
the type by layout and by the functions that use it, not by name. Otherwise name it
the way SMT3 and HD do:

- `<module><Name>_t`, with the module's lowercase prefix: `sprHandle_t`,
  `fldAreaWork_t`, `effBlurScaleWork_t`, `mnuPanelState_t`.
- Battle (`sbtl`) types use HD's uppercase form `SBTL<WORDS>_<KIND>`:
  `SBTLCAMERA_WORK`, `SBTLITEM_LISTWORK`.
- Members are lowerCamel (`texture`, `slots`); HD's Hungarian `p`/`h` prefixes
  (`pLink`, `hBillboard`) are kept where the member is HD's.
- A size check is `<name without _t>SizeCheck`.

Before starting, claim the unit in `config/<version>/claims.tsv` and set the row
to `done` when you stop, finished or not. The file is advisory and nothing in the
build reads it. Check it before you pick a unit.

## 2. Decompile

Replace the `INCLUDE_ASM` line with your C, in the same position (link order
matters). Leave `INCLUDE_RODATA` and `INCLUDE_SDATA` lines where they are:
`configure.py` recomputes their placement. [docs/shared-c.md](shared-c.md) covers
shared units and [docs/smt3-names.md](smt3-names.md) covers names carried over
from smt3-decomp. smt3-decomp has C for many of the same functions; copy the body,
drop `static`, and adapt types.
[smt3-decomp's matching levers](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/matching-levers.md)
list what to try when a function is close.
[docs/matching/](matching/README.md) records what was already tried on each
draft here; grep its `INDEX.tsv` for the function first.

### Non-matching C

Keep a draft that does not match in its unit, with the assembly fallback in
native order, and a comment saying how close it is:

```c
#ifdef NON_MATCHING
/* 2 of 41 instructions differ: the loop counter and the pointer swap
 * registers. Behavior is the same. */
void unfinished(void) {
    /* Reconstructed behaviour. */
}
#else
INCLUDE_ASM(const s32, "unit/path", unfinished);
#endif
```

The normal build selects the assembly; builds defining `NON_MATCHING` compile the
C. The closeness comment states the size difference (from `check_unit.py -v`) and
what differs: register choice, instruction order, block layout, a field offset the
header does not have yet. Non-matching C earns no matching credit. Preserve
retail's behavior, including its bugs, and declare every callee before first use.

`python3 tools/check_nonmatching.py` compiles every unit with a draft under
`NON_MATCHING` and `SKIP_ASM`, once per version that builds it, with implicit
declarations as errors. It checks that the draft compiles and nothing else.

Each draft also gets an entry in `docs/matching/<unit>.md`, headed
`## <function> (<unit>, <game> <size>)` with one size per game that builds it:
the distance from `check_unit.py --cflags=-DNON_MATCHING` ("N of M words
differ", plus emitted and native sizes when they differ), what blocks the match,
the shapes tried, and a link to the smt3-decomp lever page that names the cause.
Then run `python3 tools/docs_index.py` to regenerate the README and `INDEX.tsv`.
`tools/test_docs_index.py` fails when a draft has no entry, an entry has no
draft, or the index is stale.

Before drafting, `python3 tools/find_body.py NAME...` looks for an existing body
in smt3-decomp and dds-decomp under the function's name, its SMT3 alias and its
Raidou HD name, and in the shape index: the other game's matched C for the
function, and matched functions of the same asm shape in any of the five
executables (ratio 0.9 or better). It says whether each body matches in its own tree. It compares
the retail code of both functions (instruction count, call targets, opcode
sequence) and prints a confidence that they are the same function; below 0.6 the
hit is a different function sharing the name. `--compile` splices the body into
the Raidou unit and runs `check_unit.py`: `match` and `near` compile as found,
`adapt` fails only on its signature, callee arity or one global (usually a
global the SMT3 code reads that Raidou passes as an argument), `rework` lacks
several declarations or struct members and is a reference only. `python3 tools/find_similar.py --for NAME [--body]` ranks
matched functions of both Raidou games, SMT3 and DDS 1/2 by retail instruction
shape: the other game's copy, numbered wrappers such as
`sbtlCallBtlGetSkilltbl8`/`10`/`16`, and same-shape functions elsewhere. Read
its `sub` count, the instructions the template does not supply, not the ratio.
Both tools read only the shape index under
`~/.local/state/raidou-decomp-work/similar/`: build it once with
`python3 tools/similar_index.py refresh` (about 90 seconds) and rerun that after
matching; it re-tokenises only functions whose bytes changed, re-reads matched
status, and updates the precomputed neighbours of every unmatched function.
`python3 tools/shape_clusters.py` groups the unmatched Raidou 1 functions by
shape (ratio 0.9) into `~/.local/state/raidou-decomp-work/clusters/`, each with
the exemplar whose cluster covers the most words. Drafts made from a cluster,
outside any batch, go to the draft store with `python3 tools/draft_store.py
save NAME DRAFT.c --exemplar CORPUS:NAME [--decls FILE]`, which records the
address, size, a hash of the retail bytes, the declarations the draft assumes
and the `check_unit.py` result. `find_body.py` reads the store by address and
leaves out an entry whose address, size, bytes or definition name disagree; a
store draft ranks after matched definitions of the function and before
same-shape siblings. Regular batches do not run the
permuter. As an opt-in, `python3 tools/perm_unit.py import <unit> NAME --batch D`
sets up [decomp-permuter](https://github.com/simonlindholm/decomp-permuter) on
the unit's NON_MATCHING draft under `D/permuter/`, scored on both games for a
shared unit. `perm_unit.py scan DIR` rejects outputs that add a local only ever
set to a constant or copied from one variable. A permuter result lands only once
it is ordinary source.

## 3. Verify

```sh
python3 tools/check_unit.py src/<version>/<unit>.c [-v] [--func NAME]
```

The unit is clean when no function differs. Check the whole unit after an edit,
and both versions for a shared unit. Then:

```sh
ninja                                # fails unless each ELF is byte-identical to retail
python3 tools/check_config.py
python3 tools/check_nonmatching.py
```

A passing `check_unit.py` is not the proof; `ninja` is. After a header change,
run `ninja`: any unit that includes it can change.
