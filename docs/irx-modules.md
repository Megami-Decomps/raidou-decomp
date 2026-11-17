# The IRX modules

Each game ships IOP modules in `USERIRX/` and `SOUNDIRX/`. Seven per game build
byte-identical from source, with iop-gcc 2.8.1, their own linker script and the
IRX container packer. `configure.py` adds them to `build.ninja`, and `ninja`
ends each one in a verify step against `orig/<version>/irx/<module>.IRX`, so a
green build is the proof. The method is smt3-decomp's: its
[irx-modules.md](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/irx-modules.md)
covers the toolchain, the container, splitting, the import tables and what
iop-gcc wants ([irx-idioms.md](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/irx-idioms.md)).
What the modules do is in its
[sdf-irx-modules.md](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/sdf-irx-modules.md),
which the C comments cite.

```
python tools/download_tools.py   # brings in iop-gcc 2.8.1 with the EE compilers
python tools/extract.py          # the ELF and the IRX files from each disc
python configure.py              # splits each module present in orig/
ninja                            # or: ninja raidou1-irx
```

A module missing from `orig/` is left out of the build with a message, so the
ELF builds without them.

## Modules

| Module | raidou1 | raidou2 | C |
| --- | --- | --- | --- |
| `SDFIOP.IRX` | = SMT3 | = SMT3 | smt3-decomp's, complete |
| `SDFIOPFL.IRX` | = SMT3 | = SMT3 | smt3-decomp's, complete |
| `LIBSD.IRX`, `MODHSYN.IRX`, `MODMIDI.IRX`, `MODMSIN.IRX` | = SMT3 | = SMT3 | smt3-decomp's, every function has C |
| `SDFSDMAN.IRX` | own build | own build | SMT3's where a function is unchanged |

"= SMT3" means the file is byte-identical to Nocturne's, so its C, symbols and
headers are smt3-decomp's unchanged: `src/irx/shared/<module>.c`,
`config/<version>/irx/<module>/symbol_addrs.txt` and `include/irx/`.

Not set up yet: `SDRDRV.IRX` (identical in all three games, with no C in
smt3-decomp either) and Raidou 2's `DDS3/userirx/sdfprint.irx`.

## Layout

- `config/<version>/irx/<module>/<module>.yaml`: the retail sha1, the path on
  the disc, the `.iopmod` fields, an optional `idb:` for `tools/irx_symbols.py`
  and `irx_headers.py`, and `tus:` when the C is more than one file.
- `config/<version>/irx/<module>/symbol_addrs.txt`: splat's format; function
  bounds give the splitter one chunk per function.
- `src/irx/<version>/<unit>.c` or `src/irx/shared/<unit>.c` (the per-game file
  wins), as for the ELF. They sit outside `src/<version>` and `src/shared`,
  whose tools treat every `.c` there as an ELF unit.
- `asm/<version>/irx/<module>/` and `build/<version>/irx/` are generated;
  `INCLUDE_ASM` reads the version's asm through `ASM_ROOT`.

`tools/irx_config.py` resolves all of it; a module is named by its key,
`raidou1/SDFSDMAN`, or by a bare module name for both games.

## SDFSDMAN

The sound driver is the one module each game rebuilt. Both add code around a
table of 30 reference counts in the work area, Raidou 1's `.bss` is 131 KB
(SMT3's is 83 KB), Raidou 2 drops the "Hard timer" messages, and both drop
SMT3's BGM resume points (`sdfSndSaveResumePoint`, `sdfSndFindResumePoint`, `sdfSndBgmResume`).

`tools/irx_match.py` pairs each Raidou function with SMT3's. The function
bounds are the Raidou IDB's (`idb_funcs.json`, exported with
`irx_symbols.collect`). A function is `same` when its words equal SMT3's with
the relocated fields masked, else `similar` by order and opcode similarity
between those anchors, or `hand` from `hand_pairs.txt`, which also names data
whose layout changed. Data names come from the operands of aligned equal
instructions and from string contents. The result is `smt3_pairs.tsv` and,
with `--symbols`, `symbol_addrs.txt`. New functions stay `func_<address>`.

`tools/irx_carry.py` then writes the C: a `same` function takes smt3-decomp's
text (the matching C, or its `NON_MATCHING` draft with the `INCLUDE_ASM`),
zero padding that ends on 16 becomes `ALIGN_TEXT(4)`, and the import tables,
byte-identical in all three games, are SMT3's. Everything else is
`INCLUDE_ASM`. The last translation units (the EE RPC call, the ADPCM stream
and the import tables) are the same text in both games and are
`src/irx/shared/SDFSDMAN_adpcm.c`.

| | raidou1 | raidou2 |
| --- | --- | --- |
| functions (without import stubs) | 72 | 73 |
| SMT3 C, matching | 17 | 13 |
| SMT3 `NON_MATCHING` drafts | 9 | 9 |
| `INCLUDE_ASM` only | 46 | 51 |
| new, `func_` | 6 | 7 |

## Checks

- `ninja`: every module byte-identical to retail.
- `tools/check_irx_nonmatching.py`: compiles each module's C with
  `NON_MATCHING`, with and without `FIX_BUGS`, using the host gcc (a syntax
  check). `tools/check_nonmatching.py` skips `src/irx`.
- `tools/irx_diff.py raidou1/SDFSDMAN [function]`: retail and built `.text`
  side by side, one chunk at a time.
