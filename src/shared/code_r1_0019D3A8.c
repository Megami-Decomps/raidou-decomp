#include "common.h"
#include "sdfDraw.h"
#include "sdf2D.h"
#include "dds3GlobalWork.h"
#include "dds3AdminiProcess.h"
#include "dds3KernelCore.h"
#include "sdfPrim.h"
#include "sdf_Decomp.h"
#include "sdfModel.h"
#include "mdlManager.h"

#include "fileManager.h"
#include "model.h"
#include "mdlEffect.h"
#include "sdfDevCons.h"
#include "sdfSound.h"
#include "sdk_libc.h"
#include "include_asm.h"
#include "kernel.h"
#include "draw3d.h"
#include "sdf_resource.h"
#include "billboard.h"
#include "sdf_types.h"
#include "draw.h"
#include "libsdf.h"
#include "sdf_movie_types.h"
#include "sdf_lighting.h"
#include "motion.h"
#include "ps2asm.h"
#include "code_r1_0019D3A8.h"
#include "dds3PadManager.h"
#include "dds3DebugStuff.h"
#include "dds3KernelDraw.h"
#include "dds3CameraObjectBasic.h"
#include "sdfPacketBuff.h"
#include "sdf3D.h"
#include "sdfQuat.h"
#include "kernel_draw.h"
#include "sdfMotion.h"

enum {
    MDL_MOTION_ABSENT = 0,
    MDL_MOTION_PRESENT = 1
};

/* Variable-size serialized chunk streams share only this eight-byte header.
 * The word at +8 belongs to each chunk's payload. */
typedef struct sdfChunkHeader_t {
    u32 id;   /* 0x00: zero terminates the chunk stream */
    u32 size; /* 0x04: byte extent to the next chunk */
} sdfChunkHeader_t;

typedef char sdfChunkHeader_size_must_be_8[(sizeof(sdfChunkHeader_t) == 8) ? 1 : -1];

/* MPOS has an eight-byte opaque prefix after the common header, then 0x40-byte
 * rows. Its enclosing chunk extent remains variable and is stored in size. */
typedef struct sdfMapPositionChunkPrefix_t {
    sdfChunkHeader_t header;
    u8 payload08[8];
} sdfMapPositionChunkPrefix_t;

typedef char sdfMapPositionChunkPrefix_size_must_be_0x10[
    (sizeof(sdfMapPositionChunkPrefix_t) == 0x10) ? 1 : -1];

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

/* The referenced halfword is promoted to a word-sized SDK result. */

/* Allocate a distinct caller-owned context linked to the registered group.
 * Contexts share the group node and assets; release each context with mdlFree. */
mdlObj_t *mdlLoad(u32 group, u32 id);
void mdlLoopMotion(mdlObj_t *ctx, s32 searchId, s32 motionIndex);

void mdlSmoothLoopMotion(mdlObj_t *ctx, s32 searchId, s32 motionIndex, f32 blendLeadFrames,
                          f32 blendDurationFrames);

void mdlSmoothSingleMotion(mdlObj_t *ctx, s32 searchId, s32 motionIndex, f32 blendLeadFrames,
                       f32 blendDurationFrames);


/* Key for a part-info record list; -1 selects the context-wide list. */
typedef s32 MdlPartInfoListKey;

enum {
    MDL_PART_INFO_LIST_CONTEXT_WIDE = -1
};

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

/* Viewer-wide state for the model viewer task. Fields that are
 * only written by a defaults initialiser and never read are
 * left as unkNN on purpose: there is no read to earn a role from. */
typedef struct mdlViewState_t {
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
    s16 selectedEntryId;  /* 0x24: key passed to the mdlAddEntry* family */
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
    s8 unkC4;
} mdlViewState_t;

extern mdlViewState_t mdlViewerState;
extern u8 sdfHddValid;
extern f32 mdlUpdateViewerSelectedModelFromPadData[4];

typedef struct mdlViewerPhaseTask_t {
    const char *name;
    dds3ProcessFunc_t update;
    void *data;
} mdlViewerPhaseTask_t;

extern mdlViewerPhaseTask_t mdlRestartViewerPhaseTaskData[];

static inline s8 mdlGetViewerDisplayMode(mdlViewState_t *state) {
    return state->assetDisplayMode;
}

extern s8 mdlRunViewerSelectionLabelTaskWork[];

typedef struct mdlViewerTaskDef_t {
    const char *name;
    dds3ProcessFunc_t update;
    s32 data;
} mdlViewerTaskDef_t;

typedef struct mdlSystemTask_t {
    const char *name;
    s32 arg;
} mdlSystemTask_t;

extern char mdlStrModel03[];
extern char D_003ABCB0[];
extern char D_003ABCC8[];
extern char D_003ABCD8[];
extern char D_003BBC28[];
s32 mdlUpdateViewerCursor(s16 *, s32);
void mdlUpdateViewerSelectedModelFromPad(void);
void mdlDrawViewerSelectionLabel(void);
void mdlCleanupViewerTasksAndResources(void);

/* Follow the relative links in a resource's part-info table to find a list key. */

/* The first eight bytes belong to the enclosing list, not its first record. */

/* A terminal kind returns NULL; the link is a byte offset, not a pointer. */
extern char D_003ABA80[], D_003ABA90[], D_003ABAA0[], D_003ABAB0[], D_003ABAC0[], D_003ABAD0[];
extern char D_003BBB80[], D_003BBB88[];

/* Draw the scalar marker fields, then four colors with four byte channels each; negative selection hides the cursor. */

#define MDL_PART_BILLBOARD 0

#define MDL_PART_EFFECT 1

#define MDL_PART_OBJECT 3

u64 *mdlBuildViewerRectanglePacket(s32 x, s32 y, s32 depth, s32 width, s32 height);
void mdlAppendViewerRectToDrawList(s32 x, s32 y, s32 depth, s32 width, s32 height, s32 unused);
extern const char mdlDrawViewerMotionTimelineData[];
extern s32 mdlDrawViewerSettingsData[];
extern const char *mdlDrawViewerSettingsData2[];
extern const char *mdlDrawViewerSettingsData3[];
extern const char mdlDrawViewerSettingsData4[];
extern const char mdlDrawViewerSettingsData5[];
extern const char mdlDrawViewerSettingsData6[];
extern const char mdlData16[];

typedef struct mdlViewerRegsPacket_t {
    sdfGrPacketHeader_t header;
    u64 alphaData;
    u64 alphaReg;
    u64 testData;
    u64 testReg;
} mdlViewerRegsPacket_t;
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
extern u128 mdlInitializeViewerTasksAndResourcesData;
extern u128 mdlInitializeViewerTasksAndResourcesData2;
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

