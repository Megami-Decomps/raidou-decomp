#include "common.h"
#include "sdfDraw.h"
#include "sdf2D.h"
#include "dds3GlobalWork.h"
#include "dds3AdminiProcess.h"
#include "dds3KernelCore.h"
#include "sdfPrim.h"
#include "sdf_Decomp.h"
#include "sdfModel.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "fileManager.h"
#include "model.h"
#include "sdfDevCons.h"
#include "sdk_libc.h"
#include "dds/dds3_task_lifecycle.h"
#include "dds/mdl_motion_api.h"
#include "dds/sdf_chunk.h"
#include "dds/sdf_sif_command.h"
#include "dds/sdf_texture_draw_packet.h"

/* Set bit 1 in the active administration task's flags; that task must exist. */
struct sdfDrawTag_t *dds3FontReqAllocDrawTag(void);

/* Return a borrowed scheduler-owned handle from the starting, active, or
 * destroy-pending queue. The handle remains valid through its destroy callback
 * and is freed when finalization completes; return NULL when no name matches. */

/* Model-context lifetime, per-frame update, and broadcast accessors. */
/* Destroy one context. If it leaves its resource-owning group empty, group
 * teardown also releases that group's registered assets. */
void mdlFree(mdlObj_t *ctx);

/* Context-level motion APIs; per-sdfMotionCtrlr_t lifecycle methods are in sdf_motion.h. */
MdlMotionIndexResult mdlGetMotionNum(const mdlObj_t *ctx, MdlMotionSearchId searchId);

/* The referenced halfword is promoted to a word-sized SDK result. */
s32 mdlGetNumOfCut(const mdlObj_t *ctx, s32 searchId);
MdlMotionPresence mdlHasMotionEx(const mdlObj_t *ctx, MdlMotionSearchId searchId);

/* Allocate a distinct caller-owned context linked to the registered group.
 * Contexts share the group node and assets; release each context with mdlFree. */
mdlObj_t *mdlLoad(u32 group, u32 id);
void mdlLoopMotion(mdlObj_t *ctx, s32 searchId, s32 motionIndex);

void mdlSmoothLoopMotion(mdlObj_t *ctx, s32 searchId, s32 motionIndex, f32 blendLeadFrames,
                          f32 blendDurationFrames);

void mdlSmoothSingleMotion(mdlObj_t *ctx, s32 searchId, s32 motionIndex, f32 blendLeadFrames,
                       f32 blendDurationFrames);

void *sdfDevPrintfEx(SifCommand *packet, const char *format, ...);
void sdfDevSetPrintCB(SifCommand *packet, s32 source, s32 end, s32 argument, s32 index);





#include "dds/mdl_part_entry.h"
#include "dds/mdl_resource_entries.h"
#include "dds/mdl_mark_params.h"
#include "dds/pcp_vu0.h"
#include "dds/ee_mmi.h"

#define MDL_VIEWER_RESOURCE_SLOTS 12

#define MDL_RECORD_LIST_HEADER_BYTES 8

#define MDL_RECORD_END_KIND 0xFFFF

#define MDL_MARK_COLOR_COUNT 4

#define MDL_COLOR_CHANNEL_COUNT 4

#define MDL_COLOR_BYTE_MASK 0xFF

#define MDL_COLOR_BYTE_BITS 8

#define MDL_MARK_LAST_FIELD 0x16

#define MDL_MARK_COLOR_FIRST_FIELD 7

#define MDL_PAD_REPEAT_FLAG 2

#define MDL_PART_SLOT_BYTES 0x10

#define MDL_PART_VALUE_SIZE_THRESHOLD 0x11

#define MDL_MAP_POSITION_TAG 0x534F504D

typedef enum MdlViewerRecordKind {
    MDL_VIEWER_RECORD_KIND_BILLBOARD_PART = 1,
    MDL_VIEWER_RECORD_KIND_EFFECT_PART = 2,
    MDL_VIEWER_RECORD_KIND_TRACK_POLY = 3,
    MDL_VIEWER_RECORD_KIND_STREAM = 4,
    MDL_VIEWER_RECORD_KIND_OBJECT_PART = 5
} MdlViewerRecordKind;

/* Viewer-wide state for the model viewer task (DDS1 game/code_00218B48 and
 * DDS2 game/code_00233660 share this layout field for field). Fields that are
 * only written by a defaults initialiser and never read in either game are
 * left as unkNN on purpose: there is no read to earn a role from. */
typedef struct MdlViewState {
    dds3ProcessID_t *unk00;
    dds3ProcessID_t *viewerTask;
    s8 exitCleanupStarted; /* 0x08: one-shot guard for viewer task/resource teardown. */
    s8 selectionHighlightPhase;
    s8 taskPhase; /* 0x0A: one-based index into D_00367A40 */
    s8 unk0B;
    s8 unitStepMode;  /* 0x0C: toggled by the step button (D_003D24510[0x23]) */
    s8 unitStepSign;  /* 0x0D: +1/-1, derived from the input keys at 0x24/0x25 */
    s8 unk0E;
    s8 assetDisplayMode; /* 0x0F: toggles asset labels and cursor paging. */
    s8 yawStepMode; /* 0x10: one yaw step per left/right button press */
    u8 pad11[3];
    s16 selectedTaskIndex;
    s16 resourceCount;
    s16 resourceGroup; /* 0x18: group passed to the resource loader */
    s16 resourceId;    /* 0x1A: ID passed to the resource loader */
    s16 selectedResourceGroup; /* 0x1C: resource group under the selection cursor. */
    s16 selectedResourceId; /* 0x1E: resource ID under the selection cursor. */
    s16 selectionCursor; /* 0x20: selects one of five controls in the asset panel. */
    s16 activeEntryId;    /* 0x22: assigned from selectedEntryId when adding */
    s16 selectedEntryId;  /* 0x24: key passed to the mdlAddEntry* family;
                           signed in DDS1, unsigned in DDS2. */
    s16 selectedNodeId;   /* 0x26: checked by mdlHasMotionEx before adding */
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 entryHeight;      /* 0x2E: low bound offered to mdlAddEntry* */
    s16 entryWidth;       /* 0x30: high bound, limited to entryHeight when drawing */
    u8 pad32[2];
    s16 labelIndexA;      /* 0x34: indexes mdlDrawViewerIndexedLabelOverlayData */
    s16 labelIndexB;      /* 0x36: indexes mdlDrawViewerSelectionLabelData */
    s16 scrollPage;  /* 0x38: page stepped by mdlUpdateViewerCursorWithPageStep */
    s16 nodeCursor;  /* 0x3A: selection within the loaded node count */
    s16 settingsCursor; /* 0x3C: selected row among the ten viewer settings. */
    s16 settingsPage;
    s16 editorMode; /* 0x40: 0 selects records, 1 edits a mark record */
    s16 selectedRecordIndex; /* 0x42: index across the shared and active-entry record lists */
    s16 effectListScroll; /* 0x44: first visible effect row */
    s16 markFieldCursor; /* 0x46: selected row in the mark parameter editor */
    s16 workspaceSetupPhase; /* 0x48: capture workspace setup advances through phases 0, 1, 2. */
    s16 captureScale; /* 0x4A: model capture scale setting */
    s16 captureDivX;  /* 0x4C: model capture horizontal division setting */
    s16 captureDivY;  /* 0x4E: model capture vertical division setting */
    s16 unk50;
    s16 unk52;
    f32 viewerScale;      /* 0x54: viewer zoom, in hundredths */
    u8 pad58[8];
    u128 unk60[3]; /* 0x60: quadword slots; their alignment makes the state quadword aligned */
    mdlObj_t *resources[MDL_VIEWER_RESOURCE_SLOTS]; /* 0x90 */
    sdfDrawTag_t *packetList; /* 0xC0: drawing packet destination */
} MdlViewState;

