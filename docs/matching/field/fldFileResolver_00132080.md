# Draft notes: field/fldFileResolver_00132080

## fldLoadFile (field/fldFileResolver_00132080, raidou1 0x70, raidou2 0x70)

Raidou 1: 2 of 28 words differ, first at +0x48.
Raidou 2: 2 of 28 words differ, first at +0x48.

store order only: retail stores the floor (nameU, 0x68) before the field (amSBlock, 0x60); all six orders of the three fldGlobalBase stores and a (u16) mapNo local give the same swap; checked against the asm (vector row truncated), same on R2

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).

## fldLoadFile_FromMemory (field/fldFileResolver_00132080, raidou1 0xE8, raidou2 0xE8)

Raidou 1: 8 of 58 words differ, first at +0x78.
Raidou 2: 8 of 58 words differ, first at +0x78.

scheduling only: retail puts the `if (pF1 != 0) pF2 = pF1` movn in the first fldAddressResolverReset delay slot and stores the FL2 base before the jal; the if before or after fldFileResolverFL2 compiles the same, a single ternary buffer is worse; vector checked (boot), same on R2

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).
