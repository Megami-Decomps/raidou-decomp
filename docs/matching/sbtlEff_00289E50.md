# Draft notes: sbtlEff_00289E50

## sbtlFunc (sbtlEff_00289E50, raidou1 0x144)

Raidou 1: 58 of 79 words differ, first at +0x8; emitted 316 bytes, native 324 bytes.

The table copy picks v1/a3 for source and destination where retail uses a3/t0, which shifts every later register.

Batch notes: Table copy picks v1/a3 for src/dst where retail uses a3/t0, shifting every later register (same issue as sbtlBtlLoadPlayerOnMemDataSub); logic same. Menu call records: id 4..18, ret follows the call. SbtlCallBtlSearchEffectData2 and sbtlBtlLoadPlayerOnMemDataSub third/second param now void *pData (call records pass a data pointer)

## sbtlFunc6 (sbtlEff_00289E50, raidou2 0x80)

Raidou 2: 6 of 30 words differ, first at +0x14; emitted 120 bytes, native 128 bytes.

Retail shares the table address and branch layout between the two ranges differently; six words. One draft only. 6 words; behaviour equal (nm_behaviour clean).

## sbtlBtlCheckEffectDataSub (sbtlEff_00289E50, raidou2 0x38)

Raidou 2: 9 of 13 words differ, first at +0x10; emitted 52 bytes, native 56 bytes.

Retail addresses the pack entry from &effpac[id].num (base + 0x10) and reads status at +6; the draft reads both from the entry base. 9 words; behaviour equal (nm_behaviour reports only the base+0x10 addressing (same two addresses)).

## sbtlSearchEffect_SAB (sbtlEff_00289E50, raidou2 0x74)

Raidou 2: 21 of 30 words differ, first at +0x20; emitted 132 bytes, native 116 bytes.

Retail enters the list loop with a jump to its bottom test; cc1 rotates the loop and duplicates the test at the top. Tried for/continue, nested ifs and an early return on num. 21 words; behaviour equal (nm_behaviour reports only the duplicated loop test).

## sbtlBtlForceDeleteEffectWorkIDSub2 (sbtlEff_00289E50, raidou2 0x48)

Raidou 2: 14 of 18 words differ, first at +0x4.

Same six stores; retail picks other registers and orders the address sums differently. 14 words; behaviour equal (nm_behaviour clean).

## sbtlCallIsSceanStop2 (sbtlEff_00289E50, raidou2 0x4C)

Raidou 2: 14 of 19 words differ, first at +0x4.

Retail copies the argument to $6 and loads the flag word into $4; the draft keeps the argument in $4. Tried a single && test and an early return. 14 words; behaviour equal (nm_behaviour clean).

## DeletePanel (sbtlEff_00289E50, raidou2 0x84)

Raidou 2: 3 of 33 words differ, first at +0x24.

Retail branches to the shared return with a likely branch and sets the result 1 before reloading wk_sbtl. Tried a result variable for both paths (worse). 3 words; behaviour equal (nm_behaviour clean).

## sbtlDeleteEffectData_SkillIDType (sbtlEff_00289E50, raidou2 0x94)

Raidou 2: 17 of 36 words differ, first at +0x8; emitted 144 bytes, native 148 bytes.

Retail tests st - 1 < 2 first, ORs 8 in both arms from one flg load and adds 0x80000000 in the other arm; the draft ORs 8 after the if. Tried HD's flag variable, both if orders and per-arm |= (all worse). 17 words; behaviour equal (nm_behaviour clean).

## sbtlForceDelete_ID_Acter (sbtlEff_00289E50, raidou2 0xE4)

Raidou 2: 5 of 57 words differ, first at +0x94.

Only the register of flg differs (v1 for v0) in the st arms. Tried an s32 flg, a block-scoped flg and stores without the local (worse). 5 words.

## sbtlForceDelete_ID_SummonFR (sbtlEff_00289E50, raidou2 0x9C)

Raidou 2: 20 of 38 words differ, first at +0x8; emitted 152 bytes, native 156 bytes.

Retail loads flg in the delay slot of the st test and ORs 0x80000000 in each arm (plus 8 in one) from one lui; the draft ORs it before the test. Tried HD's two constants in both test orders (worse). 20 words; behaviour equal (nm_behaviour clean).

## sbtlCallBtlSearchDelWorkSide (sbtlEff_00289E50, raidou2 0x12C)

Raidou 2: 36 of 75 words differ, first at +0x5C.

Inner loop register use differs from the first skill test on. Tried a for loop, a while loop and a pointer over haveskill. 36 words.

## sbtlSetSummonEffect_Nakama (sbtlEff_00289E50, raidou2 0xD4)

Raidou 2: 10 of 53 words differ, first at +0x3C.

Retail cross-jumps case 0 into the ND call and loads effList in the beql slot. Tried HD case order and return per case (both worse). 10 words.

## sbtlMainDraw (sbtlEff_00289E50, raidou2 0x13C)

Raidou 2: 71 of 80 words differ, first at +0x10; emitted 320 bytes, native 316 bytes.

The panel tests branch and allocate wk_sbtl and panel in different registers from the first test on. Tried if/else on resources and a result variable. 71 words.

## sbtlMainDraw2 (sbtlEff_00289E50, raidou2 0xC0)

Raidou 2: 38 of 46 words differ, first at +0x18; emitted 184 bytes, native 192 bytes.

Retail keeps wk_sbtl in $5 and the panel in $4, takes the resources test as a likely branch into the flag tests and jumps back for the zero return; the draft allocates the other way and falls through. Tried one && chain and an if/else on resources (worse). 38 words; behaviour equal (nm_behaviour clean).
