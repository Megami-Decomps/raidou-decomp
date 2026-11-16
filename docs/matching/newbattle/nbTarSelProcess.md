# Draft notes: newbattle/nbTarSelProcess

## itfMenuGetMenuNum (newbattle/nbTarSelProcess, raidou1 0x20, raidou2 0x20)

- Raidou 1: 0 of 8 words differ.
- Raidou 2: 1 of 8 words differ, first at +0x10.

`check_unit.py` reports no differing word in Raidou 1: the draft now compiles to retail's text there. Promote it after confirming the unit's data and the full build.

Note from when the draft was written: Same bytes as retail, but 0xB66 is a raw offset into process data whose struct is not defined yet. The callee takes no argument here, unlike the dds3GetProcessData(task) other units declare.
