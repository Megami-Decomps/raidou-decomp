# Draft notes: basic/dds3Basic

## dds3AllocBasic (basic/dds3Basic, raidou1 0xD8, raidou2 0xD8)

Raidou 1: 6 of 54 words differ, first at +0x58.
Raidou 2: 6 of 54 words differ, first at +0x58.

smt3 body with BASIC_TYPE_MAX (17) and plain NULL stores for staticdata/workdata; those two sw zero schedule ahead of flag/serialID/name (smt3 matched with *(u32 *)& casts, a false type). Moving them after basicfunction or loading funcs first: 8 words. vector checked

N1: typing staticdata u32 (DDS calls it word14 and stores 0) puts that store in retail's slot, but the workdata store still schedules ahead, so the count stays 6; workdata is used as a pointer throughout, so the header was left alone. DDS's statement order (funcs read after name) is inert.

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).

## dds3SetPosPoseSet (basic/dds3Basic, raidou1 0x2C, raidou2 0x2C)

Raidou 1: 5 of 11 words differ, first at +0x0.
Raidou 2: 5 of 11 words differ, first at +0x0.

sibling SetRot shape; retail keeps pose in a0 and dst in a2 where the matched SetRot/SetScl siblings use a2/a0. Tried no dst local, no pose local, split |=/&=: all the same 5 words (register choice only). vector checked

N2: a struct copy (12), the F32 and RWSRC copy macros and a dst-first form with no pose local (5 each) do not move pose to a0.

Related: register-allocation.
