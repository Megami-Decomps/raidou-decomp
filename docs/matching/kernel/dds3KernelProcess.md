# Draft notes: kernel/dds3KernelProcess

## dds3DrawFirstProcess (kernel/dds3KernelProcess, raidou1 0x7C0)

Raidou 1: 461 of 490 words differ, first at +0x2C; emitted 1960 bytes, native 1984 bytes.

Nothing is recorded about this draft.

First differing words:

| Offset | Draft | Retail |
|---|---|---|
| +0x2C | `sd s8,112(sp)` | `sd ra,112(sp)` |
| +0x30 | `sd ra,120(sp)` | `jal 0x3888b0` |
| +0x34 | `jal 0x0` | `li a2,16` |
| +0x38 | `li a2,16` | `lui at,0x3f80` |
