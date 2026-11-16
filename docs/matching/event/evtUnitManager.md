# Draft notes: event/evtUnitManager

## UnitAnime (event/evtUnitManager, raidou1 0x1340, raidou2 0x1340)

Raidou 1 and Raidou 2: 70 of 1232 words differ, first at +0xACC.

Same length and structure; registers differ from the colour blend of the second light onward (address and scale temporaries swapped).

Tried: nothing beyond the draft. Open: register order in the second light's colour block.

## evtUnitManagerPrimaryFunc (event/evtUnitManager, raidou1 0x6E8)

Raidou 1: 18 of 442 words differ, first at +0x398.

Same length; the case 2 tail orders its flag load and compare differently, and the 0x1000000 test keeps a mask register in retail.
