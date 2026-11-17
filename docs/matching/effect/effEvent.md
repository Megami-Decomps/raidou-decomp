# Draft notes: effect/effEvent

## effSimpleBEDDelete (effect/effEvent, raidou1 0x40, raidou2 0x40)

Raidou 1: 1 of 16 words differ, first at +0x1C.
Raidou 2: 1 of 16 words differ, first at +0x1C.

1 word off: retail beq with a0=s0 in the delay slot, ours beql (annul bit); a duplicated free in both arms and a local for the original were tried

Related: branch shape does not reach the annul bit.
