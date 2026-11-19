# Draft notes: code_002F5CF0

## SetAddMag (code_002F5CF0, raidou2 0x60)

Raidou 2: 9 of 21 words differ, first at +0x20; emitted 84 bytes, native 96 bytes.

Retail stores the per-frame step in each branch of the >0 test with the quotient copied in the delay slot, and orders the slti before the bias add; the draft merges the stores. Tried a ternary, an inverted test and a direct quotient test. 9 words; behaviour equal (nm_behaviour reports only the merged store of the constant 1).

## sbtlInitSBtlSub2 (code_002F5CF0, raidou2 0x5C)

Raidou 2: 22 of 24 words differ, first at +0x8; emitted 96 bytes, native 92 bytes.

Same stores; retail keeps them in source order with one -1 register, the draft groups the three byte stores first with a second -1 register. 22 words; behaviour equal (nm_behaviour clean).

## sbtlDrawNum4 (code_002F5CF0, raidou2 0x54)

Raidou 2: 19 of 19 words differ, first at +0x0; emitted 76 bytes, native 84 bytes.

Retail copies a_num and a_x into other registers and computes the three compares before the conditional moves. Tried chained ifs and HD's three-variable form. 19 words; behaviour equal (nm_behaviour reports only retail's xori 0 and the draft's 60 by conditional move).

## sbtlSelectTargetCursor (code_002F5CF0, raidou2 0x6C)

Raidou 2: 18 of 28 words differ, first at +0x1C; emitted 120 bytes, native 108 bytes.

Retail keeps tgtList in $6 and the zero result in $2 from the first test; the draft allocates them the other way. Tried a tgtList local with one && test and a result variable (both worse). 18 words; behaviour equal (nm_behaviour clean).

## sbtlDrawFuncTalk (code_002F5CF0, raidou2 0xF0)

Raidou 2: 27 of 60 words differ, first at +0x2C; emitted 272 bytes, native 240 bytes.

Retail sends every result branch to one shared exit; the draft splits the epilogue. Tried the if chain, a switch and nested ifs in retail test order. 27 words.

## sbtlCheckProcEnd (code_002F5CF0, raidou2 0x88)

Raidou 2: 18 of 29 words differ, first at +0x28; emitted 116 bytes, native 136 bytes.

Retail masks both the count and the count + 1 before each test and picks one; the draft masks after the add. Tried a zero start with four ifs and a ternary per check (worse). 18 words; behaviour equal (nm_behaviour clean).

## sbtlHelpEx (code_002F5CF0, raidou2 0x174)

Raidou 2: 6 of 93 words differ, first at +0xF8.

Code matches except the font colour: retail builds 0xA09DC300 in v1 before the shift and divide, cc1 swaps v1 and a1. Tried the constant first, a u32 cast and *128. 6 words. Behaviour equal (nm_behaviour clean).

## sbtlBarCtrl (code_002F5CF0, raidou2 0x21C)

Raidou 2: 2 of 135 words differ, first at +0x1E8.

The final evtBtlBarCtrlSub argument (field41FC - 126) * 16 is compiled as sll then subtract 0x7E0 where retail subtracts 126 first (2 words). Tried a switch (jump table), if chains and the zero case as the else arm. Vector checked (R2 battle/menu rows, the paths they take). nm_behaviour reports the missing -0x7E immediate: the draft folds (field41FC - 126) * 16 into x*16 - 2016, the same value.

## sbtlPlayerData (code_002F5CF0, raidou2 0x288)

Raidou 2: 118 of 157 words differ, first at +0x2C; emitted 628 bytes, native 648 bytes.

The bullet animation counter (118 words): retail keeps the incremented sbtlPlayerDataData byte in a register stored on both arms and sign-extended at each use; this reloads it with lb after the store. Tried an s8 local set on both arms, ++ on the global, and a u8 global with (s8) casts. Vector checked: R2 battle/menu rows are TRUNC with no inputs or stores, checked against the asm.
