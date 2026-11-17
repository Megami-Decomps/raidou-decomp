# The IOP modules per game

Which IOP modules each game ships, which of them are SMT3's, and how the sound
driver differs. What the shared modules do is in smt3-decomp's
[sdf-irx-modules.md](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/sdf-irx-modules.md);
how they are built is in [irx-modules.md](irx-modules.md).

| Module | Size (.text) | Functions | raidou1 | raidou2 | Role |
| --- | --- | --- | --- | --- | --- |
| `SDFIOP.IRX` | 0x510 | 5 | = SMT3 | = SMT3 | RPC server that loads further IRX modules |
| `SDFIOPFL.IRX` | 0x570 | 7 | = SMT3 | = SMT3 | file I/O forwarded to the EE |
| `LIBSD.IRX` | 0x4840 | 60 | = SMT3 | = SMT3 | Sony's SPU2 library |
| `MODHSYN.IRX` | 0xb200 | 188 | = SMT3 | = SMT3 | Sony's hardware synthesiser |
| `MODMIDI.IRX` | 0x3800 | 54 | = SMT3 | = SMT3 | Sony's MIDI sequencer |
| `MODMSIN.IRX` | 0x3a0 | 9 | = SMT3 | = SMT3 | Sony's MIDI message input queue |
| `SDRDRV.IRX` | 0xf20 | 10 | = SMT3 | = SMT3 | Sony's sound RPC server (libsdr) |
| `SDFSDMAN.IRX` | 0x61a0 / 0x5ff0 | 72 / 73 | own | own | the sound driver |
| `sdfprint.irx` | 0x10a0 | 19 | | own | network debug print server, not loaded |

Function counts leave out the import stubs. "= SMT3" means byte-identical to
Nocturne's file, so `SDRDRV.IRX` is also the same in all three games, and
smt3-decomp has no C for it either.

## SDFSDMAN.IRX

Each game rebuilt the driver from SMT3's source. 118 of the 133 named
functions (with the import stubs) are the same in both Raidou games.

| | SMT3 | raidou1 | raidou2 |
| --- | --- | --- | --- |
| .text | 0x57d0 | 0x61a0 | 0x5ff0 |
| .bss | 0x14528 (83 KB) | 0x20108 (131 KB) | 0x200f8 (131 KB) |
| BGM resume points | yes | no | no |
| hard timer | yes | yes | no |
| `.SMG` area unlock | no | yes | yes |

- Both Raidou games add a table of 30 reference counts to the work area and an
  unlock path for the `.SMG` bank areas ("smg unlock area err!"). Raidou 1's
  version also reports "devil err unlock" and "skil err unlock"; Raidou 2's
  reports "smg unlock area err! nakama" instead.
- Both drop SMT3's BGM resume points (`sdfSndSaveResumePoint`,
  `sdfSndFindResumePoint`, `sdfSndBgmResume`) and its "44.1kHz" message.
- Raidou 2 drops the IOP hard timer and its five "Hard timer ..." messages.
- Both enlarge `.bss` by about 48 KB over SMT3's.

The pairing with SMT3's functions is `config/<version>/irx/SDFSDMAN/smt3_pairs.tsv`.

## SDRDRV.IRX

Sony's `sdr_driver` 4.1: an RPC server on the IOP that runs libsd calls for
the EE's libsdr. Both ELFs load it from `SOUNDIRX/`. No IDB is
open for it, so its bounds come from `tools/irx_scan.py`, and only the entry,
`start`, has a name; the 47 import stubs are named from the import ordinals.
Persona 3 FES ships a later build (`86aa1be2`), whose export table adds
`sceSdrChangeThreadPriority` and `sceSdrSetUserCommandFunction`.

## sdfprint.irx

Found in Raidou 2's `DDS3.IMG` under `/userirx/`, beside copies of `SDFIOP.IRX`
and `sdfiopfl.irx`. Its strings describe a debug server that sends printf output
and memory dumps to a host over the network (`printd_thread`, `get_req`,
`send_mem`). Neither ELF names it, so it is never loaded: a debug leftover, not a
decompilation target. Its `.iopmod` has no moduleinfo block
(0xFFFFFFFF) and an empty module name. `tools/extract.py` reads it through
`DDS3.DDT` (`archive:` in its yaml) rather than from the ISO file system.
