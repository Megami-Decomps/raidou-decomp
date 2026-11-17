# Draft notes: TitleMain

## evtLeaf31 (TitleMain, raidou1 0x110)

Raidou 1: 9 of 68 words differ, first at +0x1C.

placement fields read into locals at the top (64 to 13 words); left: register allocation of the work-entry address and the order of the two bounds stores; pointer local and mixed spelling tried

N2: bounds[2] stored before bounds[3] fixes the store order (13 -> 9). Left: the work-entry index/base pair takes v0/t4 swapped; splitting the pointer arithmetic does not move it.

Related: regalloc.
