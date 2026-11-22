# Draft notes: sbtlDMG

## sbtlCheckHitArea_sbtlDMG (sbtlDMG, raidou1 0x270)

Raidou 1: 71 of 154 words differ, first at +0x38; emitted 616 bytes, native 624 bytes.

The hittype tests and the return 0/1 tails are ordered differently (retail tests 1, below 2, 0, then 2).

Batch notes: 154 vs 156 words: hittype switch order and return-0/1 tails differ (retail tests ==1, <2, ==0, ==2 and sets v0 per tail). Logic walked against asm for all three hittypes. Menu call records (3, rows scenario): hittype 0, flg&0x800 clear (GetCylinderData path), ret 0; hittype 1/2 and the 0x800 path checked against asm only.

## sbtlCreateHitC_New (sbtlDMG, raidou1 0x884)

Raidou 1: 294 of 545 words differ, first at +0x4.

Same size; the first 251 words match, then the scheduling and register order of the field copies differ.

Batch notes: 545 words, same size; first 251 words match, then scheduling/register order of the field copies differ. Every store, call and branch walked against asm. Menu call record (1 row, truncated: args and ret only; ewk NULL, skillid 149, serialID2 -1, ret 1 = lwk->serialID) so the body is checked against asm only. SbtlGetSide.c changed, same diff). Uses the {1,1,1,1} initializer at sbtlCreateHitC_New_sbtlDMGTable 0x458D20.

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).

## sbtlGetSide (sbtlDMG, raidou1 0x114)

Raidou 1: 53 of 70 words differ, first at +0x18; emitted 284 bytes, native 276 bytes.

Retail tests the field414 bits with andi and sltu where ee-gcc folds them to shifts, and recomputes acterside & 0xFF; argument registers differ from there.

Batch notes: 71 vs 70 words: retail tests field414 bits with andi+sltu/sltiu where ee-gcc folds mine to shifts, and recomputes acterside&0xff instead of keeping a copy; argument registers permuted from there. Menu call record (1 row, area 0x8021) agrees; bst 9/10 path checked against asm only.

## sbtlHitCProc_Idou (sbtlDMG, raidou1 0xA7C)

Raidou 1: 614 of 651 words differ, first at +0x3C; emitted 2604 bytes, native 2684 bytes.

The draft is 20 words shorter: retail calls sbtlResetHitPos_XYZ twice, once with the effect and once with NULL, where this passes the pointer; scheduling differs throughout.

Batch notes: Draft from decoded asm control flow (decode.txt), 651 vs 671 words: retail splits ResetHitPos_XYZ into ewk/NULL calls; 32 menu call records all take the empty-list return (lwk->wk NULL), every other path checked against the asm.

N3 guard sweep: the adjacent same-value guards merged into one || guard, a few words closer; the residual is elsewhere.

Related: [smt3 levers: duplicated tails and cross-jumping](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/cross-jumping.md), [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).

## sbtlLoadAttackHitData (sbtlDMG, raidou1 0x3E4)

Raidou 1: 84 of 245 words differ, first at +0xC; emitted 980 bytes, native 996 bytes.

Register allocation of the stream pointer and handles and the shared return blocks differ; same calls and stores.

## sbtlLoadAttackHitData_Decode (sbtlDMG, raidou1 0x12C)

Raidou 1: 12 of 75 words differ, first at +0x4.

a1 and a2 swap (match flag and wk_sbtl). Retail has a dead field414 & 2 test, reproduced by the else-if branch with the same call.

N2: declaring match and exist first is the same 12; match = 1 with the inverted test is worse.

Related: [smt3 levers: cross-block locals with swapped registers](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/cross-block-locals.md).

## sbtlSetHitPos (sbtlDMG, raidou1 0x4B4)

Raidou 1: 293 of 302 words differ, first at +0x4; emitted 1216 bytes, native 1204 bytes.

Retail copies the angle table (sbtlSetHitPosTable) after the three memsets, this copies it at the declaration; registers differ from there.

Batch notes: 302 vs 301 words: retail copies the angle table (sbtlSetHitPosTable 0x458D50, 9 floats) after the three memsets, mine at the declaration (retail likely struct-assigns a named table), shifting registers from +0x4; logic walked against asm and both jump tables. Menu call records (32 rows, two callers): iti 1, takasa 1, muki 0, ewk NULL, all three outputs; iti 2/3/6/7, helper-mark (iti&0xF0), takasa 0/2/3/5 and muki 8 checked against asm only. HD sBtlGetGroundPos, rename candidate)

