# Draft notes: sbtlSub

## sbtlCheckCrossCircle_Rot (sbtlSub, raidou1 0x180)

Raidou 1: 27 of 96 words differ, first at +0xE8.

retail copies the asm length output f2 to f1 for the zero test; early returns and the nested positive form tried

Related: regalloc.

## sbtlSetPushPos (sbtlSub, raidou1 0x208)

Raidou 1: 52 of 130 words differ, first at +0x70.

r1+r2 lives in f1 in retail and is copied to f12 for the Cross call on the unadjusted path; the sqrt operands are squared dis first; mine allocates dis to f13 and CSEs &base. Tried one dis variable (63), a separate call distance (63), the GETLENGTH output reused (52), dis ahead of the flag test (95).

Related: regalloc.

## sbtlSetPullPos (sbtlSub, raidou1 0x390)

Raidou 1: 98 of 222 words differ, first at +0x7C; emitted 888 bytes, native 912 bytes.

Retail loads wk1->flg on both arms of the ppos.y test and never after the adjust block, so its adjust-block stores leave memory unclobbered, and it rematerialises addiu sp,16 at every &base access. Tried direct flg reads (105), a flg local (99), one dis variable written back by the sqrt (98). STOREVECTOR_NOMEM there would let cc1 drop base.y = ground, so not used.

Related: load placement.
