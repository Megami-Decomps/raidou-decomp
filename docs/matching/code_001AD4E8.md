# Draft notes: code_001AD4E8

## evtMoonTeisatuCntReset (code_001AD4E8, raidou1 0x10)

Raidou 1: 4 of 4 words differ, first at +0x0; emitted 20 bytes, native 16 bytes.

s8 field store of 1 plus return 1: ee-gcc emits two li (QImode and SImode constants, base in a0, 5 words) where retail shares one li v0,1 and stores from v0 in the jr slot; return-assign, s32/s8 local, u8 return, pointer local all give the same 5 words

Related: constants.
