# Draft notes: sbtl

## sbtlSetAutoSkill_GBID (sbtl, raidou1 0xC0)

Raidou 1: 16 of 48 words differ, first at +0x30.

FPU register allocation: retail holds the backup value in f3 and divides into f0 in both cases; the draft swaps them. Behaviour matches (nm_behaviour clean).
