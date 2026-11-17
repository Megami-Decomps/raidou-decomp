# Draft notes: field/fldFileResolver

## mapSummonDvGet (field/fldFileResolver, raidou1 0x40, raidou2 0x40)

Raidou 1: 14 of 14 words differ, first at +0x0; emitted 56 bytes, native 64 bytes.
Raidou 2: 14 of 14 words differ, first at +0x0; emitted 56 bytes, native 64 bytes.

Retail copies the row pointer through a3, t0 and v0 and adds the mapWork2 base last, as an inlined getter would. A pointer local, direct mapWork2[i][k], a static inline getter and re-reading the index per access all add the base first without the copies. Checked against retail call records (ingame).

N2: a static inline getter called per access reloads the index after each float store (15 of 16); the copies look like pointer parameters of nested inlines, not reconstructed.

Related: [smt3 levers: scalar spill slots](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scalar-spills.md).

## mapAiboTask (field/fldFileResolver, raidou1 0x9A8)

Raidou 1: 246 of 618 words differ, first at +0x8C.

Raidou 1: 246 of 618 words differ with the same size and flow; retail schedules the y update first in the position blocks, where the x, y, z source order (which matched in mapSummonDvTask) does not. Two else-if arms with identical bodies are cross-jumped into one unless the second assigns speed before mode. Raidou 2's body differs, so Raidou 2 keeps the asm. Vector checked (menu).

Fragile: the -DNON_MATCHING full-unit result moves with the declarations the unit includes; giving the mdlManager.h prototype mdlLink_NoDraw its second parameter (sdfModelOt_t *aModelOt or void *) alone turns CONTEXT 319/618 into DIFF 323/618.

Related: scheduling of the position update.

## mapSummonDvTask (field/fldFileResolver, raidou1 0x944)

Raidou 1: 8 of 593 words differ, first at +0x444.

Raidou 1: 8 of 593 words differ, one temp register (f0 vs f8) in the spd_max clamp arm. A separate follow flag from the per-branch mode keeps the mode=0 stores; rot declared before axis puts axis at sp+16; the extern sdfVector4_t table copied at its declaration gives the prologue ldl/ldr copy. Raidou 2's body differs, so Raidou 2 keeps the asm. Vector checked (menu).

Related: register choice in the spd_max clamp.
