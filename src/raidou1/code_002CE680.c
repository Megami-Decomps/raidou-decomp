#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_002C2620.c */
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

#include "dds/kwln.h"

#include "dds/mdl_flag_api.h"

#include "dds/sdf_bordered_rectangle_packet.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_packet_alloc.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_sif_command.h"

#include "dds/snd_track_playback_api.h"

void dds3FadeIn(s32 red, s32 green, s32 blue, s32 duration);

/* Foreground alpha transitions, stepped by dds3FadeUpdate. */
void dds3FadeOut(s32 red, s32 green, s32 blue, s32 duration);

/* Return the borrowed secondary world node, or NULL when no world is active. */
struct dds3Basic_t *dds3GetControlScene(void);

dds3TaskKillRequestResult_t dds3KillProcessName(const char *name, s32 delayTicks);

u64 *dds3PrimFrameRect(s32 x, s32 y, s32 depth, s32 width, s32 height, u32 colorA, u32 colorB);

/* Return a borrowed scheduler-owned handle from the starting, active, or
 * destroy-pending queue. The handle remains valid through its destroy callback
 * and is freed when finalization completes; return NULL when no name matches. */
dds3ProcessID_t *dds3SearchProcessName(const char *name);

/* Setting either selection does not transfer ownership. */
void dds3SetActiveScene(struct dds3Basic_t *primaryObject);

/*
 * Allocate a task and place it in scheduler queues. Returns NULL on allocation
 * failure. On success, the scheduler owns the task handle and frees it after
 * its destroy callback; callers may borrow the handle only until finalization.
 */
dds3ProcessID_t *dds3StartProcess(const char *name, u32 priority, s32 startDelay,
                         s32 destroyDelay, dds3ProcessFunc_t update,
                         dds3ProcessShutdownFunc_t destroy, u32 userValue);

/* Return whether the selected flag bit is set, as EvtBitTestResult. */
EvtBitTestResult evtBitCheck(s32 flag);

void evtBitOff(s32 flag);

/* Set and clear one flag using the providers' signed-index adjustment. */
void evtBitOn(s32 flag);

/* Create and select the world object for an area/room pair; returns 1 after setup. */
s32 evtLoadArea(s32 area, s32 room);

/* Return the signed control-state byte as a query result, not a boolean or a
 * stored phase. NO_ACTIVE_CONTROL (0) covers a missing window and control state
 * two hidden by the gate; stored zero also passes through. Visible stored states
 * one, two and three report ACTIVE, GATED and CLEANUP. Other stored values pass
 * through unchanged. */
EvtMessageWindowControlResult fclChkMessage(void);

/* Close the active display channel and return its status. */
s32 fclDestroyMessage(void);

s32 fclInitMessage(struct ItfMesSub *definition);

/* Start a display entry; return zero if no window is open, one if it starts. */
s32 fclStartMessage(s32 entry);

/* Request the existing message-window worker's mode-one transition. */
void fclUpdateMessage(void);

void p2sdPlayBGM(s32 trackId);

void *sdfDevPrintfEx(SifCommand *packet, const char *format, ...);

void sdfDevSetPrintCB(SifCommand *packet, s32 source, s32 end, s32 argument, s32 index);

void sdfDrawTagInit(struct sdfDrawTag_t *list);

void sdfDrawTagLink(struct sdfDrawTag_t *list, u32 packetAddress);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 size);

/* Queue the two heap owners separately: the general heap stores descriptors,
 * while the chip heap stores the address of the cell to release. */
void sdfMemFreeDelayedDirect(struct sdfMemHandle_t *allocation);

#include "dds/sdk_ps2.h"

#include "dds/snd_packed_track_load_status.h"

#include "dds/dsp_entry_api.h"

#include "dds/dsp_message_window_api.h"

#include "dds/dsp_channel.h"

#include "dds/evt_message_window_api.h"

#include "dds/common.h"

#include "dds/snd_track_playback_api.h"

#include "dds/dds3_world_selection.h"

#include "dds/evt_world_object_api.h"

#include "dds/sdf_packet_list.h"

#include "dds/kwln.h"

#include "dds/dds3_task_runner.h"

#include "dds/dds3_task_create.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf.h"

#include "dds/itf_grid_text.h"

