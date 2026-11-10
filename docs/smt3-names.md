# SMT3 names for raidou1 and raidou2

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

## Pipeline

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

## Rules

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
  `tools/xgame/match_funcs.py` paired the same games by another method. Where it
  places the SMT3 function elsewhere, or a different SMT3 function at the
  target, the name is not applied. Its silence is not a disagreement.

## Counts (2026-11-10)

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
