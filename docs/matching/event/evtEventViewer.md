# Draft notes: event/evtEventViewer

## Process3 (event/evtEventViewer, raidou1 0x2F8, raidou2 0x2F8)

Raidou 1 and Raidou 2: 47 of 190 words differ, first at +0x1DC.

Same length; two branch delay slots (after the flags & 8 test in the tail) stay unfilled where retail fills them.

## evtCallGetProcessData11 (event/evtEventViewer, raidou1 0x90, raidou2 0x90)

Raidou 1 and Raidou 2: 7 of 36 words differ, first at +0x1C.

7 words off, register allocation: the work pointer lands in a different register.

Tried: u16 locals and passing the fields directly as arguments; neither moved the work pointer's register.