/* Loaded-resource count chain. */
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
pibHeaderChunk_t *mdlGetSelectedRecord(void);
extern pibHeaderChunk_t *mdlGetSelectedRecord(void);
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
void mdlSubmitViewerResourceDrawPacket(void);
void mdlViewerTaskDestroy(void);
extern f32 mdlDrawMapPositionRecordsData[][4];
extern u32 mdlDrawMapPositionRecordsData2[];
void mdlDrawMapPositionRecords(mdlObj_t *resource);
void mdlFreeViewResources(void);

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
    sdfDevPrintCB_t packet;
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
    sdfDevPrintCB_t packet;
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

/* body: matched in dds @ 18d1fa88f: src/dds1/model/mdlEffect.c:1211, rework, same function 1.00 (words 171/171, calls 1.00, shape 1.00) */
INCLUDE_RODATA(const s32, "code_r1_0019D3A8", mdlStrModel032);

INCLUDE_RODATA(const s32, "code_r1_0019D3A8", mdlStr0202);

INCLUDE_RODATA(const s32, "code_r1_0019D3A8", mdlStr02022);

void mdlHandleViewerAssetSelection(void) {
    s32 pageStep;
    s32 motionIndex;
    s32 entryCount;
    s32 referenceCount;

    if (mdlUpdateViewerCursor(&mdlViewerState.selectionCursor, 5) != 0) {
        return;
    }
    pageStep = mdlViewerState.assetDisplayMode == 0 ? 10 : 16;
    if (dds3PadBitTable[0] < 0) {
        mdlViewerState.selectedNodeId++;
        if (mdlViewerState.selectedNodeId == 4) {
            mdlViewerState.selectedNodeId = 0;
        }
        motionIndex = mdlGetMotionNum(mdlViewerState.resources[0], mdlViewerState.selectedNodeId);
        if (motionIndex < 0) {
            motionIndex = 0;
        }
        mdlViewerState.selectedEntryId = motionIndex;
    }
    switch (mdlViewerState.selectionCursor) {
    case 0:
        if (mdlUpdateViewerCursorWithPageStep(&mdlViewerState.selectedResourceGroup, mdlGetNumOfMajor(), 1) != 0) {
            entryCount = mdlGetNumOfMinor(mdlViewerState.selectedResourceGroup);
            if (mdlViewerState.selectedResourceId < entryCount) {
                return;
            }
            mdlViewerState.selectedResourceId = entryCount - 1;
            return;
        }
        break;
    case 1:
        if (mdlUpdateViewerCursorWithPageStep(&mdlViewerState.selectedResourceId, mdlGetNumOfMinor(mdlViewerState.selectedResourceGroup), pageStep) != 0) {
            return;
        }
        break;
    case 2:
        referenceCount = mdlGetNumOfCut(mdlViewerState.resources[0], mdlViewerState.selectedNodeId);
        if (referenceCount == 0) {
            return;
        }
        if (mdlUpdateViewerCursorWithPageStep(&mdlViewerState.selectedEntryId, referenceCount, pageStep) != 0) {
            return;
        }
        break;
    case 3:
        if (mdlUpdateViewerCursorWithPageStep(&mdlViewerState.entryHeight, 100, 10) != 0) {
            return;
        }
        break;
    case 4:
        if (mdlUpdateViewerCursorWithPageStep(&mdlViewerState.entryWidth, 100, 10) != 0) {
            return;
        }
        break;
    }
    if (dds3PadBitTable[12] < 0) {
        if (sdfHddValid == 0) {
            return;
        }
        mdlFreeViewResources();
        mdlMakeHddFileList();
        if (mdlViewerState.resourceGroup == mdlGetNumOfMajor() - 1) {
            mdlViewerState.resourceId = 0;
            mdlViewerState.selectedResourceGroup = mdlViewerState.resourceGroup;
            mdlViewerState.selectedResourceId = 0;
        }
        mdlLoadViewerResourceAndResetCursors();
        return;
    }
    if (mdlViewerState.selectionCursor < 2) {
        if (dds3PadBitTable[3] >= 0) {
            return;
        }
        if (mdlViewerState.resourceGroup == mdlViewerState.selectedResourceGroup && mdlViewerState.resourceId == mdlViewerState.selectedResourceId) {
            return;
        }
        mdlFree(mdlViewerState.resources[0]);
        mdlViewerState.resources[0] = NULL;
        mdlViewerState.resourceGroup = mdlViewerState.selectedResourceGroup;
        mdlViewerState.resourceId = mdlViewerState.selectedResourceId;
        mdlLoadViewerResourceAndResetCursors();
        return;
    } else if (dds3PadBitTable[3] < 0) {
        mdlAddViewEntryFlagged();
    } else if (dds3PadBitTable[1] < 0) {
        mdlAddPlainViewerEntryForSelectedNode();
    }
}

INCLUDE_SDATA(const s32, "code_r1_0019D3A8", mapData153);

INCLUDE_SDATA(const s32, "code_r1_0019D3A8", mdlDrawViewerModeLabelData);

INCLUDE_SDATA(const s32, "code_r1_0019D3A8", mdlData15);

