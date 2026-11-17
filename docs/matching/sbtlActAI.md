# Draft notes: sbtlActAI

## sbtlCreate (sbtlActAI, raidou1 0xAC)

Raidou 1: 8 of 43 words differ, first at +0x70.

store schedule only: retail sw field10C right after flags then the floats, mine emits field108/field10C together after the floats; tried flags-first, HD order, field10C before flags; lever right-rotation not applicable; vector checked (menu)

Related: [smt3 levers: stores](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/stores.md).

## sbtlCheckEquation (sbtlActAI, raidou1 0x90)

Raidou 1: 14 of 36 words differ, first at +0xC.

register allocation: retail keeps wk copy in a1 and reuses a1 for model, aiwk a2, const 1 a3, v0=0 in each delay slot; mine aiwk a3, model a2; tried early returns with and without model local (same), nested ifs with ret local (worse, 19); vector checked (menu)

Related: regalloc.

## SetUseskl (sbtlActAI, raidou1 0x3C)

Raidou 1: 10 of 15 words differ, first at +0x0.

v0/v1 swapped through the whole body (cond and index); tried sklid / 32, evtBitOn's explicit (no < 0) ? no + 0x1F : no index (matched sibling template), an s32 parameter, a cached element pointer (9 words, swaps the addu); SBTLUNIT_MODEL gains useskl[36] at 0x194 (HD useskl, bounded by field224); vector checked (menu row: bit 8 of word 0 set)

Related: regalloc.

## sbtlCheckEquationProc (sbtlActAI, raidou1 0x32C)

Raidou 1: 106 of 193 words differ, first at +0x40; emitted 772 bytes, native 812 bytes.

wk->model->field inside a loop with a call: gcse puts the wk->model reload after the call and reloads the field at the loop top, as retail; a model local reassigned after the call lets gcse keep the field in a register; one s32 local reused for func/sel/jmp/code gives retail's single s0; vector checked (menu, 32 rows)

Related: regalloc.

## sbtlCallBtlCreateSideList2 (sbtlActAI, raidou1 0xE0)

Raidou 1: 53 of 54 words differ, first at +0x4; emitted 216 bytes, native 224 bytes.

body: store C1080 00270818.c; body matches except retail copies r to v0 and sets a1=1 before the cvt in the prologue, shifting every word by one; tried r passed directly (53 of 55) and f32 radius = r local (53 of 54, kept); vector checked (menu: a1=200, ret 0/1)

Related: prologue.

## sbtlUseTableDataProc (sbtlActAI, raidou1 0x170)

Raidou 1: 79 of 90 words differ, first at +0x4; emitted 360 bytes, native 368 bytes.

(pat & 0xFFFF0000) >> 16 on s32 pat: the first use folds to srl, the loop-carried second keeps lui mask + and + srl, as retail; vector checked (menu, 30 rows)

Related: constants.

## sbtlEAIInit_None (sbtlActAI, raidou1 0xA0)

Raidou 1: 11 of 40 words differ, first at +0x28.

a local s16 stat = 1 (HD sBtlSetAIWkStat(wk, 1)) puts 1 in s1 from the prologue (35 -> 11 words); left: retail loads aiwk into a0 before model and an extra b-delay nop; tried 4 orders; vector checked (menu)

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).

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

Related: [smt3 levers: callee visibility and branch-likely](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/callee-visibility.md).

## sbtlSetNormalAction (sbtlActAI, raidou1 0x354)

Raidou 1: 4 of 213 words differ, first at +0x1DC.

model = wk->model inside both arms of an if around a STOREVECTOR gives retail's bnel with the model load in the delay slot; (s32)GetRand() for the cvt.s.w (u32 return adds the unsigned fixup); flags (f | C) & ~M keeps ori before and | the 4 words are li order of the mode/cmpframe constants in CTACT and SEARCH; mode-before-cmpframe statement order swaps the registers too (8 words), reverted; vector checked (menu, 32 rows)

Related: constants.

## GetSenkaiPointSub (sbtlActAI, raidou1 0x244)

Raidou 1: 57 of 145 words differ, first at +0x4.

retail loop entered by a jump to a bottom test holding the calls = while (i < 2 && (callA || !callB)) {...} then if (i < 2) store A else store B; a for with an in-loop return duplicates the simple exit test; point stores are STOREVECTOR_MEMOPS (wk->model stays CSE'd across them); vector checked (menu, 3 rows)

Related: loop-shape.

## sbtlCheckMapHit (sbtlActAI, raidou1 0x1C)

Raidou 1: 3 of 5 words differ, first at +0x8; emitted 20 bytes, native 28 bytes.

retail jal+epilogue wrapper of sbtlCheckMapHit_Cross; return f(), result local both sibcall (j), if-form adds sltu; vector checked (menu, a0..a2 passed through)

Related: sibcall.

## sbtlCallMemset14 (sbtlActAI, raidou1 0x408)

Raidou 1: 125 of 256 words differ, first at +0xA8; emitted 1024 bytes, native 1032 bytes.

2 words short, the rest is shift: retail recomputes sp+32 for the flg-path VerticalArea arg, has a dead nop after the flg path's branch pair, stores mode after cmpframe/frame/flags in the hit block and case 16 (mine hoists the sh), and beq not beql after sbtlCheckStatus_Move; tried one model local (a3 for all, fixed by wk->model in the switch), early-local for the sltiu (fixed), store orders mode-first/flags-first (no change); vector checked (menu, case 14 hit path) | shapes: one SBTLUNIT_MODEL local reused across the frame clamp and the switch is one pseudo and lands in a3; retail splits it (a0 then a1), so the switch reads wk->model; hit = !(frame-1 < cmpframe) needs early = ...; hit = early == 0 for sltiu (addressing-and-types 709)

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).

## sbtlRefSbtl22 (sbtlActAI, raidou1 0x38)

Raidou 1: 11 of 14 words differ, first at +0x0.

register allocation only: retail wk_sbtl in a2, result in v0 from the start; tried && chain, ret local, nested ifs with id local (all wk_sbtl in a1, result in a2 or two exits); find_similar no matched sibling; vector checked (menu, ret 0)

Related: regalloc.
