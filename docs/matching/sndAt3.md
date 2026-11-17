# Draft notes: sndAt3

## sndBgmLoadId (sndAt3, raidou1 0x160, raidou2 0x164)

- Raidou 1: 57 of 86 words differ, first at +0x28; emitted 344 bytes, native 352 bytes.
- Raidou 2: 52 of 86 words differ, first at +0x28; emitted 344 bytes, native 356 bytes.

Start loading BGM id; reuses the loaded track when id is already current. Register allocation and store scheduling around the table lookup.

Retail keeps the decoder pointer in `$7` and shares the status base in `$17`.

Tried: extra locals and a late assignment of status.

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).

## sndBgmLoadId_XF (sndAt3, raidou1 0x144, raidou2 0x144)

- Raidou 1: 2 of 81 words differ, first at +0xFC.
- Raidou 2: 4 of 81 words differ, first at +0x20.

Request a crossfade to BGM id; sndData2 records the outcome for sndBgmStatus_XF. Raidou 1 (delay slot order before the last SignalSema), Raidou 2 4 (also a register in the index shift).

## sndCallSeStop (sndAt3, raidou1 0x98C)

Raidou 1: 589 of 603 words differ, first at +0x0; emitted 2412 bytes, native 2444 bytes.

Vectors: the 26 snapshot fails differ only in sceSdRemote packet words 2-5 (0x507bc8-0x507bdb), the a3-t2 register garbage passed as unused varargs through mnuCopySoundStreamOnStatusChange's sceSdRemote(1, 0x8100, 1); every call and defined argument equals retail.

First differing words:

| Offset | Draft | Retail |
|---|---|---|
| +0x0 | `addiu sp,sp,-64` | `addiu sp,sp,-48` |
| +0x8 | `sd s0,0(sp)` | `sd s3,24(sp)` |
| +0xC | `lui a2,0x0` | `lui s3,0x4c` |
| +0x10 | `sd s1,8(sp)` | `sd s0,0(sp)` |

## sndCallMemFreeDirect (sndAt3, raidou1 0x60, raidou2 0x60)

Raidou 1: 2 of 24 words differ, first at +0x38.
Raidou 2: 2 of 24 words differ, first at +0x38.

store order of a reset block: source order loadState/allocation/compressedData/samples then sndData5/6/7 is closest (7 words), retail puts sndData6 between loadState and allocation

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).
