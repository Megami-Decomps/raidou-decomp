# Draft notes: event/evtEventViewer

## Process3 (event/evtEventViewer, raidou1 0x2F8, raidou2 0x2F8)

Raidou 1 and Raidou 2: 59 of 188 words differ, first at +0x98; mine is 8 bytes shorter.

Branch targets differ by two words from +0x98 on; the length changed from 190 to 188 words when the batch's headers landed (cause not isolated, it was 47 of 190 before).

## evtCallGetProcessData11 (event/evtEventViewer, raidou1 0x90, raidou2 0x90)

Raidou 1 and Raidou 2: 7 of 36 words differ, first at +0x1C.

7 words off, register allocation: the work pointer lands in a different register.

Tried: u16 locals and passing the fields directly as arguments; neither moved the work pointer's register.

## RunFrame1 (event/evtEventViewer, raidou1 0x74C, raidou2 0x74C)

Raidou 1: 1 of 467 words differ, first at +0x134.
Raidou 2: 1 of 467 words differ, first at +0x134.

case 18's 'nowframe == frame->frame' guard is bnel where retail has bne (delay slot ld s0 from the epilogue; reorg sees s0 live on the fall-through), the code is otherwise identical on both games; tried return after AddBED, inverted early-break guard, swapped operands; lever row addressing-and-types 526 (not reachable from the branch shape); find_similar has only SMT3 RunFrame1 (0.37); vectors (opening) reach types 2/6/10/12, not 18, checked against the asm; tail calls need return, not break; a 4-label switch needs explicit empty case 3..5 to get retail's jump table

Related: [smt3 levers: callee visibility and branch-likely](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/callee-visibility.md).

## ValidateAt3Bgm (event/evtEventViewer, raidou1 0x19C, raidou2 0x19C)

Raidou 1: 86 of 104 words differ, first at +0x18; emitted 428 bytes, native 412 bytes.
Raidou 2: 86 of 104 words differ, first at +0x18; emitted 428 bytes, native 412 bytes.

block layout matches through the status switch; retail shares the clear tail (1stLoad=0, flag=0) between the flags&1 stop path and the play path and jumps the At3BgmFlag==0 return into its v0=0, and inverts the status!=2 test; tried flags&1 first/last, explicit early-return clear, switch with break-to-return. vector checked (opening reaches the flag-0 and the play path)

Related: [smt3 levers: control flow](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/control-flow.md).

## Validate1stAt3Bgm (event/evtEventViewer, raidou1 0x148, raidou2 0x148)

Raidou 1: 34 of 82 words differ, first at +0x20; emitted 336 bytes, native 328 bytes.
Raidou 2: 34 of 82 words differ, first at +0x20; emitted 336 bytes, native 328 bytes.

retail search loop has no loop notes (constant 1 not hoisted to s2): a goto loop removes the hoist, but retail then re-tests found after the found path where gcc threads the jump; tried for, while(TRUE)+break, if-chain, goto with break. vector checked (opening reaches only the At3BgmFlag-set path; search path checked against the asm)

Related: [smt3 levers: control flow](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/control-flow.md).

## OutFrame1 (event/evtEventViewer, raidou1 0x120, raidou2 0x120)

Raidou 1: 45 of 72 words differ, first at +0x20; emitted 304 bytes, native 288 bytes.
Raidou 2: 45 of 72 words differ, first at +0x20; emitted 304 bytes, native 288 bytes.

Raidou adds the fade (index unless sb[8]) alpha via effSetEffectColorSubSub and case 33; retail loads the slot twice (lb for the test, lbu then sll/sra at uses) and puts the shared Del block before the color tail; tried s8/u8 slot, ||, nested ifs, cross-jumped double Del. vectors do not reach (opening has types 2,4,6,10,19,36 only), checked against the asm

## RunFrame2 (event/evtEventViewer, raidou1 0xC44, raidou2 0xC44)

Raidou 1: 770 of 786 words differ, first at +0x18; emitted 3152 bytes, native 3140 bytes.
Raidou 2: 770 of 786 words differ, first at +0x18; emitted 3152 bytes, native 3140 bytes.

