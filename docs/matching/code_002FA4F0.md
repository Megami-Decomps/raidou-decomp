# Draft notes: code_002FA4F0

## fldCallMemGetPtr11 (code_002FA4F0, raidou1 0xE4)

Raidou 1: 13 of 57 words differ, first at +0x20.

two empty count loops: count/i register pair swapped (a0/v1) and second loop bnel in retail; empty for over a field hoists into v0 then a0

N2: s32 i with the field as the bound gives a count-down loop (12, different shape); u32 i that way is 44; count declared before i is the same 13.

Related: register allocation.

## fclCallSetFontColorNo (code_002FA4F0, raidou1 0x6E0)

Raidou 1: 370 of 435 words differ, first at +0x8; emitted 1740 bytes, native 1760 bytes.

regalloc: switch(page) with case 3, 1-2, 0 order gives retail's block layout (case 0 init then jump to its loop test at the end); retail hoists the color constants 5 and 1 into s4/s8 and keeps col in s7, mine keeps col in s8 and leaves 1 unhoisted (movz instead of movn), shifting every s register by one; if/else chain, ternary colors, x=0 hoisting and u8/s32/u32 color tried; vector checked (opening, title-ng: page 0, mode 0)

Related: regalloc.

## fclCallSetFontColorNo3 (code_002FA4F0, raidou1 0x118)

Raidou 1: 4 of 70 words differ, first at +0x44.

register/schedule: with a u16 id local the i++ lands before the slot addu instead of the lhu branch delay slot; without the local the count loads into a1 not a2 (4 words each); visible counter n separate from i (x + n*dx) found from asm; vectors do not reach, checked against the asm. Finding: a u16 local for a reloaded lhu field moves the loop increment out of the branch delay slot; dropping it moves the count load from a2 to a1

Related: loop increment placement and count register (a1 vs a2).
