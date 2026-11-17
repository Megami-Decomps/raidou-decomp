# Draft notes: event/evtEventViewerMenu

## evtMainMenu (event/evtEventViewerMenu, raidou1 0xCC, raidou2 0xCC)

Raidou 1: 10 of 51 words differ, first at +0x74.
Raidou 2: 10 of 51 words differ, first at +0x74.

result=0 lands before the &fileMode addiu instead of in the bne delay slot; tried result var, early return, cursor local (worse), result at top (worse)

Related: placement of result=0 relative to the bne delay slot.

## evtSelectMenu (event/evtEventViewerMenu, raidou1 0xFC, raidou2 0xFC)

Raidou 1: 8 of 63 words differ, first at +0xA4.
Raidou 2: 8 of 63 words differ, first at +0xA4.

result=0 hoisted before the &select_y addiu, retail has it in the bne delay slot; same cause as evtMainMenu; tried early return, result var, else arm (worse)

Related: placement of result=0 relative to the bne delay slot.

## evtSOUTAI (event/evtEventViewerMenu, raidou1 0x2C8, raidou2 0x2C8)

Raidou 1: 156 of 177 words differ, first at +0x40; emitted 708 bytes, native 712 bytes.
Raidou 2: 156 of 177 words differ, first at +0x40; emitted 708 bytes, native 712 bytes.

register/order drift from +0x40: retail keeps group->type in a0 and builds the evtData6+2 base separately; tried a cached type local re-read each iteration (155 words); draft uses extern s8 evtSOUTAIColumnWidth[] which needs a symbol at 0x46CB58 (renames.tsv)

Related: group->type kept in a0 and the evtData6+2 base built separately.

## evtCallConsPanelTagLinkZ (event/evtEventViewerMenu, raidou1 0x42C, raidou2 0x42C)

Raidou 1: 194 of 267 words differ, first at +0x1C.
Raidou 2: 194 of 267 words differ, first at +0x1C.

register allocation drift from +0x1C: retail keeps i in s3 and the selected-marker colour in s8 from the start and stores 0x80800000 to sp+0x30 before the first printf (second store 0x80808080 before the rect loop); tried the dds locals order (221 words, worse); helpers inlined as retail has none; added dds3PrimLine prototype and curveComponent field

Related: i in s3 and marker colour in s8 from the start.
