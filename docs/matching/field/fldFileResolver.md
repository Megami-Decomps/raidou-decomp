# Draft notes: field/fldFileResolver

## mapSummonDvGet (field/fldFileResolver, raidou1 0x40, raidou2 0x40)

Raidou 1: 14 of 14 words differ, first at +0x0; emitted 56 bytes, native 64 bytes.
Raidou 2: 14 of 14 words differ, first at +0x0; emitted 56 bytes, native 64 bytes.

Retail copies the row pointer through a3, t0 and v0 and adds the mapWork2 base last, as an inlined getter would. A pointer local, direct mapWork2[i][k], a static inline getter and re-reading the index per access all add the base first without the copies. Checked against retail call records (ingame).

Related: [smt3 levers: scalar spill slots](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scalar-spills.md).
