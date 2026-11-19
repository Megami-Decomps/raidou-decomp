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
