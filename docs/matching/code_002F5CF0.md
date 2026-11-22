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

## sbtlChkEnableDecideMenu (code_002F5CF0, raidou2 0x2F8)

Raidou 2: 56 of 190 words differ, first at +0x124.

56 words: the bad-status range test is negated with xori where retail uses a second sltiu, and retail builds the sbtlIsStart(0) || sbtlIsStart(1) result in another register and copies it. The two-statement negation with a temporary made it worse (72). Vectors do not reach, checked against the asm. nm_behaviour reports one extra conditional branch: the sbtlIsStart(0) || sbtlIsStart(1) codegen; the original computes the same 0/1 result.

## sbtlProcess (code_002F5CF0, raidou2 0x7E0)

Raidou 2: 400 of 468 words differ, first at +0x64; emitted 1872 bytes, native 2016 bytes.

First draft, more than 30% off: the C emits 468 of the 504 words because ee-gcc cross-jumps the repeated cancel tails of steps 3 to 7, 9 and 10 (clear wk_sbtl->flg 0x80000, sbtlCallEnd or back to the previous menu, sbtlUseItemProcSBtlPlayerSub) that the original keeps as separate copies, so every branch offset after +0x64 differs. Only the first draft was tried. Vector checked (battle rows).

## sbtlDrawProcess_sbtlMenu (code_002F5CF0, raidou2 0x32C)

Raidou 2: 3 of 203 words differ, first at +0x120.

The font colour's lui 0xA09D lands two words after retail's, behind the 255 and the sll of the divu (3 words). Tried a color local before the icon call (fixed the register homes), both operand orders, a |= split and a u8 alpha with 255U. Vector checked (R2 battle/menu rows, the paths they take).

## DrawAddMag (code_002F5CF0, raidou2 0x2D4)

Raidou 2: 21 of 181 words differ, first at +0x6C.

21 words: the 48/24 constants before the first movn are materialised in the other order, the gAddMagNumNow clamp selects with movn (sum as default) where retail copies gAddMagNum and uses movz, and one bne is bnel in retail. Tried the clamp as a ternary both ways and through a local initialised from gAddMagNum. The 0.8 constant is written 0.79999996f, the literal that gives the original 0x3F4CCCCC under ee-gcc (0.79999995f gives 0x3F4CCCCB). Vector checked (R2 battle/menu rows, the paths they take).

## sbtlSummonDevilAshimoto (code_002F5CF0, raidou2 0x5A0)

Raidou 2: 44 of 360 words differ, first at +0x14.

The main marker and marker 2 use s20/s19 instead of s19/s20; asimoto_mode loads schedule differently (44 words). The first difference is sd s19 vs sd s20. Read the register-priority and cross-block-locals levers; find_similar found no same-shape sibling. Vectors do not reach, checked against the asm. The size test reads SBTL_MOTION_VISUAL byte 0, the HD sdw_flg (Raidou 2 keeps kind in byte 1); nm_behaviour is clean with that field.

## sbtlEnemyData (code_002F5CF0, raidou2 0x404)

Raidou 2: 248 of 258 words differ, first at +0x14; emitted 1044 bytes, native 1028 bytes.

The address of the local pos vector (SDF_VU0_STOREVECTOR/LOADVECTOR) is hoisted out of the unit loop into s20, where retail rematerialises addiu sp+0x10 at each use; that one extra saved register shifts the whole body. The status test (3, 6, 31) pivots at 6 as a switch, where retail tests 3 then status < 4; an if chain and a default arm were tried. Retail reads wk_sbtl->flg2 with ld at the 0x4400000 test and lw at the 0x1000 test. Vector checked (R2 battle/menu rows, the paths they take). nm_behaviour reports the ld of wk_sbtl->flg2 (the draft uses lw; the 0x4400000 mask is in the low word) and the switch pivot constant 4 against 7: same behaviour.

## sbtlTargetCursor (code_002F5CF0, raidou2 0x654)

Raidou 2: 44 of 405 words differ, first at +0xEC.

44 words, all in the smoothing arm after the first div.s: retail keeps v.x and v.y in $f13/$f12 and 5.0 in $f14, one register above this draft, and divides dx, dy, dz in that order where the draft starts with dz. Tried the trail stores in Data5, Data3, Data4, Data2 order (72 words) against Data2..Data5 order (44) and a named difference vector before fsqrtf (70). The two screen points are one SBTLPROJECTEDSCREEN_POINT pt[2]: two separate 8-byte structs each take a 16-byte slot and grow the frame to 0xC0. Vectors do not reach (vsum.py: no scenario); checked against the asm.

## DrawStatusBar (code_002F5CF0, raidou2 0x870)

Raidou 2: 377 of 540 words differ, first at +0x8; emitted 2216 bytes, native 2160 bytes.

First draft, more than 30% off: the C is 14 words longer than the original and allocates one callee-saved register fewer (the original keeps 2*idx in $fp for the 12-byte aRedGuage/aStatusBar strides and keeps cTex and the flash alpha on the stack), so the registers differ from +0x8 on. Only the first draft was tried. Vector checked (battle rows). nm_behaviour's one missing sbtlCallGetSmallFontFRQ4 is the two name calls (item name or player name) merged into one call by the compiler; the source keeps both.

## sbtlSummonDevilData_Sub (code_002F5CF0, raidou2 0x8EC)

Raidou 2: 549 of 562 words differ, first at +0x0; emitted 2248 bytes, native 2284 bytes.

First draft from the asm, more than 30% off and recorded at once: the original uses a 0x70 frame with $fp holding x - 105 and 4*idx spilled to 0x10($sp), where the C allocates one register fewer; the switch on status (cases 3, 6, 31) compiles with its case tree rooted at 6 instead of 3; later branches compute x - 97 as (x - 100) + 3 in the original. Behaviour checked against the asm branch by branch (vectors do not reach it). nm_behaviour's one extra immediate (7) is the case tree's range test rooted at 6.