typedef struct MdlPadState {
    u8 pad00;
    s8 confirm;
    u8 pad02;
    s8 cancel;
    s8 stepDownA;  /* 0x04 */
    s8 stepUpA;    /* 0x05 */
    s8 stepDownB;  /* 0x06 */
    s8 stepUpB;    /* 0x07 */
} MdlPadState;

typedef struct MdlCtrlState {
    MdlPadState *pad;
    u8 unk04;
    u8 blinkTick;
    s16 selection;
    sdfDrawTag_t *packetList;
} MdlCtrlState;

extern MdlViewState mdlViewerState;

static inline s8 mdlGetViewerDisplayMode(MdlViewState *state) {
    return state->assetDisplayMode;
}

extern s8 mdlRunViewerSelectionLabelTaskWork[];

typedef struct MdlViewerTaskDef {
    const char *name;
    dds3ProcessFunc_t update;
    s32 data;
} MdlViewerTaskDef;

typedef struct MdlSystemTask {
    const char *name;
    s32 arg;
} MdlSystemTask;

extern char mdlStrModel03[];
extern char D_003ABCB0[];
extern char D_003ABCC8[];
extern char D_003ABCD8[];
extern char D_003BBC28[];
s32 mdlUpdateViewerCursor(s16 *, s32);
void mdlUpdateViewerSelectedModelFromPad(void);
void mdlDrawViewerSelectionLabel(void);
void mdlCleanupViewerTasksAndResources(void);
s32 mdlPibGetChunkItemCount(MdlRecord *listHeader);

/* Follow the relative links in a resource's part-info table to find a list key. */
MdlRecord *mdlPibGetChunk(mdlObj_t *resource, MdlPartInfoListKey partInfoListKey);

/* The first eight bytes belong to the enclosing list, not its first record. */
MdlRecord *mdlPibGetFirstChunkItem(MdlRecord *listHeader);

/* A terminal kind returns NULL; the link is a byte offset, not a pointer. */
MdlRecord *mdlPibGetNextChunkItem(MdlRecord *currentRecord);
s32 mdlPibCheckItemType(MdlRecord *record, s32 kind);
extern char D_003ABA80[], D_003ABA90[], D_003ABAA0[], D_003ABAB0[], D_003ABAC0[], D_003ABAD0[];
extern char D_003BBB80[], D_003BBB88[];
void mdlPibDrawEffectInfo(sdfDrawTag_t *packetList, s32 x, s32 y, s32 depth, s32 textStyle, MdlRecord *record);

/* Draw the scalar marker fields, then four colors with four byte channels each; negative selection hides the cursor. */
void mdlPibDrawTrackInfo(sdfDrawTag_t *packetList, s32 x, s32 y, s32 depth, EffMarkParams *params, s32 selectedField);

#define MDL_PART_BILLBOARD 0

#define MDL_PART_EFFECT 1

#define MDL_PART_OBJECT 3

u64 *mdlBuildViewerRectanglePacket(s32 x, s32 y, s32 depth, s32 width, s32 height);
void mdlAppendViewerRectToDrawList(s32 x, s32 y, s32 depth, s32 width, s32 height, s32 unused);
extern s8 dds3PadBitTable[];
s32 mdlUpdateViewerCursor(s16 *cursor, s32 entryCount);
s32 mdlUpdateViewerCursorWithPageStep(s16 *cursor, s32 entryCount, s32 pageStep);
void mdlLoadViewerResourceAndResetCursors(void);

typedef struct {
    f32 x, y, z, w;
} __attribute__((aligned(16))) MdlEyeVec;

extern MdlEyeVec mapCallFromRotationBWork;
extern MdlEyeVec mapCallFromRotationBWork2;
extern u128 mapCallFromRotationBWork3;
extern MdlEyeVec mdlData10;
extern MdlEyeVec mdlData11;
extern u128 mdlResetViewerBasisVectorsData;
extern s16 mdlResetViewerBasisVectorsWork[];
void mdlResetViewerBasisVectors(void);
void mdlRotateViewResourcesRight(void);
void mdlRotateViewList(void);
extern const char *mdlData9[];
extern char D_003BBC20[];
void mdlDrawViewerModeLabel(void);
void mdlDrawViewerOptionLabels(void);
void mdlDrawViewerModelAndMotionSummary(void);
void mdlAddViewEntryFlagged(void);
void mdlAddPlainViewerEntryForSelectedNode(void);
extern void mdlFreeViewResources(void);
void mdlHandleViewerAssetSelection(void);

/* Draw the pending resource details and the current selection in each model node. */
void mdlDrawViewerAssetSelection(void);
u32 mdlRunViewerAssetSelectionTask(void);

