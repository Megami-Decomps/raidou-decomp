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
  both games is `src/shared/<unit>.c`, compiled into each version (its `INCLUDE_ASM`
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
  to the Raidou data at that address, and `check_unit.py` compares a
  relocation against one at that address. An entry is the object retail uses
  at the carried call site, which is not always SMT3's object of that name.
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
A pair whose words differ (`differs` in `pairs.tsv`, or a same-name DDS map
partner in `pairs_dds.tsv`) is carried too when its `smt3_c.tsv` reason starts
with `compiles identical`: the C compiles to the Raidou words once Raidou's
headers carry its layout. Mark such rows `asm	compiles identical` and run
`port --retry '^compiles identical$'`; a failure keeps the prefix, so the next
`port` puts it back to asm.

dds-decomp and SMT3 sometimes name one member differently in a type both
define with the same layout. `raidou_dds.py members` probes each such type's
member offsets and sizes on both sides and writes `config/dds_members.txt`
(type, DDS name, SMT3 name). `config/dds_members_hand.txt` holds rows the probe
cannot find, checked by hand: types SMT3 names differently or defines in a
source file. A DDS body whose `smt3_c.tsv` reason starts with
`SMT3 member names` is carried with SMT3's names where the access's base type
can be followed: a variable or parameter declared in the function or the
file, a `((T *)p)` cast, and the members along the chain. Unmarked bodies keep
DDS's names, since some TUs see dds-decomp's definition of the type. SMT3's
names are never changed.

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

## .data as C

`.data` is one asm chunk except for the ranges in `config/<v>/data_c.tsv` (unit,
start, end vram). `tools/split_data.py <v> --write` turns each range into a
`[rom, .data, <unit>]` subsegment, so the unit's C defines every object in it,
and keeps the rest as `data_<vram>` asm chunks. A shared unit needs the same range
in both games (`config/xgame/r1_r2_data_pairs.tsv` gives the addresses). A name the
C defines leaves `smt3_symbols.txt`, which would otherwise override it at link time.

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
  `#if VERSION_DDS1/2` in the source is decided for that game.
- Raidou has no copy of dds-decomp's headers, and `check_config.py` refuses
  `VERSION_DDS` and `#include "dds/` in `src/` and `include/`. `port` prints
  each dds-decomp header a block would need; move what the unit uses into
  `include/<unit>.h`, as SMT3 does, and keep prototypes of functions the unit
  defines later as forward declarations in the `.c` file or in that header.
  dds-decomp defines many SMT3 types differently, so a unit with SMT3 bodies
  gets no DDS bodies, and a unit takes one DDS game (the one most of its
  functions pair with).
- DDS data definitions are not carried yet.
- Both tools read dds-decomp at the revision `config/dds_decomp.txt` pins and
  refuse a checkout at another one, or one with edits in `src/` or `include/`:
  the declaration blocks and the carried bodies are that revision's text. `--dds` defaults to
  `~/.local/state/raidou-decomp-work/dds-<first 8 hex digits>` when it exists.
  Moving the pin means regenerating both and verifying the build.
- A differing pair takes the unit's DDS game when both games have a partner.
- smt3-decomp is read at its HEAD, not pinned: its renames and new names should
  reach Raidou on the next port. A type whose Raidou layout differs from the
  SMT3 or DDS type of the same name goes in `config/raidou_types.h`; port writes
  that definition in place of the source's in every declaration block.
- Carried data provides its own name, so its `smt3_symbols.txt` row goes,
  except when the other game aliases the name (`common_names.py check` pairs
  the games by these rows); the link script gives such a row `PROVIDE`.
- Generated text spells an SMT3 or DDS placeholder alias (`D_<addr>` in
  `smt3_symbols.txt`) as the Raidou name at its address, under the rule
  `tools/xgame/common_names.py` applies to the tree, so port and that rename
  agree.

```sh
git -C <dds-decomp> worktree add --detach <dir> $(tail -1 config/dds_decomp.txt)   # copy orig/, configure.py
python3 tools/raidou_c.py pairs && python3 tools/raidou_dds.py pairs --dds <dir>
python3 tools/raidou_c.py port --dds <dir>
python3 configure.py && ninja -k 0 > log; python3 tools/raidou_c.py verify --dds <dir> --log log
```

`port` and `verify` rerun both `pairs` steps when `configure.py --clean` has
deleted their lists. `verify` rewrites only the `smt3_symbols.txt` rows it
decides; names added by hand or `scripts/rename.py` stay.

## SMT3 names for raidou1 and raidou2

Both Raidou games are built on Nocturne's engine. Where a Raidou function is the
same code as a function in smt3-decomp (`SLUS_209.11`), it takes that function's
name. The method is the one smt3-decomp used for Digital Devil Saga (its
`docs/dds-names.md`, and dds-decomp's `docs/smt3-names.md` for the rules).

Every carried name is `inferred` in `config/<v>/name_sources.txt`:

```
0x<addr>  <name>  inferred  SMT3HD name via SLUS_209.11 0x<smt3 addr>, <evidence>; <map verdict>
```

`SMT3HD name` is the IDB's HD-recovered name. `smt3-decomp name` marks the cases
where smt3-decomp's `symbol_addrs.txt` corrects the IDB (the soft-float runtime,
`libc_*` placeholders, `_StartThread`); its spelling always wins, so SDK and libc
names match smt3-decomp exactly.

### Pipeline

Three pairing runs with smt3-decomp's `tools/pair_build.py` (IDBs read only):

```sh
W=~/.local/state/raidou-decomp-work/names
P=~/projects/smt3-decomp/tools/pair_build.py
python3 $P --from-instance SLUS_209.11 --to-instance SLUS_214.31 \
    --to-elf orig/raidou1/SLUS_214.31 --to-text-end 0x0039E9FC \
    --cache-dir $W/cache_smt3_raidou1 --output $W/smt3_raidou1.tsv
python3 $P --from-instance SLUS_209.11 --to-instance SLUS_218.45 \
    --to-elf orig/raidou2/SLUS_218.45 --to-text-end 0x00418E48 \
    --cache-dir $W/cache_smt3_raidou2 --output $W/smt3_raidou2.tsv
python3 $P --from-instance SLUS_214.31 --to-instance SLUS_218.45 \
    --from-elf orig/raidou1/SLUS_214.31 --from-text-end 0x0039E9FC \
    --to-elf orig/raidou2/SLUS_218.45 --to-text-end 0x00418E48 \
    --cache-dir $W/cache_raidou1_raidou2 --output $W/raidou1_raidou2.tsv
```

Then, in a configured tree (the tool reads `asm/<v>/code_*.s` for splat's
function starts):

```sh
M=~/.local/state/smt3-port-work/raidou-map
python3 tools/raidou_names.py \
    --raidou1 $W/smt3_raidou1.tsv $W/cache_smt3_raidou1 \
    --raidou2 $W/smt3_raidou2.tsv $W/cache_smt3_raidou2 \
    --raidou1-raidou2 $W/raidou1_raidou2.tsv \
    --map $M/raidou1_map.tsv $M/raidou2_map.tsv --out-dir $W/out
python3 tools/raidou_names.py --apply --out-dir $W/out
python3 configure.py && ninja
```

### Rules

As dds-decomp's, in order: byte-identical unique pairs are forced, then
relocation-identical ones; other pairs must be high tier with a hash unique on
both sides, or share a hash only inside a constant-delta run. HD address tags,
`gcc2_compiled` markers, IDA `_N` suffixes and placeholder names are rejected.
A pair the raidou1-to-raidou2 alignment contradicts is dropped from both games,
and a name accepted in one game carries to its strong-hash twin in the other.

Two rules are new here:

- **Splat function starts only.** A name lands only on an address splat emits
  as a `glabel`; a function IDA found but splat did not is left for the TU work.
- **The independent map must not disagree.** smt3-port's
  function matcher paired the same games by another method. Where it
  places the SMT3 function elsewhere, or a different SMT3 function at the
  target, the name is not applied. Its silence is not a disagreement.

### Counts (2026-11-10)

```
raidou1: accepted=3076  map-disagrees=9   shared-hash-off-run=169  tier-low=205
         tier-medium=154  address-tag=80  smt3-placeholder=18  not-a-splat-function=7
raidou2: accepted=3043  map-disagrees=11  shared-hash-off-run=165  tier-low=205
         tier-medium=160  address-tag=80  smt3-placeholder=17  not-a-splat-function=8
```

3026 names are in both games. Of the accepted names, 2975 (raidou1) and 2943
(raidou2) agree with the map; the other 101 and 100 are names the map has no
entry for.

The 20 disagreements are of two kinds. Eight are syscall stubs
(`_iEnableIntc` to `_iDisableDmac`) whose raw bytes are unique on both sides,
where the map is one stub off; the pairing is most likely right, but the names
are held back until someone checks. The other twelve are `shared-hash-in-run`
or unmarked-region pairs, where a function family shares one masked body and
the two methods chose different members.

## Translation units

`.text` from `code_001001D8` to `.vutext` is split into one subsegment per
translation unit (TU) where there is evidence for one, and `code_<vram>` asm
gaps where there is none. `config/<v>/tu_evidence.tsv` lists every subsegment
with its evidence and the smt3-decomp unit it corresponds to.

| | raidou1 | raidou2 |
|---|---|---|
| functions in range | 12,003 | 13,707 |
| TUs (c units) | 300 | 302 |
| tied to an SMT3 unit | 290 | 288 |
| functions in TUs | 5,155 | 5,315 |
| gaps (asm units) | 158 | 163 |

### Pipeline

```sh
python3 tools/sdk_objects.py raidou1 --sdk .../sce/296 --sdk .../sce/204 --sdk .../sce \
    --range 0x377660:0x39E9FC -o config/raidou1/sdk_objects.tsv
python3 tools/raidou_tus.py raidou1 --write      # .text subsegments + tu_evidence.tsv
python3 configure.py                             # split with the new units
python3 tools/split_rodata.py raidou1 --section rodata --write
python3 tools/split_rodata.py raidou1 --section lit4 --write
python3 tools/split_rodata.py raidou1 --section sdata --write
python3 configure.py && ninja
```

raidou2 uses `--range 0x3ED2A0:0x418E48`. `raidou_tus.py` reads the split
assembly, so it needs a configured tree, and re-running it on a split tree gives
the same units.

### Evidence

`tools/raidou_tus.py` labels each function, strongest source first:

1. **SMT3 pair.** The SMT3 function's unit in smt3-decomp's yaml, through the
   pair in `config/<v>/name_sources.txt`, else the cross-game map
   (`raidou<N>_map.tsv` in smt3-port's work data). smt3-decomp's own
   `code_<vram>` gaps are not units and give no label.
2. **DDS pair**, for code DDS shares and SMT3 lacks
   (`dds_unique_in_raidou.tsv`), with dds-decomp's unit when both DDS games
   agree. Otherwise DDS1's (else DDS2's) `code_<vram>` file, which in
   dds-decomp is a TU too; the Raidou TU is then named `code_<its vram>`.
   `raidou_tus.py <v> --gaps-only --write` keeps the current TUs and splits
   only the gaps this way, clipping a DDS run that reaches into a current TU.
3. **SDK archive member** (`config/<v>/sdk_objects.tsv`, from
   `tools/sdk_objects.py`, matching by masked bytes or by a name defined in one
   member). In the SDK region this replaces the SMT3 label: smt3-decomp's SDK
   units are whole archives, the member is the object. The trees on hand are
   2.0.4, 2.96 and 3.0.3; Raidou's 2.5.0 is not among them, so 27% of the
   region is unattributed.

Pairs of functions under 6 words are ignored (tiny bodies recur everywhere).

A unit's labelled functions form a run; unlabelled functions inside the run
belong to it, those at its edges go to gaps. A run is cut where:

- a short run of another label sits inside it (dropped as noise; listed as
  `dropped label` in `build/<v>/tu_plan.tsv`),
- two members hold copies of the same string (`find_tus.py`'s boundary rule),
- Raidou has more than 0x800 bytes and twice as much code between two members
  as SMT3 has between their partners (new code that is likely another file).

The largest piece keeps the unit name; other pieces with two or more members
are named `<unit>_<vram>` and marked `part`. Literal cores (functions sharing a
string, jump table or `.lit4` constant, `tools/find_tus.py`) extend a TU over
unlabelled neighbours and rejoin pieces of one unit. A core spanning two
different units would be a conflict; there are none.

### Limits

- TU edges are the first and last paired function. A TU may in truth extend
  into the neighbouring gap.
- `reordered` TUs have paired functions out of SMT3's order. Kept as one TU
  because no other unit's function lies inside.
- raidou2's `.text` ends in `.gnu.linkonce.t` code (`code_004179A8`, g++ inline
  and template functions): it links after all objects while its literals stay
  in each object's `.rodata`, so it is left out of the literal-order rule
  (`LINKONCE_TEXT` in `raidou_tus.py`).
- `.data` is not split per object. `.rodata`, `.lit4` and `.sdata` are, by
  `tools/split_rodata.py` (from smt3-decomp), for c units only; data of gaps
  stays in asm chunks.
- An `.sdata` block that starts unaligned with no aligned symbol of its own (a
  lone `char`) stays in the chunk before it: an ee-as `.sdata` section is
  word-aligned, so the object could not start there.
- One TU split in several: raidou1 `code_00144130` through at least
  `code_00153438` (0x144130-0x1566D0, all evidence `none`). mapMapObjectRefresh
  (`code_0014B660`) matches only with mapObjCheck (`code_00144130`) defined
  above it in the same unit, and mapCutSet_From_Pos (`code_00153438`) closes
  its `bgezl` only with mapObjOnOff (`code_00144130`) defined in the unit; no
  string is duplicated across the range, and its `.rodata` gaps fit one
  object (ee-gcc 2.96 aligns jump tables to 16, strings to 8). Not merged
  yet: the range holds four shared units (`code_r1_0014B328`, `_0014B4B8`,
  `_0014B610`, `_00151980`) that raidou2 places between its own asm gaps, and
  unowned `.rodata`/`.sdata` chunks sit between the units, so the merge needs
  the raidou2 TU and the data chunks resolved with it.

### Source layout

A TU both games compile the same way is `src/shared/<unit>.c`; a TU that
differs is `src/<version>/<unit>.c`, and the per-game file wins.
`tools/units.py` resolves a unit to its file; shared units are described [above](#layout).
