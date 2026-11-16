# Draft notes: dds3KernelDraw_0010BEE0

## dds3FilterClear (dds3KernelDraw_0010BEE0, raidou1 0x3C)

Raidou 1: 3 of 15 words differ, first at +0xC.

Data37 store is scheduled ahead of Data34/36 (35,37,34,36 vs retail 35,34,36,37); tried left-rotation lever, moving 37 later, HD block order (35,34 then source order, 37/36 last); vector checked boot: stores 0x80/1/0/1/0... as retail

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).

## dds3FilterCalc (dds3KernelDraw_0010BEE0, raidou1 0x188)

Raidou 1: 0 of 98 words differ; the unit's .rodata does not.

The code matches alone. Its switch emits a jump table, so the unit would have to own .rodata from 0x444A58, and its .rodata section is 16-aligned (the jump table) where retail starts it at an 8-aligned address: the string fldStrMapAllFilterFld before it belongs to the same original TU as kernel/dds3KernelDraw. Lands when that chunk joins kernel/dds3KernelDraw.