/* vu0 routine: move the viewer camera or orbit it about the look-at point. */
void mdlUpdateViewerCameraFromPad(void);
extern const char *mdlDrawViewerIndexedLabelOverlayData[];
void mdlDrawViewerIndexedLabelOverlay(void);
u32 mdlRunViewerIndexedLabelTask(void);
extern const char *mdlDrawViewerSelectionLabelData[];
s32 mdlRunViewerSelectionLabelTask(void);

/* Change the viewer scale in hundredths, with larger steps at larger values. */
void mdlAdjustViewerScale(void);

/* Draw the motion progress bar: timeline frame, playhead marker and "[time/length]" label, then the zoom value. */
void mdlDrawViewerMotionTimeline(void);
u32 mdlUpdateViewerScaleTask(void);
extern s8 mdlData14[];
extern u8 mdlUpdateViewerWorkspaceSetupWork;
extern s8 sdf3DScreenAdjust;
extern f32 sdf3DScreenAdjustScaleX;
extern f32 sdf3DScreenAdjustScaleY;
extern f32 sdf3DScreenAdjustOffsetX;
extern f32 sdf3DScreenAdjustOffsetY;
void mdlUpdateViewerWorkspaceSetup(void);
u32 mdlUpdateViewerWorkspaceSetupTask(void);
void mdlApplyViewerResourceMenuAction(void);
extern const char *mdlDrawViewerResourceMenuData[];
void mdlDrawViewerResourceMenu(void);
u32 mdlRunViewerResourceMenuTask(void);

/* Loaded-resource count chain, identical to the DDS2 model viewer. */
void mdlHandleViewerNodeCursorInput(void);
extern char D_003ABDA8[];
extern char D_003BBC88[];
void mdlDrawViewerTexturePreview(void);
u32 mdlUpdateViewerNodeCursorTask(void);
s32 mdlIsDebugTimeGraph(void);

/* Debug menu: page 0 selects an action, pages 1-4 edit the color channels and value steps. */
void mdlUpdateViewerSettingsInput(void);
void mdlDrawViewerSettings(void);
u32 mdlRunViewerSettingsTask(void);
s32 mdlCountActiveRecords(void);
MdlRecord *mdlGetSelectedRecord(void);
extern MdlRecord *mdlGetSelectedRecord(void);
void mdlUpdateViewerMarkEditorInput(void);
extern const char D_003ABEB0[];
extern const char D_003BBCB0[];
void mdlDrawViewerEffectRecordList(void);
u32 mdlRunViewerEffectEditorTask(void);
s32 mdlRequestViewerExitOnce(void);
extern sdfGrOt_t mdlData5;
extern f32 mdlSubmitViewerIntermediateDrawPacketData[][4];
extern u32 mdlSubmitViewerIntermediateDrawPacketData2[];
void mdlSubmitViewerIntermediateDrawPacket(void);
extern f32 mdlSubmitViewerResourceDrawPacketData[][4];
extern u32 mdlSubmitViewerResourceDrawPacketData2[];
extern s32 D_003BBCB8;
extern void sdfDumpHexLine(s32 packet, s32 arg);
void mdlSubmitViewerResourceDrawPacket(void);
void mdlViewerTaskDestroy(void);
extern f32 mdlDrawMapPositionRecordsData[][4];
extern u32 mdlDrawMapPositionRecordsData2[];
void mdlDrawMapPositionRecords(mdlObj_t *resource);
void mdlFreeViewResources(void);

typedef struct MdlValueEdit {
    s32 y;
    f32 *target;   /* 0x04 */
    s32 min;       /* 0x08 */
    s32 max;       /* 0x0C */
    const char *label;  /* 0x10 */
    const char *format; /* 0x14 */
} MdlValueEdit;

u64 *mdlBuildViewerRectanglePacket(s32 x, s32 y, s32 depth, s32 width, s32 height) {
    return dds3PrimFrameRect(x, y, depth, width, height, 0x30000000, 0x60404040);
}

void mdlAppendViewerRectToDrawList(s32 x, s32 y, s32 depth, s32 width, s32 height, s32 unused) {
    sdfDrawTag_t *packet;

    packet = mdlViewerState.packetList;
    sdfDrawTagLink(packet, (u32)(mdlBuildViewerRectanglePacket(x, y, depth, width, height)));
}

/* Return 1 after a cursor update: fresh presses wrap, while repeated presses stop at the ends. */
s32 mdlUpdateViewerCursor(s16 *cursor, s32 entryCount) {
    s32 lastIndex = entryCount - 1;
    s32 selectedIndex = *cursor;
    s32 changed = 0;
    if (dds3PadBitTable[5] < 0) {
        if (selectedIndex < lastIndex) {
            selectedIndex += 1;
            changed = 1;
        } else {
            selectedIndex = 0;
            changed = 1;
        }
    } else if (((u8)dds3PadBitTable[5] & MDL_PAD_REPEAT_FLAG) != 0) {
        if (selectedIndex < lastIndex) {
            selectedIndex += 1;
            changed = 1;
        }
    } else if (dds3PadBitTable[4] < 0) {
        if (selectedIndex > 0) {
            selectedIndex -= 1;
            changed = 1;
        } else {
            selectedIndex = lastIndex;
            changed = 1;
        }
    } else if ((((u8)dds3PadBitTable[4] & MDL_PAD_REPEAT_FLAG) != 0) && (selectedIndex > 0)) {
        selectedIndex -= 1;
        changed = 1;
    }
    if (changed != 0) {
        *cursor = selectedIndex;
        mdlViewerState.selectionHighlightPhase = 0;
        return 1;
    }
    return 0;
}

