#include "common.h"
#include "sdk_libc.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "sdfDevCons.h"
#include "sdf_resource.h"
#include "kernel_draw.h"
#include "dds3KernelMisc.h"
#include "dds3PadManager.h"
#include "EventBit.h"
#include "evtBitEditor.h"

#ifdef VERSION_RAIDOU1
/* body: matched in smt3 @ 9401f79e: src/smt3/event/evtBitEditor.c:85, rework, same function 1.00 (words 273/273, calls 1.00, shape 1.00) */
s32 dds3BitEditorProcess(void) {
    char glyph[2];
    sdfDrawTag_t *packets;
    s32 column, row, style, result;
    s32 cellWidth = 0xC0, cellHeight = 0x60;

    memcpy(glyph, dds3BitEditorGlyphTemplate, sizeof(glyph));
    packets = (sdfDrawTag_t *)sdfGrAllocPacketBuff(sizeof(*packets));
    sdfDrawTagInit(packets);
    dds3ConsPanelTagLink(packets, 50, 50, 23, 20);
    sdfDrawTagLink(packets, EE_ADDR(sdfDevPrintf(
        0x7320, 0x7A90, 0xFF0000, 0, "BitEditor")));
    if (evtBitCheck((dds3BitEditorRowBase + dds3BitEditorTable2) * 16 + dds3BitEditorTable)) {
        sdfDrawTagLink(packets, EE_ADDR(sdfDevPrintf(
            0x7AA0, 0x7A90, 0xFF0000, 7, "%04X = ON",
            (dds3BitEditorRowBase + dds3BitEditorTable2) * 16 + dds3BitEditorTable)));
    } else {
        sdfDrawTagLink(packets, EE_ADDR(sdfDevPrintf(
            0x7AA0, 0x7A90, 0xFF0000, 7, "%04X = OFF",
            (dds3BitEditorRowBase + dds3BitEditorTable2) * 16 + dds3BitEditorTable)));
    }
    for (column = 0; column < 16; column++) {
        style = dds3BitEditorTable == column ? 14 : 7;
        sdfDrawTagLink(packets, EE_ADDR(sdfDevPrintf(
            0x7320 + (column + 6) * cellWidth, 0x7A90 + cellHeight * 2,
            0xFF0000, style, dds3BitEditorColumnFormat, column)));
    }
    for (row = 0; row < 16; row++) {
        style = dds3BitEditorTable2 == row ? 14 : 7;
        sdfDrawTagLink(packets, EE_ADDR(sdfDevPrintf(
            0x73E0, 0x7A90 + (row + 3) * cellHeight, 0xFF0000,
            style, dds3BitEditorRowFormat, (dds3BitEditorRowBase + row) * 16)));
        for (column = 0; column < 16; column++) {
            if (evtBitCheck((dds3BitEditorRowBase + row) * 16 + column)) {
                glyph[0] = 'O';
                style = dds3BitEditorTable == column || dds3BitEditorTable2 == row ? 6 : 0;
            } else {
                glyph[0] = '+';
                style = dds3BitEditorTable == column || dds3BitEditorTable2 == row ? 14 : 8;
            }
            if (dds3BitEditorTable == column && dds3BitEditorTable2 == row) {
                style = 4;
            }
            sdfDrawTagLink(packets, EE_ADDR(sdfDevPrintf(
                0x7320 + (column + 6) * cellWidth,
                0x7A90 + (row + 3) * cellHeight, 0xFF0000, style, glyph)));
        }
    }
    {
        sdfGrOt_t *ot = &sdfGrOtTable[110];
        ot->regFunc(ot, packets);
    }
    if ((dds3PadBitTable[24] & 2) && dds3BitEditorRowBase > 0) {
        dds3BitEditorRowBase -= 16;
        if (dds3BitEditorRowBase < 0) dds3BitEditorRowBase = 0;
    }
    if (dds3PadBitTable[26] & 2) {
        if (dds3BitEditorRowBase + 16 < 256) {
            if (dds3BitEditorRowBase + 32 > 256) dds3BitEditorRowBase = 240;
            else dds3BitEditorRowBase += 16;
        }
    }
    result = dds3SelectMenu(1, 16, 256, 16, 16, NULL,
        &dds3BitEditorRowBase, &dds3BitEditorTable, &dds3BitEditorTable2);
    if (result == -1) return -1;
    if (result == 1) {
        if (evtBitCheck((dds3BitEditorRowBase + dds3BitEditorTable2) * 16 + dds3BitEditorTable)) {
            evtBitOff((dds3BitEditorRowBase + dds3BitEditorTable2) * 16 + dds3BitEditorTable);
        } else {
            evtBitOn((dds3BitEditorRowBase + dds3BitEditorTable2) * 16 + dds3BitEditorTable);
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "event/evtBitEditor", dds3BitEditorProcess);
#endif
#ifdef VERSION_RAIDOU2
INCLUDE_SDATA(const s32, "event/evtBitEditor", dds3BitEditorProcessData);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "event/evtBitEditor", dds3BitEditorRowBase);
#else
INCLUDE_SDATA(const s32, "event/evtBitEditor", dds3BitEditorProcessData2);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "event/evtBitEditor", dds3BitEditorTable);
#else
INCLUDE_SDATA(const s32, "event/evtBitEditor", dds3BitEditorProcessData3);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "event/evtBitEditor", dds3BitEditorTable2);

INCLUDE_SDATA(const s32, "event/evtBitEditor", dds3BitEditorGlyphTemplate);
#else
INCLUDE_SDATA(const s32, "event/evtBitEditor", dds3BitEditorProcessData4);
#endif

INCLUDE_SDATA(const s32, "event/evtBitEditor", dds3BitEditorColumnFormat);

INCLUDE_SDATA(const s32, "event/evtBitEditor", dds3BitEditorRowFormat);

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "event/evtBitEditor", evtPendingEventSelection);

#endif
