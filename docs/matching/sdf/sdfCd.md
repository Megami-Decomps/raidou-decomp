# Draft notes: sdf/sdfCd

## sdfCdImageSearchFile (sdf/sdfCd, raidou1 0x1B4, raidou2 0x1B4)

Raidou 1 and Raidou 2: 0 of 109 words differ.

`check_unit.py` reports no differing word in Raidou 1 and Raidou 2: the draft now compiles to retail's text there. Promote it after confirming the unit's data and the full build.

Note from when the draft was written: Branch after memcmp is bnel with the load in its slot; retail is bne.

Related: [smt3 levers: callee visibility and branch-likely](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/callee-visibility.md).
