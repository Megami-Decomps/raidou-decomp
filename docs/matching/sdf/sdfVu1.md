# Draft notes: sdf/sdfVu1

## corePktSkinPointsFirst (sdf/sdfVu1, raidou1 0x88, raidou2 0x88)

Raidou 1: 34 of 34 words differ, first at +0x0.
Raidou 2: 34 of 34 words differ, first at +0x0.

Reference implementation, not a match candidate: retail is assembly that loads the matrix and light rows into VU0 and runs `vcallms 0` per vertex.

## corePktSkinPointsAccum (sdf/sdfVu1, raidou1 0xC8, raidou2 0xC8)

Raidou 1: 50 of 50 words differ, first at +0x0.
Raidou 2: 50 of 50 words differ, first at +0x0.

Reference implementation, not a match candidate: as corePktSkinPointsFirst, with `vcallms 0x48`.
