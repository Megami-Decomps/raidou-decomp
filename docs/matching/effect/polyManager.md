# Draft notes: effect/polyManager

## dds3SpPoly_RollRingDraw (effect/polyManager, raidou1 0x194, raidou2 0x194)

Raidou 1 and Raidou 2: 73 of 97 words differ, first at +0x54; emitted 388 bytes, native 404 bytes.

Nothing is recorded about this draft.

Raidou 1, first differing words:

| Offset | Draft | Retail |
|---|---|---|
| +0x54 | `blez s7,0x100` | `blez s7,0x110` |
| +0x60 | `ori v1,v1,0x1` | `slti v0,s4,2` |
| +0x64 | `sw v1,4(sp)` | `ori v1,v1,0x1` |
| +0x68 | `lw s0,0(s3)` | `sw v0,8(sp)` |
