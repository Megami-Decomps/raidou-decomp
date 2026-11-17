# Draft notes: basic/dds3DbgBasic

## dds3DebugBasic (basic/dds3DbgBasic, raidou1 0x124, raidou2 0x124)

Raidou 1: 32 of 74 words differ, first at +0x1C; emitted 300 bytes, native 292 bytes.
Raidou 2: 32 of 74 words differ, first at +0x1C; emitted 300 bytes, native 292 bytes.

32 words off: retail puts li -1 in the beq delay slot and shares one return tail (bgezl/lui); if/else result var and early returns did not reproduce it; vectors do not reach, no vector file
