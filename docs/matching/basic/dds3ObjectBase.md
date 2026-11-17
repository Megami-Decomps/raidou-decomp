# Draft notes: basic/dds3ObjectBase

## dds3CallMemset (basic/dds3ObjectBase, raidou1 0xE0, raidou2 0xE0)

Raidou 1: 12 of 56 words differ, first at +0x50.
Raidou 2: 12 of 56 words differ, first at +0x50.

retail rematerialises &aViewEye (lui/addiu) in the mark-pos arm where gcc copies the earlier pseudo (cse follows the branch, per the addressing lever at 1757); sdfSqrt not fsqrtf (no fpu.h); tried flipped arms (38 words). vector checked ingame (mark-pos path)

N2: the =m and F32 store macros give the same 12.

Related: addressing.
