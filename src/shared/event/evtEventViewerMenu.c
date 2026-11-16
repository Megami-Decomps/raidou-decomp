#include "common.h"
#include "sdfDraw.h"
#include "mdlManager.h"
#include "sdfQuat.h"
#include "dds3DebugStuff.h"
#include "dds3KernelCore.h"
#include "dds3KernelMisc.h"

#include "newdata.h"
#include "sdfDevCons.h"
#include "sdk_libc.h"
#include "sdk_libkernl.h"
#include "include_asm.h"
#include "kernel.h"
#include "draw3d.h"
#include "basic.h"
#include "evt_world.h"
#include "sdf_resource.h"
#include "billboard.h"
#include "sdf_types.h"
#include "draw.h"
#include "libsdf.h"
#include "sdf_movie_types.h"
#include "field.h"
#include "ps2asm.h"
#include "code_r1_001C4B78.h"
#include "evtEventViewerMenu.h"
#include "evtEventViewer.h"

/* Setup remains ready after processing until its control byte resets it. */
typedef s8 EffEventSetupReadiness;

enum {
    EFF_EVENT_SETUP_NOT_READY = 0,
    EFF_EVENT_SETUP_READY = 1
};

#define DDS3_TASK_UPDATE_FINISH_CALLBACK (-1)

/* The primary object returned by dds3GetActiveScene is distinct from the active
 * world's WorldInfo payload. Event lookup, lip-sync and viewer code share this chain:
 * object +0x18 -> table +0x08 -> 0x0C-byte slot entries, each with head +0x04.
 * Node payloads depend on the slot. */
enum {
    EVT_WORLD_SLOT_MOVIE = 3, /* viewer movie-time lists: byte offset 0x28 */
    EVT_WORLD_SLOT_CAMERA = 4, /* linked cameras; active nodes supply the view transform */
    EVT_WORLD_SLOT_UNIT = 5   /* unit/group/lip-sync lists: byte offset 0x40 */
};

/* Convert the quaternion in vf10 into the primary matrix bank vf28-vf31. */
struct sdfDrawTag_t *dds3FontReqAllocDrawTag(void);
EffEventSetupReadiness evtViewerCheckDirty(void);

/* Borrow a controller owned by ctx. The pointer is read-only and expires when
 * that controller is removed or the model context is freed. */

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */

typedef s32 EvtMenuTableEntryAvailability;

enum {
    EVT_MENU_TABLE_ENTRY_DISABLED = 0,
    EVT_MENU_TABLE_ENTRY_ENABLED = 1
};

/* Shared 0x40-byte event-viewer control state. Unobserved input slots remain
 * opaque; the named bytes are consumed by both titles' editor handlers. */
typedef struct evtPad_t {
    u8 pad00[6];
    s8 frameEnd; /* +0x06: jump from group properties to the last key. */
    s8 frameStart; /* +0x07: jump from group properties to the first key. */
    u8 pad08[0x18];
    s8 syncKey; /* 0x20 */
    s8 confirm; /* 0x21 */
    u8 pad22;
    s8 cancel; /* 0x23 */
    s8 decOne; /* 0x24 */
    s8 incOne; /* 0x25 */
    s8 decTen; /* 0x26 */
    s8 incTen; /* 0x27 */
    u8 decHun; /* 0x28 */
    s8 coarseDecrease; /* 0x29: coarse decrease in the property editor. */
    u8 incHun; /* 0x2A */
    s8 coarseIncrease; /* 0x2B: coarse increase in the property editor. */
    s8 apply; /* 0x2C */
    s8 select; /* 0x2D: port 0 Select in input bank 1. */
    u8 pad2E[0x12]; /* Remaining two-bank, two-port input backing. */
} evtPad_t;

typedef char evtPad_size_must_be_0x40[(sizeof(evtPad_t) == 0x40) ? 1 : -1];

struct SoundMixer;

typedef struct {
    s16 enabled;
    s8 fields[8]; /* Per-row signed lookup values at +0x2. */
} EvtTblEntry; /* 0xA bytes */

extern EvtTblEntry evtData6[];
typedef s32 (*EvtMenuHeaderFn)();
typedef void (*EvtMenuRowFn)();