/* Apply single-entry or optional page steps using the native wrap/clamp and input-precedence rules. */
s32 mdlUpdateViewerCursorWithPageStep(s16 *cursor, s32 entryCount, s32 pageStep) {
    s32 lastIndex = entryCount - 1;
    s32 selectedIndex = *cursor;
    s32 changed = 0;
    if (dds3PadBitTable[7] < 0) {
        if (selectedIndex < lastIndex) {
            selectedIndex += 1;
        } else {
            selectedIndex = 0;
        }
        changed = 1;
    } else if (((u8)dds3PadBitTable[7] & MDL_PAD_REPEAT_FLAG) != 0) {
        if (selectedIndex < lastIndex) {
            selectedIndex += 1;
            changed = 1;
        }
    } else if (dds3PadBitTable[6] < 0) {
        if (selectedIndex > 0) {
            selectedIndex -= 1;
        } else {
            selectedIndex = lastIndex;
        }
        changed = 1;
    } else if (((u8)dds3PadBitTable[6] & MDL_PAD_REPEAT_FLAG) != 0) {
        if (selectedIndex > 0) {
            selectedIndex -= 1;
            changed = 1;
        }
    } else if (pageStep != 0) {
        if (dds3PadBitTable[11] < 0) {
            if (selectedIndex < lastIndex) {
                selectedIndex += pageStep;
                if (selectedIndex > lastIndex) {
                    selectedIndex = lastIndex;
                }
            } else {
                selectedIndex = 0;
            }
            changed = 1;
        } else if (((u8)dds3PadBitTable[11] & MDL_PAD_REPEAT_FLAG) != 0) {
            if (selectedIndex < lastIndex) {
                selectedIndex += pageStep;
                if (selectedIndex > lastIndex) {
                    selectedIndex = lastIndex;
                }
                changed = 1;
            }
        } else if (dds3PadBitTable[10] < 0) {
            if (selectedIndex > 0) {
                selectedIndex -= pageStep;
                if (selectedIndex < 0) {
                    selectedIndex = 0;
                }
            } else {
                selectedIndex = lastIndex;
            }
            changed = 1;
        } else if ((((u8)dds3PadBitTable[10] & MDL_PAD_REPEAT_FLAG) != 0) && (selectedIndex > 0)) {
            selectedIndex -= pageStep;
            if (selectedIndex < 0) {
                selectedIndex = 0;
            }
            changed = 1;
        }
    }
    if (changed != 0) {
        *cursor = selectedIndex;
        mdlViewerState.selectionHighlightPhase = 0;
        return 1;
    }
    return 0;
}

void mdlLoadViewerResourceAndResetCursors(void) {
    mdlObj_t *resource = mdlLoad(mdlViewerState.resourceGroup, mdlViewerState.resourceId);

    mdlViewerState.resources[0] = resource;
    mdlViewerState.activeEntryId = 0;
    mdlViewerState.selectedEntryId = 0;
    mdlViewerState.selectedNodeId = 0;
    mdlViewerState.unk28 = 0;
    mdlViewerState.unk2A = 0;
    mdlViewerState.unk2C = 0;
    if (resource->first != NULL) {
        mdlLoopMotion(resource, 0, 0);
    }
    mdlViewerState.nodeCursor = 0;
}

void mdlResetViewerBasisVectors(void) {
    SDF_VU0_COPYVECTOR(&mapCallFromRotationBWork, &mdlData10);
    SDF_VU0_COPYVECTOR(&mapCallFromRotationBWork2, &mdlData11);
    SDF_VU0_COPYVECTOR(&mapCallFromRotationBWork3, &mdlResetViewerBasisVectorsData);
    mdlResetViewerBasisVectorsWork[0] = 0;
}

/* Rotate the viewer resource list in place without moving its allocation. */
void mdlRotateViewResourcesRight(void) {
    s32 resourceIndex = mdlViewerState.resourceCount - 1;
    mdlObj_t *lastResource = mdlViewerState.resources[resourceIndex];

    if (resourceIndex > 0) {
        do {
            mdlViewerState.resources[resourceIndex] = mdlViewerState.resources[resourceIndex - 1];
            resourceIndex -= 1;
        } while (resourceIndex > 0);
    }
    mdlViewerState.resources[0] = lastResource;
}

/* Move the first resource to the end of the active list without changing its allocation. */
void mdlRotateViewList(void) {
    s32 resourceIndex;
    s32 resourceCount = mdlViewerState.resourceCount;
    mdlObj_t *firstResource = mdlViewerState.resources[0];

    for (resourceIndex = 0; resourceIndex < resourceCount - 1; resourceIndex++) {
        mdlViewerState.resources[resourceIndex] = mdlViewerState.resources[resourceIndex + 1];
    }
    mdlViewerState.resources[resourceIndex] = firstResource;
}

/* Draw the MODE header and current or saved viewer mode label. */
void mdlDrawViewerModeLabel(void) {
    SifCommand packet;
    s32 phase;

    mdlAppendViewerRectToDrawList(0x7150, 0x7948, 0xFF0080, 0xBA0, 0x90, 0);
    sdfDevSetPrintCB(&packet, 0x7180, 0x7960, 0xFF0080, 0);
    sdfDrawTagLink(mdlViewerState.packetList, (u32)(sdfDevPrintfEx(&packet, D_003BBC20)));
    phase = mdlViewerState.taskPhase;
    if (phase == 0) {
        phase = mdlViewerState.unk0B - 1;
    } else {
        phase = phase - 1;
    }
    sdfDrawTagLink(mdlViewerState.packetList, (u32)(sdfDevPrintfEx(&packet, mdlData9[phase])));
}

/* Draw the phase-zero option list in two columns, with cursor/blink styling. */
void mdlDrawViewerOptionLabels(void) {
    const char **label;
    u32 i;

    if (mdlViewerState.taskPhase != 0) {
        return;
    }

    mdlAppendViewerRectToDrawList(0x7690, 0x7A08, 0xFF0080, 0xC60, 0x2D0, 0);
    label = mdlData9;
    i = 0;
    do {
        s32 style = (i == mdlViewerState.selectedTaskIndex && mdlViewerState.selectionHighlightPhase < 0x14) ? 4 : 0;
        s32 xOffset;
        u32 row;
        u32 packet;

        if (i >= 7) {
            xOffset = 0x600;
            row = i - 7;
        } else {
            xOffset = 0;
            row = i;
        }
        packet = (u32)sdfDevPrintf(0x76C0 + xOffset,
                                                   0x7A20 + row * 0x60,
                                                   0xFF0080, style, *label);
        i++;
        sdfDrawTagLink(mdlViewerState.packetList, packet);
        label++;
    } while (i != 13);
}

