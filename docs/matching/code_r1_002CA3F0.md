# Draft notes: code_r1_002CA3F0

## effDrawTextureSlot (code_r1_002CA3F0, raidou1 0x2CC, raidou2 0x2D4)

Raidou 1: 2 of 179 words differ, first at +0x9C.
Raidou 2: 2 of 181 words differ, first at +0xA4.

Retail evaluates fclDrawPanelSprite's id before loading ot into the call's delay slot. Raidou 2 adds a slot >= set->count return. Checked against retail call records (menu, 32 rows): both the cmpFunc3/cmpFunc4 path and effSelectPresetAndDispatch.

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).