/* Draw a framed debug menu: the optional header returns how many rows it used, the row callback fills the rest,
 * and blinking scroll markers appear above/below when entries precede `first` or follow the last drawn one. */
void evtDrawMenuFrame(sdfDrawTag_t *list, s32 x, s32 y, s32 width, s32 rows, s32 first, s32 total, void *callbackContext,
                   EvtMenuHeaderFn header, EvtMenuRowFn row);

extern sdfGrOt_t sdfDebugGrOt;
extern evtPad_t dds3PadButtonState;
extern char evtFmtStr[];

void evtViewerDrawPendingNodeRow(sdfDrawTag_t *list, s32 x, s32 y, s32 index, evtViewerInfo_t *ctx);
extern void evtViewerDrawPendingNodeRow();
s32 mnuDrawInfoWindowB(s32 x, s32 y, evtViewerInfo_t *work);
s32 mnuDrawCutFlagLabel(sdfDrawTag_t *target, s32 x, s32 y);
s32 mnuDrawMotionChangeLabel(sdfDrawTag_t *target, s32 x, s32 y);

typedef struct evtMotionBits_t {
    s32 group : 8;
    s32 motion : 8;
    s32 loop : 8;
    s32 hokan : 8;
} evtMotionBits_t;

typedef union EvtMotionValue {
    s32 word;
    evtMotionBits_t bits;
} EvtMotionValue;

extern char evtDrawGroupListRowData2[];
extern char evtDrawGroupListRowData[];
extern char evtStrUnitAll[];
void evtDrawGroupListRow(sdfDrawTag_t *list, s32 x, s32 y, s32 index, evtViewerInfo_t *ctx);
s32 evtUpdateGroupListDialog(s32 x, s32 y, evtViewerInfo_t *ctx);
extern f32 sdfViewMatrix[];
void evtRotateViewerPositionFromPad(f32 *position);

/* Select and draw one light-color bank at the current panel offset. */
static inline void evtDrawLightRgbComponents(sdfDrawTag_t *list, evtViewerInfo_t *runtime,
                                             s32 selected, s32 y, s32 firstComponent) {
    if ((u32)selected >= 3U || runtime->lightEditorEditMode != 1) {
        selected = -1;
    }
    dds3PrimDebugTextBox(list,
                     (s32)(0x8740u + ((u32)runtime->lightPanelOffset << 4)),
                     y, selected, &runtime->commandMatrix[firstComponent]);
}

/* Convert the panel offset into the fixed-point X origin used by captions. */
static inline s32 evtLightCaptionX(s32 panelOffset) {
    return (s32)(0x8740u + ((u32)panelOffset << 4));
}

extern char evtViewerDrawGroupRowData[];
void evtViewerDrawGroupRow(sdfDrawTag_t *list, s32 x, s32 y, s32 index, evtViewerInfo_t *ctx);
s32 evtViewerDrawGroupWindow(s32 x, s32 y, evtViewerInfo_t *ctx);

typedef struct evtSelectionCache_t {
    u8 pad00[0x24];
    s32 selectedEntry; /* 0x24 */
} evtSelectionCache_t;

dds3TaskUpdateResult_t evtPollRuntimeControlReady(dds3ProcessID_t *task);

typedef char EvtViewKey_size_must_be_0x38[(sizeof(EvtRuntimeChild) == 0x38) ? 1 : -1];

typedef char evtCameraColorPayload_size_must_be_0x40[
    (sizeof(evtCameraColorPayload_t) == 0x40) ? 1 : -1];

extern s32 (*evtDispatchActionByIndexData[])(s32, s32, evtViewerInfo_t *);

/* Fixed prefix shared by PMD directory streams. */
typedef struct evtViewerFileHeader_t {
    u8 reserved00;
    u8 reserved01;
    u16 reserved02;
    s32 byteLength;
    char tag[4];
    s32 reserved0C;
    s32 count;
    s32 kind;
    s32 reserved18;
    s32 reserved1C;
} evtViewerFileHeader_t;

typedef char evtViewerFileHeader_size_check[sizeof(evtViewerFileHeader_t) == 0x20 ? 1 : -1];

