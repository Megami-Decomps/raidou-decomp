# Translation units

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

## Pipeline

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

## Evidence

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

## Limits

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
