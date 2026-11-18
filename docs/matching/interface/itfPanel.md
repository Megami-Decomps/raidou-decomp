# Draft notes: interface/itfPanel

## itfCallMiscDrawRectanglePrim (interface/itfPanel, raidou1 0x5B8, raidou2 0x5B8)

Raidou 1: 320 of 323 words differ, first at +0x0; emitted 1292 bytes, native 1464 bytes.
Raidou 2: 320 of 323 words differ, first at +0x0; emitted 1292 bytes, native 1464 bytes.

Retail copies the colour table from itfCallMiscDrawRectanglePrimTable and the vertex and colour index quads from itfCallMiscDrawRectanglePrimData3 and Data4, and scales each frame alpha with a signed divide by 128.
