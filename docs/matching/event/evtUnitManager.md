# Draft notes: event/evtUnitManager

## UnitAnime (event/evtUnitManager, raidou1 0x1340, raidou2 0x1340)

Raidou 1 and Raidou 2: 70 of 1232 words differ, first at +0xACC.

Same length and structure; registers differ from the colour blend of the second light onward (address and scale temporaries swapped).

Tried: nothing beyond the draft. Open: register order in the second light's colour block.

## evtUnitManagerPrimaryFunc (event/evtUnitManager, raidou1 0x6E8)

Raidou 1: 18 of 442 words differ, first at +0x398.

Same length; the case 2 tail orders its flag load and compare differently, and the 0x1000000 test keeps a mask register in retail.

## MakeLightPacket (event/evtUnitManager, raidou1 0x1F4, raidou2 0x1F4)

Raidou 1: 89 of 115 words differ, first at +0x14; emitted 460 bytes, native 500 bytes.
Raidou 2: 89 of 115 words differ, first at +0x14; emitted 460 bytes, native 500 bytes.

three lights + custom fallback; constant 0.0078125 reloaded per unpack via mtc1/mfc1 in retail, mine hoists it; checked against the asm: same stores and calls

## evtCalcFrame_UnitMoveType0 (event/evtUnitManager, raidou1 0x254, raidou2 0x254)

Raidou 1: 131 of 147 words differ, first at +0x38; emitted 588 bytes, native 596 bytes.
Raidou 2: 131 of 147 words differ, first at +0x38; emitted 588 bytes, native 596 bytes.

only real difference is the pose-copy loops: retail bases the induction pointer at poseset+0x50 (rot) with pos at -0x10, mine at poseset with +0x40/+0x50, and the stack slots and registers cascade; DDS matched sibling evtUnitPrepareVerticalMoveSteps indexes f32 pos[4]/rotation[4] arrays where Raidou's dds3PoseSet_t has sdfVector4_t (cast needed); tried swapped load order (worse); added evtUnit_t.moveYFrames at 0x204; vectors do not reach, checked against the asm

Related: pose-copy loop induction pointer base (dds3PoseSet_t pos/rot as f32 arrays).
