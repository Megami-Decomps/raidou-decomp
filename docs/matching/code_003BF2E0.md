# Draft notes: code_003BF2E0

## CheckFlagTGS (code_003BF2E0, raidou2 0x48)

Raidou 2: 9 of 17 words differ, first at +0x20; emitted 68 bytes, native 72 bytes.

Branch layout: retail tests ==3 with v0=1 in the delay slot, shared by the ==5 path, and falls to v0=0. Tried &&/|| returns, a switch and a return variable. 9 of 18 words; same return values (nm_behaviour differs only in how 1 is formed).

## SetFlagMenu (code_003BF2E0, raidou2 0x98)

Raidou 2: 14 of 38 words differ, first at +0x48; emitted 184 bytes, native 152 bytes.

Sibling call: cc1 tail-calls itfMenuSetFlagMenu (j after the restores) where retail uses jal and the epilogue. 14 words; behaviour equal (nm_behaviour clean).

## SetRootMenu (code_003BF2E0, raidou2 0x94)

Raidou 2: 15 of 38 words differ, first at +0x3C; emitted 172 bytes, native 148 bytes.

Sibling call: cc1 tail-calls itfMenuSetRootMenu where retail uses jal and the epilogue. 15 words; behaviour equal (nm_behaviour clean).

## jmsLeaf12 (code_003BF2E0, raidou2 0x10)

Raidou 2: 3 of 4 words differ, first at +0x0.

Register: retail builds the vtable address in $3, cc1 in $2; a non-void return type with no return statement matches but is not allowed. 3 words; behaviour equal (nm_behaviour clean).

## jmsRefDDS3GBWK (code_003BF2E0, raidou2 0x7C)

Raidou 2: 2 of 31 words differ, first at +0x5C.

Store order: retail stores unkB8 (99, in v0) before unkAC. Tried all four store orders with return 99, and return wk->unkB8 = 99. 2 words; behaviour equal (nm_behaviour clean).

## jmsMenuProcessEnd (code_003BF2E0, raidou2 0x68)

Raidou 2: 10 of 26 words differ, first at +0xC; emitted 116 bytes, native 104 bytes.

Sibling call: cc1 tail-calls datDebugPrintf where retail uses jal and its own return. Tried FAKEMATCH_ASM_READ(work) after the call (worse). 16 of 26 words; behaviour equal (nm_behaviour clean).

## jmsJimusyoJmsQuest (code_003BF2E0, raidou2 0x70)

Raidou 2: 12 of 28 words differ, first at +0x10.

Register allocation: retail keeps the object in $6 and the vtable slot in $3, cc1 the object in $3. Tried no slot local, separate declarations and delta + object order. 16 of 28 words; behaviour equal (nm_behaviour clean).

## rstResultStartWaitProcess (code_003BF2E0, raidou2 0x6C)

Raidou 2: 17 of 27 words differ, first at +0x14.

Scheduling: cc1 hoists li -1 above the flag tests where retail sets v0 to 0 first and loads -1 in the delay slot. Tried a flags local, an if/else and direct returns. 10 of 27 words; behaviour equal (nm_behaviour clean).

## rstCalcProcess (code_003BF2E0, raidou2 0xD4)

Raidou 2: 21 of 51 words differ, first at +0x2C; emitted 204 bytes, native 212 bytes.

Loop form: retail forms the stock offset i*0x178 + 0x24E0 before adding DDS3_GBWK, cc1 adds the base first and steps a pointer. Tried now/old pointers and indexing old. 30 of 51 words; nm_behaviour differs only in the offset immediates, same addresses.

## rstCallGetGlobalEncountNo (code_003BF2E0, raidou2 0x98)

Raidou 2: 31 of 38 words differ, first at +0x4.

Branch form: cc1 folds the 0x20000 and bit 0 tests into one (flags & 0x20001) == 1 compare, retail tests each bit. Tried nested ifs, a flags local and a result local. 7 of 38 words; nm_behaviour differs only in that folded immediate, same condition.

## rstResultNakamaLevelSub (code_003BF2E0, raidou2 0x68)

Raidou 2: 20 of 26 words differ, first at +0x8.

Register allocation: retail keeps the table in $3 and a_x in $8, cc1 uses $8/$9. Tried no steps local, a base local and an s32 return. 6 of 26 words; behaviour equal (nm_behaviour clean).

