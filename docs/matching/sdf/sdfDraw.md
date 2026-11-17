# Draft notes: sdf/sdfDraw

## sdfGrStdDrawEnvPredraw (sdf/sdfDraw, raidou1 0x1B8, raidou2 0x1B8)

Raidou 1: 25 of 110 words differ, first at +0x88.
Raidou 2: 25 of 110 words differ, first at +0x88.

a3/t0 argument registers swapped at +0x88 and one call scheduled a slot later

Related: regalloc.

## sdfGrDrawFrame (sdf/sdfDraw, raidou2 0x480)

Raidou 2: 17 of 288 words differ, first at +0x3F4.

retail puts the volatile sdfDrawPublishFlag store in the beqz delay slot on firstFlag, mine fills the slot with the epilogue ld; same body matches in R1; 17 words

Related: delay-slot fill after the publish flag store.
