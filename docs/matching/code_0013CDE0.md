# Draft notes: code_0013CDE0

## mapBgFScrollOn (code_0013CDE0, raidou1 0x80)

Raidou 1: 22 of 32 words differ, first at +0xC.

store order only: with each arm's retail-second store written last (sndSePlay_sndAt3 pattern) the arms no longer cross-jump (27 -> 22), but sched2 regroups the swc1 runs by source register (x, y, z stores pulled together) where retail keeps 68/65/69/66/70/67 interleaved; vectors do not reach, checked against the asm

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).

## sndSePlay_sndAt3 (code_0013CDE0, raidou1 0x80)

Raidou 1: 8 of 32 words differ, first at +0x50.

zero arm matches: f32 zero local (mtc1 + swc1, lever addressing-and-types.md:195) and one store per statement; sched2 hoists the source-last store to just after the first, so retail's second store (mapData64) is written last; nonzero arm is the same shape, next try: mapBgSetScrollData3 = hokan written last there (budget spent); vectors do not reach, checked against the asm

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).

## fldBgZoomTest (code_0013CDE0, raidou1 0x2B4)

Matches.

Fakematch: three FAKEMATCH_ASM_LIVE(mode) after the mapZoomSetGoal calls that must stay jal. Without them the function is 14 words off; the same result with C dead stores matches but is not allowed.

## mapCutBgLoad (code_0013CDE0, raidou1 0x88)

Matches.

Fakematch: one FAKEMATCH_ASM_LIVE(cut) after mapBgFree2. Without it the function is 30 words off; the same result with a C dead store matches but is not allowed.
