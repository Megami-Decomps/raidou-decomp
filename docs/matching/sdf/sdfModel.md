# Draft notes: sdf/sdfModel

## sdfCallModelMake2 (sdf/sdfModel, raidou1 0x9C, raidou2 0x9C)

Raidou 1 and Raidou 2: 35 of 36 words differ, first at +0x0; emitted 144 bytes, native 156 bytes.

Behavior is the same.

## sdfModelLinkOt_sdfModel (sdf/sdfModel, raidou1 0x4E0, raidou2 0x4E0)

Raidou 1 and Raidou 2: 285 of 311 words differ, first at +0x60; emitted 1244 bytes, native 1248 bytes.

The frame is 192 bytes, not 176, because fog spills to the stack. Retail's first parameter is sdfModelOt_t *. Behavior is the same.

Related: [smt3 levers: scalar spill slots](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scalar-spills.md).