regalloc: retail gives the case-10 %hi(RunFrame2Work) pseudo s0 (shared with alpha) and colour s2, mine allocates the lo_sum address first so the hi gets an eighth saved register s7 and every s-register after the prologue shifts (OVER by 12 bytes); with s-registers masked only 83 lines differ, all from that allocation except case 6 reloading nearframe->index after the !=0 test and case 8 keeping elapsed in a saved register. Shapes tried: declaration order of alpha/enable/color (no change), from||to wrap of case 10 (no change), case-10 light pointer local (drops s7 but strength-reduces the loop, 544 words), kind==1 light pointer, FadeModeSet before the colour set, fade as ternary/else/nested if; bezier x/y interleaved store order fixed the lb load order. Vector checked (boot reaches types 6, 8, 10 and 12: fade calls, flag 4 clear plus ScreenBlurFlag=0, empty type 10, fldfilter set agree with the draft); other cases checked against the asm. lever crossblock

Related: [smt3 levers: cross-block locals with swapped registers](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/cross-block-locals.md).

## RunFrameUnit (event/evtEventViewer, raidou1 0x39C, raidou2 0x39C)

Raidou 1: 6 of 231 words differ, first at +0x1CC.
Raidou 2: 6 of 231 words differ, first at +0x1CC.

v0/v1 swap of cur->frame/next->frame in the ratio block (both games); tried ==/!= both operand orders, (s32) casts, split divide, diff!=0 (OVER); key shapes: do-while first loop, cur->frame != next->frame, switch {2,3} for the 0x8000 range test (slti 4/slti 2 tree); permuter skipped (coordinator); vectors reach it (opening, 32 rows) but not the shadow-key path, register-only diff checked against the asm; adds evtUnit_t ShadowAlpha 0x173/ShadowY 0x174 (HD names), evtGetUnitHandle2/sdfModelGetUniqueID/evtHokanFloat prototypes; lever grep (subu, mtc1) has no row for it; find_similar's template evtViewApplyMovieModeKeys (0.876) has no shadow block

## ValidateRenderTex (event/evtEventViewer, raidou1 0x1F8, raidou2 0x1F8)

Raidou 1: 61 of 126 words differ, first at +0x20; emitted 512 bytes, native 504 bytes.
Raidou 2: 61 of 126 words differ, first at +0x20; emitted 512 bytes, native 504 bytes.

register assignment (found in a3 vs a2) and key->frame kept live out of the third loop; shapes tried: head local vs re-read obj->children (re-read -17), s16 from local vs re-reading found->p08.sh[1] (re-read gives retail's lhu + sll/sra at each use, -5), ratio if/else orders, declaration order (no effect). vectors reach only the object scan (no type-31 object in opening); key logic checked against the asm

## DrawMessage (event/evtEventViewer, raidou1 0x1C0, raidou2 0x1C0)

Raidou 1: 48 of 110 words differ, first at +0x20; emitted 440 bytes, native 448 bytes.
Raidou 2: 48 of 110 words differ, first at +0x20; emitted 440 bytes, native 448 bytes.

register choice for the movie-frame result (retail reuses v0) and branch-likely placement around GetSelPhase; shapes tried: early returns, single exit, nested/ternary SelPhase, separate result local, HD-order frame tests. vector checked (opening, non-movie path ret=1)

## oldEventSceneUnload (event/evtEventViewer, raidou1 0xAC, raidou2 0xAC)

Raidou 1: 42 of 44 words differ, first at +0x4; emitted 188 bytes, native 172 bytes.
Raidou 2: 42 of 44 words differ, first at +0x4; emitted 188 bytes, native 172 bytes.

retail keeps a register copy of the data pointer and re-tests it at the loop top each iteration (no jump threading); mine hoists or threads the null test. Tried do/while with local, while(&&) with break, for-loop, memory test. vector checked (opening: one PM2 and one PM3 cut stored, same as the draft). Name is a poor HD hit (HD body is a Unity scene unload)

Related: [smt3 levers: control flow](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/control-flow.md).

## ReqMessage (event/evtEventViewer, raidou1 0x1E8)

Raidou 1: 4 of 122 words differ, first at +0xFC.

Raidou 1 only. The body matched (0 of 122) until declarations were added to headers the unit includes (a prototype in dds3KernelDraw.h or itfMesManager.h moves it), so it is kept NON_MATCHING and no declaration is moved to protect it.

Fragile: matches only with the declaration set that evtEventViewer.c and its headers carried before the R16 batch; any added declaration moves a register in the second half.
