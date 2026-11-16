# Draft notes: event/evtPolygonMovie

## evtHokanSceneLight (event/evtPolygonMovie, raidou1 0x194, raidou2 0x194)

Raidou 1 and Raidou 2: 38 of 101 words differ, first at +0x18.

Same length and structure, but the loop counts up and the registers are numbered differently (retail counts down from 2).

## evtCheckSeqEnd (event/evtPolygonMovie, raidou1 0x64, raidou2 0x64)

Raidou 1: 3 of 25 words differ, first at +0x4C.
Raidou 2: 3 of 25 words differ, first at +0x4C.

last guard: retail beq on pInfo->flags&0x80 with v0=0 reused, mine bne+li 1/li 0; positive workdata guard (if workdata!=NULL{...}return 1) fixed the beql and v0 reg (9->3); tried ||, goto end, ret var, inverted last guard; vector checked (opening), same on R2

Related: [smt3 levers: callee visibility and branch-likely](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/callee-visibility.md).
