# Draft notes: evtTmxPreLoad

## evtLeaf28 (evtTmxPreLoad, raidou1 0x4C)

Raidou 1: 17 of 20 words differ, first at +0x8; emitted 84 bytes, native 76 bytes.

retail clears s32 used[2] with one sd; brace init gives memset, element stores give two sw

Related: local-array-init.

## EndCallback_evtTmxPreLoad (evtTmxPreLoad, raidou1 0x180)

Raidou 1: 53 of 94 words differ, first at +0xA4; emitted 376 bytes, native 384 bytes.

the mask block schedules the pointer add before the fileSize compare and leaves its delay slots unfilled; ternary, flipped if, a separate mask pointer and a swapped compare tried

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).