#include "dds/itf_grid_text_color_api.h"

#include "dds/sdf_grid_seek_api.h"

#include "dds/sdf_sif_command.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/mdl_flag_api.h"

#include "dds/dds3_fade.h"

#include "dds/sdf_counter_display.h"

#include "dds/fld_lmap_task.h"

#include "dds/fld_lmap_task_presence.h"

extern void fldShutdownLmapResources(void);

extern char fldLocalMapTaskName[];

extern LmapCounterSelectionPhase D_003BD25C;

extern u32 D_003BD260;

extern u32 D_003BD970;

extern s32 D_003BD974;

extern void fclSetMessageVar(s32, const char *);

extern char D_003BD250[];

extern void p2sdStopBGM(u32);

extern s32 fldReleaseLocalMapResources(void);

extern s64 pbCheckPreviousRecord(void);

extern void fldReleaseMapRequestQueues(void);

extern void fldReleaseCameraColorEffect(void);

extern void sdfDisableFlagListViewTargetRelativeMode(void);

extern void evtDisableMoonDraw(void);

extern void evtFreeStage(void);

extern sdfMemHandle_t *D_003BD254;

extern s32 fldLocalMapTrackSlotFromMode(s32 mode);

extern dds3TaskUpdateResult_t fldLmapTaskUpdate(dds3ProcessID_t *task);

extern s32 D_003BD23C;

extern s32 D_003BD96C;

extern void fldInitializeLmapState(void);

extern s32 fldStartAndPollLocalMapTrack(void);

extern void fldInitializeLocalMapScene(void);

extern s32 fldUpdateLocalMapController(void);

extern void fldDrawLocalMapOverlay(void);

extern SndPackedTrackLoadStatus p2sdLoadCheckMidi(s32);

extern void p2sdLoadSMGFile(s32);

extern s32 fldLocalMapTrackState;

#include "dds/dds3_world_secondary.h"

extern void fldCreateLocalMapCamera(void);

extern s32 mdlCollectLowFlagBits(void);

extern s32 fldCountMaskBitsBeforeOrdinal(s32 mask, s32 ordinal);

extern s32 sdfCreateMaskedCounterChannels(s32 mask, s32 index);

extern s32 sdfLoadMapCounterCameraVectorsForMask(s32);

extern void sdfInitializeMapCounterSelection(s32 selectedCounterIndex, s32 cameraVectorCount);

extern s32 fldLoadLocalMapResources();

extern void fldCreateMapRequestQueues(void);

extern void fldApplyLightSetIndex(s32);

extern void fldInitializeCameraColorResource(void);

extern void sdfEnableFlagListViewTargetRelativeMode(void);

extern void sdfCounterInitializeDisplayAnimation(void);

extern s32 sdfHandleMapCounterSelectionInput(void);

extern void sdfCounterTickCountdownAndMapTimers(void);

extern void fldUpdateMapRequestQueues(s32);

extern void sdfCounterStepDownAnimationValue(void);

extern void sdfLatchBaseVectorsForSelection(void);

extern s32 sdfStepSelectedMapCameraTransition(void);

extern s32 fldAdvanceLocalMapCameraTransition(void);

extern void sdfCounterAdvanceBoundedAnimationValue(void);

extern s32 pipeMessDraw(void);

extern void fldDisplayLocalMapCounterMessage(void);

extern s32 sdfUpdateSelectedCounterMapCameraTransition(void);

extern s8 evtGetCapturedWindowPanelValue(void);

/* The retail caller passes color bytes as promoted ints; the callee narrows them. */
extern void sdfClearCounterDisplayFlags(void);

extern s32 sdfCounterGetDisplayValue(void);

extern void fldSetPendingSceneAction(u32);

extern void sdfInitInnerVectors(void);

extern void fldUpdateSelectedValueTransition(void);

extern void fldSkyCalcFilter(void);

extern u32 D_003BA8EC;

extern s32 D_003BD240;

extern u32 D_003BD258;

typedef GridTextListItem LmapNode;

typedef GridTextWidget LmapList;

extern LmapNode *fldLmapAdvanceWindowStart(LmapList *);

extern u32 itfGetGridListLinkFlags(LmapList *);

extern void uiDrawUniformRgbRange(s32 *, s32 *, s32, u32, s32);

