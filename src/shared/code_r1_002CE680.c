#include "common.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "dds3GlobalWork.h"

#include "kernel_draw.h"
#include "sdf_resource.h"
#include "sdk_libc.h"
#include "include_asm.h"
#include "code_r1_0019D3A8.h"
#include "sdfDevCons.h"
#include "code_r1_002CD5A8.h"
#include "code_r1_002CAFD8.h"


/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */

SdfGridSeekResult sdfGridSeekFirstNode(itfGridTextWidget_t *widget);

SdfGridSeekResult sdfGridSeekLastNode(itfGridTextWidget_t *widget);

typedef itfGridTextListItem_t LmapNode;

typedef itfGridTextWidget_t LmapList;

extern LmapNode *fldLmapAdvanceWindowStart(LmapList *);
extern LmapNode *fldLmapExpandWindowBackward(LmapList *);
LmapNode *fldLmapAdvanceWindowStart(LmapList *list);
LmapNode *fldLmapExpandWindowBackward(LmapList *list);
LmapNode *fldLmapAdvanceCursor(LmapList *list);
LmapNode *fldLmapRewindCursor(LmapList *list);
LmapNode *fldLmapAdvanceThroughWindow(LmapList *list);
LmapNode *fldLmapRewindThroughWindow(LmapList *list);

extern void fldLmapSubmitScaledSpritePacket(s32 x, s32 y, s32 z, s32 width, s32 height,
                                            s32 fillColor, s32 borderColor, s32 surfaceIndex);

void fldLmapSubmitScaledSpritePacket(s32 x, s32 y, s32 z, s32 width, s32 height, s32 fillColor, s32 borderColor, s32 surfaceIndex);

/* Move the visible start forward without moving the cursor. */
LmapNode *fldLmapAdvanceWindowStart(LmapList *list) {
    LmapNode *cursor = list->selected;
    LmapNode *windowStart = list->firstVisible;

    if (cursor == list->tail) {
        return cursor;
    }
    windowStart = windowStart->next;
    if (windowStart == NULL) {
        return cursor;
    }
    list->cursorRow--;
    list->firstVisible = windowStart;
    return cursor;
}

/* Move the visible start backward only when a full forward span is available. */
LmapNode *fldLmapExpandWindowBackward(LmapList *list) {
    LmapNode *cursor = list->selected;
    LmapNode *windowStart = list->firstVisible;
    LmapNode *scanNode;
    s32 stepIndex;

    if (cursor == list->head) {
        return cursor;
    }
    scanNode = windowStart;
    for (stepIndex = 0; stepIndex < list->rows; stepIndex++) {
        if (scanNode == NULL) {
            return cursor;
        }
        scanNode = scanNode->next;
    }
    windowStart = windowStart->previous;
    list->firstVisible = windowStart;
    list->cursorRow++;
    return cursor;
}

/* Advance the unlocked cursor, wrapping at the range end and following its window. */
LmapNode *fldLmapAdvanceCursor(LmapList *list) {
    LmapNode *cursor = list->selected;
    LmapNode *nextNode;

    if (cursor == 0) {
        return 0;
    }
    if (list->flags & 1) {
        return cursor;
    }
    if (cursor == list->tail) {
        sdfGridSeekFirstNode(list);
        return list->selected;
    }
    nextNode = cursor->next;
    if (nextNode == 0) {
        return cursor;
    }
    cursor = nextNode;
    list->selected = cursor;
    list->cursorRow++;
    if (list->cursorRow >= list->rows - 1) {
        cursor = fldLmapAdvanceWindowStart(list);
    }
    return cursor;
}

/* Rewind the unlocked cursor, wrapping at the range start and following its window. */
LmapNode *fldLmapRewindCursor(LmapList *list) {
    LmapNode *cursor = list->selected;
    LmapNode *previousNode;

    if (cursor == 0) {
        return 0;
    }
    if (list->flags & 1) {
        return cursor;
    }
    if (cursor == list->head) {
        sdfGridSeekLastNode(list);
        return list->selected;
    }
    previousNode = cursor->previous;
    if (previousNode == 0) {
        return cursor;
    }
    cursor = previousNode;
    list->selected = cursor;
    list->cursorRow--;
    if (list->cursorRow <= 0) {
        cursor = fldLmapExpandWindowBackward(list);
    }
    return cursor;
}

/* Jump forward by twice the window capacity minus the cursor's window offset. */
LmapNode *fldLmapAdvanceThroughWindow(LmapList *list) {
    LmapNode *result = 0;
    s32 i;
    s32 steps;

    if (list->flags & 1) {
        return 0;
    }
    steps = list->rows * 2 - list->cursorRow;
    for (i = 0; i < steps; i++) {
        result = fldLmapAdvanceCursor(list);
    }
    return result;
}

