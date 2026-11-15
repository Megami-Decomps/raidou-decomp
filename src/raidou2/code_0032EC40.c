#include "common.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "kernel_draw.h"
#include "sdf_resource.h"
#include "sdk_libc.h"
#include "dds/dds3_fade.h"
#include "dds/dds3_task_create.h"
#include "dds/dds3_task_lifecycle.h"
#include "dds/dds3_world_secondary.h"
#include "dds/dds3_world_selection.h"
#include "dds/dsp_channel.h"
#include "dds/dsp_entry_api.h"
#include "dds/dsp_message_window_api.h"
#include "dds/evt_message_window_api.h"
#include "dds/evt_world_object_api.h"
#include "dds/dds3_process.h"
#include "dds/mdl_flag_api.h"
#include "dds/sdf_bordered_rectangle_packet.h"
#include "dds/sdf_heap_api.h"
#include "dds/sdf_packet_alloc.h"
#include "dds/sdf_packet_list.h"
#include "dds/sdf_sif_command.h"
#include "dds/snd_track_playback_api.h"

u64 *dds3PrimFrameRect(s32 x, s32 y, s32 depth, s32 width, s32 height, u32 colorA, u32 colorB);
void *sdfDevPrintfEx(SifCommand *packet, const char *format, ...);
void sdfDevSetPrintCB(SifCommand *packet, s32 source, s32 end, s32 argument, s32 index);
void sdfDrawTagInit(struct sdfDrawTag_t *list);
void sdfDrawTagLink(struct sdfDrawTag_t *list, u32 packetAddress);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 size);

#include "dds/sdk_ps2.h"
#include "dds/snd_packed_track_load_status.h"
#include "dds/common.h"
#include "dds/dds3_task_runner.h"
#include "dds/sdf.h"
#include "dds/itf_grid_text.h"
#include "dds/itf_grid_text_color_api.h"
#include "dds/sdf_grid_seek_api.h"
#include "dds/sdf_counter_display.h"
#include "dds/fld_lmap_task.h"
#include "dds/fld_lmap_task_presence.h"

typedef GridTextListItem LmapNode;

typedef GridTextWidget LmapList;

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

void fldLmapSubmitPositionedCommandPacket(s32 x, s32 y, s32 z,
                                          ItfGridTextColorTableIndex colorTableIndex,
                                          const char *command, s32 surfaceIndex);

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

INCLUDE_ASM(const s32, "code_0032EC40", fldLocalMapDrawScrollIndicators);

INCLUDE_ASM(const s32, "code_0032EC40", fldLmapDrawListTree);

/* Build one positioned SIF command and submit it on the requested draw surface. */
void fldLmapSubmitPositionedCommandPacket(s32 x, s32 y, s32 z,
                                          ItfGridTextColorTableIndex colorTableIndex,
                                          const char *command, s32 surfaceIndex) {
    sdfDrawTag_t *packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfGrOt_t *drawSurface;
    SifCommand packetHeader;

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

INCLUDE_SDATA(const s32, "code_0032EC40", fldLmapDrawListTreeData);

