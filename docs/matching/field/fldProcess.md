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

## fldSaveLabel (field/fldProcess, raidou1 0x14D4)

Raidou 1: 1291 of 1334 words differ, first at +0x0; emitted 5352 bytes, native 5332 bytes.

Complete body, checked against the asm block by block (flow, call arguments, store order, the kind 9/10/4/14 if-chain, the word138 jump table); 5352 vs 5332 bytes, untuned. mapIgnite and mapIfDrawActionMaster take 6 arguments (t1 = word2A8); mapCallSetPlayerTargetData and mapCallSetPlayerTargetData2 take the r (f14) and attr (a0) arguments retail passes. Raidou 1 only. Vector checked (ingame: eveHitOn path with a NULL label, no target, no warp).

Related: register allocation and frame: retail holds the constant 1 in s3 from entry (reused for kind; the mwall popup stores s3 as word298), frame 112 vs 96 with s7 = &fldDbg and s8 the string base; retail's second word148 test is word148 == 0x2000 unmasked.