/* Jump backward by the window capacity plus the cursor's window offset. */
LmapNode *fldLmapRewindThroughWindow(LmapList *list) {
    LmapNode *result = 0;
    s32 i;
    s32 steps;

    if (list->flags & 1) {
        return 0;
    }
    steps = list->rows + list->cursorRow;
    for (i = 0; i < steps; i++) {
        result = fldLmapRewindCursor(list);
    }
    return result;
}

void fldLocalMapDrawScrollIndicators(s32 offsetX, s32 offsetY, s32 z, u32 color, itfGridTextWidget_t *list,
                                     s32 channel) {
    u32 coordinates[2][3];
    u32 flags = itfGetGridListLinkFlags(list);
    s32 halfWidth = list->width / 2;
    s32 baseX = list->x + offsetX;
    coordinates[0][0] = baseX + (halfWidth << 4);
    coordinates[0][1] = baseX + ((halfWidth - 6) << 4);
    coordinates[0][2] = baseX + ((halfWidth + 6) << 4);
    if (flags & 1) {
        s32 baseY = list->y + offsetY;
        coordinates[1][0] = baseY - 80;
        coordinates[1][2] = coordinates[1][1] = baseY - 24;
        uiDrawUniformRgbRange(coordinates[0], coordinates[1], z, color, channel);
    }
    if (flags & 2) {
        s32 height = list->height;
        s32 baseY = list->y + offsetY;
        coordinates[1][0] = baseY + ((height + 10) << 3);
        coordinates[1][2] = coordinates[1][1] = baseY + ((height + 4) << 3);
        uiDrawUniformRgbRange(coordinates[0], coordinates[1], z, color, channel);
    }
}

void fldLmapDrawListTree(s32 x, s32 y, s32 z, LmapList *list, s32 channel) {
    u32 color;

    fldLmapDrawListTreeData++;
    color = (list->flags & 4) ? 0x80608060 : 0x80606060;
    if (!(list->flags & 0x80)) {
        fldLmapSubmitScaledSpritePacket(list->x + x, list->y + y, z, list->width, list->height, 0x60000000, color,
                                        channel);
        if (list->flags & 4) {
            fldLocalMapDrawScrollIndicators(x, y, z, 0x60806080, list, channel);
        }
    }
    fclCallDrawBoxF(x, y, z, list, channel);
    if (list->onDraw != NULL) {
        list->onDraw(list->x + x, list->y + y, z, list, channel);
    }
    if ((list->flags & 0x20) && fldLmapDrawListTreeData >= 2) {
        list->flags &= ~1;
        list->flags &= ~4;
        fldLmapDrawListTreeWork->flags |= 4;
        fldLmapDrawListTreeWork->flags &= ~1;
        list->flags &= ~0x20;
    } else {
        fldLmapDrawListTreeWork = list;
        if (list->flags & 0x41) {
            if (list->selected->child != NULL) {
                fldLmapDrawListTree(x + ((list->width + 8) << 4), y, z, list->selected->child, channel);
            }
        } else if ((list->flags & 4) && list->onSelect != NULL) {
            list->onSelect(list);
        }
    }
    fldLmapDrawListTreeData--;
}

/* Build one positioned SIF command and submit it on the requested draw surface. */
void fldLmapSubmitPositionedCommandPacket(s32 x, s32 y, s32 z,
                                          ItfGridTextColorTableIndex colorTableIndex,
                                          const char *command, s32 surfaceIndex) {
    sdfDrawTag_t *packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfGrOt_t *drawSurface;
    sdfDevPrintCB_t packetHeader;

    sdfDrawTagInit(packetList);
    sdfDevSetPrintCB(&packetHeader, x + 0x7000, y + 0x7900, z, colorTableIndex);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintfEx(&packetHeader, command));
    drawSurface = &sdfGrOtTable[surfaceIndex];
    drawSurface->regFunc(drawSurface, packetList);
}

/* Build an untextured rectangle with a separate outline color. */
void fldLmapSubmitScaledSpritePacket(s32 x, s32 y, s32 z, s32 width, s32 height, s32 fillColor, s32 borderColor, s32 surfaceIndex) {
    sdfDrawTag_t *packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfGrOt_t *drawSurface;

    sdfDrawTagInit(packetList);
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(x + 0x7000, y + 0x7900, z, width * 16, height * 8, fillColor, borderColor));
    drawSurface = &sdfGrOtTable[surfaceIndex];
    drawSurface->regFunc(drawSurface, packetList);
}

INCLUDE_SDATA(const s32, "code_r1_002CE680", fldLmapDrawListTreeData);

