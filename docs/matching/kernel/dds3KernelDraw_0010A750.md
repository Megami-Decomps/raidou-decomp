# Draft notes: kernel/dds3KernelDraw_0010A750

## dds3CallGrAllocPacketBuff8 (kernel/dds3KernelDraw_0010A750, raidou1 0x208, raidou2 0x208)

Raidou 1: 15 of 130 words differ, first at +0x54.
Raidou 2: 15 of 130 words differ, first at +0x54.

which stack float params get callee-saved registers follows their live length: retail reloads x3..z3 from the arg slots, so their stores come last in source; vectors: menu 32 rows, ingame and opening (vsum), no own stores, callee mix sdfGrAllocPacketBuff/sdf3DPrimMakePoly as the draft calls

Related: register-allocation.