## rstLvUpWaitCalcProcess (code_003BF2E0, raidou2 0x124)

Raidou 2: 37 of 74 words differ, first at +0x30; emitted 296 bytes, native 292 bytes.

Register allocation: after rstGetGlobalEncountNo cc1 copies v0 to a0 before forming the sbtlData100 row offset, one extra move that shifts the later branches. Tried a flags local, the set path first and unk10 - 1 != 0. 36 of 73 words; behaviour equal (nm_behaviour clean).

## jmsFunc17 (code_003BF2E0, raidou2 0x5C)

Raidou 2: 12 of 23 words differ, first at +0x10.

Register allocation: retail keeps the object in $5 and the vtable slot in $3, cc1 the object in $3 and the slot in $2 (same as jmsJimusyoJmsQuest). Tried a slot local and direct wk->vtbl[n] access. 11 of 23 words.

## rstPreCalcUnit (code_003BF2E0, raidou2 0x158)

Raidou 2: 2 of 86 words differ, first at +0x34.

Two independent pointer setups (pData+0x118 and pData+0x5AA0) are emitted in swapped order. Tried both statement orders and while/for loop forms. 84 of 86 words.

## rstResultLevelUpAnimSub (code_003BF2E0, raidou2 0x14C)

Raidou 2: 2 of 83 words differ, first at +0x88.

Register swap: retail builds the sprNo * 0xA0 slot offset in $4 and copies it to $5, cc1 builds it in $5 and copies to $4. Tried a slot pointer per block, a u16 index and the y update first. 81 of 83 words.

Fragile: on bdf2c164 the R2-S3 part 9 declarations moved the full unit from CONTEXT 2/83 to CONTEXT 6/83, and reverting fld.h or sBtlEff.h alone made it match, so it moves with the count of declarations, not its own C. On 0469e6c5 it is back at 2/83.

## rstResultGetExpCyuuseiSub (code_003BF2E0, raidou2 0x15C)

Raidou 2: 6 of 87 words differ, first at +0x84.

It matched until the R2-C3 header declarations; the body is unchanged, so it is kept NON_MATCHING and no declaration is moved to protect it. The difference is the two stack slots the full unit loads into v0/v1 and the register added to them, swapped.

Fragile: the full-unit result moves with declarations in headers the unit includes. The R2-C3 part 3 edits to sBtlEff.h, sbtlDMG.h and sbtlWork.h turn the -DNON_MATCHING full unit from OK into CONTEXT 6/87 (reverting any one of them alone restores it), and the part 4 edits do the same to the default build.

## cmpCallGetProcessData33 (code_003BF2E0, raidou2 0x1E0)

Raidou 2: 9 of 120 words differ, first at +0x6C.

Three guards after calls (dds3ExistProcess, itfMenuGetFlag, alpha) branch to the epilogue skipping the $2 = 0 that retail sets in each beql slot; the default arm's movn operands are swapped. Tried: early returns, nested ifs, goto to a trailing return 0, ternary and if/else defaults.

## cmpCallBitCheck10 (code_003BF2E0, raidou2 0x148)

Raidou 2: 23 of 81 words differ, first at +0xC0; emitted 324 bytes, native 328 bytes.

Loop entry: retail keeps -1 in a saved register and loads the table entry at the loop top; this draft peels the first load into each of the four entry paths. Tried a while over *p++, a for(;;) with break and an indexed walk. 58 of 81 words. nm_behaviour reports only the peeled load (one memx load and one branch moved); the walk and its calls are the same.

## rstResultLevelUpAnim (code_003BF2E0, raidou2 0x1D0)

Raidou 2: 12 of 116 words differ, first at +0x10.

Twelve words: the key table's address and pSpr swap $22 and $23 (every use of either). Tried: for and do/while loops, key/table declaration order (34 words), three spellings of the step select.

## cmpLoadLocalizeText (code_003BF2E0, raidou2 0x1C4)

Raidou 2: 8 of 113 words differ, first at +0xA8.

Eight words: the store of evtCallCheckRankUpData2 against the count local, and the g_pUnitDst/g_pUnitSrc stores in the second loop, are scheduled in a different order. First draft; not tuned.

