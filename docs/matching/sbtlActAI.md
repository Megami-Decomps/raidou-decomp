# Draft notes: sbtlActAI

## sbtlCreate (sbtlActAI, raidou1 0xAC)

Raidou 1: 5 of 43 words differ, first at +0x70.

store schedule only: retail sw field10C right after flags then the floats, mine emits field108/field10C together after the floats; tried flags-first, HD order, field10C before flags; lever right-rotation not applicable; vector checked (menu)

N2: field10C = -1 written after field108 puts it right after the float stores (5 of 43); retail has it between the flags store and the floats. Chained 104/108 assignment and 10C after mode are worse (7, 8).

Related: [smt3 levers: stores](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/stores.md).

## SetUseskl (sbtlActAI, raidou1 0x3C)

Raidou 1: 10 of 15 words differ, first at +0x0.

v0/v1 swapped through the whole body (cond and index); tried sklid / 32, evtBitOn's explicit (no < 0) ? no + 0x1F : no index (matched sibling template), an s32 parameter, a cached element pointer (9 words, swaps the addu); SBTLUNIT_MODEL gains useskl[36] at 0x194 (HD useskl, bounded by field224); vector checked (menu row: bit 8 of word 0 set)

N2: a model pointer local, an index local and s32 useskl elements leave the v0/v1 swap unchanged (10).

Related: regalloc.

## sbtlCheckEquationProc (sbtlActAI, raidou1 0x32C)

Raidou 1: 106 of 193 words differ, first at +0x40; emitted 772 bytes, native 812 bytes.

wk->model->field inside a loop with a call: gcse puts the wk->model reload after the call and reloads the field at the loop top, as retail; a model local reassigned after the call lets gcse keep the field in a register; one s32 local reused for func/sel/jmp/code gives retail's single s0; vector checked (menu, 32 rows)

Related: regalloc.

## sbtlUseTableDataProc (sbtlActAI, raidou1 0x170)

Raidou 1: 79 of 90 words differ, first at +0x4; emitted 360 bytes, native 368 bytes.

(pat & 0xFFFF0000) >> 16 on s32 pat: the first use folds to srl, the loop-carried second keeps lui mask + and + srl, as retail; vector checked (menu, 30 rows)

Related: constants.

## sbtlEAIProc_Chase (sbtlActAI, raidou1 0x118)

Raidou 1: 30 of 62 words differ, first at +0x1C; emitted 248 bytes, native 280 bytes.

retail keeps two copies of the sbtlResetAAI/sbtlSetAIWkStat tail (frame-out and vertical-hit share one, lost-target has its own); mine cross-jumps them into one; also model in a1; tried nested if, goto lost, cmpframe-first compare; vector checked (menu)

Related: cross-jump.

## sbtlEAIProc_Summonw (sbtlActAI, raidou1 0x124)

Raidou 1: 69 of 72 words differ, first at +0x4; emitted 288 bytes, native 292 bytes.

body identical, shifted by one word: retail tests the flag with bne into the body (wk copy to s0 in its delay slot) and b to the shared exit with v0=0, mine beq to exit; tried early return 0 and the whole body inside if (flag) (worse, OVER); vector checked (menu)

Related: [smt3 levers: callee visibility and branch-likely](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/callee-visibility.md).

## sbtlSetNoMP (sbtlActAI, raidou1 0x80)

Raidou 1: 17 of 18 words differ, first at +0x0; emitted 72 bytes, native 128 bytes.

retail evaluates (flags&0x100) && !(flags&0x80) && wk_sbtl->mode==6 && (side&1) and discards it (paths converge), a compiled-out body; an empty if is deleted by gcc and a dead store is banned; returns model->field100 if mode<12 and (battleMode==3 or !(side&0x80)) else field104; name is fuzzy (HD sBtlSetNoMP differs); vector checked

Related: deadcode.

## sbtlBtlMVSupportSub (sbtlActAI, raidou1 0x4C)

Raidou 1: 13 of 19 words differ, first at +0x0.

retail keeps the last test as beq around v0=1 with sltu in the delay slot, mine if-converts to movn; side byte in v0 vs v1; tried early-return chain, ret local, || tail (worse); vector checked (menu)

N2: ret = 1 override after ret = side != sidelock (13), gotos to one return 1 (14) and a ret = 1 seed with nested tests (16) do not give retail's beq around v0 = 1.