/* PM1 section 2 and section 3 use the same 0x20-byte package record. */
typedef struct evtViewerPackageRecord_t {
    s32 nameIndex;
    s32 reserved04;
    s32 packageId;
    s32 partId;
    u32 offset;
    s32 length;
    s32 loadOrder;
    s32 reserved1C;
} evtViewerPackageRecord_t;

/* PM1 auxiliary and texture section entries each occupy 0x10 bytes. */
typedef struct evtViewerResourceRecord_t {
    s32 nameIndex;
    u32 offset;
    s32 kind;
    s32 reserved0C;
} evtViewerResourceRecord_t;

/* The section-21 wire record is also the group's link subobject at +0x18. */
typedef char evtViewerPackageRecord_size[(sizeof(evtViewerPackageRecord_t) == 0x20) ? 1 : -1];

typedef char evtViewerResourceRecord_size[(sizeof(evtViewerResourceRecord_t) == 0x10) ? 1 : -1];

typedef struct { u8 bytes[0x18]; } EvtRowPayload18;

typedef struct { u8 bytes[0x20]; } EvtRowPayload20;

typedef struct { u8 bytes[0x24]; } EvtRowPayload24;

typedef struct { u8 bytes[0x28]; } EvtRowPayload28;

typedef struct { u8 bytes[0x2C]; } EvtRowPayload2C;

typedef struct { u8 bytes[0x30]; } EvtRowPayload30;

typedef struct { u8 bytes[0x40]; } EvtRowPayload40;


INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtMenuDisp);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFloatChangeHead);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrValueChange);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFloatChangeItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFloatChangeMenu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtIntChangeHead);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrRrEnter);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrRdCancel);

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", jtbl_00450010);
#else
INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", jtbl_004B61E0);
#endif

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrLRValue);
#ifdef VERSION_RAIDOU1

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtIntChangeItemTable);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtEventViewerMenuStr10);
#endif

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtIntChangeItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtIntChangeMenu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFrameChangeHead);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFrameChangeItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFrameChangeMenu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtMainMenuItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtMainMenu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSelectHead);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSelectItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSelectMenu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFileNameHead);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFileNameItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFileNameMenu);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBlackOut2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrWhiteOut3);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtMainMenuItemTable2);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFileName2Head);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFileName2Item);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFileName2Menu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtObjectListSub);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtObjectListItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtOjbectListMenu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFrameListHead);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrWhiteOut22);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrWhiteIn22);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBlackOutB2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBlackInB2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrWhiteOutB2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrWhiteInB2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBlackIn2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrWhiteIn3);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrMaskOff2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrVshadowOn2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrMshadowOn2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrShadowOff2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrDispOff2);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtNORMAL);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFrameListItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCallMenuDisp);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtBasicListItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtBasicListMenu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtEventPosItem);

/* Draw the pending-node selector for group types 20/21; other types return -1. */
s32 mnuDrawInfoWindowB(s32 x, s32 y, evtViewerInfo_t *work) {
    sdfDrawTag_t *packets = dds3FontReqAllocDrawTag();
    s32 rows;
    switch (work->framelist_target->type) {
    case 20:
        rows = 2;
        break;
    case 21:
        rows = 4;
        break;
    default:
        return -1;
    }
    evtDrawMenuFrame(packets, x, y, 0x1C, rows, 0, rows, work, 0, evtViewerDrawPendingNodeRow);
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, packets);
    if (work->nowmenu != 0xC) {
        return 0;
    }
    return dds3SelectMenu(0, 1, rows, 1, rows, 0, 0, 0, &work->eventpos_y);
}

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtMESSAGEMENUMESMAX);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBranchRes0);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBranchRes1);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBranchRes2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBranchRes3);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBranchRes4);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBranchRes5);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBranchRes6);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBranchRes7);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBranchRes8);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBranchRes9);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtNONEMESDATATable);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtNONEMESDATA);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCallMenuDisp2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrUDSelect2);

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", jtbl_004508C0);
#else
INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", jtbl_004B6A90);
#endif

s32 mnuDrawCutFlagLabel(sdfDrawTag_t *target, s32 x, s32 y) {
    sdfDrawTagLink(target, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, "CUTFLAG MENU"));
    return 2;
}

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtCMPVALUETable);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCMPVALUE);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCallMenuDisp3);

