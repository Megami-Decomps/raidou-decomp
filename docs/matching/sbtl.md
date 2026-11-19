# Draft notes: sbtl

## sbtlSetAutoSkill_GBID (sbtl, raidou1 0xC0)

Raidou 1: 16 of 48 words differ, first at +0x30.

FPU register allocation: retail holds the backup value in f3 and divides into f0 in both cases; the draft swaps them. Behaviour matches (nm_behaviour clean).

## sbtlCheckHaveskill_Atk (sbtl, raidou1 0xA8)

Raidou 1: 7 of 42 words differ, first at +0x40.

Register allocation: retail keeps the skill table pointer in a0 and presets v0 = 1 before the type load; the draft keeps it in a1. Behaviour matches.

Related: Declaration order and block-scoped locals did not change it..
