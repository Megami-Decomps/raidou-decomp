# Draft notes: code_r1_001CA8F8

## evtGetValueNewSLightFrame (code_r1_001CA8F8, raidou1 0x238, raidou2 0x238)

Raidou 1: 15 of 142 words differ, first at +0x68.
Raidou 2: 15 of 142 words differ, first at +0x68.

register allocation only: key2 in a2 not t0 and soften1/soften2 swapped f21/f22; key2 != NULL arm written first fixed the block order (98->15); float declaration order (all 120 perms) does not move it; vector checked (opening: key1 NULL calls evtInitCustomSceneLightSub, copy arm stores full table plus out_soften/out_bias)

## mnuAdjustTimelineDurations (code_r1_001CA8F8, raidou1 0x1DC, raidou2 0x1DC)

Raidou 1: 9 of 119 words differ, first at +0x8.
Raidou 2: 9 of 119 words differ, first at +0x8.

t4/t5 swap of end and i (declaration order tried 3 ways) plus dependent words; key span read via evtFrameInfo_t.index (a union index/duration at +2 shifted RunFrame1 in evtEventViewer.c, so reverted)

Related: t4/t5 allocation of end and i.
