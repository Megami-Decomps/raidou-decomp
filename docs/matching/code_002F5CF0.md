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
