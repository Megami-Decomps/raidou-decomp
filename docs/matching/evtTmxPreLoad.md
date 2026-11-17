# Draft notes: evtTmxPreLoad

## EndCallback_evtTmxPreLoad (evtTmxPreLoad, raidou1 0x180)

Raidou 1: 53 of 94 words differ, first at +0xA4; emitted 376 bytes, native 384 bytes.

the mask block schedules the pointer add before the fileSize compare and leaves its delay slots unfilled; ternary, flipped if, a separate mask pointer and a swapped compare tried

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).
