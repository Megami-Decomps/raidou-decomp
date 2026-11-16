# Draft notes: mc/slMain

## slDrawError (mc/slMain, raidou1 0x4A0)

Raidou 1: 5 of 296 words differ, first at +0xD4.

same residual as slDrawInfo: bnel for bne on the if (gslErrorCode == N) chain while slDrawWindow is INCLUDE_ASM

Related: callee-visibility.

## slDrawInfo (mc/slMain, raidou1 0x39C)

Raidou 1: 7 of 231 words differ, first at +0x14.

each if (gslInfoCode == N) test after a block that calls the INCLUDE_ASM slDrawWindow is bnel where retail has bne with the next constant in the slot; expected to close when slCallSetOt5 is C above it (idioms.md 12)

Related: callee-visibility.

## fmProcTrigSequence_slMain (mc/slMain, raidou1 0x300)

Raidou 1: 68 of 190 words differ, first at +0x16C; emitted 760 bytes, native 768 bytes.

mode-4 arm stores fresh constants 1 (flag and mode) where the draft reuses s0 = on, two words short, which shifts the rest; v0/v1 swap on DDS3_GBWK in mode 2; Raidou 1 only

Related: [smt3 levers: stores](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/stores.md).

## slQueryYesNoDraw (mc/slMain, raidou1 0x960)

Raidou 1: 11 of 600 words differ, first at +0x134.

y = 306; if (m != 5 && m != 6) y = 258; folds to addiu/sltiu/movz; nested if (m != 5) { if (m == 6) y = 306; else y = 258; } gives retail's beq + xori/movn; remaining residual is slDrawWindow callee-visibility

The draft's local initialized array of the 20 "Loading file N." strings emits its own rodata in the NON_MATCHING build, so that build's .rodata differs from retail at slQueryYesNoDrawTable (0x455298); the default build keeps INCLUDE_RODATA and matches.

Related: control-flow.
