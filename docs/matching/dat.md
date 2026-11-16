# Draft notes: dat

## datCheckSkillEnable (dat, raidou1 0x144)

Raidou 1: 63 of 76 words differ, first at +0x1C; emitted 304 bytes, native 324 bytes.

Raidou 1 only (the Raidou 2 body differs and stays INCLUDE_ASM). Branch layout: retail tests prio == 1 first but lays the prio == 2 block inline and returns 0 through bnel. Shapes tried: if / else if, switch, prio == 2 first, nested prio != 1.

Related: [smt3 levers: control flow](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/control-flow.md).
