# Draft notes: interface/frFont

## MessageRequestNS (interface/frFont, raidou1 0x40, raidou2 0x40)

Raidou 1 and Raidou 2: 7 of 15 words differ, first at +0x20; emitted 60 bytes, native 64 bytes.

Nothing is recorded about this draft.

Raidou 1, first differing words:

| Offset | Draft | Retail |
|---|---|---|
| +0x20 | `jal 0x12e0` | `sw zero,0(sp)` |
| +0x24 | `sw zero,0(sp)` | `jal 0x181090` |
| +0x28 | `ld ra,24(sp)` | `sb zero,-21960(gp)` |
| +0x2C | `sb s0,0(gp)` | `ld ra,24(sp)` |

Raidou 2, first differing words:

| Offset | Draft | Retail |
|---|---|---|
| +0x20 | `jal 0x1320` | `sw zero,0(sp)` |
| +0x24 | `sw zero,0(sp)` | `jal 0x196080` |
| +0x28 | `ld ra,24(sp)` | `sb zero,-19728(gp)` |
| +0x2C | `sb s0,0(gp)` | `ld ra,24(sp)` |
