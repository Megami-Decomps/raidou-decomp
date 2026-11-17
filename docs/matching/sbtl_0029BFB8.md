# Draft notes: sbtl_0029BFB8

## CalcBadStatus (sbtl_0029BFB8, raidou1 0x3B0)

Raidou 1: 204 of 218 words differ, first at +0x1C; emitted 872 bytes, native 944 bytes.

the badst_cnt2 clamp if-converts to movz unless written as a ternary, and the ternary form loses the hp - dmg branch; retail keeps one dmg variable in s0 for poison damage and both heals. Jump table jtbl_0045BD70 and the 0.01 doubles CalcBadStatusTable/CalcBadStatusTable2 sit in the shared rodata_0045B9C0 segment

Related: register allocation and if-conversion.

## sbtlRefSbtl62 (sbtl_0029BFB8, raidou1 0xF8)

Raidou 1: 20 of 61 words differ, first at +0x30; emitted 244 bytes, native 248 bytes.

loop temporaries swap v0/a0 and i++ moves into a delay slot; hoisted wk != NULL flag matches the prologue; continue and nested-if forms tried

Related: regalloc.

## CalcBtlMagicStatus (sbtl_0029BFB8, raidou1 0x6E4)

Raidou 1: 5 of 441 words differ, first at +0x5FC.

only the tdan expiry clear differs: retail stores tdanhitcnt, tdancnt, tdancounter, tdancmpcnt and puts the s32 tdanpara store in the beq delay slot; ee-gcc schedules the tdanpara sw first and moves tdancmpcnt into the slot. Same five stores, functionally identical. Tried before the budget ran out: hp clamp as three stores (74 words), clamp to a local (129), ternary on hp = now.hp += v (280), ternary on hp = now.hp + v then store (14), tdan block nested like slip instead of early returns (5).

Related: sched.