void mdlDrawViewerAssetSelection(void) {
    sdfDevPrintCB_t packet;
    char basename[32];
    s32 panelTop;
    s32 panelHeight;
    s32 basenameWidth;
    s32 rowY;
    s32 index;
    void *formatted;
    const char *format;

    if (mdlViewerState.selectedNodeId == 0) {
        panelTop = 0x8560;
        panelHeight = 0x90;
    } else {
        panelTop = 0x8440;
        panelHeight = 0x1B0;
    }
    mdlAppendViewerRectToDrawList(0x8410, panelTop - 0x18, 0xFF0080, 0xAE0,
                                 panelHeight, 0);

    mdlGetFilebase(mdlViewerState.selectedResourceGroup, mdlViewerState.selectedResourceId, basename, 0x20);
    basenameWidth = (s32)strlen(basename) * 3 << 6;
    mdlAppendViewerRectToDrawList(0x8E90 - basenameWidth,
                                 panelTop - 0xD8, 0xFF0080,
                                 basenameWidth + 0x60, 0x90, 0);
    formatted = sdfDevPrintf(0x8EC0 - basenameWidth,
                                             panelTop - 0xC0, 0xFF0080, 0,
                                             basename);
    sdfDrawTagLink(mdlViewerState.packetList, (u32)formatted);

    sdfDevSetPrintCB(&packet, 0x8440, panelTop + mdlViewerState.selectedNodeId * 0x60,
               0xFF0080, mdlViewerState.selectionHighlightPhase < 0x14 && mdlViewerState.selectionCursor == 0 ? 4 : 0);
    formatted = sdfDevPrintfEx(&packet, "%c", mdlViewerState.selectedResourceGroup + 0x30);
    sdfDrawTagLink(mdlViewerState.packetList, (u32)formatted);

    sdfPktSetCmd(&packet, 0);
    formatted = sdfDevPrintfEx(&packet, "-");
    sdfDrawTagLink(mdlViewerState.packetList, (u32)formatted);

    sdfPktSetCmd(&packet,
                 mdlViewerState.selectionHighlightPhase < 0x14 && mdlViewerState.selectionCursor == 1 ? 4 : 0);
    if (mdlViewerState.assetDisplayMode == 0) {
        format = "%03d";
    } else {
        format = "%03x";
    }
    formatted = sdfDevPrintfEx(&packet, format, mdlViewerState.selectedResourceId);
    sdfDrawTagLink(mdlViewerState.packetList, (u32)formatted);

    sdfPktSetCmd(&packet,
                 mdlViewerState.selectionHighlightPhase < 0x14 && mdlViewerState.selectionCursor == 3 ? 4 : 0);
    formatted = sdfDevPrintfEx(&packet, "    %02d", mdlViewerState.entryHeight);
    sdfDrawTagLink(mdlViewerState.packetList, (u32)formatted);

    sdfPktSetCmd(&packet,
                 mdlViewerState.selectionHighlightPhase < 0x14 && mdlViewerState.selectionCursor == 4 ? 4 : 0);
    formatted = sdfDevPrintfEx(&packet, ",%02d", mdlViewerState.entryWidth);
    sdfDrawTagLink(mdlViewerState.packetList, (u32)formatted);

    rowY = panelTop;
    index = 0;
    while (mdlViewerState.selectedNodeId == 0 ? index <= 0 : index < 4) {
        s32 referenceCount = mdlGetNumOfCut(mdlViewerState.resources[0], index);

        if (mdlViewerState.selectedNodeId == index) {
            s32 style = mdlViewerState.selectionHighlightPhase < 0x14 && mdlViewerState.selectionCursor == 2 ? 4 : 0;

            if (referenceCount == 0) {
                formatted = sdfDevPrintf(
                    0x88C0, rowY, 0xFF0080, style, "--");
            } else {
                if (mdlViewerState.assetDisplayMode == 0) {
                    format = "%02d";
                } else {
                    format = "%02x";
                }
                formatted = sdfDevPrintf(
                    0x88C0, rowY, 0xFF0080, style, format,
                    (s16)mdlViewerState.selectedEntryId);
            }
        } else {
            referenceCount = mdlGetNumOfCut(mdlViewerState.resources[0], index);
            if (referenceCount == 0) {
                formatted = sdfDevPrintf(
                    0x88C0, rowY, 0xFF0080, 0, D_003BBC28);
            } else {
                s32 motionIndex;

                referenceCount--;
                motionIndex = mdlGetMotionNum(mdlViewerState.resources[0], index);
                if (mdlViewerState.assetDisplayMode == 0) {
                    format = mdlStr0202;
                } else {
                    format = mdlStr02022;
                }
                formatted = sdfDevPrintf(
                    0x88C0, rowY, 0xFF0080, 0, format,
                    motionIndex, referenceCount);
            }
        }
        rowY += 0x60;
        index++;
        sdfDrawTagLink(mdlViewerState.packetList, (u32)formatted);
    }
}

u32 mdlRunViewerAssetSelectionTask(void) {
    mdlHandleViewerAssetSelection();
    mdlDrawViewerAssetSelection();
    return 0;
}

void mapCallFromRotationB(void) {
    f32 speed;
    s32 rotated;

    if (dds3PadBitTable[0xC] < 0) {
        mdlResetViewerBasisVectors();
        return;
    }
    if (dds3PadBitTable[0] < 0) {
        if (++mdlViewerState.labelIndexA == 2) {
            mdlViewerState.labelIndexA = 0;
        }
    }
    if (mdlViewerState.labelIndexA == 0) {
        speed = 1.25f;
        if (dds3PadBitTable[3] == 0) {
            speed = 20.0f;
            if (dds3PadBitTable[1] == 0) {
                speed = 5.0f;
            }
        }
        SDF_VU0_LOADIDENTITYVECTOR();
        SDF_VU0_LOADVECTOR_B_MTXCOL(&sdfViewMatrix.m[0]);
        SDF_VU0_SCALEVECTOR_B_V0(speed);
        if (dds3PadBitTable[5] != 0) {
            SDF_VU0_ADDVECTOR();
        } else if (dds3PadBitTable[4] != 0) {
            SDF_VU0_SUBVECTOR();
        }
        SDF_VU0_LOADVECTOR_B_MTXCOL(&sdfViewMatrix.m[2]);
        SDF_VU0_SCALEVECTOR_B_V0(speed);
        if (dds3PadBitTable[6] != 0) {
            SDF_VU0_ADDVECTOR();
        } else if (dds3PadBitTable[7] != 0) {
            SDF_VU0_SUBVECTOR();
        }
        SDF_VU0_LOADVECTOR_B_MTXCOL(&sdfViewMatrix.m[1]);
        SDF_VU0_SCALEVECTOR_B_V0(speed);
        if (dds3PadBitTable[9] != 0) {
            SDF_VU0_ADDVECTOR();
        } else if (dds3PadBitTable[8] != 0) {
            SDF_VU0_SUBVECTOR();
        }
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(&mapCallFromRotationBWork);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(&mapCallFromRotationBWork);
        SDF_VU0_LOADVECTOR(&mapCallFromRotationBWork2);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(&mapCallFromRotationBWork2);
    } else {
        speed = 0.25f * 3.14159265f / 180.0f;
        if (dds3PadBitTable[3] == 0) {
            speed = 4.0f * 3.14159265f / 180.0f;
            if (dds3PadBitTable[1] == 0) {
                speed = 1.0f * 3.14159265f / 180.0f;
            }
        }
        rotated = 0;
        SDF_VU0_LOADIDENTITYVECTOR();
        if (dds3PadBitTable[5] != 0) {
            SDF_VU0_LOADVECTOR_B_MTXCOL(&sdfViewMatrix.m[1]);
            FromRotation_B(speed);
            Mult();
            rotated = 1;
        } else if (dds3PadBitTable[4] != 0) {
            SDF_VU0_LOADVECTOR_B_MTXCOL(&sdfViewMatrix.m[1]);
            FromRotation_B(-speed);
            Mult();
            rotated = 1;
        }
        if (dds3PadBitTable[6] != 0) {
            SDF_VU0_LOADVECTOR_B_MTXCOL(&sdfViewMatrix.m[0]);
            FromRotation_B(speed);
            Mult();
            rotated = 1;
        } else if (dds3PadBitTable[7] != 0) {
            SDF_VU0_LOADVECTOR_B_MTXCOL(&sdfViewMatrix.m[0]);
            FromRotation_B(-speed);
            Mult();
            rotated = 1;
        }
        if (dds3PadBitTable[8] != 0) {
            SDF_VU0_LOADVECTOR_B_MTXCOL(&sdfViewMatrix.m[2]);
            FromRotation_B(speed);
            Mult();
            rotated = 1;
        } else if (dds3PadBitTable[9] != 0) {
            SDF_VU0_LOADVECTOR_B_MTXCOL(&sdfViewMatrix.m[2]);
            FromRotation_B(-speed);
            Mult();
            rotated = 1;
        }
        if (rotated) {
            ToMatrix();
            SDF_VU0_LOADVECTOR(&mapCallFromRotationBWork2);
            SDF_VU0_LOADVECTOR_B(&mapCallFromRotationBWork);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_APPLYMATRIX_XYZ1();
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(&mapCallFromRotationBWork2);
            SDF_VU0_LOADVECTOR(&mapCallFromRotationBWork3);
            SDF_VU0_APPLYMATRIX_XYZ1();
            SDF_VU0_STOREVECTOR(&mapCallFromRotationBWork3);
        }
    }
}

