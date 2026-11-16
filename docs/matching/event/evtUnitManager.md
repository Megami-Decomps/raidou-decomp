# Draft notes: event/evtUnitManager

## UnitAnime (event/evtUnitManager, raidou1 0x1340, raidou2 0x1340)

Raidou 1 and Raidou 2: 70 of 1232 words differ, first at +0xACC.

Same length and structure; registers differ from the colour blend of the second light onward (address and scale temporaries swapped).

Tried: nothing beyond the draft. Open: register order in the second light's colour block.

## evtSetUnitMove2 (event/evtUnitManager, raidou1 0xC0, raidou2 0xC0)

Raidou 1 and Raidou 2: 1 of 48 words differ, first at +0x58.

Retail codegen differs in the null-check and call order.

## evtSetUnitRot2 (event/evtUnitManager, raidou1 0x6C, raidou2 0x6C)

Raidou 1 and Raidou 2: 1 of 27 words differ, first at +0x30.

Retail codegen differs in the null-check and call order.

## evtUnitManagerPrimaryFunc (event/evtUnitManager, raidou1 0x6E8)

Raidou 1: 18 of 442 words differ, first at +0x398.

Same length; the case 2 tail orders its flag load and compare differently, and the 0x1000000 test keeps a mask register in retail.
