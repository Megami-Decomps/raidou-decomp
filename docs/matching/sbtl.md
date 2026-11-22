# Draft notes: sbtl

## sbtlSetAutoSkill_GBID (sbtl, raidou1 0xC0)

Raidou 1: 16 of 48 words differ, first at +0x30.

FPU register allocation: retail holds the backup value in f3 and divides into f0 in both cases; the draft swaps them. Behaviour matches (nm_behaviour clean).

## sbtlCheckHaveskill_Atk (sbtl, raidou1 0xA8)

Raidou 1: 7 of 42 words differ, first at +0x40.

Register allocation: retail keeps the skill table pointer in a0 and presets v0 = 1 before the type load; the draft keeps it in a1. Behaviour matches.

Related: Declaration order and block-scoped locals did not change it..

## sbtlSetWeakBit (sbtl, raidou1 0x120)

Raidou 1: 4 of 72 words differ, first at +0xC.

Register allocation: the devilid / 32 rounding swaps v0 and v1 (slti result and the quotient candidate) in the first four instructions. Behaviour matches.

Related: A word index local, a bit local, a GBWK pointer local, plain assignment instead of |= and HD case order give the same 4 words or more; case order 96, 104, 126, 136, 214, 235, 234, 236 fixes the jump-table bodies..

## CalcAutoSkill (sbtl, raidou1 0x310)

Raidou 1: 40 of 196 words differ, first at +0x78.

FPU register allocation in the percent scaling: retail puts (f32)percent in f2, 100.0f in f0 and the scaled value in f1; the draft rotates the three registers in every case body. Same operations in the same order, same case layout and jump table.

Related: value * (percent / 100.0f) gives 43 words, a per-case f32 rate local 40, *=, an explicit (f32) cast and a hoisted float percent give 43 or worse..

## sbtlSetStaticInit (sbtl, raidou2 0x44)

Raidou 2: 2 of 17 words differ, first at +0x1C.

Schedule only: retail stores sbtlGetIdlingData[2] between the 1.0f and 100.0f float stores; cc1 puts it after both. Tried the IDA order, the HD sBtlSetStaticInit order and the retail store order (9 to 11 words). 2 words. Vector checked.

## sbtlCheckBadStatus (sbtl, raidou2 0x50)

Raidou 2: 13 of 20 words differ, first at +0x1C; emitted 88 bytes, native 80 bytes.

Register only: cc1 copies the sbtlGetBadStatus result into $4 before the (bst - 20) < 2 range test, where retail works on $2 and moves wk into $4 in the branch delay slot. Tried a switch, a BST local (unsigned compare, more words) and the inverted test; all 13 words. Vector checked.

## sbtlResetBadStatus (sbtl, raidou2 0xCC)

Raidou 2: 3 of 51 words differ, first at +0x14.

Register swap only: retail keeps $2 for the 0 return value in the first branch's delay slot and tests flg & 0x800 in $3; cc1 tests in $2. Tried early return, the if-block form (3 words) and a result local (46 words). 3 words. Vector checked.

## sbtlSetAutoSkill_ID (sbtl, raidou2 0x110)

Raidou 2: 48 of 65 words differ, first at +0x28; emitted 260 bytes, native 272 bytes.

Block order and flag targets match (0x5000 kuishibBit/flg first, 0x1000 deadbombBit/flg2 second, from attr != 0x1000 nesting); retail keeps idx in $3 and copies it to $5, and every return takes ld $16 in a bnel delay slot before a shared li 1. Tried if/else if (55), switch (47), explicit returns per HD (44, block order lost). 48 words. Vector checked.

## sbtlAnimWaku (sbtl, raidou2 0x7C)

Raidou 2: 4 of 31 words differ, first at +0x40.

HD sBtlAnimWaku over the seven SBTLDVLTK_WAKUWORK frames. Register only: in the st 0 clamp retail loads a fresh 1 into v0 for st = 1 and stores alpha = 1 from a2, the constant the case 1 compare holds; cc1 stores both from a2. Tried a frame pointer local (27 words), a switch over dwk->wk[i] (4) and st before alpha (4). 4 words. Vectors do not reach, checked against the asm.

## sbtlGetDamage_ATK (sbtl, raidou2 0x114)

Raidou 2: 43 of 68 words differ, first at +0x3C; emitted 272 bytes, native 276 bytes.

HD sBtlGetDamage_ATK. The original computes the damage into f20, copies it to f0 for the zero test and the 1.0 clamp, and copies it back in the jal delay slot; the HD if/else with the expression written twice stops 100.0f being held in f21 across the call (57 to 43 words) but tests the expression in f0 and moves it to f20 only on the nonzero path. Tried one damage local with an if clamp (57), a base local plus the clamp (58), the if/else (43) with both operand orders. 43 words. Vector checked (battle).

## sbtlAutoRec (sbtl, raidou2 0x120)

Raidou 2: 5 of 72 words differ, first at +0x44.

HD sBtlAutoRec. In the battleMode 2 path the original halves para from a1 into a2 and tests the result with slt 0 < v before the movn; cc1 halves the a2 copy and tests slti v < 1 in the branch delay slot. Tried the body inside the hp < maxhp test (9 words), an early return (12), a value copy with the ternary in both senses (5). 5 words. Vectors do not reach, checked against the asm.

## sbtlSetWeakBit (sbtl, raidou2 0x124)

