# Draft notes: code_r1_002CAFD8

## cmpDrawBoxG (code_r1_002CAFD8, raidou1 0x190, raidou2 0x190)

Raidou 1: 8 of 100 words differ, first at +0xC0.
Raidou 2: 8 of 100 words differ, first at +0xC0.

Retail fills the first cmpExpandColor delay slot with the colors[1] load and sets a1 before a0 at the register call; the same difference in both games. d and e are added unscaled (unlike SMT3). Checked against retail call records (menu): the prim xy stores match.

N1: writing prim[2] before prim[3] in the first vertex gives retail's instruction order but swaps the zero-extended left and c between s18 and s19 (4 words on both games); declaring top before left, and a u64 local for (u32)c, are worse.

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).
