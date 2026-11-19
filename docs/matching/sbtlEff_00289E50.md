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