EvtMenuTableEntryAvailability evtIsMenuTableEntryEnabled(s32 *index) {
    return evtData6[*index].enabled != EVT_MENU_TABLE_ENTRY_DISABLED;
}

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtLeaf10);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSOUTAI);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrSlightType);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrUnitAll);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtUpdateGroupPropertyDialog);

s32 mnuDrawMotionChangeLabel(sdfDrawTag_t *target, s32 x, s32 y) {
    sdfDrawTagLink(target, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, "MOTION CHANGE MENU"));
    return 2;
}

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtGROUPMAX);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCallMenuDisp4);

void evtDrawGroupListRow(sdfDrawTag_t *list, s32 x, s32 y, s32 index, evtViewerInfo_t *ctx) {
    evtViewerObject_t *group;
    s32 color;
    s32 n;

    color = 4;
    if (ctx->work_base + ctx->work_y != index) {
        color = 0;
    }
    if (index == 0) {
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, evtDrawGroupListRowData2));
    } else if (index == 1) {
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, evtDrawGroupListRowData));
        return;
    } else if (index == 2) {
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, evtStrUnitAll));
        return;
    }
    n = 3;
    for (group = ctx->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 1) {
            if (n == index) {
                sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, evtFmtStr, ((char *)group->info->name)));
                return;
            }
            n++;
        }
    }
}

s32 evtUpdateGroupListDialog(s32 x, s32 y, evtViewerInfo_t *ctx) {
    evtViewerObject_t *group;
    s32 count = 0;
    sdfDrawTag_t *packets;
    s32 shown = 15;

    for (group = ctx->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 1) {
            count++;
        }
    }
    count += 3;
    packets = dds3FontReqAllocDrawTag();
    evtDrawMenuFrame(packets, x, y, 20, 15, ctx->work_base, count, ctx, NULL, evtDrawGroupListRow);
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, packets);
    if (ctx->nowmenu != 17) {
        return 0;
    }
    if (count < shown) {
        shown = count;
    }
    return dds3SelectMenu(0, 1, count, 1, shown, 0, &ctx->work_base, 0, &ctx->work_y);
}

void evtLeaf11(void) {
}

/* vu0 routine: apply the selected camera-axis rotations to a position. */
void evtRotateViewerPositionFromPad(f32 *position)
{
    if (dds3PadButtonState.incOne != 0) {
        SDF_VU0_LOADVECTOR_MTXCOL(sdfViewMatrix + 1);
        FromRotation(2.0f * 3.14159265f / 180.0f);
        ToMatrix();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_APPLYMATRIX_XYZ1();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(position);
    } else if (dds3PadButtonState.decOne != 0) {
        SDF_VU0_LOADVECTOR_MTXCOL(sdfViewMatrix + 1);
        FromRotation(-2.0f * 3.14159265f / 180.0f);
        ToMatrix();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_APPLYMATRIX_XYZ1();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(position);
    }
    if (dds3PadButtonState.incTen != 0) {
        SDF_VU0_LOADVECTOR_MTXCOL(sdfViewMatrix);
        FromRotation(-2.0f * 3.14159265f / 180.0f);
        ToMatrix();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_APPLYMATRIX_XYZ1();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(position);
    } else if (dds3PadButtonState.decTen != 0) {
        SDF_VU0_LOADVECTOR_MTXCOL(sdfViewMatrix);
        FromRotation(2.0f * 3.14159265f / 180.0f);
        ToMatrix();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_APPLYMATRIX_XYZ1();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(position);
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCOPYCOLOR);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCallMenuDisp5);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFunc5);

void evtLeaf12(void) {
}

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCallMenuDisp6);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCallDevPrintf);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCallMenuDisp7);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtLeaf13);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp5Table);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrFaceDiv);

void evtViewerDrawGroupRow(sdfDrawTag_t *list, s32 x, s32 y, s32 index, evtViewerInfo_t *ctx) {
    evtViewerObject_t *group;
    s32 color;
    s32 n;

    color = 4;
    if (ctx->work_base + ctx->work_y != index) {
        color = 0;
    }
    if (index == 0) {
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, evtViewerDrawGroupRowData));
        return;
    }
    if (index == 1) {
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, "DEFFAULT"));
        return;
    }
    n = 2;
    for (group = ctx->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x18) {
            if (n == index) {
                sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, evtFmtStr, ctx->nametable[group->entryHeader]));
                return;
            }
            n++;
        }
    }
}

