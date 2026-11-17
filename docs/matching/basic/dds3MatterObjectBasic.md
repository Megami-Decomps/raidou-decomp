# Draft notes: basic/dds3MatterObjectBasic

## dds3MatterObjectBasicDrawProcedure (basic/dds3MatterObjectBasic, raidou1 0x1EC, raidou2 0x1EC)

Raidou 1: 114 of 119 words differ, first at +0x0; emitted 476 bytes, native 492 bytes.
Raidou 2: 114 of 119 words differ, first at +0x0; emitted 476 bytes, native 492 bytes.

114 words off: retail hoists result=1 into the beq delay slot and uses bnel for the alpmode test, flag in v1 not v0; nested if, && form and goto-to-shared-tail all give the same 114-word layout; ModelOt[8 + idx] and ModelOt[10] read from the 0x3AE898/0x3AE8B8 offsets; vectors do not reach, no vector file