extern LmapNode *fldLmapExpandWindowBackward(LmapList *);

/* Move the visible start forward without moving the cursor. */
LmapNode *fldLmapAdvanceWindowStart(LmapList *list);

/* Move the visible start backward only when a full forward span is available. */
LmapNode *fldLmapExpandWindowBackward(LmapList *list);

/* Advance the unlocked cursor, wrapping at the range end and following its window. */
LmapNode *fldLmapAdvanceCursor(LmapList *list);

/* Rewind the unlocked cursor, wrapping at the range start and following its window. */
LmapNode *fldLmapRewindCursor(LmapList *list);

/* Jump forward by twice the window capacity minus the cursor's window offset. */
LmapNode *fldLmapAdvanceThroughWindow(LmapList *list);

/* Jump backward by the window capacity plus the cursor's window offset. */
LmapNode *fldLmapRewindThroughWindow(LmapList *list);

void fldLocalMapDrawScrollIndicators(s32 offsetX, s32 offsetY, s32 z, u32 color,
                   LmapList *list, s32 channel);

extern s32 D_003BD238;

extern LmapList *D_003BD968;

extern void fldLmapSubmitScaledSpritePacket(s32 x, s32 y, s32 z, s32 width, s32 height,
                                            s32 fillColor, s32 borderColor, s32 surfaceIndex);

extern void itfDrawGridTextRows(s32, s32, s32, LmapList *, s32);

void fldLmapDrawListTree(s32 x, s32 y, s32 z, LmapList *list, s32 channel);

#include "dds/sdf_packet_alloc.h"

#include "dds/sdf_bordered_rectangle_packet.h"

/* Build one positioned SIF command and submit it on the requested draw surface. */
void fldLmapSubmitPositionedCommandPacket(s32 x, s32 y, s32 z,
                                          ItfGridTextColorTableIndex colorTableIndex,
                                          const char *command, s32 surfaceIndex);

/* Build an untextured rectangle with a separate outline color. */
void fldLmapSubmitScaledSpritePacket(s32 x, s32 y, s32 z, s32 width, s32 height, s32 fillColor, s32 borderColor, s32 surfaceIndex);

dds3TaskUpdateResult_t fldLmapTaskUpdate(dds3ProcessID_t *task);

void fldStartLmapTask(s32 mode);

void fldStopLmapTask(void);

u8 fldLmapTaskExists(void);

void fldLocalMapDebugPrintfNoop(const char *fmt, ...);

/* Field-map mode index -> track slot. Indices 7 and 12 are the only values in
 * range with no arm of their own, so they fall through to the default of 1. */
s32 fldLocalMapTrackSlotFromMode(s32 mode);

void fldInitializeLmapState(void);

void fldShutdownLmapResources(void);

s32 fldStartAndPollLocalMapTrack(void);

extern void evtSetSolarOverlayFullyVisible(void);

void fldInitializeLocalMapScene(void);

s32 fldUpdateLocalMapController(void);

extern void fldSkyDrawFilter(void);

extern void eglSetOt(u32);

extern void evtSubmitPrimaryAlphaBlendMode(s32);

extern void evtSubmitDefaultDepthGradientRect(s32, s32, s32, s32, u32, s32, s32, s32);

extern void sdfAdjustCounterValueAndRedraw(s32);

extern void fldDrawCounterMapMarker(void);

extern void fldDrawLocalMapTransition(s32);

extern s32 fldDrawCameraColorLayerPass(void);

void fldDrawLocalMapOverlay(void);

s32 mdlCollectLowFlagBits(void);

s32 mdlCollectFlagBitsIntoMask(void);

/* Highest set flag wins; the final 0x410 test is still executed even
 * though its result does not affect the returned stage. */
u32 fldGetLmapStage(void);

void fldDisplayLocalMapCounterMessage(void);

s32 pipeMessDraw(void);

void sdfClearCounterDisplayFlags(void);
/* End of declarations from src/dds1/game/code_002C2620.c */

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

INCLUDE_ASM(const s32, "code_002CE680", fldLocalMapDrawScrollIndicators);

INCLUDE_ASM(const s32, "code_002CE680", fldLmapDrawListTree);

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

INCLUDE_SDATA(const s32, "code_002CE680", D_r1_0046E7D8);

