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
