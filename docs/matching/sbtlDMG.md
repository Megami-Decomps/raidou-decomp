# Draft notes: sbtlDMG

## sbtlCheckHitArea_sbtlDMG (sbtlDMG, raidou1 0x270)

Raidou 1: 71 of 154 words differ, first at +0x38; emitted 616 bytes, native 624 bytes.

The hittype tests and the return 0/1 tails are ordered differently (retail tests 1, below 2, 0, then 2).

Batch notes: 154 vs 156 words: hittype switch order and return-0/1 tails differ (retail tests ==1, <2, ==0, ==2 and sets v0 per tail). Logic walked against asm for all three hittypes. Menu call records (3, rows scenario): hittype 0, flg&0x800 clear (GetCylinderData path), ret 0; hittype 1/2 and the 0x800 path checked against asm only.

## sbtlCreateHitC_New_sbtlDMG (sbtlDMG, raidou1 0x884)

Raidou 1: 294 of 545 words differ, first at +0x4.

Same size; the first 251 words match, then the scheduling and register order of the field copies differ.

Batch notes: 545 words, same size; first 251 words match, then scheduling/register order of the field copies differ. Every store, call and branch walked against asm. Menu call record (1 row, truncated: args and ret only; ewk NULL, skillid 149, serialID2 -1, ret 1 = lwk->serialID) so the body is checked against asm only. SbtlGetSide.c changed, same diff). Uses the {1,1,1,1} initializer at sbtlCreateHitC_New_sbtlDMGTable 0x458D20.

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).

## sbtlGetSide (sbtlDMG, raidou1 0x114)

Raidou 1: 53 of 70 words differ, first at +0x18; emitted 284 bytes, native 276 bytes.

Retail tests the field414 bits with andi and sltu where ee-gcc folds them to shifts, and recomputes acterside & 0xFF; argument registers differ from there.

Batch notes: 71 vs 70 words: retail tests field414 bits with andi+sltu/sltiu where ee-gcc folds mine to shifts, and recomputes acterside&0xff instead of keeping a copy; argument registers permuted from there. Menu call record (1 row, area 0x8021) agrees; bst 9/10 path checked against asm only.

## sbtlHitCProc_Idou (sbtlDMG, raidou1 0xA7C)

Raidou 1: 617 of 651 words differ, first at +0x3C; emitted 2604 bytes, native 2684 bytes.

The draft is 20 words shorter: retail calls sbtlResetHitPos_XYZ twice, once with the effect and once with NULL, where this passes the pointer; scheduling differs throughout.

Batch notes: Draft from decoded asm control flow (decode.txt), 651 vs 671 words: retail splits ResetHitPos_XYZ into ewk/NULL calls; 32 menu call records all take the empty-list return (lwk->wk NULL), every other path checked against the asm.

Related: [smt3 levers: duplicated tails and cross-jumping](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/cross-jumping.md), [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).

## sbtlLoadAttackHitData (sbtlDMG, raidou1 0x3E4)

Raidou 1: 84 of 245 words differ, first at +0xC; emitted 980 bytes, native 996 bytes.

Register allocation of the stream pointer and handles and the shared return blocks differ; same calls and stores.

## sbtlLoadAttackHitData_Decode (sbtlDMG, raidou1 0x12C)

Raidou 1: 12 of 75 words differ, first at +0x4.

a1 and a2 swap (match flag and wk_sbtl). Retail has a dead field414 & 2 test, reproduced by the else-if branch with the same call.

Related: [smt3 levers: cross-block locals with swapped registers](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/cross-block-locals.md).

## sbtlReleaseHitC_List (sbtlDMG, raidou1 0x158)

Raidou 1: 69 of 84 words differ, first at +0x2C; emitted 336 bytes, native 344 bytes.

Retail loads num into v0 and sets an explicit return 0 after it (two more words); the rest shifts.

Batch notes: 84 vs 86 words: retail loads num into v0 and sets an explicit return 0 after it (2 extra words), everything after shifts; body otherwise lines up (cmp shows only that hunk). Menu call records (32, rows scenario) all take the empty-list exit (ret 0); unlink/free path checked against asm.

## sbtlSetHitPos (sbtlDMG, raidou1 0x4B4)

Raidou 1: 293 of 302 words differ, first at +0x4; emitted 1216 bytes, native 1204 bytes.

Retail copies the angle table (sbtlSetHitPosTable) after the three memsets, this copies it at the declaration; registers differ from there.

Batch notes: 302 vs 301 words: retail copies the angle table (sbtlSetHitPosTable 0x458D50, 9 floats) after the three memsets, mine at the declaration (retail likely struct-assigns a named table), shifting registers from +0x4; logic walked against asm and both jump tables. Menu call records (32 rows, two callers): iti 1, takasa 1, muki 0, ewk NULL, all three outputs; iti 2/3/6/7, helper-mark (iti&0xF0), takasa 0/2/3/5 and muki 8 checked against asm only. HD sBtlGetGroundPos, rename candidate)

Related: [dds idioms: switches and jump tables](https://github.com/Megami-Decomps/dds-decomp/blob/main/docs/idioms.md#switches-and-jump-tables).