void mdlDrawViewerModelAndMotionSummary(void) {
    SifCommand packet;
    const char *format;
    s32 displayMode;
    s32 nodeCount;
    void *formatted;

    mdlAppendViewerRectToDrawList(0x7E10, 0x8608, 0xFF0080, 0xEA0, 0x90, 0);
    sdfDevSetPrintCB(&packet, 0x7E40, 0x8620, 0xFF0080, 0);

    if ((displayMode = mdlGetViewerDisplayMode(&mdlViewerState)) == 0) {
        format = mdlStrModel03;
    } else {
        format = D_003ABCB0;
    }
    sdfDrawTagLink(mdlViewerState.packetList, (u32)(sdfDevPrintfEx(&packet, format, mdlViewerState.resourceGroup, mdlViewerState.resourceId)));

    nodeCount = mdlGetNumOfCut(mdlViewerState.resources[0], 0);
    if (nodeCount == 0) {
        formatted = sdfDevPrintfEx(&packet, D_003BBC28);
    } else {
        if (mdlViewerState.assetDisplayMode == 0) {
            format = D_003ABCC8;
        } else {
            format = D_003ABCD8;
        }
        formatted = sdfDevPrintfEx(&packet, format,
                                       mdlGetMotionNum(mdlViewerState.resources[0], 0), nodeCount - 1);
    }
    sdfDrawTagLink(mdlViewerState.packetList, (u32)(formatted));
}

void mdlAddViewEntryFlagged(void) {
    f32 low;
    f32 high;
    f32 first;

    if (mdlHasMotionEx(mdlViewerState.resources[0], mdlViewerState.selectedNodeId) != MDL_MOTION_ABSENT) {
        low = mdlViewerState.entryHeight;
        high = mdlViewerState.entryWidth;
        first = high;
        if (low < high) {
            first = low;
        }
        mdlViewerState.activeEntryId = mdlViewerState.selectedEntryId;
        mdlSmoothLoopMotion(mdlViewerState.resources[0], mdlViewerState.selectedNodeId, mdlViewerState.selectedEntryId, first, low);
    }
}

void mdlAddPlainViewerEntryForSelectedNode(void) {
    f32 low;
    f32 high;
    f32 first;

    if (mdlHasMotionEx(mdlViewerState.resources[0], mdlViewerState.selectedNodeId) != MDL_MOTION_ABSENT) {
        low = mdlViewerState.entryHeight;
        high = mdlViewerState.entryWidth;
        first = high;
        if (low < high) {
            first = low;
        }
        mdlViewerState.activeEntryId = mdlViewerState.selectedEntryId;
        mdlSmoothSingleMotion(mdlViewerState.resources[0], mdlViewerState.selectedNodeId, mdlViewerState.selectedEntryId, first, low);
    }
}

INCLUDE_RODATA(const s32, "code_001B2E60", mdlStrModel032);

INCLUDE_RODATA(const s32, "code_001B2E60", mdlStr0202);

INCLUDE_RODATA(const s32, "code_001B2E60", mdlStr02022);

INCLUDE_ASM(const s32, "code_001B2E60", mdlHandleViewerAssetSelection);

INCLUDE_ASM(const s32, "code_001B2E60", mdlDrawViewerAssetSelection);

u32 mdlRunViewerAssetSelectionTask(void) {
    mdlHandleViewerAssetSelection();
    mdlDrawViewerAssetSelection();
    return 0;
}

INCLUDE_ASM(const s32, "code_001B2E60", mapCallFromRotationB);

void mdlDrawViewerIndexedLabelOverlay(void) {
    mdlAppendViewerRectToDrawList(0x8A10, 0x7948, 0xFF007F, 0x4E0, 0x90, 0);
    sdfDrawTagLink(mdlViewerState.packetList, (u32)(sdfDevPrintf(0x8A40, 0x7960, 0xFF0080, 0, mdlDrawViewerIndexedLabelOverlayData[mdlViewerState.labelIndexA])));
}

u32 mdlRunViewerIndexedLabelTask(void) {
    mdlUpdateViewerCameraFromPad();
    mdlDrawViewerIndexedLabelOverlay();
    return 0;
}

INCLUDE_ASM(const s32, "code_001B2E60", mdlUpdateViewerSelectedModelFromPad);

void mdlDrawViewerSelectionLabel(void) {
    mdlAppendViewerRectToDrawList(0x8A10, 0x7948, 0xFF007F, 0x4E0, 0x90, 0);
    sdfDrawTagLink(mdlViewerState.packetList, (u32)(sdfDevPrintf(0x8A40, 0x7960, 0xFF0080, 0, mdlDrawViewerSelectionLabelData[mdlViewerState.labelIndexB])));
}

