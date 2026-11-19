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

## rstCallGetProcessData39 (code_003BF2E0, raidou2 0x124)

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

## rstResultGetExpCyuuseiSub (code_003BF2E0, raidou2 0x15C)

Raidou 2: 6 of 87 words differ, first at +0x84.

It matched until the R2-C3 header declarations; the body is unchanged, so it is kept NON_MATCHING and no declaration is moved to protect it. The difference is the two stack slots the full unit loads into v0/v1 and the register added to them, swapped.

Fragile: the full-unit result moves with declarations in headers the unit includes. The R2-C3 part 3 edits to sBtlEff.h, sbtlDMG.h and sbtlWork.h turn the -DNON_MATCHING full unit from OK into CONTEXT 6/87 (reverting any one of them alone restores it), and the part 4 edits do the same to the default build.