void mdlDrawViewerIndexedLabelOverlay(void) {
    mdlAppendViewerRectToDrawList(0x8A10, 0x7948, 0xFF007F, 0x4E0, 0x90, 0);
    sdfDrawTagLink(mdlViewerState.packetList, (u32)(sdfDevPrintf(0x8A40, 0x7960, 0xFF0080, 0, mdlDrawViewerIndexedLabelOverlayData[mdlViewerState.labelIndexA])));
}

u32 mdlRunViewerIndexedLabelTask(void) {
    mdlUpdateViewerCameraFromPad();
    mdlDrawViewerIndexedLabelOverlay();
    return 0;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/model/mdlEffect.c:1567, rework, same function 1.00 (words 245/245, calls 1.00, shape 1.00) */
void mdlUpdateViewerSelectedModelFromPad(void) {
    f32 speed;
    f32 angle;

    if (dds3PadBitTable[0x13] < 0) {
        mdlViewerState.yawStepMode ^= 1;
    }
    if (dds3PadBitTable[0] < 0) {
        mdlViewerState.labelIndexB ^= 1;
        return;
    }
    if (dds3PadBitTable[0xC] < 0) {
        SDF_VU0_LOADIDENTITYVECTOR();
        mdlSetPosition(mdlViewerState.resources[0]);
        mdlSetRotate(mdlViewerState.resources[0]);
        return;
    }
    if (mdlViewerState.yawStepMode != 0) {
        mdlGetRotate(mdlViewerState.resources[0]);
        if (dds3PadBitTable[5] < 0) {
            SDF_VU0_LOADVECTOR_B(sdf3DUpVector);
            angle = 5.0f * 3.14159265f / 180.0f;
        } else if (dds3PadBitTable[4] < 0) {
            SDF_VU0_LOADVECTOR_B(sdf3DUpVector);
            angle = -5.0f * 3.14159265f / 180.0f;
        } else {
            return;
        }
        FromRotation_B(angle);
        Mult();
        mdlSetRotate(mdlViewerState.resources[0]);
    } else if (mdlViewerState.labelIndexB == 0) {
        speed = 1.25f;
        if (dds3PadBitTable[3] == 0) {
            speed = 20.0f;
            if (dds3PadBitTable[1] == 0) {
                speed = 5.0f;
            }
        }
        mdlGetPosition(mdlViewerState.resources[0]);
        SDF_VU0_LOADVECTOR_B_MTXCOL(&sdfViewMatrix);
        SDF_VU0_SCALEVECTOR_B_V0(speed);
        if (dds3PadBitTable[5] != 0) {
            SDF_VU0_ADDVECTOR();
        } else if (dds3PadBitTable[4] != 0) {
            SDF_VU0_SUBVECTOR();
        }
        SDF_VU0_LOADVECTOR_B_MTXCOL(&sdfViewMatrix.m[2]);
        SDF_VU0_SCALEVECTOR_B_V0(speed);
        if (dds3PadBitTable[6] != 0) {
            SDF_VU0_ADDVECTOR();
        } else if (dds3PadBitTable[7] != 0) {
            SDF_VU0_SUBVECTOR();
        }
        SDF_VU0_LOADVECTOR_B_MTXCOL(&sdfViewMatrix.m[1]);
        SDF_VU0_SCALEVECTOR_B_V0(speed);
        if (dds3PadBitTable[9] != 0) {
            SDF_VU0_ADDVECTOR();
        } else if (dds3PadBitTable[8] != 0) {
            SDF_VU0_SUBVECTOR();
        }
        mdlSetPosition(mdlViewerState.resources[0]);
    } else {
        speed = 0.5f * 3.14159265f / 180.0f;
        if (dds3PadBitTable[3] == 0) {
            speed = 8.0f * 3.14159265f / 180.0f;
            if (dds3PadBitTable[1] == 0) {
                speed = 2.0f * 3.14159265f / 180.0f;
            }
        }
        mdlGetRotate(mdlViewerState.resources[0]);
        if (dds3PadBitTable[5] != 0) {
            SDF_VU0_LOADVECTOR_B(sdf3DUpVector);
            FromRotation_B(speed);
            Mult();
        } else if (dds3PadBitTable[4] != 0) {
            SDF_VU0_LOADVECTOR_B(sdf3DUpVector);
            FromRotation_B(-speed);
            Mult();
        }
        if (dds3PadBitTable[6] != 0) {
            SDF_VU0_LOADVECTOR_B(sdfUnitXVector);
            FromRotation_B(speed);
            Mult();
        } else if (dds3PadBitTable[7] != 0) {
            SDF_VU0_LOADVECTOR_B(sdfUnitXVector);
            FromRotation_B(-speed);
            Mult();
        }
        if (dds3PadBitTable[9] != 0) {
            SDF_VU0_LOADVECTOR_B(mdlUpdateViewerSelectedModelFromPadData);
            FromRotation_B(speed);
            Mult();
        } else if (dds3PadBitTable[8] != 0) {
            SDF_VU0_LOADVECTOR_B(mdlUpdateViewerSelectedModelFromPadData);
            FromRotation_B(-speed);
            Mult();
        }
        mdlSetRotate(mdlViewerState.resources[0]);
    }
}

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

/* body: matched in dds @ 18d1fa88f: src/dds1/model/mdlEffect.c:1685, adapt, same function 1.00 (words 74/74, calls 1.00, shape 1.00) */
void mdlAdjustViewerScale(void) {
    s32 scaleHundredths = (s32)(mdlViewerState.viewerScale * 100.0f + 0.5f);

    /* Keep the distinct up/down thresholds and the native float expression order. */
    if (dds3PadButtonState[0x27] & MDL_PAD_REPEAT_FLAG) {
        if (scaleHundredths < 0x32) {
            scaleHundredths += 1;
        } else if (scaleHundredths < 0x1F4) {
            scaleHundredths += 10;
        } else {
            scaleHundredths += 100;
            if (scaleHundredths >= 0x7D1) {
                scaleHundredths = 0x7D0;
            }
        }
        mdlViewerState.viewerScale = scaleHundredths * 0.01f;
    } else if (dds3PadButtonState[0x26] & MDL_PAD_REPEAT_FLAG) {
        if (scaleHundredths < 0x33) {
            scaleHundredths -= 1;
            if (scaleHundredths < 5) {
                scaleHundredths = 5;
            }
        } else if (scaleHundredths < 0x1F5) {
            scaleHundredths -= 10;
        } else {
            scaleHundredths -= 100;
        }
        mdlViewerState.viewerScale = scaleHundredths * 0.01f;
    }
    if (dds3PadButtonState[0x21] < 0) {
        mdlViewerState.unitStepMode ^= 1;
    }
    mdlViewerState.unitStepSign = 0;
    if (mdlViewerState.unitStepMode != 0) {
        if (dds3PadButtonState[0x25] != 0) {
            mdlViewerState.unitStepSign = 1;
        } else if (dds3PadButtonState[0x24] != 0) {
            mdlViewerState.unitStepSign = -1;
        }
    }
}

void mdlDrawViewerMotionTimeline(void) {
    sdfDrawTag_t *packetList;
    mdlObj_t *resource;
    sdfMotionCtrlr_t *motion;
    s32 playheadX;
    s32 textStyle;

    mdlAppendViewerRectToDrawList(0x81D0, 0x7948, 0xFF007F, 0xD20, 0xF0, 0);
    packetList = mdlViewerState.packetList;
    sdfPrimLinkLine(packetList, 0x80303030, 0, 0x8200, 0x7990, 0x8EC0, 0x7990, 0xFF0080, 0);
    sdfPrimLinkLine(packetList, 0x80303030, 0, 0x8200, 0x7960, 0x8200, 0x79C0, 0xFF0080, 0);
    sdfPrimLinkLine(packetList, 0x80303030, 0, 0x8EC0, 0x7960, 0x8EC0, 0x79C0, 0xFF0080, 0);
    resource = mdlViewerState.resources[0];
    motion = resource->first;
    if (motion != NULL) {
        playheadX = (s32)(motion->frame * 3264.0f / motion->frameLength);
        if (playheadX < 0) {
            playheadX = 0;
        }
        playheadX += 0x8200;
        sdfPrimLinkLine(packetList, 0x800000E0, 0, playheadX, 0x7960, playheadX, 0x79C0, 0xFF0090, 0);
        sdfDrawTagLink(mdlViewerState.packetList, (u32)(sdfDevPrintf(0x8200, 0x79C0, 0xFF0080, 0, "[%5.1f/%-3d]", motion->frame, motion->frameLength)));
    } else {
        sdfDrawTagLink(mdlViewerState.packetList, (u32)(sdfDevPrintf(0x8200, 0x79C0, 0xFF0080, 0, "[---.-/---]")));
    }
    textStyle = mdlViewerState.unitStepMode != 0 ? 2 : 0;
    sdfDrawTagLink(mdlViewerState.packetList, (u32)(sdfDevPrintf(0x8B00, 0x79C0, 0xFF0080, textStyle, mdlDrawViewerMotionTimelineData, mdlViewerState.viewerScale)));
}

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

/* body: matched in dds @ 18d1fa88f: src/dds1/model/mdlEffect.c:1821, rework, same function 1.00 (words 130/130, calls 1.00, shape 1.00) */
void mdlApplyViewerResourceMenuAction(void) {
    s32 i;
    mdlObj_t *resource;
    s32 entryId;
    sdfMotionCtrlr_t *motion;

    if (mdlUpdateViewerCursorWithPageStep(&mdlViewerState.scrollPage, 5, 1) != 0) {
        return;
    }
    if (dds3PadBitTable[3] >= 0) {
        return;
    }
    switch (mdlViewerState.scrollPage) {
    case 0:
        if (mdlViewerState.resourceCount != MDL_VIEWER_RESOURCE_SLOTS) {
            mdlViewerState.resources[mdlViewerState.resourceCount] = 0;
            mdlViewerState.resourceCount++;
            mdlRotateViewResourcesRight();
            mdlLoadViewerResourceAndResetCursors();
        }
        break;
    case 1:
        if (mdlViewerState.resourceCount >= 2) {
            mdlFree(mdlViewerState.resources[0]);
            mdlViewerState.resources[0] = 0;
            mdlRotateViewList();
            mdlViewerState.resourceCount--;
        }
        break;
    case 2:
        if (mdlViewerState.resourceCount >= 2) {
            mdlRotateViewResourcesRight();
        }
        break;
    case 3:
        if (mdlViewerState.resourceCount >= 2) {
            mdlRotateViewList();
        }
        break;
    case 4:
        for (i = 0; i < mdlViewerState.resourceCount; i++) {
            resource = mdlViewerState.resources[i];
            motion = resource->first;
            if (motion != NULL) {
                entryId = resource->current.h.motionIndex;
                if (motion->playType == 0) {
                    sdfMotionChange(motion, entryId, 0);
                } else {
                    sdfMotionChange(motion, entryId, 1);
                }
            }
        }
        break;
    }
    mdlViewerState.selectedResourceGroup = mdlViewerState.resourceGroup = mdlGetMajorNum(mdlViewerState.resources[0]);
    mdlViewerState.selectedResourceId = mdlViewerState.resourceId = mdlGetMinorNum(mdlViewerState.resources[0]);
    i = mdlViewerState.resources[0]->current.h.motionIndex;
    if (i < 0) {
        i = 0;
    }
    mdlViewerState.selectedEntryId = mdlViewerState.activeEntryId = i;
}

/* Draw resource actions with independent eligibility and selection styles. */
void mdlDrawViewerResourceMenu(void) {
    sdfDevPrintCB_t packet;
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

INCLUDE_RODATA(const s32, "code_r1_0019D3A8", mdlStrSyncMotion);

INCLUDE_RODATA(const s32, "code_r1_0019D3A8", mdlStrSelectPrev);

INCLUDE_RODATA(const s32, "code_r1_0019D3A8", mdlStrSelectNext);

INCLUDE_RODATA(const s32, "code_r1_0019D3A8", mdlStrDeleteModel);

INCLUDE_RODATA(const s32, "code_r1_0019D3A8", mdlStrNewModel);

INCLUDE_RODATA(const s32, "code_r1_0019D3A8", mdlStr22);

s32 mdlIsDebugTimeGraph(void) {
    return dds3SearchProcessName("DebugTimeGrph") != 0;
}

INCLUDE_ASM(const s32, "code_r1_0019D3A8", mapModelViewerTmx);

/* NON_MATCHING: 111 of 189 words match.  */
#ifdef NON_MATCHING
void mdlDrawViewerSettings(void) {
    s8 enabled[4];
    sdfDrawTag_t *packetList;
    s32 i;
    s32 y;
    s32 style;
    s32 value;
    const char **label;

    mdlAppendViewerRectToDrawList(0x7150, 0x7A08, 0xFF007F, 0x1320, 0x630, 0);
    packetList = mdlViewerState.packetList;
    enabled[0] = mdlViewerState.unk0E;
    enabled[1] = mdlIsDebugTimeGraph();
    enabled[2] = mdlViewerState.assetDisplayMode;
    enabled[3] = mdlViewerState.unkC4;
    for (i = 0, y = 0x7A80; i < 4; i++, y += 0x60) {
        sdfDrawTagLink(packetList, (u32)(sdfDevPrintf(
            0x7300, y, 0xFF0080, 0, mdlDrawViewerSettingsData2[i],
            enabled[i] == 0 ? mdlDrawViewerSettingsData4 : mdlDrawViewerSettingsData5)));
    }
    if (mdlViewerState.selectionHighlightPhase < 20) {
        sdfDrawTagLink(packetList, (u32)(sdfDevPrintf(
            0x7240, mdlDrawViewerSettingsData[mdlViewerState.settingsCursor], 0xFF0080, 0, mdlDrawViewerSettingsData6)));
    }
    dds3PrimDebugTextBoxRGB(packetList, 0x7300, mdlDrawViewerSettingsData[4],
                            mdlViewerState.settingsPage == 1 ? mdlViewerState.settingsCursor - 4 : -1,
                            dds3BackColor, 0);
    sdfDrawTagLink(packetList, (u32)(sdfDevPrintf(0x7B40, 0x7CC0, 0xFF0080, 0, "BACK COLOR")));
    for (i = 0, label = mdlDrawViewerSettingsData3, y = 0x7DE0; i < 3; i++, label++, y += 0x60) {
        value = 0;
        switch (i) {
        case 0:
            value = mdlViewerState.captureScale;
            break;
        case 1:
            value = mdlViewerState.captureDivX;
            break;
        case 2:
            value = mdlViewerState.captureDivY;
            break;
        }
        sdfDrawTagLink(packetList, (u32)(sdfDevPrintf(0x7300, y, 0xFF0080, 0, *label, value)));
        style = 0;
        if (i == mdlViewerState.settingsCursor - 7) {
            style = 6;
            if (i != mdlViewerState.settingsPage - 2) {
                style = 0;
            }
        }
        sdfDrawTagLink(packetList, (u32)(sdfDevPrintf(0x7F00, y, 0xFF0080, style, mdlData16, value)));
    }
    sdfDrawTagLink(packetList, (u32)(sdfDevPrintf(0x7300, 0x7F60, 0xFF0080, 0, "SAVE CONFIG")));
}
#else
INCLUDE_RODATA(const s32, "code_r1_0019D3A8", mdlStrBgViewerTmx);

INCLUDE_RODATA(const s32, "code_r1_0019D3A8", mdlStrModelHexNum);

INCLUDE_RODATA(const s32, "code_r1_0019D3A8", mdlStrRunTimeGraph);

INCLUDE_RODATA(const s32, "code_r1_0019D3A8", mdlStrDisplayMark);

INCLUDE_RODATA(const s32, "code_r1_0019D3A8", mdlStrCaptureDivY);

INCLUDE_RODATA(const s32, "code_r1_0019D3A8", mdlStrCaptureDivX);

INCLUDE_RODATA(const s32, "code_r1_0019D3A8", mdlStrCaptureScale);

INCLUDE_ASM(const s32, "code_r1_0019D3A8", mdlDrawViewerSettings);
#endif

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
pibHeaderChunk_t *mdlGetSelectedRecord(void) {
    mdlObj_t *resource = mdlViewerState.resources[0];
    pibHeaderChunk_t *firstList =
        mdlPibGetChunk(resource, MDL_PART_INFO_LIST_CONTEXT_WIDE);
    pibHeaderChunk_t *secondList = mdlPibGetChunk(resource, mdlViewerState.activeEntryId);
    s32 index = mdlViewerState.selectedRecordIndex;
    pibHeaderChunk_t *record;

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

void mdlUpdateViewerMarkEditorInput(void) {
    mdlViewState_t *state = &mdlViewerState;
    s32 count;
    pibHeaderChunk_t *record;

    switch (state->editorMode) {
    case 0:
        count = mdlCountActiveRecords();
        if (dds3PadButtonState[0x27] < 0) {
            state->selectedRecordIndex++;
            if (state->selectedRecordIndex >= count) {
                state->selectedRecordIndex = 0;
            }
        } else if (((u8)dds3PadButtonState[0x27] & MDL_PAD_REPEAT_FLAG) != 0) {
            if (state->selectedRecordIndex < count - 1) {
                state->selectedRecordIndex++;
            }
        } else if (dds3PadButtonState[0x26] < 0) {
            if (state->selectedRecordIndex == 0) {
                state->selectedRecordIndex = count - 1;
            } else {
                state->selectedRecordIndex--;
            }
        } else if (((u8)dds3PadButtonState[0x26] & MDL_PAD_REPEAT_FLAG) != 0) {
            if (state->selectedRecordIndex > 0) {
                state->selectedRecordIndex--;
            }
        } else if (dds3PadButtonState[0x23] < 0) {
            record = mdlGetSelectedRecord();
            if (record != NULL && mdlPibCheckItemType(record, MDL_VIEWER_RECORD_KIND_TRACK_POLY)) {
                state->editorMode = 1;
            }
        }
        break;
    case 1:
        record = mdlGetSelectedRecord();
        if (dds3PadButtonState[0x21] >= 0 && record != NULL && mdlPibCheckItemType(record, MDL_VIEWER_RECORD_KIND_TRACK_POLY)) {
            if (dds3PadButtonState[0x23] < 0) {
                mdlAddPlainViewerEntryForSelectedNode();
            } else {
                mdlPibEditTrackInfo((mdlTrackParams_t *)record, &state->markFieldCursor);
            }
        } else {
            state->editorMode = 0;
        }
        break;
    }
}

void mdlDrawViewerEffectRecordList(void) {
    mdlViewState_t *state = &mdlViewerState;
    mdlObj_t *resource;
    pibHeaderChunk_t *firstList;
    pibHeaderChunk_t *secondList;
    pibHeaderChunk_t *record;
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
        pibHeaderChunk_t *markRecord = mdlGetSelectedRecord();

        if (markRecord != NULL && mdlPibCheckItemType(markRecord, MDL_VIEWER_RECORD_KIND_TRACK_POLY)) {
            mdlAppendViewerRectToDrawList(0x8590, 0x7A08, 0xFF007F, 0x960, 0xA50, 0);
            mdlPibDrawTrackInfo(packetList, 0x85C0, 0x7A20, 0xFF0080,
                                   (mdlTrackParams_t *)markRecord,
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
    sdfGrPacketHeader_t *drawPacket;
    mdlObj_t *resource;

    if (mdlViewerState.taskPhase == 3) {
        packetList = dds3FontReqAllocDrawTag();
        resource = mdlViewerState.resources[0];
    VU0_LOAD_MATRIX(resource->inner->tm.m);
        drawPacket = sdfDrawRotatedModel(mdlSubmitViewerResourceDrawPacketData, mdlSubmitViewerResourceDrawPacketData2, 4, 0x80);
        if ((dds3PadBitTable[13] < 0) & (D_003BBCB8 == 0)) {
            D_003BBCB8 = 1;
            sdfDumpHexLine(drawPacket, 0x100);
        }
        sdfDrawTagLink(packetList, EE_ADDR(drawPacket));
        mdlData5.regFunc(&mdlData5, packetList);
    }
}

void mdlViewerTaskDestroy(void) {
    if (mdlViewerState.viewerTask != NULL) {
        dds3KillProcess(mdlViewerState.viewerTask, 0);
        mdlViewerState.viewerTask = NULL;
    }
}

void mdlRestartViewerPhaseTask(void) {
    mdlViewerTaskDestroy();
    mdlViewerState.viewerTask = dds3StartProcess(mdlRestartViewerPhaseTaskData[mdlViewerState.taskPhase - 1].name, 0x2AFE, 1, 0, mdlRestartViewerPhaseTaskData[mdlViewerState.taskPhase - 1].update, NULL, mdlRestartViewerPhaseTaskData[mdlViewerState.taskPhase - 1].data);
    dds3LinkProcess(mdlViewerState.unk00, mdlViewerState.viewerTask);
}

/* NON_MATCHING: 80 of 219 words match.  */
#ifdef NON_MATCHING
s32 mdlViewer(dds3ProcessID_t *task) {
    sdfModelRootNode_t *model;
    mdlViewerRegsPacket_t *packet;
    s32 lodCount;
    s32 selection;

    mdlViewerState.selectionHighlightPhase += 2;
    if (mdlViewerState.selectionHighlightPhase == 30) {
        mdlViewerState.selectionHighlightPhase = 0;
    }
    if (dds3PadBitTable[0xE] < 0) {
        model = mdlViewerState.resources[0]->inner;
        lodCount = sdfGetLodChunkValue(model);
        if (lodCount > 0) {
            if (model->lod == lodCount) {
                model->lod = 0;
            } else {
                model->lod++;
            }
        }
    }
    if (mdlViewerState.taskPhase == 0) {
        selection = mdlViewerState.selectedTaskIndex;
        if ((dds3PadButtonState[0x27] & 2) && (u32)selection < 12) {
            mdlViewerState.selectedTaskIndex = selection + 1;
        } else if (dds3PadButtonState[0x27] < 0) {
            if (selection == 12) {
                mdlViewerState.selectedTaskIndex = 0;
            } else {
                mdlViewerState.selectedTaskIndex = selection + 1;
            }
        } else if ((dds3PadButtonState[0x26] & 2) && selection > 0) {
            mdlViewerState.selectedTaskIndex = selection - 1;
        } else if (dds3PadButtonState[0x26] < 0) {
            if (selection == 0) {
                mdlViewerState.selectedTaskIndex = 12;
            } else {
                mdlViewerState.selectedTaskIndex = selection - 1;
            }
        } else if (dds3PadButtonState[0x25] < 0 || dds3PadButtonState[0x24] < 0) {
            if ((u32)selection < 7) {
                if ((u32)(selection + 7) < 13) {
                    mdlViewerState.selectedTaskIndex = selection + 7;
                }
            } else {
                mdlViewerState.selectedTaskIndex = selection - 7;
            }
        } else if (dds3PadButtonState[0x23] < 0) {
            mdlViewerState.selectionHighlightPhase = 0;
            mdlViewerState.taskPhase = mdlViewerState.selectedTaskIndex + 1;
            mdlRestartViewerPhaseTask();
        } else if (dds3PadButtonState[0x22] < 0) {
            mdlViewerState.taskPhase = mdlViewerState.unk0B;
            mdlRestartViewerPhaseTask();
        }
    } else if (mdlViewerState.taskPhase >= 0 && dds3PadButtonState[0x22] < 0) {
        mdlViewerTaskDestroy();
        mdlViewerState.unk0B = mdlViewerState.taskPhase;
        mdlViewerState.taskPhase = 0;
        mdlViewerState.selectedTaskIndex = mdlViewerState.unk0B - 1;
    }
    SDF_VU0_COPYVECTOR(&aViewEye, &mapCallFromRotationBWork);
    SDF_VU0_COPYVECTOR(&aViewTarget, &mapCallFromRotationBWork2);
    SDF_VU0_COPYVECTOR(&aViewDown, &mapCallFromRotationBWork3);
    mdlViewerState.packetList = dds3CallGrAllocPacketBuff9();
    packet = (mdlViewerRegsPacket_t *)sdfGrAllocPacketBuff(0x40);
    sdfGrSetRegsPacketHeader(&packet->header, 2);
    packet->alphaData = 0x44;
    packet->testData = 0x3000D;
    packet->alphaReg = 0x42;
    packet->testReg = 0x47;
    sdfDrawTagLink(mdlViewerState.packetList, (u32)packet);
    if (mdlViewerState.yawStepMode == 0) {
        mdlDrawViewerModeLabel();
        mdlDrawViewerModelAndMotionSummary();
        mdlDrawViewerOptionLabels();
        mdlSubmitViewerIntermediateDrawPacket();
        mdlSubmitViewerResourceDrawPacket();
    }
    if (mdlViewerState.unkC4 && mapData153) {
        eglSetOt(0xD);
        eglLinkAlpha(0);
        eglLinkTest(1, 0, 128, 3, 0, 0, 1, 1);
        eglLinkSprite(0, 0, 512, 448, 0, 0, 512, 448, 0x80808080, 0x80808080, 0x80808080, 0x80808080, mapData153);
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_0019D3A8", mdlViewer);
#endif

/* Walk native 64-byte map-position records and submit their visualization in one packet list. */
void mdlDrawMapPositionRecords(mdlObj_t *resource) {
    s32 recordCount = sdfModelGetMarkPosCount(resource->inner);
    s32 recordIndex;
    sdfDrawTag_t *packetList;
    sdfModelMarkPos_t *recordCursor;

    if (recordCount > 0) {
        packetList = dds3FontReqAllocDrawTag();
        recordCursor = (sdfModelMarkPos_t *)(
            (sdfMapPositionChunkPrefix_t *)sdfModelGetExtraChunk(resource->inner, MDL_MAP_POSITION_TAG) + 1);
        for (recordIndex = 0; recordIndex != recordCount; recordIndex++) {
            sdfModelMakeMarkPosMatrix(resource->inner, recordCursor);
            recordCursor++;
            sdfDrawTagLink(packetList, (u32)((s32)sdfDrawRotatedModel(mdlDrawMapPositionRecordsData, mdlDrawMapPositionRecordsData2, 6, 0x80)));
        }
        mdlData5.regFunc(&mdlData5, packetList);
    }
}

INCLUDE_ASM(const s32, "code_r1_0019D3A8", mapTrainCheckStartPos);

INCLUDE_RODATA(const s32, "code_r1_0019D3A8", mdlStrNoEffect);

INCLUDE_ASM(const s32, "code_r1_0019D3A8", mdlLoadViewerConfig);

INCLUDE_ASM(const s32, "code_r1_0019D3A8", mdlBgColorEye);

void mdlInitializeViewerTasksAndResources(void) {
    s32 i;
    mdlDebugProcessEntry_t *task;

    mdlInitializeViewerTasksAndResourcesData3 = 0;
    mdlViewerState.unkC4 = 0;
    mdlLoadViewerConfig();
    mdlViewerState.viewerScale = 0.5f;
    mdlViewerState.assetDisplayMode = mdlViewerState.unk0B = mdlViewerState.taskPhase = 1;
    mdlViewerState.captureDivY = mdlViewerState.captureDivX = mdlViewerState.captureScale = mdlViewerState.selectedResourceGroup = mdlViewerState.resourceGroup = 2;
    mdlViewerState.resourceId = 0;
    mdlViewerState.selectedResourceId = 0;
    mdlViewerState.exitCleanupStarted = 0;
    mdlViewerState.selectionHighlightPhase = 0;
    mdlViewerState.selectionCursor = 0;
    mdlViewerState.entryHeight = 0;
    mdlViewerState.entryWidth = 0;
    mdlViewerState.labelIndexB = 0;
    mdlViewerState.scrollPage = 0;
    mdlViewerState.unitStepMode = 0;
    mdlViewerState.unk0E = 0;
    mdlViewerState.workspaceSetupPhase = 0;
    mdlResetViewerBasisVectors();
    dds3ScreenParam.fovy = 0.4363323f;
    if (mdlInitializeViewerTasksAndResourcesData3 == 0) {
        SDF_VU0_COPYVECTOR(&mdlInitializeViewerTasksAndResourcesData, &mdlInitializeViewerTasksAndResourcesData2);
    }
    for (i = 0; i != 12; i++) {
        mdlViewerState.resources[i] = NULL;
    }
    mdlLoadViewerResourceAndResetCursors();
    mdlViewerState.resourceCount = 1;
    mdlMakeHddFileList();
    for (i = 0, task = mdlData12; i != 3; i++, task++) {
        fclEncycFreeEntries(task->name);
    }
    mdlViewerState.unk00 = dds3StartProcess(mdlStrModelViewer, 0x2AFD, 1, 0, mdlViewer, NULL, NULL);
    dds3StartProcess(mdlStrModelViewerEnd, 0x2AFF, 1, 0, mapTrainCheckStartPos, NULL, NULL);
    mdlViewerState.viewerTask = NULL;
    mdlRestartViewerPhaseTask();
}

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

void mapTitleEnd(void) {
    s32 i;

    if (mapData153 != NULL) {
        sdfTexFreeDelayed(mapData153);
        mapData153 = NULL;
    }
    mdlFreeViewResources();
    dds3KillProcessName(mdlStrModelViewer, 0);
    dds3KillProcessName(mdlStrModelViewerEnd, 0);
    for (i = 0; i != 3; i++) {
        if (dds3SearchProcessName(mdlData12[i].name) == NULL) {
            fclEncycAllocEntry(mdlData12[i].name, mdlData12[i].update, NULL, NULL);
        }
    }
}

INCLUDE_RODATA(const s32, "code_r1_0019D3A8", mdlStrModelViewer);

INCLUDE_RODATA(const s32, "code_r1_0019D3A8", mdlStrModelViewerEnd);

INCLUDE_RODATA(const s32, "code_r1_0019D3A8", mdlDebugLabelAmbient);

INCLUDE_RODATA(const s32, "code_r1_0019D3A8", softenBiasTitle);

INCLUDE_SDATA(const s32, "code_r1_0019D3A8", mdlDrawViewerIndexedLabelOverlayDataEntry);

INCLUDE_SDATA(const s32, "code_r1_0019D3A8", mdlDrawViewerIndexedLabelOverlayDataEntry2);

INCLUDE_SDATA(const s32, "code_r1_0019D3A8", mdlDrawViewerMotionTimelineData);

INCLUDE_SDATA(const s32, "code_r1_0019D3A8", mdlDrawViewerTexturePreviewData);

INCLUDE_SDATA(const s32, "code_r1_0019D3A8", mdlDrawViewerSettingsData4);

INCLUDE_SDATA(const s32, "code_r1_0019D3A8", mdlDrawViewerSettingsData5);

INCLUDE_SDATA(const s32, "code_r1_0019D3A8", mdlDrawViewerSettingsData6);

INCLUDE_SDATA(const s32, "code_r1_0019D3A8", mdlData16);

INCLUDE_SDATA(const s32, "code_r1_0019D3A8", mdlDrawViewerEffectRecordListData);

INCLUDE_SDATA(const s32, "code_r1_0019D3A8", mdlSubmitViewerResourceDrawPacketData4);

INCLUDE_SDATA(const s32, "code_r1_0019D3A8", mdlInitializeViewerTasksAndResourcesData3);

INCLUDE_SDATA(const s32, "code_r1_0019D3A8", mdlLoadViewerConfigData);

INCLUDE_SDATA(const s32, "code_r1_0019D3A8", mdlLoadViewerConfigData2);

INCLUDE_SDATA(const s32, "code_r1_0019D3A8", pbDebugFloatFormat);

INCLUDE_SDATA(const s32, "code_r1_0019D3A8", mdlLoadViewerConfigData3);

INCLUDE_SDATA(const s32, "code_r1_0019D3A8", mdlLoadViewerConfigData4);

INCLUDE_SDATA(const s32, "code_r1_0019D3A8", mdlLoadViewerConfigData5);

INCLUDE_SDATA(const s32, "code_r1_0019D3A8", mdlDebugLabelLight2);

INCLUDE_SDATA(const s32, "code_r1_0019D3A8", mdlDebugLabelLight1);

INCLUDE_SDATA(const s32, "code_r1_0019D3A8", mdlDebugLabelLight0);