s32 mdlRunViewerSelectionLabelTask(void) {
    mdlUpdateViewerSelectedModelFromPad();
    if (mdlRunViewerSelectionLabelTaskWork[0] == 0) {
        mdlDrawViewerSelectionLabel();
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_001B2E60", mdlAdjustViewerScale);

INCLUDE_ASM(const s32, "code_001B2E60", mdlDrawViewerMotionTimeline);

u32 mdlUpdateViewerScaleTask(void) {
    mdlAdjustViewerScale();
    mdlDrawViewerMotionTimeline();
    return 0;
}

/* Final workspace publication: see docs/idioms.md, "Publishing viewer workspace". */
void mdlUpdateViewerWorkspaceSetup(void) {
    s32 initialPhase = 1;

    switch (mdlViewerState.workspaceSetupPhase) {
    case 0:
        if (mdlData14[0] < 0) {
            f32 scale = mdlViewerState.captureScale;

            mdlViewerState.yawStepMode = initialPhase;
            mdlViewerState.unk50 = 0;
            mdlViewerState.unk52 = 0;
            mdlViewerState.taskPhase = -1;
            mdlUpdateViewerWorkspaceSetupWork = mdlViewerState.unitStepMode;
            mdlViewerState.unitStepMode = initialPhase;
            sdf3DScreenAdjust = initialPhase;
            sdf3DScreenAdjustScaleY = sdf3DScreenAdjustScaleX = scale;
            mdlViewerState.workspaceSetupPhase = initialPhase;
        }
        break;
    case 1:
        sdf3DScreenAdjustOffsetX = mdlViewerState.captureDivX * 0x100 -
                   mdlViewerState.unk50 * 0x200 - 0x100;
        sdf3DScreenAdjustOffsetY = mdlViewerState.captureDivY * 0x70 -
                   mdlViewerState.unk52 * 0xE0 - 0x70;
        mdlViewerState.workspaceSetupPhase = 2;
        break;
    case 2:
        /* 0x0021C218..0x0021C228: completed phase retained in retail dispatch. */
        break;
    }
}

void mdlAfterViewerWorkspaceUpdate(void) {
}

/* Run workspace setup and its separate hook; keep the task result zero. */
u32 mdlUpdateViewerWorkspaceSetupTask(void) {
    mdlUpdateViewerWorkspaceSetup();
    mdlAfterViewerWorkspaceUpdate();
    return 0;
}

INCLUDE_ASM(const s32, "code_001B2E60", mdlApplyViewerResourceMenuAction);

/* Draw resource actions with independent eligibility and selection styles. */
void mdlDrawViewerResourceMenu(void) {
    SifCommand packet;
    s32 row;
    s32 enabled;
    s32 style;

    mdlAppendViewerRectToDrawList(0x8590, 0x7948, 0xFF007F, 0x960, 0x210, 0);
    for (row = 0; row != 5; row++) {
        enabled = 0;
        switch (row) {
        case 0:
            if (mdlViewerState.resourceCount < MDL_VIEWER_RESOURCE_SLOTS) {
                enabled = 1;
            }
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            if (mdlViewerState.resourceCount >= 2) {
                enabled = 1;
            }
            break;
        }
        if (mdlViewerState.selectionHighlightPhase < 20 && mdlViewerState.scrollPage == row) {
            style = enabled ? 4 : 12;
        } else {
            style = enabled ? 0 : 8;
        }
        sdfDevSetPrintCB(&packet, 0x85C0, 0x7960 + row * 0x60, 0xFF0080, style);
        sdfDrawTagLink(mdlViewerState.packetList,
            (u32)sdfDevPrintfEx(&packet, mdlDrawViewerResourceMenuData[row]));
    }
}

u32 mdlRunViewerResourceMenuTask(void) {
    mdlApplyViewerResourceMenuAction();
    mdlDrawViewerResourceMenu();
    return 0;
}

void mdlHandleViewerNodeCursorInput(void) {
    sdfModelTexList_t *firstNode = mdlViewerState.resources[0]->inner->texList;

    if (firstNode != 0) {
        s16 nodeCount = firstNode->count;

        if (nodeCount > 0) {
            mdlUpdateViewerCursor(&mdlViewerState.nodeCursor, nodeCount);
        }
    }
}

void mdlDrawViewerTexturePreview(void) {
    s32 index = 0;
    s32 count = 0;
    s32 width, height;
    sdfModelTexList_t *node;
    sdfDrawTag_t *packetList;
    s32 displayWidth, displayHeight;
    s32 selectedNumber = 0;

    mdlAppendViewerRectToDrawList(0x7150, 0x7A08, 0xFF007F, 0x1060, 0x8F0, 0);
    node = mdlViewerState.resources[0]->inner->texList;
    if (node != NULL) {
        count = node->count;
        index = mdlViewerState.nodeCursor;
        selectedNumber = index + (count > 0);
    }
    packetList = mdlViewerState.packetList;
    sdfDrawTagLink(packetList, (u32)(sdfDevPrintf(0x7180, 0x7A20,
        0xFF0080, 0, D_003ABDA8, selectedNumber, count)));
    if (count > 0) {
        width = node->items[index]->width;
        height = node->items[index]->height;
        sdfDrawTagLink(packetList, (u32)(sdfDevPrintf(0x7780, 0x7A20,
            0xFF0080, 0, D_003BBC88, width, height)));
        sdf2DLinkTexPacket(packetList, node->items[index], SDF_GS_CONTEXT_1);
        displayWidth = width << 4;
        displayHeight = height << 3;
        if (width < height) {
            if (height > 256) {
                displayWidth = ((width << 8) / height) << 4;
                displayHeight = 0x800;
            }
        } else {
            if (width > 256) {
                displayWidth = 0x1000;
                displayHeight = ((height << 8) / width) << 3;
            }
        }
        sdfPrimLinkSprite(packetList, 0x80808080, 0, 0x7180, 0x7AE0,
            0, 0, displayWidth + 0x7180, displayHeight + 0x7AE0,
            width << 4, height << 4, 0xFF0080, 0);
    }
}

u32 mdlUpdateViewerNodeCursorTask(void) {
    mdlHandleViewerNodeCursorInput();
    mdlDrawViewerTexturePreview();
    return 0;
}

INCLUDE_RODATA(const s32, "code_001B2E60", mdlStrSyncMotion);

INCLUDE_RODATA(const s32, "code_001B2E60", mdlStrSelectPrev);

INCLUDE_RODATA(const s32, "code_001B2E60", mdlStrSelectNext);

INCLUDE_RODATA(const s32, "code_001B2E60", mdlStrDeleteModel);

INCLUDE_RODATA(const s32, "code_001B2E60", mdlStrNewModel);

INCLUDE_RODATA(const s32, "code_001B2E60", mdlStr22);

s32 mdlIsDebugTimeGraph(void) {
    return dds3SearchProcessName("DebugTimeGrph") != 0;
}

INCLUDE_ASM(const s32, "code_001B2E60", mapModelViewerTmx);

INCLUDE_RODATA(const s32, "code_001B2E60", mdlStrBgViewerTmx);

INCLUDE_RODATA(const s32, "code_001B2E60", mdlStrModelHexNum);

INCLUDE_RODATA(const s32, "code_001B2E60", mdlStrRunTimeGraph);

INCLUDE_RODATA(const s32, "code_001B2E60", mdlStrDisplayMark);

INCLUDE_RODATA(const s32, "code_001B2E60", mdlStrCaptureDivY);

INCLUDE_RODATA(const s32, "code_001B2E60", mdlStrCaptureDivX);

INCLUDE_RODATA(const s32, "code_001B2E60", mdlStrCaptureScale);

INCLUDE_ASM(const s32, "code_001B2E60", mdlDrawViewerSettings);

u32 mdlRunViewerSettingsTask(void) {
    mdlUpdateViewerSettingsInput();
    mdlDrawViewerSettings();
    return 0;
}

/* Sum context-wide and selected-entry list counts, retaining both lookups. */
s32 mdlCountActiveRecords(void) {
    mdlObj_t *resource = mdlViewerState.resources[0];
    s32 firstListCount =
        mdlPibGetChunkItemCount(mdlPibGetChunk(resource, MDL_PART_INFO_LIST_CONTEXT_WIDE));
    s32 activeListCount =
        mdlPibGetChunkItemCount(mdlPibGetChunk(resource, mdlViewerState.activeEntryId));

    return firstListCount + activeListCount;
}

/* Count the context-wide list before the selected-entry list for cursor lookup. */
MdlRecord *mdlGetSelectedRecord(void) {
    mdlObj_t *resource = mdlViewerState.resources[0];
    MdlRecord *firstList =
        mdlPibGetChunk(resource, MDL_PART_INFO_LIST_CONTEXT_WIDE);
    MdlRecord *secondList = mdlPibGetChunk(resource, mdlViewerState.activeEntryId);
    s32 index = mdlViewerState.selectedRecordIndex;
    MdlRecord *record;

    if (firstList != NULL) {
        record = mdlPibGetFirstChunkItem(firstList);
        while (record != NULL) {
            if (index == 0) {
                return record;
            }
            index--;
            record = mdlPibGetNextChunkItem(record);
        }
    }
    if (secondList == NULL) {
        return NULL;
    }
    record = mdlPibGetFirstChunkItem(secondList);
    while (record != NULL) {
        if (index == 0) {
            return record;
        }
        index--;
        record = mdlPibGetNextChunkItem(record);
    }
    return NULL;
}

INCLUDE_ASM(const s32, "code_001B2E60", mdlUpdateViewerMarkEditorInput);

void mdlDrawViewerEffectRecordList(void) {
    MdlViewState *state = &mdlViewerState;
    mdlObj_t *resource;
    MdlRecord *firstList;
    MdlRecord *secondList;
    MdlRecord *record;
    s32 firstCount;
    s32 secondCount;
    s32 totalCount;
    s32 firstRemaining;
    s32 selected;
    s32 scroll;
    s32 skipCount;
    s32 rowCount;
    s32 markerY;
    s32 descriptionY;
    sdfDrawTag_t *packetList;

    mdlAppendViewerRectToDrawList(0x7150, 0x7A08, 0xFF007F, 0xF60, 0x270, 0);
    resource = state->resources[0];
    packetList = state->packetList;
    firstList = mdlPibGetChunk(resource, MDL_PART_INFO_LIST_CONTEXT_WIDE);
    firstCount = mdlPibGetChunkItemCount(firstList);
    secondList = mdlPibGetChunk(resource, state->activeEntryId);
    secondCount = mdlPibGetChunkItemCount(secondList);
    totalCount = firstCount + secondCount;

    if (totalCount == 0) {
        sdfDrawTagLink(packetList,
                        (u32)sdfDevPrintf(0x7300, 0x7A20,
                                                         0xFF0080, 0,
                                                         D_003ABEB0));
    } else {
        s32 maxScroll = totalCount - 6;

        if (maxScroll < 0) {
            maxScroll = 0;
        }
        scroll = state->effectListScroll;
        if (scroll > maxScroll) {
            scroll = maxScroll;
        }

        firstRemaining = firstCount;
        if (firstCount != 0) {
            record = mdlPibGetFirstChunkItem(firstList);
        } else {
            record = mdlPibGetFirstChunkItem(secondList);
        }

        selected = mdlViewerState.selectedRecordIndex;
        if (selected >= totalCount) {
            selected = totalCount - 1;
            mdlViewerState.selectedRecordIndex = selected;
        }
        if (selected < scroll) {
            scroll = selected;
        } else if (selected >= scroll + 6) {
            scroll = selected - 5;
        }
        mdlViewerState.effectListScroll = scroll;

        skipCount = scroll;
        rowCount = 0;
        if (record != NULL) {
            descriptionY = 0x7A20;
            do {
                s32 selectionMarker = ' ';

                if (skipCount > 0) {
                    skipCount--;
                } else {
                    s32 listMarker = ' ';
                    s32 selectedStyle = 0;

                    if (selected == 0) {
                        if (mdlViewerState.editorMode == 0) {
                            selectionMarker = '>';
                        } else {
                            selectedStyle = 4;
                        }
                    }

                    if (firstRemaining > 0) {
                        listMarker = '*';
                    }

                    markerY = 0x7A20 + rowCount * 0x60;
                    if (selectionMarker != ' ' || listMarker != ' ') {
                        sdfDrawTagLink(packetList,
                                        (u32)sdfDevPrintf(
                                            0x7180, markerY, 0xFF0080, selectedStyle,
                                            D_003BBCB0, selectionMarker, listMarker));
                    }

                    mdlPibDrawEffectInfo(packetList, 0x7300, descriptionY, 0xFF0080,
                                  selectedStyle, record);
                    descriptionY += 0x60;
                    rowCount++;
                }

                if (firstRemaining > 0 && --firstRemaining == 0) {
                    record = secondList != NULL ? mdlPibGetFirstChunkItem(secondList) : NULL;
                } else {
                    record = mdlPibGetNextChunkItem(record);
                }
                selected--;
                if (rowCount >= 6) {
                    break;
                }
            } while (record != NULL);
        }

        if (scroll != 0) {
            sdfPrimLinkF3(packetList, 0x8000A0C0, 0,
                                 0x8040, 0x7A30, 0x8000, 0x7A70,
                                 0x8080, 0x7A70, 0xFF0080, NULL);
        }
        if (record != NULL) {
            sdfPrimLinkF3(packetList, 0x8000A0C0, 0,
                                 0x8000, 0x7C10, 0x8080, 0x7C10,
                                 0x8040, 0x7C50, 0xFF0080, NULL);
        }
    }

    if (mdlViewerState.editorMode == 1) {
        MdlRecord *markRecord = mdlGetSelectedRecord();

        if (markRecord != NULL && mdlPibCheckItemType(markRecord, MDL_VIEWER_RECORD_KIND_TRACK_POLY)) {
            mdlAppendViewerRectToDrawList(0x8590, 0x7A08, 0xFF007F, 0x960, 0xA50, 0);
            mdlPibDrawTrackInfo(packetList, 0x85C0, 0x7A20, 0xFF0080,
                                   (EffMarkParams *)markRecord,
                                   mdlViewerState.markFieldCursor);
        }
    }
}

u32 mdlRunViewerEffectEditorTask(void) {
    mdlUpdateViewerMarkEditorInput();
    mdlDrawViewerEffectRecordList();
    return 0;
}

s32 mdlRequestViewerExitOnce(void) {
    if (mdlViewerState.exitCleanupStarted == 0) {
        mdlViewerState.exitCleanupStarted = 1;
        mdlCleanupViewerTasksAndResources();
        dds3AdminiExitSequence();
    }
    return 0;
}

/* Submit the intermediate packet with an identity basis in phases 2 and 3. */
void mdlSubmitViewerIntermediateDrawPacket(void) {
    sdfDrawTag_t *packetList;

    if (mdlViewerState.taskPhase < 4) {
        if (mdlViewerState.taskPhase >= 2) {
            packetList = dds3FontReqAllocDrawTag();
            SDF_VU0_LOADIDENTITY();
            sdfDrawTagLink(packetList, (u32)((s32)sdfDrawRotatedModel(mdlSubmitViewerIntermediateDrawPacketData, mdlSubmitViewerIntermediateDrawPacketData2, 6, 0x80)));
            mdlData5.regFunc(&mdlData5, packetList);
        }
    }
}

/* Submit the selected resource in phase 3; keep the native bitwise input/once-flag test. */
void mdlSubmitViewerResourceDrawPacket(void) {
    sdfDrawTag_t *packetList;
    s32 drawPacket;
    mdlObj_t *resource;

    if (mdlViewerState.taskPhase == 3) {
        packetList = dds3FontReqAllocDrawTag();
        resource = mdlViewerState.resources[0];
    VU0_LOAD_MATRIX(resource->inner->tm.m);
        drawPacket = (s32)sdfDrawRotatedModel(mdlSubmitViewerResourceDrawPacketData, mdlSubmitViewerResourceDrawPacketData2, 4, 0x80);
        if ((dds3PadBitTable[13] < 0) & (D_003BBCB8 == 0)) {
            D_003BBCB8 = 1;
            sdfDumpHexLine(drawPacket, 0x100);
        }
        sdfDrawTagLink(packetList, (u32)(drawPacket));
        mdlData5.regFunc(&mdlData5, packetList);
    }
}

void mdlViewerTaskDestroy(void) {
    if (mdlViewerState.viewerTask != NULL) {
        dds3KillProcess(mdlViewerState.viewerTask, 0);
        mdlViewerState.viewerTask = NULL;
    }
}

INCLUDE_ASM(const s32, "code_001B2E60", mdlRestartViewerPhaseTask);

INCLUDE_ASM(const s32, "code_001B2E60", mdlViewer);

/* Walk native 64-byte map-position records and submit their visualization in one packet list. */
void mdlDrawMapPositionRecords(mdlObj_t *resource) {
    s32 recordCount = sdfModelGetMarkPosCount(resource->inner);
    s32 recordIndex;
    sdfDrawTag_t *packetList;
    sdfModelMarkPos_t *recordCursor;

    if (recordCount > 0) {
        packetList = dds3FontReqAllocDrawTag();
        recordCursor = (sdfModelMarkPos_t *)(
            (SdfMapPositionChunkPrefix *)sdfModelGetExtraChunk(resource->inner, MDL_MAP_POSITION_TAG) + 1);
        for (recordIndex = 0; recordIndex != recordCount; recordIndex++) {
            sdfModelMakeMarkPosMatrix(resource->inner, recordCursor);
            recordCursor++;
            sdfDrawTagLink(packetList, (u32)((s32)sdfDrawRotatedModel(mdlDrawMapPositionRecordsData, mdlDrawMapPositionRecordsData2, 6, 0x80)));
        }
        mdlData5.regFunc(&mdlData5, packetList);
    }
}

INCLUDE_ASM(const s32, "code_001B2E60", mapTrainCheckStartPos);

INCLUDE_RODATA(const s32, "code_001B2E60", mdlStrNoEffect);

INCLUDE_ASM(const s32, "code_001B2E60", mdlLoadViewerConfig);

INCLUDE_ASM(const s32, "code_001B2E60", mdlBgColorEye);

INCLUDE_ASM(const s32, "code_001B2E60", mdlInitializeViewerTasksAndResources);

/* Clear each occupied resource slot before destroying its context; retain the native scan form. */
void mdlFreeViewResources(void) {
    s32 resourceIndex;
    for (resourceIndex = 0; resourceIndex != MDL_VIEWER_RESOURCE_SLOTS; resourceIndex++) {
        mdlObj_t *resource = mdlViewerState.resources[resourceIndex];
        if (resource != 0) {
            mdlViewerState.resources[resourceIndex] = 0;
            mdlFree(resource);
        }
    }
}

INCLUDE_ASM(const s32, "code_001B2E60", mapTitleEnd);

INCLUDE_RODATA(const s32, "code_001B2E60", mdlStrModelViewer);

INCLUDE_RODATA(const s32, "code_001B2E60", mdlStrModelViewerEnd);

INCLUDE_RODATA(const s32, "code_001B2E60", mdlDebugLabelAmbient);

INCLUDE_RODATA(const s32, "code_001B2E60", softenBiasTitle);

INCLUDE_SDATA(const s32, "code_001B2E60", mapData153);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlDrawViewerModeLabelData);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlData15);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlDrawViewerAssetSelectionData);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlDrawViewerAssetSelectionData2);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlDrawViewerAssetSelectionData3);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlDrawViewerAssetSelectionData4);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlDrawViewerAssetSelectionData5);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlDrawViewerAssetSelectionData6);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlDrawViewerAssetSelectionData7);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlDrawViewerAssetSelectionData8);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlDrawViewerIndexedLabelOverlayDataEntry);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlDrawViewerIndexedLabelOverlayDataEntry2);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlDrawViewerMotionTimelineData);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlDrawViewerTexturePreviewData);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlDrawViewerSettingsData4);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlDrawViewerSettingsData5);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlDrawViewerSettingsData6);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlData16);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlDrawViewerEffectRecordListData);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlSubmitViewerResourceDrawPacketData4);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlInitializeViewerTasksAndResourcesData3);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlLoadViewerConfigData);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlLoadViewerConfigData2);

INCLUDE_SDATA(const s32, "code_001B2E60", pbDebugFloatFormat);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlLoadViewerConfigData3);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlLoadViewerConfigData4);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlLoadViewerConfigData5);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlDebugLabelLight2);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlDebugLabelLight1);

INCLUDE_SDATA(const s32, "code_001B2E60", mdlDebugLabelLight0);

