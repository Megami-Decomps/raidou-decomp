# C from smt3-decomp

Carried from smt3-decomp d1569b46.

Functions Raidou shares with SMT3 are compiled from smt3-decomp's C where the
Raidou code is the same, one function at a time. Everything else in a unit
stays `INCLUDE_ASM`.

| | raidou1 | raidou2 |
|---|---|---|
| identical to SMT3 C (`pairs`) | 2,627 | 2,595 |
| compiled from C | 2,305 | 2,285 |
| units with C | 203 | 204 |
| SMT3 names in `smt3_symbols.txt` | 941 | 937 |

123 of the TUs are one file both games compile; the rest have a file per game.

## Layout

- Each TU is one file with its C bodies inline in link order and
  `INCLUDE_ASM` for the rest, as in smt3-decomp. A TU with the same text in
  both games is `src/<unit>.c`, compiled into each version (its `INCLUDE_ASM`
  lines resolve against that version's `asm/<version>/`). A TU that differs
  (other `func_`/`D_` names, other bodies) is `src/<version>/<unit>.c`; the
  per-game file wins. `tools/units.py` resolves a unit to its file, and
  `tools/splat_split.py` lets splat read the shared file.
- A carried body is smt3-decomp's text verbatim, minus `static` (other Raidou
  objects call these functions and read these objects by name).
- The TU holds every declaration of the SMT3 file its bodies come from (types,
  macros, prototypes, inline helpers, data as `extern`) between
  `/* Declarations from smt3-decomp src/smt3/<file>.c */` and its end comment,
  after the leading `#include`s. `port` adds and drops these blocks.
- `include/` carries smt3-decomp's headers.
- `config/<v>/smt3_symbols.txt`: linker definitions for the SMT3 names the
  bodies use where Raidou has no name, at the Raidou address (`ttyState =
  0x0050ACD0;`). The body text never changes between games; the address does.
  `include_rodata.py` and `include_sdata.py` treat these names as references
  to the Raidou data at that address.
- `config/<v>/smt3_c.tsv`: every function considered, `c` or `asm`, with the
  reason it stays asm.
- SDK members that compile SMT3 C get SMT3's compiler and flags for that
  library (the generated block in `config/<v>/compilers.txt`, `cflags.txt`,
  `asflags.txt`).
- `port` writes both games' text for a TU and keeps one shared file when they
  are equal. When the second version's `include_rodata.py`/`include_sdata.py`
  run would change a shared file, the TU is split into per-game files.

## Pipeline

```sh
python3 tools/raidou_c.py pairs                # build/raidou_c/pairs.tsv
python3 tools/raidou_c.py port                 # write bodies, edit units (--units sdf sdk ...)
python3 configure.py && ninja -k 0 > log 2>&1  # objects; the link may fail
python3 tools/raidou_c.py verify --log log     # check, write names and failures
python3 tools/raidou_c.py port                 # failures back to INCLUDE_ASM
python3 configure.py && ninja                  # repeat verify/port until clean
```

`pairs` takes a Raidou function only when its SMT3 partner (from
`name_sources.txt`) is C in smt3-decomp, has the same name, and has the same
instruction words with j/jal targets and `%hi/%lo/%gp_rel` fields masked.
smt3-decomp's split assembly gives the SMT3 words, keyed by address.

`verify` reads each compiled object, not the source: every word must equal
retail except relocated fields, and each relocation's symbol must resolve to
the Raidou address the retail word implies. A name Raidou already has must
already be at that address; a new name goes into `smt3_symbols.txt`; a name
two bodies want at different addresses fails both. Compile errors are traced
to the body by its line in the TU (or, for a clash in a declarations block, to every body of that SMT3 file in
the unit). `configure.py` must run after `port` reverts a function, because
splat moves the asm of a C function from `nonmatchings/` to `matchings/`.

## Why a function stays asm

| reason | meaning |
|---|---|
| `emits .rodata data`, `emits .lit4 data` | the body writes strings, jump tables or float constants itself; the unit's data is still `INCLUDE_RODATA`, so it would be there twice and in the wrong order |
| `its rodata is in its asm file` | splat moved data only this function uses into its asm file and drops it once the function is C |
| `compile error`, `declarations clash` | the SMT3 declarations do not compile in this unit (a struct both an SMT3 file and an included header define, an old-style definition, file-scope statics) |
| `local jump` | a jump to code elsewhere in the unit at another distance than in SMT3 |
| `<name> is/at 0x...` | a name the body uses would need two Raidou addresses (a static of the same name in two SMT3 files, or a table laid out differently) |
| `N words differ`, `size` | same masked words in the split, different object code here (context) |
| nested function | `movieParseHeader` defines `movieMemcpy` inside it; Raidou has both as functions |

Data-emitting bodies are the main remainder. Carrying them needs the unit's
`INCLUDE_RODATA`/`.lit4` lines placed around the C in object order, or the data
defined in C.
