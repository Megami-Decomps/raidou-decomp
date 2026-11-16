# Draft notes: sbtl_0029BFB8

## CalcBadStatus (sbtl_0029BFB8, raidou1 0x3B0)

Raidou 1: 204 of 218 words differ, first at +0x1C; emitted 872 bytes, native 944 bytes.

the badst_cnt2 clamp if-converts to movz unless written as a ternary, and the ternary form loses the hp - dmg branch; retail keeps one dmg variable in s0 for poison damage and both heals. Jump table jtbl_0045BD70 and the 0.01 doubles CalcBadStatusTable/CalcBadStatusTable2 sit in the shared rodata_0045B9C0 segment

Related: register allocation and if-conversion.

## sbtlRefSbtl62 (sbtl_0029BFB8, raidou1 0xF8)

Raidou 1: 20 of 61 words differ, first at +0x30; emitted 244 bytes, native 248 bytes.

loop temporaries swap v0/a0 and i++ moves into a delay slot; hoisted wk != NULL flag matches the prologue; continue and nested-if forms tried

Related: regalloc.
