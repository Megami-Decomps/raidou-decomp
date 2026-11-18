# Draft notes: basic/dds3MatterObjectBasic

## dds3MatterObjectBasicDrawProcedure (basic/dds3MatterObjectBasic, raidou1 0x1EC, raidou2 0x1EC)

Raidou 1: 94 of 123 words differ, first at +0x0.
Raidou 2: 94 of 123 words differ, first at +0x0.

114 words off: retail hoists result=1 into the beq delay slot and uses bnel for the alpmode test, flag in v1 not v0; nested if, && form and goto-to-shared-tail all give the same 114-word layout. Every link but ModelOt[10] uses ModelOt[OtLinkIdx].