Related: [smt3 levers: callee visibility and branch-likely](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/callee-visibility.md).

## sbtlSetNormalAction (sbtlActAI, raidou1 0x354)

Raidou 1: 4 of 213 words differ, first at +0x1DC.

model = wk->model inside both arms of an if around a STOREVECTOR gives retail's bnel with the model load in the delay slot; (s32)GetRand() for the cvt.s.w (u32 return adds the unsigned fixup); flags (f | C) & ~M keeps ori before and | the 4 words are li order of the mode/cmpframe constants in CTACT and SEARCH; mode-before-cmpframe statement order swaps the registers too (8 words), reverted; vector checked (menu, 32 rows)

Related: constants.

## GetSenkaiPointSub (sbtlActAI, raidou1 0x244)

Raidou 1: 57 of 145 words differ, first at +0x4.

retail loop entered by a jump to a bottom test holding the calls = while (i < 2 && (callA || !callB)) {...} then if (i < 2) store A else store B; a for with an in-loop return duplicates the simple exit test; point stores are STOREVECTOR_MEMOPS (wk->model stays CSE'd across them); vector checked (menu, 3 rows)

Related: loop-shape.

## sbtlAAIProc_Attack (sbtlActAI, raidou1 0x408)

Raidou 1: 125 of 256 words differ, first at +0xA8; emitted 1024 bytes, native 1032 bytes.

2 words short, the rest is shift: retail recomputes sp+32 for the flg-path VerticalArea arg, has a dead nop after the flg path's branch pair, stores mode after cmpframe/frame/flags in the hit block and case 16 (mine hoists the sh), and beq not beql after sbtlCheckStatus_Move; tried one model local (a3 for all, fixed by wk->model in the switch), early-local for the sltiu (fixed), store orders mode-first/flags-first (no change); vector checked (menu, case 14 hit path) | shapes: one SBTLUNIT_MODEL local reused across the frame clamp and the switch is one pseudo and lands in a3; retail splits it (a0 then a1), so the switch reads wk->model; hit = !(frame-1 < cmpframe) needs early = ...; hit = early == 0 for sltiu (addressing-and-types 709)

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).

## sbtlRefSbtl22 (sbtlActAI, raidou1 0x38)

Raidou 1: 11 of 14 words differ, first at +0x0.

register allocation only: retail wk_sbtl in a2, result in v0 from the start; tried && chain, ret local, nested ifs with id local (all wk_sbtl in a1, result in a2 or two exits); find_similar no matched sibling; vector checked (menu, ret 0)

N2: if (a && b) return x == y; return 0; is the same 11, the inverted early return is 14, a ret = 1 flag is 11.

Related: regalloc.

## sbtlGetUseTable_Target_sbtlActAI (sbtlActAI, raidou1 0x64C)

Raidou 1: 346 of 381 words differ, first at +0x3C; emitted 1524 bytes, native 1612 bytes.

Flow follows retail with shared search/fail/found tails; wk_sbtl reload register in cases 1/2 and s-register rotation differ, so case 2's tail cross-jumps. Vector checked.

Related: regalloc.

## Proc (sbtlActAI, raidou1 0x2F00)

Raidou 1: 2834 of 3008 words differ, first at +0x8; emitted 12044 bytes, native 12032 bytes.

Size and control flow match retail when compiled alone (12032 bytes, 12044 inside the unit); case order follows the jump table, the shared tail is labelled at the end of case 51.

Related: register allocation (retail holds &wk->rot in a 7th saved register) and a lw test of field414 & 0x82 that points at a u32 bitfield container at 0x414.

## SetChkskl (sbtlActAI, raidou1 0x3C)

Raidou 1: 10 of 15 words differ, first at +0x0.

v0/v1 swapped through the body, the SetUseskl shape (docs/matching/sbtlActAI.md); the &= ~ siblings sbtlLeaf65/sbtlResetUseskl match with the same C; a bit temp gets worse (14); SBTLUNIT_MODEL useskl[36] split into useskl[17] + chkskl[19] at 0x1D8 (HD chkskl); vectors do not reach, checked against the asm

Related: regalloc.

Vectors (battle, calls1): pass, 15 of 16 words covered.

## sbtlRefSbtl21 (sbtlActAI, raidou1 0x48)

