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

## SwitchOK (interface/frFont, raidou1 0x20, raidou2 0x20)

- Raidou 1: 1 of 8 words differ, first at +0x10.
- Raidou 2: 2 of 8 words differ, first at +0xC.

Nothing is recorded about this draft.

Raidou 1, first differing words:

| Offset | Draft | Retail |
|---|---|---|
| +0x10 | `sw v0,64(a0)` | `sw v0,76(a0)` |

Raidou 2, first differing words:

| Offset | Draft | Retail |
|---|---|---|
| +0xC | `li a1,128` | `li a1,14` |
| +0x10 | `sw v0,64(a0)` | `sw v0,80(a0)` |

## frCallEventVoiceSub (interface/frFont, raidou1 0x74)

Raidou 1: 1 of 29 words differ, first at +0x24.

Nothing is recorded about this draft.

First differing words:

| Offset | Draft | Retail |
|---|---|---|
| +0x24 | `addiu a1,v0,84` | `addiu a1,v0,100` |

## frCallGetStrWidth3 (interface/frFont, raidou1 0xCC)

Raidou 1: 1 of 51 words differ, first at +0x54.

Nothing is recorded about this draft.

First differing words:

| Offset | Draft | Retail |
|---|---|---|
| +0x54 | `addiu s2,v0,84` | `addiu s2,v0,100` |

## frCallSetFontString (interface/frFont, raidou1 0x68, raidou2 0x68)

Raidou 1 and Raidou 2: 13 of 26 words differ, first at +0x24.

Nothing is recorded about this draft.

Raidou 1, first differing words:

| Offset | Draft | Retail |
|---|---|---|
| +0x24 | `move t0,zero` | `sd ra,8(sp)` |
| +0x28 | `sd ra,8(sp)` | `jal 0x17eee0` |
| +0x2C | `jal 0x0` | `move t0,zero` |
| +0x30 | `move t1,zero` | `move v1,v0` |

Raidou 2, first differing words:

| Offset | Draft | Retail |
|---|---|---|
| +0x24 | `move t0,zero` | `sd ra,8(sp)` |
| +0x28 | `sd ra,8(sp)` | `jal 0x193b60` |
| +0x2C | `jal 0x0` | `move t0,zero` |
| +0x30 | `move t1,zero` | `move v1,v0` |

## frFontGetGlyphPositionAtIndex (interface/frFont, raidou1 0x8C)

Raidou 1: 1 of 35 words differ, first at +0x30.

Nothing is recorded about this draft.

First differing words:

| Offset | Draft | Retail |
|---|---|---|
| +0x30 | `addiu v1,v0,84` | `addiu v1,v0,100` |

## frLinkUserImage (interface/frFont, raidou1 0x50, raidou2 0x50)

Raidou 1 and Raidou 2: 15 of 20 words differ, first at +0x4.

Nothing is recorded about this draft.

Raidou 1, first differing words:

| Offset | Draft | Retail |
|---|---|---|
| +0x4 | `sd s0,0(sp)` | `move a2,zero` |
| +0x8 | `move s0,a1` | `sd s0,0(sp)` |
| +0xC | `sd ra,8(sp)` | `move s0,a1` |
| +0x10 | `jal 0x0` | `sd ra,8(sp)` |

## frSetMessageDefStepY (interface/frFont, raidou1 0xC, raidou2 0xC)

Raidou 1 and Raidou 2: 1 of 3 words differ, first at +0x0.

Nothing is recorded about this draft.

Raidou 1, first differing words:

| Offset | Draft | Retail |
|---|---|---|
| +0x0 | `li v0,20` | `li v0,25` |
