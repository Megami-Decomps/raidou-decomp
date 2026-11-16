# Draft notes: code_0013AAC8

## mapGetScreenDir (code_0013AAC8, raidou1 0x390)

Raidou 1: 156 of 224 words differ, first at +0x18; emitted 896 bytes, native 912 bytes.

retail keeps a second copy of cy (f26) in the loop and in the fldPoint3ToZ pair, which pushes the 180/3.14 constant back into the loop; a local copy of cy and inline helpers did not reproduce it

Related: register allocation.
