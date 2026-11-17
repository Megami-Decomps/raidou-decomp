# Draft notes: kernel/dds3KernelDraw_0010A750

## dds3CallGrAllocPacketBuff8 (kernel/dds3KernelDraw_0010A750, raidou1 0x208, raidou2 0x208)

Raidou 1: 15 of 130 words differ, first at +0x54.
Raidou 2: 15 of 130 words differ, first at +0x54.

which stack float params get callee-saved registers follows their live length: retail reloads x3..z3 from the arg slots, so their stores come last in source; vectors: menu 32 rows, ingame and opening (vsum), no own stores, callee mix sdfGrAllocPacketBuff/sdf3DPrimMakePoly as the draft calls

Related: register-allocation.

## dds3CallMatNew (kernel/dds3KernelDraw_0010A750, raidou1 0x224, raidou2 0x224)

Raidou 1: 6 of 137 words differ, first at +0x48.
Raidou 2: 6 of 137 words differ, first at +0x48.

6 words off: float arg saved-reg order (f14/f18/f19 land in f29/f27/f28 instead of f27/f28/f29), vertex store order variant tried (worse); retail has no sdf3DPrimLinkResetTm; vectors do not reach, no vector file
