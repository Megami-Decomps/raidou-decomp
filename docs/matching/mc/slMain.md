# Draft notes: mc/slMain

## fmProcTrigSequence_slMain (mc/slMain, raidou1 0x300)

Raidou 1: 68 of 190 words differ, first at +0x16C; emitted 760 bytes, native 768 bytes.

mode-4 arm stores fresh constants 1 (flag and mode) where the draft reuses s0 = on, two words short, which shifts the rest; v0/v1 swap on DDS3_GBWK in mode 2; Raidou 1 only

Related: [smt3 levers: stores](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/stores.md).

## slQueryYesNoDraw (mc/slMain, raidou1 0x960)

Raidou 1: 0 of 600 words differ.

The text now matches all 600 words (slDrawWindow is C), but it stays NON_MATCHING: the local array's strings and pointer table land in this unit's .rodata, which then differs from retail at 0x455270 while slQueryYesNoDrawTable stays INCLUDE_RODATA. Promoting it needs that rodata moved into the C.

y = 306; if (m != 5 && m != 6) y = 258; folds to addiu/sltiu/movz; nested if (m != 5) { if (m == 6) y = 306; else y = 258; } gives retail's beq + xori/movn; remaining residual is slDrawWindow callee-visibility

The draft's local initialized array of the 20 "Loading file N." strings emits its own rodata in the NON_MATCHING build, so that build's .rodata differs from retail at slQueryYesNoDrawTable (0x455298); the default build keeps INCLUDE_RODATA and matches.

Related: control-flow.