s32 evtViewerDrawGroupWindow(s32 x, s32 y, evtViewerInfo_t *ctx) {
    evtViewerObject_t *group;
    s32 count = 0;
    sdfDrawTag_t *packets;
    s32 shown = 15;

    for (group = ctx->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x18) {
            count++;
        }
    }
    count += 2;
    packets = dds3FontReqAllocDrawTag();
    evtDrawMenuFrame(packets, x, y, 20, 15, ctx->work_base, count, ctx, NULL, evtViewerDrawGroupRow);
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, packets);
    if (ctx->nowmenu != 21) {
        return 0;
    }
    if (count < shown) {
        shown = count;
    }
    return dds3SelectMenu(0, 1, count, 1, shown, 0, &ctx->work_base, 0, &ctx->work_y);
}

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrAlphaYzahyo);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtViewerDrawShadowConfigRow);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCallGetPIDfromUID6);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCallGetProcessData19);

/* Poll solid-rectangle setup until it clears runtime control and finishes. */
dds3TaskUpdateResult_t evtPollRuntimeControlReady(dds3ProcessID_t *task) {
    evtViewerInfo_t *runtime;

    runtime = (evtViewerInfo_t *)dds3GetProcessData(task);
    if (evtViewerCheckDirty() == EFF_EVENT_SETUP_NOT_READY) {
        runtime->menuProcess = 0;
        return DDS3_TASK_UPDATE_FINISH_CALLBACK;
    }
    return DDS3_TASK_UPDATE_RETAIN_CALLBACK;
}

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtBLEND);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCallConsPanelTagLinkZ);
INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrX);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrY);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrRight);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrQuick);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtMenuDispData);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtScrollUpMark);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtScrollDownMark);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFmtIndentInt);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtGROUPMAXData);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtMainMenuItemDataEntry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtMainMenuItemData);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtMainMenuItemTable2Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtMainMenuItemData2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFmtStr);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFmtSpaceStr);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFmtNameExt);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFmtChar);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtLabelEvent);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFmtEventNo);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtLabelCut);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFileName2ItemData);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFileName2ItemData2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtLabelName);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFrameListHeadData2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData3Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData3Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData3Entry3);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData3Entry4);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData3Entry5);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData3Entry6);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData3Entry7);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData4Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData4Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData4Entry3);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData4Entry4);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData4Entry5);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData5Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData5Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData6Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData6Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData7Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData7Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData8Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData8Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData9Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData9Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData12);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData10Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData13);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData11Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData14);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData15);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtStrEllipsis);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData16);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtStrOn);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData17);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData18);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData19);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData20);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData21);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData22);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData23);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData24);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData25);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", nbDebugMotionNameOut);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData26);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFmtFloat1);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData27);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData28);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFmtName11);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData29);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData30);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData31);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData32);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData33);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData34);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData35);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData36);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData37);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFmtPlayerNo);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFmtIndentStr);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNONEMESDATATableEntry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtData17);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCMPVALUETableEntry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtSOUTAIDataEntry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtSOUTAIDataEntry2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtDrawGroupListRowData);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtDrawGroupListRowData2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtData18);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtSOUTAIData3);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtSOUTAIData4);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtGROUPMAXData2Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtGROUPMAXData2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtGROUPMAXData3);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtGROUPMAXData4);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtData19);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCOPYCOLORData2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp5TableEntry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp5TableEntry2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp5TableEntry3);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp5TableEntry4);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp5TableEntry5);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp5TableEntry6);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp5Data2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp6Data2Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp6Data2Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp6Data2Entry3);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp6Data3);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp6Data4);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp6Data5);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtViewerDrawGroupRowData);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtViewerDrawShadowConfigRowDataEntry2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtViewerDrawShadowConfigRowDataEntry3);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtData20);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtData21);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtBLENDData);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtBLENDData2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtBLENDData3);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtBLENDData4);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallConsPanelTagLinkZData);

