# Draft notes: jmsMisc

## jmsChkCommonInput (jmsMisc, raidou1 0x2DC, raidou2 0x2DC)

Raidou 1: 1 of 183 words differ, first at +0x18.
Raidou 2: 1 of 183 words differ, first at +0x18.

1 words: x<0 stored as flag: slti retail vs srl 31 (lever: unsolved shape; tried s32/u32 flags, ternary, s64 cast). Indices from dds3PadButtonState 0x3AD390 base. vectors do not reach (no vector file for it), checked against the asm: same 0/1 value.
