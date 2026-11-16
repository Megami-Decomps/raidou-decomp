# Draft notes: field/fldProcess

## ProcSequence (field/fldProcess, raidou1 0x2D08)

Raidou 1: 2571 of 2685 words differ, first at +0x0; emitted 10740 bytes, native 11528 bytes.

Raidou 1: the compile also differs from retail in its rodata at 0x00447640; rodata at 0x00447680.

Nothing is recorded about this draft.

Fragile: the -DNON_MATCHING count moves with the declarations fldProcess.c includes; retyping the model globals in fldProcess.h (pointer type only, passed to mdlMotionPause) moved it from 2565 to 2571 words.

First differing words:

| Offset | Draft | Retail |
|---|---|---|
| +0x0 | `addiu sp,sp,-176` | `addiu sp,sp,-160` |
| +0x4 | `lui v0,0x0` | `lui v1,0x3c` |
| +0x8 | `sd s4,160(sp)` | `sd s5,136(sp)` |
| +0xC | `addiu s4,sp,16` | `addiu s5,sp,16` |

## fldProcTrigSequence (field/fldProcess, raidou1 0x12C, raidou2 0x12C)

Raidou 1: 10 of 75 words differ, first at +0x48.
Raidou 2: 10 of 75 words differ, first at +0x48.

btl-return copy run: retail groups the four loads before the stores; scene/controller pointer locals as smt3; u32 alias-set copies untried (cast)

Related: alias.

## MskL2Task (field/fldProcess, raidou1 0x1E0)

Raidou 1: 8 of 120 words differ, first at +0x30.

guard registers: the jms and fldGlobalBase loads land in a0 where retail uses v0; local seq pointer matches the s1 base; fused and nested guards worse

Related: regalloc.
