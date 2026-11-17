# Draft notes: interface/itfMesManager

## itfMesMngRequestSelect_itf (interface/itfMesManager, raidou1 0x118, raidou2 0x118)

Raidou 1: 2 of 70 words differ, first at +0xE8.
Raidou 2: 2 of 70 words differ, first at +0xE8.

SMT3 body without the type printf, rows 25<<3 apart, sets select.selState[1]=4 and [0]=0; left: phase=1 and selState[1]=4 stores swap (same constant-pair pattern as CalcSelect: retail loads the later-stored constant first); tried both orders (4 and 2 off); no vectors recorded for it in any scenario, checked against the asm

Related: constant-sharing.

## CalcWindow (interface/itfMesManager, raidou1 0x1F0, raidou2 0x1F0)

Raidou 1: 50 of 123 words differ, first at +0x20; emitted 492 bytes, native 496 bytes.
Raidou 2: 50 of 123 words differ, first at +0x20; emitted 492 bytes, native 496 bytes.

SMT3 CalcWindow body (smt3 itfMesManager.c:2096) with Raidou fields; the difference is not analysed yet. Vectors reach it (menu, 32 rows from 0x189b3c, vsum), not replayed; checked against the asm.

Related: unknown.

## CalcMessage (interface/itfMesManager, raidou1 0x28C, raidou2 0x31C)

Raidou 1: 118 of 159 words differ, first at +0x20; emitted 636 bytes, native 652 bytes.
Raidou 2: 155 of 159 words differ, first at +0x0; emitted 636 bytes, native 796 bytes.

Retail places the lf_after_wait != 0 increment path before the input-wait path (bnel at +0xA8) and tests the pad bytes in a different order (cancel or confirm, then lf_wait >= 14, else triangle, ctr_key >= 5, itfData3 & 1). The draft keeps the input-wait path first; 118 of 159 words differ. Checked against retail call records (ingame, counter path) and the asm.

Related: [smt3 levers: callee visibility and branch-likely](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/callee-visibility.md).

## CalcSelect (interface/itfMesManager, raidou1 0x2EC, raidou2 0x2EC)

Raidou 1: 2 of 187 words differ, first at +0x64.
Raidou 2: 2 of 187 words differ, first at +0x64.

SMT3 CalcSelect plus a type-3 path, a phase-12 countdown placed after case 1, and itfCallChangeFontPosition tail calls; selState[2] (u16) moved into select_tag at 0x64 (all accesses use the select base); only the type-3 path's 2 and 4 swap a0/v1; tried three store orders; vectors only reach phase 0, checked against the asm

Related: constant-sharing.

## itfCalcMesFlowItfSub (interface/itfMesManager, raidou1 0x320, raidou2 0x320)

Raidou 1: 155 of 200 words differ, first at +0x4; emitted 804 bytes, native 800 bytes.
Raidou 2: 155 of 200 words differ, first at +0x4; emitted 804 bytes, native 800 bytes.

Retail keeps the table address CSEs in s0-s1 with pcurctr in s2 and a 176-byte frame; the draft needs s3 and a 192-byte frame, which shifts the schedule of the four float and two integer lerps. Key pointer temporaries (174 of 178 off), direct phase field reads (164) and s16 phase and frame (155) do not fix it. 155 of 200 words differ on both games. Checked against retail call records (ingame, idle path) and the asm.

Vectors: not reachable; the failing mutant points pinst at 0xe20000, where curctr[0].phase reads 104, so both runs index the five-key stack copy of itfCalcMesFlowItfSubTable far past its end and read their own differently sized frames; captured rows pass.

Related: register-allocation.

## itfFunc2 (interface/itfMesManager, raidou1 0x1EC, raidou2 0x1EC)

Raidou 1: 67 of 123 words differ, first at +0xC.
Raidou 2: 67 of 123 words differ, first at +0xC.

keyframe interpolation of curctr[1] over itfFunc2Table (extern struct copy, lever 471); retail computes each key field from its own sp base and schedules all loads before the stores; tried pointer key, direct phase indexing, phase/frame locals, if/else order (positive arm first fixed the layout); vectors do not reach (isdraw 0 in title-ng rows), checked against the asm; R2 same 67

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).

## itfCallGrAllocPacketBuff (interface/itfMesManager, raidou1 0x448, raidou2 0x448)

Raidou 1: 209 of 274 words differ, first at +0xE4; emitted 1112 bytes, native 1096 bytes.
Raidou 2: 209 of 274 words differ, first at +0xE4; emitted 1112 bytes, native 1096 bytes.

Retail keeps x in s1 and y in s0 and builds the fourth sprite from position.x - 256 held in s5; the draft swaps the pair and recomputes, 209 of 274 words differ on both games. A pcurctr local for curctr[1] fixed the prologue; an x2 temporary made it worse (217). Retail has no return value after the regFunc call (v0 is whatever regFunc leaves), so the draft has no final return either. Checked against retail call records (menu) and the asm.

Related: register-allocation.

## itfCallGrAllocPacketBuff2 (interface/itfMesManager, raidou1 0x4D8, raidou2 0x4D8)

Raidou 1: 145 of 310 words differ, first at +0x94.
Raidou 2: 145 of 310 words differ, first at +0x94.

size/vertex initializers built in a temp and block-copied like retail; size[1] subtracts uv[2]'s origin as retail does; remaining: x/y pseudos swap v0/v1 and mes sits in t1, then float/alpha scheduling Vector checked.

Related: regalloc.

## InitializeMessage (interface/itfMesManager, raidou2 0x4C)

Raidou 2: 11 of 19 words differ, first at +0x18.

gcc hoists the color[2] store (sb 0x1C) above the table store; source order follows the retail asm, the color[3]-first reorder was tried; 11 words, behaviour equal

Related: store scheduling of the color stores.