Raidou 1: 12 of 18 words differ, first at +0x4.

Register allocation: retail holds status in a1, the draft in a0. Three separate &= statements give retail's three unfolded and instructions; a status local or repeating wk->aiwk does not move the register.

## sbtlCallBtlDeleteHitHelper (sbtlActAI, raidou1 0x8C)

Raidou 1: 3 of 35 words differ, first at +0x14.

Register allocation: retail reads wk->flg into v0 after sbtlRefSbtl19, the draft into v1, because sbtlRefSbtl19's s32 return keeps v0 live. Declaring the callee void would free it but needs a header change.

## sbtlAAIInit_N_Renkei (sbtlActAI, raidou1 0xA4)

Raidou 1: 37 of 42 words differ, first at +0xC; emitted 168 bytes, native 164 bytes.

The draft keeps the constant 2 for mode and stat in s2, which grows the frame; retail materialises it at each use. A model local, no local and a rot pointer local all keep the shared register.

## sbtlCallBtlGetBadStatus6 (sbtlActAI, raidou1 0x48)

Raidou 1: 8 of 18 words differ, first at +0x18.

Register allocation: retail keeps flg in s0 and the result in a0; the draft swaps them. Computing the result before or after the call, or as a ternary, leaves the swap.

## sbtlCallBtlGetBadStatus6 (sbtlActAI, raidou2 0x48)

Raidou 2: 8 of 18 words differ, first at +0x18.

The Raidou 1 C; the same register swap: retail keeps flg in s0 and the result in a0, the draft swaps them.

## sbtlCheckCharmTarget (sbtlActAI, raidou1 0x54)

Raidou 1: 17 of 19 words differ, first at +0x0; emitted 76 bytes, native 84 bytes.

Retail saves s1 in a 32-byte frame and never uses it; the draft has no s1 and a smaller frame. A ret local or an early return does not bring it back.

## sbtlCheckCharmTarget (sbtlActAI, raidou2 0x54)

Raidou 2: 17 of 19 words differ, first at +0x0; emitted 76 bytes, native 84 bytes.

The Raidou 1 C; as there, retail saves an s1 it never uses and the draft does not.

## sbtlEAIInit_BtlInit (sbtlActAI, raidou1 0x70)

Raidou 1: 2 of 28 words differ, first at +0x48.

Scheduling: retail loads the constant 13 before the ld into s0 and 60 after it; the draft does the reverse. sbtlEAIInit_BtlWait, the same shape, matches. Storing mode first costs 4 words.

## sbtlEAIInit_BtlInit (sbtlActAI, raidou2 0x70)

Raidou 2: 2 of 28 words differ, first at +0x48.

The Raidou 1 C; the same scheduling of the constants 13 and 60 around the ld into s0.

## sbtlAAIInit_BAttack (sbtlActAI, raidou1 0xA4)

Raidou 1: 2 of 41 words differ, first at +0x48.

Scheduling: the loads of 14 and 180 swap. Writing cmpframe before mode gives retail's store order; the reverse order swaps the stores as well.

## sbtlAAIInit_BAttack (sbtlActAI, raidou2 0xA4)

Raidou 2: 2 of 41 words differ, first at +0x48.

The Raidou 1 C with Raidou 2's flag constants ((flags | 4) & ~0x21B); the same swap of the 14 and 180 loads.

## sbtlAAIInit_SUMMON (sbtlActAI, raidou1 0x8C)

Raidou 1: 13 of 35 words differ, first at +0x20.

Retail applies ori 4, and ~0x21B, ori 0x160 and and ~0x80 in that order with model in a0; gcc reassociates the draft's flag operations, and the frame and cmpframe stores swap. One expression, three statements and two statements all reassociate.

## sbtlAAIInit_SUMMON (sbtlActAI, raidou2 0x8C)

Raidou 2: 13 of 35 words differ, first at +0x20.

The Raidou 1 C; the same flag operation order and model register difference.

## sbtlCheckCrossUnit_HIT (sbtlActAI, raidou1 0xC0)

Raidou 1: 1 of 48 words differ, first at +0x98.

The last compare: retail ends with sltiu on (i < count), the draft with xori. !, == 0, ^ 1, a ternary and u8, s8 or s16 results all give xori.

