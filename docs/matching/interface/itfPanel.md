# Draft notes: interface/itfPanel

## itfCallMiscDrawRectanglePrim (interface/itfPanel, raidou1 0x5B8, raidou2 0x5B8)

Raidou 1 and Raidou 2: 316 of 320 words differ, first at +0x8; emitted 1280 bytes, native 1464 bytes.

Draft, 320 words against retail's 366: the colour table is a local initializer here, retail copies it from itfCallMiscDrawRectanglePrimTable (rodata) with a block-move loop, and the register use differs.
