# Draft notes: code_002FA4F0

## fldCallMemGetPtr11 (code_002FA4F0, raidou1 0xE4)

Raidou 1: 13 of 57 words differ, first at +0x20.

two empty count loops: count/i register pair swapped (a0/v1) and second loop bnel in retail; empty for over a field hoists into v0 then a0

Related: register allocation.

## fclCallSetFontColorNo (code_002FA4F0, raidou1 0x6E0)

Raidou 1: 370 of 435 words differ, first at +0x8; emitted 1740 bytes, native 1760 bytes.

regalloc: switch(page) with case 3, 1-2, 0 order gives retail's block layout (case 0 init then jump to its loop test at the end); retail hoists the color constants 5 and 1 into s4/s8 and keeps col in s7, mine keeps col in s8 and leaves 1 unhoisted (movz instead of movn), shifting every s register by one; if/else chain, ternary colors, x=0 hoisting and u8/s32/u32 color tried; vector checked (opening, title-ng: page 0, mode 0)

Related: regalloc.