Raidou 2: 8 of 73 words differ, first at +0xC.

R1 sbtlSetWeakBit with the Raidou 2 devil pairs and anlyzBit at GBWK 0xBF50. Register swap only in devilid / 32: the original puts the sign test in v1 and the copy in v0, cc1 the other way round; the R1 copy (anlyzBit at 0x5D00, an addiu offset) matches with the same C, so the difference may come from the la expansion of the 0xBF50 offset. Tried a masked shift (69 words) and a pointer local for the bit array (28). 8 words. Vector checked (battle).

## SetEnemyCPos (sbtl, raidou2 0x158)

Raidou 2: 6 of 86 words differ, first at +0xEC.

HD SetEnemyCPos: the enemies' mean x/z at the tallest enemy's height, left in vf10. Only 1.0f / n differs: the original loads the 1.0 constant into f0 before converting n into f1, cc1 converts n first. Tried the boss flag as && (78 words, folded into sltiu), as an if-assigned flag (6), the constant-first quotient lever value = 1.0f; value = value / n (8). 6 words. Vectors do not reach, checked against the asm.

## sbtlCheckBadStatus_Number (sbtl, raidou2 0x170)

Raidou 2: 14 of 92 words differ, first at +0x114.

Register only: retail copies the first index to a2 before the second search over the copied sbtlLeaf107Table and reuses its register for the second index; cc1 keeps the first index in place and gives the second a new register. Tried two early returns (84 words), one ret local (18), one loop variable for both searches (20), and the relational else guard from the lever index (14, size exact). 14 words. Vectors do not reach, checked against the asm. nm_behaviour reports only retail's no-op xori 0 before the movn.

## sbtlGetPlayerTec_ParaUp (sbtl, raidou2 0x1D0)

Raidou 2: 18 of 116 words differ, first at +0x148.

Search loop over the 13 tec thresholds: retail zeroes a separate index and the loop counter, indexes the copied table with a shift on every pass and copies the counter to the index on the break before the movz; cc1 strength-reduces the loop to a pointer and keeps the index in the counter's register. Tried resetting the counter after the loop (29 words), idx = i < 13 ? i : 0 (18), the index assigned in the loop (79) and HD's explicit first-threshold guard (33). 18 words. Vectors do not reach, checked against the asm. nm_behaviour reports the paratbl row read at +4 under another base; both read the same stack row.

## sbtlCreateDvltk (sbtl, raidou2 0x1FC)

Raidou 2: 32 of 127 words differ, first at +0x118.

Store scheduling only: retail keeps the stores after the seikaku lb in source order, cc1 hoists wk[4].st and ofs_y ahead. The copy reads sbtlCallMemAllocDirect10Data[i - 7]: both games compute %lo(sym) - 0x64 and load -0xC..0, so the original reads the 7 entries before the waku table at the symbol (R2 0x480660, R1 0x41D1E8, xref only from here); the bytes it reads are icon name pointers in both games. Tried ofs_x/ofs_y first (37 words) and wk[3].st/wk[4].st after fieldD8 (32). 32 words. Vector checked (battle rows take the early return); the copy path checked against the asm.

## CalcAisyo_sbtlCalc (sbtl, raidou2 0x210)

Raidou 2: 26 of 132 words differ, first at +0xF0.

Register only: retail gives the inner index $6, the skill table pointer $7 and the aisyo base copy $8; cc1 gives the pointer $6 and the index $9. j = 0 written before the attr test puts it in the beqz delay slot as retail has (69 to 26 words); nested ifs, the continue form and the declaration order leave the allocation as is. 26 words. Vector checked (battle). nm_behaviour reports retail's two ANDs (~0x4000, then ~0x800000) against the draft's one folded mask; same value.

## SetBadStatusCounter (sbtl, raidou2 0x218)

Raidou 2: 4 of 134 words differ, first at +0x64.

Scheduling only: retail computes attrtime * 3 before the sbtlChkHaveMemSklParty call (sll in the side test delay slot, addu in the call's) and the >> 1 in the delay slot of the test of its result; cc1 puts the sra in the call's delay slot. The call written before the multiply is 21 words. 4 words. Vector checked (battle).

## sbtlZokuseiDmg (sbtl, raidou2 0x22C)

Raidou 2: 7 of 139 words differ, first at +0xA8.

Register only, in the SHINGETDOWN case: retail computes the auto table row as weapontype * 88 in s0 and adds the base loaded into a1, where cc1 loads the base into s0 and adds the product to it. Needed the row in a local before the sbtlCheckAisyo_Old call (28 words to 13) and the 0xFFF mask as its own statement in each case (13 to 7); the index-first pointer sum changed nothing. 7 words. Vector checked (battle, menu).

## sbtlCheckKuishibari (sbtl, raidou2 0x2BC)

Raidou 2: 153 of 171 words differ, first at +0x0; emitted 684 bytes, native 700 bytes.

Frame only: cc1 hoists the constant 5 of the auto skill para compare into $fp and spills per to the stack (frame 0x60); retail keeps the li 5 inside the loop and per in $30 (frame 0x50), and every later word follows the 16-byte shift. Tried a switch with case 5 and a do-while with i++ in the call. 153 words. Vector checked (battle). nm_behaviour reports the side test: retail lbu with andi 0x80 and andi 2, cc1 lw of the aligned word with andi 0x82; same result (the byte is the low byte).
