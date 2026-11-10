# C from smt3-decomp

Carried from smt3-decomp 3837939d.

Functions Raidou shares with SMT3 are compiled from smt3-decomp's C where the
Raidou code is the same, one function at a time. Everything else in a unit
stays `INCLUDE_ASM`.

| | raidou1 | raidou2 |
|---|---|---|
| identical to SMT3 C (`pairs`) | 2,742 | 2,705 |
| compiled from C | 2,683 | 2,647 |
| units with C | 218 | 220 |
| SMT3 names in `smt3_symbols.txt` | 1,119 | 1,099 |

120 of the TUs are one file both games compile; the rest have a file per game.

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

## Names from SMT3 C

`raidou_c.py names` names a `func_` function when its masked words (as `pairs`
compares them, at least 16 words) occur once in the game and once in SMT3 and
the SMT3 function is C with a real name not yet used in the game. It writes
`build/raidou_c/names_<v>.tsv` for `raidou_names.py --apply --out-dir
build/raidou_c` (provenance `unique-masked-hash-c` in `name_sources.txt`; a
cross-game map that disagrees rejects the name). 111 names in raidou1, 104 in
raidou2.

## Names bodies want at several addresses

The masked compare hides `%lo` offsets and `jal` targets, so a body can match
SMT3 while calling another function or reading another offset. When bodies want
one SMT3 name at several Raidou addresses, `verify` gives the name the address
most of them want (not one that already has another real name, and not one
another such name took); the others stay asm. A tie leaves all of them asm.

## Owned rodata

Data only one function uses sits in that function's asm file. When the body
reaches all of it through SMT3 names, those names go into `smt3_symbols.txt`
and `include_rodata.py` keeps the data as `INCLUDE_RODATA` before the body.
`configure.py` resplits when `smt3_symbols.txt` changes for that reason, and
aligns split `.rodata`/`.data` files at 16-byte addresses to 16 as C objects
are, since a C object can end short of the next split by its padding.

## Data definitions from SMT3 C

`port` replaces an `INCLUDE_RODATA`/`INCLUDE_SDATA` line with SMT3's C
definition when the line's address carries an SMT3 name (`smt3_symbols.txt`)
and SMT3 defines that name in C: from a file whose declarations the TU already
has, else from the one file that defines it, without that file's declarations
(it stays only if it compiles alone). `static` is dropped. SMT3 placeholder names (`D_...`) are not carried, since
they would clash with the game's own. A carried name leaves `smt3_symbols.txt`
(the definition provides it); asm still reaches the address through its
`PROVIDE(D_...)` fallback. `include_rodata.py` and `include_sdata.py` read the
carried names from `config/<v>/smt3_data.tsv` and place the other lines around
the definition. `verify` checks each definition in its object (retail's bytes,
no larger than the label it replaces, the next item at retail's distance) and
in the link (at its address). Within a unit only the first displaced definition
is blamed; when none is, the last definition in the object before the first
label the link moved is. A failed one goes back to its line (`asm` in the tsv).
Some Raidou data differs from SMT3 (sizes, values, an alignment the unit's start
cannot give), which this catches. A string array SMT3 declares larger than
Raidou's label is carried with Raidou's size (last column of the tsv; the TU's
own `extern` for it loses its size), unless that size is 8 bytes or less, which
`-G 8` would put in `.sdata`. 228 definitions in raidou1, 208 in raidou2.

## Why a function stays asm

Data a body emits (.rodata strings, jump tables, .lit4 floats) is checked
against retail and kept when its section lands where retail has it.

| reason | raidou1 | raidou2 | meaning |
|---|---|---|---|
| `<name> at 0x..., others at 0x...` | 27 | 26 | the body calls another function or uses another offset than SMT3 (hidden by the masked compare) |
| `size` | 13 | 13 | the object is larger than retail (newlib float functions, qsort: other compiler settings) |
| `N words differ` | 8 | 8 | same masked words in the split, different object code here (context) |
| `<name> at 0x..., 0x...` | 6 | 6 | bodies want a name at several addresses with no majority |
| `.rodata differs` | 2 | 2 | the emitted strings differ from Raidou's |
| `local jump` | 2 | 2 | a jump to code elsewhere in the unit at another distance than in SMT3 |

A nested function with an assembler name (`movieMemcpy` in
`movieParseHeader`) is emitted by its parent's body, so `port` drops its
`INCLUDE_ASM` line while the parent is C.

## Names from dds-decomp

`raidou_dds.py names` names a `func_` function (asm, in a TU or a gap) after
its DDS1 or DDS2 partner in `dds_unique_in_raidou.tsv` when the partner's
masked words and slot kinds (as `pairs` compares them) equal Raidou's in
dds-decomp's split assembly and the name is not a placeholder or already used.
A C partner wins, then DDS1. `rename` updates the `INCLUDE_ASM` lines;
`raidou_names.py --apply --out-dir build/raidou_c/dds` writes the names. The
note in `name_sources.txt` says how dds-decomp got the name: `SMT3HD name`,
`evidence`, `descriptive` (its own descriptive names, most of them) or `no
provenance row`. 1,132 names in raidou1, 1,126 in raidou2.

```sh
python3 tools/raidou_dds.py names --dds <dds-decomp, split>
python3 tools/raidou_names.py --apply --out-dir build/raidou_c/dds
python3 tools/raidou_dds.py rename && python3 configure.py && ninja
```

## C from dds-decomp

Functions SMT3 lacks but DDS1 or DDS2 has are carried from dds-decomp's C the
same way. `raidou_dds.py pairs` writes `build/raidou_c/pairs_dds.tsv` (map
pair, same masked words and slot kinds, Raidou name equal to the DDS C name);
`port` reads it after `pairs.tsv`, and `verify` treats the bodies like SMT3's.

- A DDS source is `dds1/<path>` or `dds2/<path>` in `smt3_c.tsv` and the
  declaration block (`/* Declarations from dds-decomp src/dds1/<path>.c */`).
  `#if VERSION_DDS1/2` in the source is decided for that game; the block
  defines `VERSION_DDSn` for the headers.
- dds-decomp's headers are in `include/dds/` (`raidou_dds.py headers` copies
  those the blocks include). They define many SMT3 types differently, so a unit
  with SMT3 bodies gets no DDS bodies, and a unit takes one DDS game (the one
  most of its functions pair with).
- DDS data definitions are not carried yet.

```sh
python3 tools/raidou_c.py pairs && python3 tools/raidou_dds.py pairs --dds <dds-decomp, split>
python3 tools/raidou_c.py port --dds <...> && python3 tools/raidou_dds.py headers --dds <...>
python3 configure.py && ninja -k 0 > log; python3 tools/raidou_c.py verify --dds <...> --log log
```