Related: [dds idioms: switches and jump tables](https://github.com/Megami-Decomps/dds-decomp/blob/main/docs/idioms.md#switches-and-jump-tables).

## sbtlDamageProc (sbtlDMG, raidou1 0xD8)

Raidou 1: 42 of 51 words differ, first at +0x20; emitted 204 bytes, native 216 bytes.

Branch folding: gcc turns both acterID compares into xor/sltiu; retail keeps bnel and beq branches with separate return 0 and return 1. Comparing id == hwk->acterID gives retail's register allocation (wk in s0); an id local declared first does not.

## sbtlDamageProc (sbtlDMG, raidou2 0xD8)

Raidou 2: 42 of 51 words differ, first at +0x20; emitted 204 bytes, native 216 bytes.

Branch folding: gcc turns both acterID compares into xor/sltiu; retail keeps bnel and beq branches with separate return 0 and return 1. Comparing id == hwk->acterID gives retail's register allocation (wk in s0); an id local declared first does not.

## GetDamage (sbtlDMG, raidou1 0xBD4)

Raidou 1: 569 of 721 words differ, first at +0x4; emitted 2884 bytes, native 3028 bytes.

Retail retests `tbl->flg & 1` on every path into the buff and vice blocks and keeps the attacker in a callee-saved register across both lookups; the draft folds the repeated tests, so it is 36 words short and allocates differently from the prologue. nm_behaviour.py agrees apart from one reload of autost_cnt and the folded branches.

## sbtlSetSpecialDamage_DamageOnly (sbtlDMG, raidou1 0x14C)

Raidou 1: 66 of 80 words differ, first at +0x20; emitted 320 bytes, native 332 bytes.

Scheduling and block order: retail sets the scrStep 0 call's arguments before the scrStep test and places the shared hp/motion_setno/scrStep store before the default branch. Else-if chains, a switch, a nested default test and per-branch stores keep the arguments after the branch.

## sbtlCheckDamageTarget_Skilltbl (sbtlDMG, raidou1 0x170)

Raidou 1: 34 of 82 words differ, first at +0x34; emitted 328 bytes, native 368 bytes.

Branch folding: retail keeps the flg == 0 path's checks as separate branches with their own return 0 and return 1; gcc either cross-jumps that path into the shared CheckLiveTarget/case 9 tail or folds its last test into sltiu. Early returns, nested ifs, a ternary and an ok local for the shared tail were tried.

## sbtlUnsetKaraburi (sbtlDMG, raidou1 0x11C)

Raidou 1: 39 of 68 words differ, first at +0x60; emitted 272 bytes, native 284 bytes.

Block order: retail tests skillid < SKLID_ATK and == SKLID_ATK with both bodies (the UnsetKaraburiSub call and the devilId switch) out of line and the fall-through returning. Nested if/else, else-if chains in both orders, early returns (one word over) and a combined range test do not give it.

## sbtlCreateHitC_None (sbtlDMG, raidou1 0x20C)

Raidou 1: 4 of 131 words differ, first at +0x44.

Scheduling at the start: retail stores skillid between the acterID and actID stores; gcc hoists the skillid store above the first acter load. Tried: three orders of the three stores.

## sbtlCheckGround_sbtlDMG (sbtlDMG, raidou1 0x24C)

Raidou 1: 7 of 147 words differ, first at +0x1D4.

Block order at the end: retail stores st after the chousin effect calls and then reaches a shared return-0 block that the other branch's flg & 2 test jumps back to. gcc fills the branch delay slot with the store and puts that block last. Tried: inverted test, explicit returns in each branch, a negated flg test, swapping the branches.

## sbtlSetApos12 (sbtlDMG, raidou1 0x26C)

Raidou 1: 100 of 154 words differ, first at +0xD8; emitted 616 bytes, native 620 bytes.

Retail recomputes the address of dir into v1 for the load right after storing dir, and the scale temporaries land in a3, t2 and t3; gcc reuses the store's v0 and puts every temporary in v0/v1. Everything else matches. Tried: a barrier load of dir, a different store operand.

## sbtlCreateTargetList_csr (sbtlDMG, raidou1 0x2B8)

Raidou 1: 71 of 174 words differ, first at +0x4.

Register allocation: retail gives i s2 and lets wk and best share s3, and keeps the constants 0x8800 and 6 inside the selection loop; gcc swaps those registers and hoists both constants into s8 and s5. Tried: local declaration orders, a release in each branch, an early continue for the non-6 case.

## sbtlSetDamage (sbtlDMG, raidou1 0x318)

Raidou 1: 169 of 194 words differ, first at +0x60; emitted 776 bytes, native 792 bytes.

Retail reloads skl->flg after the s_type & 8 test on both paths and cross-jumps the final sbtlCheckBattlePara call into the flg & 0x40 path; gcc keeps a copy of flg from the first test, which shifts registers and block order from the first branch on. Tried: an s_type & 8 early return, the hp and mp clamps as the CLAMP ternary, separate 0x40 and 0x80 tests.

## sbtlGetHit_OnlyEndPos (sbtlDMG, raidou1 0x468)

Raidou 1: 39 of 281 words differ, first at +0x198; emitted 1124 bytes, native 1128 bytes.

Retail picks the found unit in the iti 2 search with movn and keeps &unit->rot in s2 across the set == 1 block, where gcc branches with bnel and recomputes it; the final scale also loads visual[0] and the zero into swapped float registers. Tried: the found unit as a ternary, the stock pick as a ternary, the iti 1/4/5/8 case moved last, the ewk->pos copy without a memory clobber, the scale operands swapped.

## sbtlSetSpecialDamage (sbtlDMG, raidou2 0x338)

Raidou 2: 55 of 206 words differ, first at +0x58.

Retail fills the delay slot of the first field414 & 2 test with the wk_sbtl load and leaves a nop later; gcc puts the skillid copy for sbtlSetDamage there, which shifts every word after it by one. Tried: nested ifs for both field414 tests, the search folded into the condition, swapped locals, the halving inside the first test.

## sbtlSetDyingDamage (sbtlDMG, raidou2 0x430)

Raidou 2: 231 of 264 words differ, first at +0xC; emitted 1056 bytes, native 1072 bytes.

gcc swaps the saved registers of wk and the attack time (retail keeps wk in s1, time in s0) and folds the devilId 455..456 test into one unsigned compare where retail tests < 457 and < 455 separately; retail also schedules the time * 3 >> 1 into the delay slots around sbtlChkHaveMemSklParty. Tried: the party check before the multiply, time declared first, the range test as a negated >= 455.

## sbtlCallBtlGetStartTimeDamage (sbtlDMG, raidou2 0xA4)

Raidou 2: 13 of 42 words differ, first at +0x24; emitted 168 bytes, native 164 bytes.

Register shape: the result local is allocated to $4 and copied to $2 at the shared exit; retail writes $2 in each arm's delay slot, and also orders the lui of the colour after the wk argument copy. Tried early returns per arm (34 of 42), the result initialised at its declaration (38), and the hoisted pos local with a result per arm (13). Vectors do not reach (no scenario), checked against the asm.

## datSetKillDevil (sbtlDMG, raidou2 0xB4)

Raidou 2: 8 of 46 words differ, first at +0x2C; emitted 184 bytes, native 180 bytes.

Tail-call shape only: retail's last switch arm (devil 391) calls evtBitOn with jal and falls into the shared epilogue, the other three sibcall with j; cc1 sibcalls all four. Tried a switch, an if/else chain and a return per arm; the smt3 lever (control-flow.md, a dead store in that arm) is not allowed here, and FAKEMATCH_ASM_READ(0) after the call stops the sibcall but adds a move $2,$0 (6 words). Vector checked (battle: devil 466, default path).

## sbtlBtlSetDamageProcSBtlDMGSub (sbtlDMG, raidou2 0xE0)

Raidou 2: 29 of 54 words differ, first at +0x4; emitted 216 bytes, native 224 bytes.

Case-tree shape: the hit-stop switch builds the same root (673) and right half (pivot 676-677) as retail, but balance_case_nodes picks 147 as the left pivot where retail tests 146 first; retail's left half counts as if one node below 146 were a range. Values per case follow the asm (144, 147, 673, 678, 681 -> 10; 146 -> 11; 148 -> 6; 672 -> 7; 674 -> 14; else 9). Vector checked (battle: 148 -> 6, 176 -> 9).

## CheckAutoSkill (sbtlDMG, raidou2 0x1C0)

Raidou 2: 76 of 109 words differ, first at +0x4; emitted 436 bytes, native 448 bytes.

Register and scheduling shape: retail zeroes the slot result in $3 ahead of the prologue stores and reloads wk_sbtl for the sbtlSetBit argument, cc1 keeps the result in $4 and reuses the cached wk_sbtl, so every later word shifts. Tried a for and a post-increment while loop over the six auto-skill slots, a result variable for the tail (84 of 112) and early returns (76). Vector checked (battle: returns 1).
