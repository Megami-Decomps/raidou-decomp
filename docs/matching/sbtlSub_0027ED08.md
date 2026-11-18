# Draft notes: sbtlSub_0027ED08

## sbtlGetSkillMenu (sbtlSub_0027ED08, raidou1 0x11C, raidou2 0x11C)

Raidou 1: 16 of 71 words differ, first at +0x24.
Raidou 2: 16 of 71 words differ, first at +0x24.

Register allocation: retail keeps ok in s0 and the haveskill pointer in s1, the draft swaps them (both games). Index or pointer walks, a down-counting loop, declaration order, ok scoped to the loop, and the pointer set before memset leave the swap or are worse.
