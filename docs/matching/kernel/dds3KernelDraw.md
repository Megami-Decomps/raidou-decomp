# Draft notes: kernel/dds3KernelDraw

## dds3CallGetFrameID (kernel/dds3KernelDraw, raidou1 0x2F0, raidou2 0x2F0)

Raidou 1 and Raidou 2: 151 of 188 words differ, first at +0x0.

Stack spills and register allocation differ; fade packet draw from SMT3 dds3DrawFade with bista offsets.

Related: [smt3 levers: scalar spill slots](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scalar-spills.md).

## dds3DrawBista (kernel/dds3KernelDraw, raidou1 0x2AC)

Raidou 1: 151 of 157 words differ, first at +0x0; emitted 628 bytes, native 684 bytes.

Nothing is recorded about this draft.

First differing words:

| Offset | Draft | Retail |
|---|---|---|
| +0x0 | `lbu a1,0(gp)` | `lbu a0,-25836(gp)` |
| +0x4 | `addiu sp,sp,-32` | `addiu sp,sp,-48` |
| +0x10 | `sll v1,a1,0x18` | `sll v1,a0,0x18` |
| +0x1C | `sd s0,0(sp)` | `sd s3,24(sp)` |

## dds3RunQuake (kernel/dds3KernelDraw, raidou1 0x1F4, raidou2 0x1F4)

Raidou 1 and Raidou 2: 65 of 124 words differ, first at +0x58; emitted 496 bytes, native 500 bytes.

Loop register use and float scheduling differ; body from SMT3, no frame gating.

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).