## rstCallBlendColor2 (code_003BF2E0, raidou2 0x2A8)

Raidou 2: 9 of 170 words differ, first at +0x68.

Nine words, registers only: retail forms the slot pointer in the index register and groups the colour or-tree as (v<<24|v)|(v<<8|v<<16); cc1 forms it in the workEntries register and regroups the ors. Tried &workEntries[i] and workEntries + i, three spellings of the or-tree.

## rstResultCyuuseiMaxAnim (code_003BF2E0, raidou2 0x2B0)

Raidou 2: 30 of 170 words differ, first at +0x134; emitted 680 bytes, native 688 bytes.

Thirty words from one kept copy: retail holds X + w*8 and Y + h*4 in $16/$17 and copies them into $4/$5 for the second rstResultLevelUpAnimSub, two words more than this draft, which coalesces them into the argument registers; everything after shifts. Tried assigning them to w/h before the call and reading the width into its own local first.

## cmpCallMenuGetMenuNum (code_003BF2E0, raidou2 0x394)

Raidou 2: 6 of 229 words differ, first at +0x230.

The jmsAnimFunc guard emits bnel where retail has bnez with the same move into $a2 in the delay slot, and the 26-entry draw func loop and the i * 10 + 36 index take $v0 where retail uses $v1. Tried a separate loop variable, a ranged inner loop and the declaration order. 7 of 229 words.

Related: docs/levers/addressing-and-types.md:526.

## Init_rstInit (code_003BF2E0, raidou2 0x604)

Raidou 2: 58 of 385 words differ, first at +0x4.

The source pointer and the new work come out in swapped callee-saved registers (retail: src in $s1, the work in $s2, the loop index reusing $s0 after the handle), which moves every later use; separate declarations and assignment order leave the pair swapped. A rank local for datGetSummonerRank() - 1 keeps retail's separate decrement. 58 of 385 words.

Related: docs/levers/cross-block-locals.md:1.

## Destroy_rstInit (code_003BF2E0, raidou2 0x330)

Raidou 2: 186 of 188 words differ, first at +0x0; emitted 752 bytes, native 816 bytes.

The reward item loop and the gift loop keep a byte offset register stepping by 12 plus one base register per field, with an addu per load, where cc1 reduces each loop to a single pointer; that frees three saved registers and shifts every word. Per-field locals and an unsigned index compile the same way. 186 of 188 words (retail 204). nm_behaviour: memx/imm facts differ only in that addressing (offset register plus field bases versus one pointer); the same fields are read.

## rstResultGetExpCyuusei (code_003BF2E0, raidou2 0x578)

Raidou 2: 0 of 350 words differ.

One annul bit: the unk8058 > 0 test of the first rstCallPlaySE block is blezl where retail has blez, with andi flags & 2 in the delay slot either way. Tried the && form and nested ifs. 1 of 350 words.

## rstResultExpGauge (code_003BF2E0, raidou2 0x400)

Raidou 2: 3 of 256 words differ, first at +0x2FC.

cc1 swaps the two stores of the gauge's then arm, so its bounds store is cross-jumped into one tail with the else arm where retail keeps a store in each arm. Tried an entry pointer and an index local. 3 of 256 words.

## rstCallBlendColor3 (code_003BF2E0, raidou2 0x3B0)

Raidou 2: 0 of 236 words differ.

The two guards before the last rstResultLevelUpAnimSub call emit blezl and bnel where retail has plain blez and bne with the ld $16 epilogue restore executed in the first slot; an early return for the stock guard and a split rate guard compile identically. 3 of 236 words.

Related: docs/levers/addressing-and-types.md:526.

## rstSetLvUpSkl (code_003BF2E0, raidou2 0x360)

Raidou 2: 45 of 216 words differ, first at +0x1BC.

The level-up skill part matches. In the omoide skill loop cc1 folds gohData7[id].memskill[i] into one (id*37+i)*4 index where retail adds i*4+0x50 to the devil pointer, enters the loop through a jump to its test, and uses bne where retail uses bnel to the epilogue. Tried a skill local, a goto out of the aOmoideSkill search, an early return for the loyalty check and nested ifs (32 bytes longer). 45 of 216 words. nm_behaviour: memx/imm facts differ only in the folded +0x50 index; the same memskill entries are read.
