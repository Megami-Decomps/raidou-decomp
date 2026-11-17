# Draft notes: dds3KernelDraw_0010BEE0

## dds3FilterClear (dds3KernelDraw_0010BEE0, raidou1 0x3C)

Raidou 1: 3 of 15 words differ, first at +0xC.

Data37 store is scheduled ahead of Data34/36 (35,37,34,36 vs retail 35,34,36,37); tried left-rotation lever, moving 37 later, HD block order (35,34 then source order, 37/36 last); vector checked boot: stores 0x80/1/0/1/0... as retail

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).

## dds3FilterDraw (dds3KernelDraw_0010BEE0, raidou1 0xA80)

Raidou 1: 3 of 672 words differ, first at +0x114.

Ported from DDS fldSkyDrawFilter with Raidou's globals (FIL_t record, gFilterFilterAlpOfs added after gFilterAnimeOfsA, evtSubmitDefaultDepthGradientRect at 64x64). 3 words differ in the gFilterMode switch's default path: retail leaves the beq to case 0 with an empty delay slot and jumps to the row = 0 store, mine fills it with the reload of current. Vector checked.

Related: sched.