## sbtlAAIInit_N_Comb_R (sbtlActAI, raidou1 0xDC)

Raidou 1: 51 of 56 words differ, first at +0xC; emitted 224 bytes, native 220 bytes.

Register allocation: the draft keeps the constant 2 for mode and stat in s2 across the three calls, as sbtlAAIInit_N_Comb_E does; retail loads it again.

## sbtlAAIInit_N_Comb_E (sbtlActAI, raidou1 0xA8)

Raidou 1: 37 of 42 words differ, first at +0xC; emitted 172 bytes, native 168 bytes.

Register allocation: the draft keeps the constant 2 for mode and stat in s2 across the three calls, with a larger frame; retail loads it again. Same as sbtlAAIInit_N_Renkei. A model local, a rot local or no local do not change it.

## sbtlAAIInit_N_Renkei (sbtlActAI, raidou2 0xA8)

Raidou 2: 36 of 42 words differ, first at +0x18; emitted 172 bytes, native 168 bytes.

Raidou 2 does not clear model->frame; otherwise the Raidou 1 difference, the constant 2 kept in s2 across the calls.

## sbtlAAIInit_N_GuardH (sbtlActAI, raidou1 0xAC)

Raidou 1: 4 of 43 words differ, first at +0x30.

Scheduling: the draft loads 90 into v1 before the mode store, retail reuses v0 after it. Every order of vec.w, mode, frame and cmpframe, with or without a model local, keeps it.

## sbtlAAIInit_N_GuardH (sbtlActAI, raidou2 0xAC)

Raidou 2: 4 of 43 words differ, first at +0x30.

Same C and the same difference as Raidou 1: 90 in v1 where retail reuses v0.

## sbtlEAIProc_Ctact (sbtlActAI, raidou1 0xE8)

Raidou 1: 44 of 58 words differ, first at +0x24; emitted 240 bytes, native 232 bytes.

Block layout: retail keeps the frame-over block inline and puts the lost-target block last, storing stat then mode; the found block's stat 1 sits in v1 apart from the return value. Early returns, nested ifs, a goto and a ret variable do not reproduce it.

## sbtlEAIProc_Ctact (sbtlActAI, raidou2 0xE8)

Raidou 2: 44 of 58 words differ, first at +0x24; emitted 240 bytes, native 232 bytes.

Same C and the same block layout difference as Raidou 1.

## sbtlDeletePlayer (sbtlActAI, raidou1 0x178)

Raidou 1: 25 of 94 words differ, first at +0x8.

Register allocation: retail keeps wk in s0 and the i<12 loop test in s1; the draft swaps them. Nested ifs and while forms of the loop leave the swap.

## sbtlAAIProc_Skl_SMV (sbtlActAI, raidou1 0x218)

Raidou 1: 0 of 134 words differ.

Delay slot: retail fills the sbtlCheckCost beq with daddu a0,s0, the draft with addiu v0,15. Inverting the if costs 22 words.

## sbtlAAIProc_N_Comb_E (sbtlActAI, raidou1 0x300)

Raidou 1: 17 of 192 words differ, first at +0x1C.

Scheduling in the unit != NULL block: retail loads skl->area before the order_sklidbk load and skillid store.

## sbtlAAIProc_Skl_S (sbtlActAI, raidou1 0x364)

Raidou 1: 33 of 217 words differ, first at +0x148.

&vertexC is CSE'd into a2 where retail recomputes sp+48 for the call.

## sbtlGetEscapePoint (sbtlActAI, raidou1 0x1F4)

Raidou 1: 75 of 125 words differ, first at +0x4.

Register allocation: retail copies area into s0 before converting it and reuses the model pointer from the rotate block in the check.

## sbtlGetMovePos_MapHit_2 (sbtlActAI, raidou1 0x260)

Raidou 1: 4 of 152 words differ, first at +0x1F4.

Reload registers: retail reloads the spilled flg into a2 and pos into a3, the draft into v0 and a1. Nested ifs for the break test do not move them.

## sbtlGetMovePos_MapHit_2 (sbtlActAI, raidou2 0x260)

Raidou 2: 4 of 152 words differ, first at +0x1F4.

Same C and the same difference as Raidou 1.

## sbtlAAIInit_N_Escape (sbtlActAI, raidou1 0x2CC)

Raidou 1: 17 of 179 words differ, first at +0x38.

