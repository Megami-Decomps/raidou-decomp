# Draft notes: code_0012ECC0

## fldSetUnit (code_0012ECC0, raidou2 0x7E4)

Raidou 2: 1 of 505 words differ, first at +0x518.

The R1 residual, ported: the else arm's `if (fldSetUnitSub()) fldCallMemFreeDelayedDirect2();` emits beql where retail has beqz; the slot holds the join's DDS3_GBWK load either way, so only the annul bit differs. R2 differences from R1 (tipsProcessCheck, one bad status, the aibo map/cut exclusions, the word204/word1A8 calls and the ppos01 id in nbData21) all match. the radius flags are byte 0 of SBTL_MOTION_VISUAL in R2 (field00, lbu 0x0), not kind at byte 1; nm_behaviour clean; vector checked (opening, two truncated rows), checked against the asm.

Related: delay-slot.
