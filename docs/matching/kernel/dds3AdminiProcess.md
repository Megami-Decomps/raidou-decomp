# Draft notes: kernel/dds3AdminiProcess

## dds3CallEncycFindFreeSlot (kernel/dds3AdminiProcess, raidou1 0x94, raidou2 0x94)

Raidou 1: 7 of 37 words differ, first at +0x3C.
Raidou 2: 7 of 37 words differ, first at +0x3C.

7 words off: dds mode-3 body via fclEncycSetEntry; two addu operands swapped (register numbering of two slot addresses); same helper variants as fclEncycAllocEntry; vectors do not reach, checked against the asm

## fclEncycAllocEntry (kernel/dds3AdminiProcess, raidou1 0xA8, raidou2 0xA8)

Raidou 1: 11 of 42 words differ, first at +0x8.
Raidou 2: 11 of 42 words differ, first at +0x8.

11 words off: dds dds3SetNamedSlotState inline (static fclEncycSetEntry in the unit); register assignment only (retail s0/s1/s2 = update/destroy/userValue, mine userValue first); tried store order kind,update,destroy,userValue (worse) and a slot pointer without the helper (addresses no longer hoisted); vectors do not reach, checked against the asm

## dds3CallEncycFindFreeSlot2 (kernel/dds3AdminiProcess, raidou1 0xAC, raidou2 0xAC)

Raidou 1: 11 of 43 words differ, first at +0x8.
Raidou 2: 11 of 43 words differ, first at +0x8.

11 words off: dds dds3SetNamedSlotState inline (static fclEncycSetEntry in the unit); register assignment only (retail s0/s1/s2 = update/destroy/userValue, mine userValue first); tried store order kind,update,destroy,userValue (worse) and a slot pointer without the helper (addresses no longer hoisted); vectors do not reach, checked against the asm

## fclEncycFindFreeSlot (kernel/dds3AdminiProcess, raidou1 0x1C4, raidou2 0x1C4)

Raidou 1: 2 of 113 words differ, first at +0x158.
Raidou 2: 2 of 113 words differ, first at +0x158.

2 words off: swap of the task (0x24) and update (0x28) slot stores; the retail store order (enabled, task, previous, update...) ordered from the asm moves 41 words of register numbering; vectors do not reach, no vector file

## fclEncycFreeEntries (kernel/dds3AdminiProcess, raidou1 0x1E8, raidou2 0x1E8)

Raidou 1: 2 of 122 words differ, first at +0x100.
Raidou 2: 2 of 122 words differ, first at +0x100.

2 words off: two adjacent slot-clear stores of fclEncycSetEntry swap order (same pair as fclEncycFindFreeSlot); reordering the inline body moved 7-11 words in its other users; vectors do not reach, no vector file