Scheduling: retail stores mode before loading flags and fills the sbtlGetNearTarget_Random delay slot differently; mfc1 register in the rotate scale.

## sbtlAAIProc_N_Escape (sbtlActAI, raidou1 0x2F0)

Raidou 1: 104 of 185 words differ, first at +0x74; emitted 740 bytes, native 752 bytes.

&front is CSE'd into s1 where retail recomputes sp+16 for the store; case 14 delay slot and one nop shift the rest.

## sbtlAAIProc_N_Escape (sbtlActAI, raidou2 0x2F0)

Raidou 2: 104 of 185 words differ, first at +0x74; emitted 740 bytes, native 752 bytes.

Same C and the same difference as Raidou 1.

## sbtlSetGroupAction (sbtlActAI, raidou1 0x300)

Raidou 1: 163 of 192 words differ, first at +0x8; emitted 772 bytes, native 768 bytes.

Retail is one word shorter. Register allocation differs (retail keeps i in s5 and targetid in s4 and never saves s8). Retail tests the three set=0 checks as an else-if chain with branches; the draft's separate ifs compile to movn, and the else-if chain makes the draft longer than retail.

## sbtlSetGroupAction (sbtlActAI, raidou2 0x300)

Raidou 2: 163 of 192 words differ, first at +0x8; emitted 772 bytes, native 768 bytes.

Same C and the same difference as Raidou 1.

## sbtlAAIInit_N_Kaihi (sbtlActAI, raidou1 0x1C8)

Raidou 1: 92 of 114 words differ, first at +0x8; emitted 464 bytes, native 456 bytes.

&wk->pos is CSE'd into s2 across sbtlGetVerticalLength_Point where retail recomputes s0+64; the extra saved register shifts the rest.

## sbtlAAIProc_N_Kaihi (sbtlActAI, raidou1 0x3E8)

Raidou 1: 41 of 250 words differ, first at +0x110.

&front is CSE'd into s1 where retail recomputes sp+16 for the store; aiwk and backmode registers swapped.

## sbtlAAIProc_N_Kaihi (sbtlActAI, raidou2 0x3E8)

Raidou 2: 41 of 250 words differ, first at +0x110.

Same C and the same difference as Raidou 1.

## sbtlGetEscapePoint (sbtlActAI, raidou2 0x290)

Raidou 2: 83 of 164 words differ, first at +0x4.

R1 C (R1 is non-matching, 75 words); the same register allocation difference from the prologue on.

## sbtlAAIInit_N_Escape (sbtlActAI, raidou2 0x2CC)

Raidou 2: 15 of 179 words differ, first at +0x38.

R1 C with sbtlGetNearTarget_Random; the same constant scheduling difference as R1.

## sbtlAAIProc_BAttack (sbtlActAI, raidou2 0x218)

Raidou 2: 4 of 134 words differ, first at +0x11C.

Register allocation in the targetID != -1 skip check: retail loads model into v0 and -1 into a0, the draft uses v1 and a1; a nested if, operand order and a model local do not move it.

## sbtlAAIInit_N_Kaihi (sbtlActAI, raidou2 0x1C8)

Raidou 2: 92 of 114 words differ, first at +0x8; emitted 464 bytes, native 456 bytes.

R1 C unchanged; the same differences as R1, 8 bytes longer than retail.

## sbtlAAIProc_Skl_SMV (sbtlActAI, raidou2 0x27C)

Raidou 2: 68 of 160 words differ, first at +0x94; emitted 644 bytes, native 636 bytes.

R1 C plus the R2 skip check; the same targetID != -1 register difference as sbtlAAIProc_BAttack leaves two extra nops, and every later branch offset moves.

## sbtlAAIProc_Skl_S (sbtlActAI, raidou2 0x3E8)

Raidou 2: 151 of 250 words differ, first at +0xE0.

R1 C plus the R2 skip check; R1's flag register difference (s2/s3 swap, bnel/bc1tl layout) plus the targetID != -1 difference shift the rest.

## sbtlAAIProc_N_Comb_E (sbtlActAI, raidou2 0x37C)

Raidou 2: 13 of 223 words differ, first at +0xD8.

Scheduling in the combine branch: retail reads skl->area before order_sklidbk and sets model->flags |= 0x4000 with a1 for the model; the draft loads them in source order.
