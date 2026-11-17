# Draft notes: effect/effPCPMisc

## effPCPShockWave02Init (effect/effPCPMisc, raidou1 0x2C0, raidou2 0x2C0)

Raidou 1: 7 of 176 words differ, first at +0x6C.
Raidou 2: 7 of 176 words differ, first at +0x6C.

7 words off: s2/s3 swap between the out pointer and i only

Related: declaration order and init at declaration (fixed effPCPZiodin04Init) did not reach it.

Vectors (calls2 snapshots): pass, 173 of 176 words covered.

## effPCPZiodin03Init (effect/effPCPMisc, raidou1 0x2C0, raidou2 0x2C0)

Raidou 1: 7 of 176 words differ, first at +0x6C.
Raidou 2: 7 of 176 words differ, first at +0x6C.

7 words off: s2/s3 swap between the out pointer and i only, as in effPCPShockWave02Init

Related: same as effPCPShockWave02Init.

Vectors (calls2 snapshots): pass, 173 of 176 words covered.
