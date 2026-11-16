# Draft notes: newbattle/nbTarSelProcess

## itfMenuGetMenuNum (newbattle/nbTarSelProcess, raidou1 0x20, raidou2 0x20)

- Raidou 1: 0 of 8 words differ.
- Raidou 2: 1 of 8 words differ, first at +0x10.

The bytes match in Raidou 1, but the draft reads a raw offset (0xB66) into process data whose type is unknown: the function has no caller in either game and no other code reads that offset, so there is nothing to name the struct from. Retail passes the caller's process ID through to `dds3GetProcessData` untouched, so the real signature takes `dds3ProcessID_t *`. It stays a draft until the process-data type is known.

Note from when the draft was written: Same bytes as retail, but 0xB66 is a raw offset into process data whose struct is not defined yet. The callee takes no argument here, unlike the dds3GetProcessData(task) other units declare.
