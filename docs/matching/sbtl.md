# Draft notes: sbtl

## sbtlSetAutoSkill_GBID (sbtl, raidou1 0xC0)

Raidou 1: 16 of 48 words differ, first at +0x30.

FPU register allocation: retail holds the backup value in f3 and divides into f0 in both cases; the draft swaps them. Behaviour matches (nm_behaviour clean).

## sbtlCheckHaveskill_Atk (sbtl, raidou1 0xA8)

Raidou 1: 7 of 42 words differ, first at +0x40.

Register allocation: retail keeps the skill table pointer in a0 and presets v0 = 1 before the type load; the draft keeps it in a1. Behaviour matches.

Related: Declaration order and block-scoped locals did not change it..

## sbtlSetWeakBit (sbtl, raidou1 0x120)

Raidou 1: 4 of 72 words differ, first at +0xC.

Register allocation: the devilid / 32 rounding swaps v0 and v1 (slti result and the quotient candidate) in the first four instructions. Behaviour matches.

Related: A word index local, a bit local, a GBWK pointer local, plain assignment instead of |= and HD case order give the same 4 words or more; case order 96, 104, 126, 136, 214, 235, 234, 236 fixes the jump-table bodies..

## CalcAutoSkill (sbtl, raidou1 0x310)

Raidou 1: 40 of 196 words differ, first at +0x78.

FPU register allocation in the percent scaling: retail puts (f32)percent in f2, 100.0f in f0 and the scaled value in f1; the draft rotates the three registers in every case body. Same operations in the same order, same case layout and jump table.

Related: value * (percent / 100.0f) gives 43 words, a per-case f32 rate local 40, *=, an explicit (f32) cast and a hoisted float percent give 43 or worse..
