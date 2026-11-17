# Draft notes: effect/effBattle

## effBTL (effect/effBattle, raidou1 0x1A4, raidou2 0x1A4)

Raidou 1: 5 of 105 words differ, first at +0x5C.
Raidou 2: 5 of 105 words differ, first at +0x5C.

5 words off: movz list select (sltiu/addiu order) and the hCutListTop/state store order

Related: statement position of the list selection; ternary, if, cutList local and goto loop forms tried.

Asm review: same behaviour as retail (register choice and store order). No usable vectors: calls2 has a pre snapshot only.
