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
#include "code_r1_001CA8F8.h"
#include "evtPolygonMovie.h"
#include "itfMesManager.h"
#include "dds3GlobalWork.h"
#include "code_r1_001CC1D0.h"
#include "effEvent.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"

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

struct sndMixer_t;

typedef s32 (*EvtMenuHeaderFn)(sdfDrawTag_t *list, s32 x, s32 y, evtViewerInfo_t *ctx);
typedef void (*EvtMenuRowFn)(sdfDrawTag_t *list, s32 x, s32 y, s32 index, evtViewerInfo_t *ctx);

/* Draw a framed debug menu: the optional header returns how many rows it used, the row callback fills the rest,
 * and blinking scroll markers appear above/below when entries precede `first` or follow the last drawn one. */
void evtDrawMenuFrame(sdfDrawTag_t *list, s32 x, s32 y, s32 width, s32 rows, s32 first, s32 total, void *callbackContext,
                   EvtMenuHeaderFn header, EvtMenuRowFn row);

extern sdfGrOt_t sdfDebugGrOt;
extern evtPad_t dds3PadButtonState;
extern char evtFmtStr[];
extern s32 evtMenuDispData;
extern char evtFileNameChars[];
extern u16 evt_file_minor;
extern u16 evtFileMajor;
extern u16 evtData14;
extern s32 evtActiveEntryFlags;
extern char *evtViewerDrawShadowConfigRowData[];
extern char *evtSOUTAIData[];
extern char *evtSOUTAIData2[];
extern s8 evtSOUTAIColumnWidth[];
extern char *evtUpdateGroupPropertyDialogData[];
extern char evtGROUPMAXData[];
extern char evtNONEMESDATATableEntry[];
extern char evtCMPVALUETableEntry[];

typedef struct EvtSelectionCache {
    u8 pad00[0x24];
    s32 selectedEntry; /* 0x24 */
} EvtSelectionCache;

extern EvtSelectionCache *evtCallGetProcessData19Data;

void evtViewerDrawPendingNodeRow(sdfDrawTag_t *list, s32 x, s32 y, s32 index, evtViewerInfo_t *ctx);
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

typedef char EvtViewKey_size_must_be_0x38[(sizeof(evtFrameInfo_t) == 0x38) ? 1 : -1];

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

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtMenuDispData);

void evtMenuDisp(sdfDrawTag_t *list, s32 x, s32 y, s32 sx, s32 sy, s32 baseitem, s32 maxitem, evtViewerInfo_t *ctx,
                 EvtMenuHeaderFn header, EvtMenuRowFn row) {
    s32 used = 0;
    s32 index;
    s32 px;
    s32 py;

    dds3ConsPanelTagLink(list, x, y, sx, sy);
    px = ((u32)x << 4) + 0x7000;
    py = ((u32)y << 3) + 0x7900;
    if (header != NULL) {
        s32 headerRows = header(list, px, py, ctx);

        py += headerRows * 96;
        used = headerRows;
    }
    index = baseitem;
    for (; used < sy; used++) {
        if (row != NULL) {
            row(list, px, py, index, ctx);
            py += 96;
            index++;
        }
    }
    if ((evtMenuDispData & 0x10) != 0) {
        py = ((u32)(y - 10) << 3) + 0x7900;
        px = ((u32)(x + sx * 5 + 6) << 4) + 0x7000;
        if (baseitem > 0) {
            sdfDrawTagLink(list, (u32)sdfDevPrintf(px, py, 0xFEFFFF, 6, "^"));
        }
        py = ((u32)(y + sy * 12 - 2) << 3) + 0x7900;
        if (index < maxitem) {
            sdfDrawTagLink(list, (u32)sdfDevPrintf(px, py, 0xFEFFFF, 6, "v"));
        }
    }
    evtMenuDispData++;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/event/evtEventViewerMenu.c:202, match, same function 1.00 (words 20/20, calls 1.00, shape 1.00) */
s32 evtFloatChangeHead(sdfDrawTag_t *tag, s32 ox, s32 oy)
{
    sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(ox, oy, 0xFEFFFF, 0, "VALUE CHANGE.")));
    return 2;
}

void evtFloatChangeItem(sdfDrawTag_t *list, s32 x, s32 y, s32 index, evtViewerInfo_t *ctx) {
    switch (index) {
    case 0:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 4, "     %.1f", (f64)ctx->fvalue));
        return;
    case 2:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " L,R = VALUE-+0.1"));
        return;
    case 3:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " U,D = VALUE-+1.0"));
        return;
    case 4:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " RR  = ENTER"));
        return;
    case 5:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " RD  = CANCEL"));
        break;
    }
}

s32 evtFloatChangeMenu(s32 x, s32 y, evtViewerInfo_t *ctx) {
    sdfDrawTag_t *list = dds3FontReqAllocDrawTag();
    f32 step;

    evtDrawMenuFrame(list, x, y, 22, 9, 0, 1, ctx, (EvtMenuHeaderFn)evtFloatChangeHead, evtFloatChangeItem);
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, list);
    if (ctx->nowmenu != 8) {
        return 0;
    }
    if (dds3PadButtonState.cancel < 0) {
        return 1;
    }
    if (dds3PadButtonState.confirm < 0) {
        return -1;
    }
    step = 0.0f;
    if ((dds3PadButtonState.decOne & 2) != 0) {
        step = -0.1f;
    } else if ((dds3PadButtonState.incOne & 2) != 0) {
        step = 0.1f;
    }
    if ((dds3PadButtonState.decTen & 2) != 0) {
        step = -1.0f;
    } else if ((dds3PadButtonState.incTen & 2) != 0) {
        step = 1.0f;
    }
    ctx->fvalue += step;
    if (ctx->fvalue < ctx->floatMin) {
        ctx->fvalue = ctx->floatMin;
    }
    if (ctx->fvalue >= ctx->floatMax) {
        ctx->fvalue = ctx->floatMax;
    }
    return 0;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/event/evtEventViewerMenu.c:268, match, same function 1.00 (words 20/20, calls 1.00, shape 1.00) */
s32 evtIntChangeHead(sdfDrawTag_t *tag, s32 ox, s32 oy)
{
    sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(ox, oy, 0xFEFFFF, 0, "VALUE CHANGE.")));
    return 2;
}

void evtIntChangeItem(sdfDrawTag_t *list, s32 x, s32 y, u32 index, evtViewerInfo_t *ctx) {
    switch (index) {
    case 0:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 4, "     %d", ctx->value));
        return;
    case 2:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " L,R = VALUE-+"));
        return;
    case 3:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " U,D = VALUE-+10"));
        return;
    case 4:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " RR  = ENTER"));
        return;
    case 5:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " RD  = CANCEL"));
        break;
    }
}

s32 evtIntChangeMenu(s32 x, s32 y, evtViewerInfo_t *ctx) {
    sdfDrawTag_t *list = dds3FontReqAllocDrawTag();
    s32 step;

    evtDrawMenuFrame(list, x, y, 22, 9, 0, 1, ctx, (EvtMenuHeaderFn)evtIntChangeHead, (EvtMenuRowFn)evtIntChangeItem);
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, list);
    if (ctx->nowmenu != 7) {
        return 0;
    }
    if (dds3PadButtonState.cancel < 0) {
        return 1;
    }
    if (dds3PadButtonState.confirm < 0) {
        return -1;
    }
    if ((dds3PadButtonState.decOne & 2) != 0) {
        step = -1;
    } else if ((dds3PadButtonState.incOne & 2) != 0) {
        step = 1;
    } else {
        step = 0;
    }
    if ((dds3PadButtonState.decTen & 2) != 0) {
        step = -10;
    } else if ((dds3PadButtonState.incTen & 2) != 0) {
        step = 10;
    }
    ctx->value += step;
    if (ctx->value < ctx->min) {
        ctx->value = ctx->min;
    }
    if (ctx->value >= ctx->max) {
        ctx->value = ctx->max;
    }
    return 0;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/event/evtEventViewerMenu.c:332, match, same function 1.00 (words 20/20, calls 1.00, shape 1.00) */
s32 evtFrameChangeHead(void *tag, s32 ox, s32 oy)
{
    sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(ox, oy, 0xFEFFFF, 0, "FRAME CHANGE.")));
    return 2;
}

void evtFrameChangeItem(sdfDrawTag_t *list, s32 x, s32 y, u32 index, evtViewerInfo_t *ctx) {
    switch (index) {
    case 0:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 4, "     %d", ctx->value));
        return;
    case 2:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " L,R = FRMAE-+"));
        return;
    case 3:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " U,D = FRAME-+10"));
        return;
    case 4:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, "L1,R1= FRAME-+100"));
        return;
    case 5:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " RR  = ENTER"));
        return;
    case 6:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " RD  = CANCEL"));
        return;
    case 7:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " RL  = NOW FRAME"));
        return;
    case 8:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " ST  = CAMERA FOCUS"));
        break;
    case 9:
        break;
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFrameChangeMenu);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtGROUPMAXData);

void evtMainMenuItem(sdfDrawTag_t *list, s32 x, s32 y, s32 index, evtViewerInfo_t *ctx) {
    char labels[11][30] = {
        "SAVE PROJ    >>",
        "LOAD PROJ    >>",
        "LOAD NEW PROJ>>",
        "SET START FRAME",
        "SET END   FRAME",
        "SET TOTAL FRAME",
        "CAPTURE        ",
        "BATCH CAPTURE  ",
        "MOVE ALLFRAME ",
        "BISTA",
        "SKIP"
    };
    const char *primaryOptions[2] = {evtGROUPMAXData, "NOUSE"};
    const char *secondaryOptions[3] = {"BLACK OUT", "WHITE OUT", "NO FADE"};
    s32 style;

    if (index < 11) {
        if (ctx->fileMode == index) {
            if (ctx->nowmenu == 1) {
                style = 4;
            } else {
                style = 5;
            }
        } else {
            style = 0;
        }
        if (index == 9) {
            sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, style,
                "%s (%s)", labels[index], primaryOptions[evtMainMenuItemSub(ctx)]));
        } else if (index == 10) {
            sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, style,
                "%s (%s)", labels[index], secondaryOptions[evtEventSkipStartSub(ctx)]));
        } else {
            sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, style,
                evtFmtStr, labels[index]));
        }
    }
}

/* NON_MATCHING: 41 of 51 words match.  */
#ifdef NON_MATCHING
s32 evtMainMenu(s32 x, s32 y, evtViewerInfo_t *ctx) {
    sdfDrawTag_t *list = dds3FontReqAllocDrawTag();

    evtDrawMenuFrame(list, x, y, 15, 11, 0, 11, ctx, NULL, evtMainMenuItem);
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, list);
    if (ctx->nowmenu == 1) {
        return dds3SelectMenu(0, 1, 11, 1, 11, NULL, NULL, NULL, &ctx->fileMode);
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtMainMenu);
#endif

s32 evtSelectHead(sdfDrawTag_t *output, s32 x, s32 y, evtViewerInfo_t *work) {
    char *title = work->select_header;

    if (title == NULL) {
        return 0;
    }
    sdfDrawTagLink(output, EE_ADDR(sdfDevPrintf(x, y, 0xFEFFFF, 0, evtFmtStr, title)));
    return 2;
}

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFmtStr);

void evtSelectItem(sdfDrawTag_t *list, s32 x, s32 y, s32 index, evtViewerInfo_t *work) {
    s32 color;

    if (index < work->menuItemCount) {
        if (work->select_y == index) {
            color = work->nowmenu == 2 ? 4 : 5;
        } else {
            color = 0;
        }
        sdfDrawTagLink(list, EE_ADDR(sdfDevPrintf(x, y, 0xFEFFFF, color, " %s", work->itemNames[index])));
    }
}

/* NON_MATCHING: 55 of 63 words match.  */
#ifdef NON_MATCHING
s32 evtSelectMenu(s32 x, s32 y, evtViewerInfo_t *ctx) {
    sdfDrawTag_t *list = dds3FontReqAllocDrawTag();
    s32 width;
    s32 result;

    width = 10;
    if (ctx->select_header) {
        width = strlen(ctx->select_header);
        if (width < 6) {
            width = 6;
        }
    }
    evtDrawMenuFrame(list, x, y, width, ctx->menuItemCount + 3, 0, ctx->menuItemCount, ctx, evtSelectHead, evtSelectItem);
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, list);
    result = 0;
    if (ctx->nowmenu == 2) {
        return dds3SelectMenu(0, 1, ctx->menuItemCount, 1, ctx->menuItemCount, NULL, NULL, NULL, &ctx->select_y);
    }
    return result;
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSelectMenu);
#endif

s32 evtFileNameHead(sdfDrawTag_t *list, s32 x, s32 y, evtViewerInfo_t *work) {
    char text[16];

    sprintf(text, "[%s.%s]", work->nameStorage, work->extension);
    sdfDrawTagLink(list, EE_ADDR(sdfDevPrintf(x, y, 0xFEFFFF, 0, evtFmtStr, text)));
    return 2;
}

void evtFileNameItem(sdfDrawTag_t *list, s32 xPosition, s32 y, s32 row, evtViewerInfo_t *work) {
    s32 drawX = xPosition + 192;
    char *table = &evtFileNameChars[12 * row];
    s32 i = 0;
    s32 color;
    s32 ch;
    s32 cellX;

    do {
        color = 0;
        if (work->filename_y == row && work->filename_x == i) {
            color = work->nowmenu == 3 ? 4 : 5;
        }
        ch = *table++;
        cellX = drawX;
        drawX += 192;
        i++;
        sdfDrawTagLink(list, (u32)sdfDevPrintf(cellX, y, 0xFEFFFF, color, "%c", ch));
    } while (i < 11);
}

s32 evtFileNameMenu(s32 x, s32 y, evtViewerInfo_t *ctx)
{
    sdfDrawTag_t *list = dds3FontReqAllocDrawTag();
    s32 input;
    s32 length;
    s32 key;

    evtDrawMenuFrame(list, x, y, 0xE, 6, 0, 4, ctx, evtFileNameHead, evtFileNameItem);
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, list);
    if (ctx->nowmenu != 3) {
        return 0;
    }
    input = dds3SelectMenu(0, 0xB, 4, 0xB, 4, NULL, NULL, &ctx->filename_x,
                           &ctx->filename_y);
    if (input < 0) {
        return -1;
    }
    if (input == 1) {
        key = evtFileNameChars[ctx->filename_x + ctx->filename_y * 12];
        if (ctx->filename_y == 3 && ctx->filename_x >= 4) {
            switch (key) {
            case 'B':
            case 'S':
                for (length = 0; ctx->nameStorage[length] != 0; length++) {
                }
                if (length > 0) {
                    ctx->nameStorage[length - 1] = 0;
                }
                break;
            case 'K':
            case 'O':
                if (ctx->nameStorage[0] != 0) {
                    return 1;
                }
                break;
            }
        } else {
            for (length = 0; ctx->nameStorage[length] != 0; length++) {
            }
            if (length < 8) {
                ctx->nameStorage[length] = key;
                ctx->nameStorage[length + 1] = 0;
            }
        }
    }
    return 0;
}

s32 evtFileName2Head(sdfDrawTag_t *tag, s32 ox, s32 oy) {
    char name[32];

    sprintf(name, "[E%3d_%03d.PM1+2+3]", evtFileMajor, evt_file_minor);
    sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(ox, oy, 0xFEFFFF, 0, evtFmtStr, name)));
    return 2;
}

void evtFileName2Item(sdfDrawTag_t *list, s32 x, s32 y, u32 index, evtViewerInfo_t *ctx) {
    s32 color = 0;

    if (ctx->filename_y == index) {
        color = 4;
    }
    switch (index) {
    case 0:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 14, " EVENT"));
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x + 0x600, y, 0xFEFFFF, color, "%3d", evtFileMajor));
        return;
    case 1:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 14, "   CUT"));
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x + 0x600, y, 0xFEFFFF, color, "%03d", evt_file_minor));
        return;
    case 2:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 14, "   SEQ"));
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x + 0x600, y, 0xFEFFFF, color, "%03d", evtData14));
        return;
    case 4:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, " L,R = NO-+"));
        return;
    case 5:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, " U,D = SELECT"));
        return;
    case 6:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, " RR  = ENTER"));
        return;
    case 7:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, " RD  = CANCEL"));
        return;
    case 8:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, " RL  = SET 600"));
        break;
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFileName2Menu);

void evtObjectListSub(sdfDrawTag_t *list, evtViewerObject_t *selected, s32 x, s32 unused, evtViewerInfo_t *runtime) {
    EvtViewerNameIndex entryHeader;

    x += 1728;
    dds3ConsPanelTagLink(list, (x - 0x7000) >> 4, 60, 28, 3);
    sdfDrawTagLink(list, (u32)sdfDevPrintf(x, 0x7AE0, 0xFEFFFF, 14, "NAME:"));
    entryHeader = selected->entryHeader;
    if (entryHeader >= 0) {
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x + 960, 0x7AE0, 0xFEFFFF, 0, evtFmtStr, runtime->nametable[entryHeader]));
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtObjectListItem);

s32 evtOjbectListMenu(s32 x, s32 y, evtViewerInfo_t *ctx) {
    sdfDrawTag_t *list = dds3FontReqAllocDrawTag();
    s32 result;
    s32 objcnt;
    s32 visible;

    evtDrawMenuFrame(list, x, y, 8, 29, ctx->objlist_base, ctx->objcnt, ctx, NULL, evtObjectListItem);
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, list);
    result = 0;
    if (ctx->nowmenu == 4) {
        objcnt = ctx->objcnt;
        if (objcnt != 0) {
            visible = 29;
            if (objcnt < 29) {
                visible = ctx->objcnt;
            }
            return dds3SelectMenu(0, 1, ctx->objcnt, 1, visible, NULL, &ctx->objlist_base, NULL, &ctx->objlist_y);
        }
    }
    return result;
}

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

void evtBasicListItem(sdfDrawTag_t *list, s32 x, s32 y, s32 index, evtViewerInfo_t *ctx) {
    s32 color;
    s32 count;
    s32 i;
    dds3Basic_t *basic;

    color = 4;
    if (ctx->work_base + ctx->work_y != index) {
        color = 0;
    }
    if (index == 0) {
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, " -----------------------"));
        return;
    }
    count = 0;
    for (i = 0; i < 17; i++) {
        if (i != 3) {
            for (basic = ((dds3SceneBasic_t *)dds3GetActiveScene()->workdata)->basictable[i].firstbasic;
                 basic != NULL; basic = basic->next) {
                if (basic->name != NULL) {
                    count++;
                    if (count == index) {
                        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, " %s", basic->name));
                        return;
                    }
                }
            }
        }
    }
}

s32 evtBasicListMenu(s32 x, s32 y, evtViewerInfo_t *ctx) {
    s32 i;
    s32 count = 0;
    sdfDrawTag_t *list;
    s32 shown = 23;
    dds3Basic_t *basic;

    list = dds3FontReqAllocDrawTag();
    for (i = 0; i < 17; i++) {
        if (i != 3) {
            for (basic = ((dds3SceneBasic_t *)dds3GetActiveScene()->workdata)->basictable[i].firstbasic;
                 basic != NULL; basic = basic->next) {
                if (basic->name != NULL) {
                    count++;
                }
            }
        }
    }
    count++;
    evtDrawMenuFrame(list, x, y, 26, 23, ctx->work_base, count, ctx, NULL, evtBasicListItem);
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, list);
    if (ctx->nowmenu != 10) {
        return 0;
    }
    if (count < shown) {
        shown = count;
    }
    return dds3SelectMenu(0, 1, count, 1, shown, NULL, &ctx->work_base, NULL, &ctx->work_y);
}

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

void evtEventPosItem(sdfDrawTag_t *list, s32 x, s32 y, s32 index, evtViewerInfo_t *ctx) {
    dds3Basic_t *basic;
    s32 color;
    s32 slot;

    basic = NULL;
    color = 4;
    if (ctx->eventpos_y != index) {
        color = 0;
    }
    slot = evtGetSelectFrameInfo(ctx)->param.sb[index + 4];
    if (slot >= 0) {
        basic = dds3SceneBasicSearchName(dds3GetActiveScene(), ctx->nametable[slot]);
    }
    sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 14, "P%d:", index));
    if (basic != NULL) {
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, "   %s", basic->name));
    } else {
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, "   -----------------------"));
    }
}

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

s32 evtMESSAGEMENUMESMAX(sdfDrawTag_t *list, s32 x, s32 y, evtViewerInfo_t *work) {
    u32 count = itfGetMessageNo(work->workdata->MesHandle);
    sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, "MESSAGE MENU (MESMAX %3d)", count));
    return 2;
}

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNONEMESDATATableEntry);

void evtNONEMESDATA(sdfDrawTag_t *list, s32 x, s32 y, u32 kind, evtViewerInfo_t *ctx) {
    char *names[11] = {evtNONEMESDATATableEntry, "Branch Res0", "Branch Res1", "Branch Res2",
        "Branch Res3", "Branch Res4", "Branch Res5", "Branch Res6", "Branch Res7", "Branch Res8", "Branch Res9"};

    switch (kind) {
    case 0: {
        s32 color = 4;
        if (ctx->unk2410 != 0) {
            color = 0;
        }
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, "     %d", ctx->value & 0xFFF));
        if (ctx->workdata->MesHandle == -1) {
            sdfDrawTagLink(list, (u32)sdfDevPrintf(x + 0x600, y, 0xFEFFFF, color, "NONE MESDATA!!"));
        } else if (itfGetMessageType(ctx->workdata->MesHandle, ctx->value & 0xFFF) == 0) {
            sdfDrawTagLink(list, (u32)sdfDevPrintf(x + 0x600, y, 0xFEFFFF, color, "(NORMAL)"));
        } else {
            sdfDrawTagLink(list, (u32)sdfDevPrintf(x + 0x600, y, 0xFEFFFF, color, "(BRANCH)"));
        }
        break;
    }
    case 1: {
        s32 color = 4;
        if (ctx->unk2410 != 1) {
            color = 0;
        }
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, "     %s", names[(ctx->value >> 12) & 0xF]));
        break;
    }
    case 3:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " L,R = VALUE-+"));
        break;
    case 4:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " U,D = SELECT "));
        break;
    case 5:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " RR  = ENTER"));
        break;
    case 6:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " RD  = CANCEL"));
        break;
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCallMenuDisp2);

s32 mnuDrawCutFlagLabel(sdfDrawTag_t *target, s32 x, s32 y) {
    sdfDrawTagLink(target, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, "CUTFLAG MENU"));
    return 2;
}

void evtCMPVALUE(sdfDrawTag_t *list, s32 x, s32 y, u32 index, evtViewerInfo_t *ctx) {
    char *labels[11] = {evtCMPVALUETableEntry, "Branch Res0", "Branch Res1", "Branch Res2",
        "Branch Res3", "Branch Res4", "Branch Res5", "Branch Res6", "Branch Res7", "Branch Res8", "Branch Res9"};
    s32 flag;

    switch (index) {
    case 0:
        flag = ctx->timedActive != 0 ? 0 : 4;
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, flag, "     %s", labels[(ctx->value >> 12) & 0xF]));
        return;
    case 1:
        flag = ctx->timedActive != 1 ? 0 : 4;
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, flag, "     %d", ctx->value & 0xFFF));
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x + 0x600, y, 0xFEFFFF, flag, "  (CMP VALUE)"));
        return;
    case 3:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " L,R = VALUE-+"));
        return;
    case 4:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " U,D = SELECT "));
        return;
    case 5:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " RR  = ENTER"));
        return;
    case 6:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " RD  = CANCEL"));
        break;
    }
}

s32 evtCallMenuDisp3(s32 x, s32 y, evtViewerInfo_t *ctx) {
    sdfDrawTag_t *list;
    s32 packed;
    s32 number;
    s32 branch;
    s32 field;
    s32 delta;

    list = dds3FontReqAllocDrawTag();
    evtDrawMenuFrame(list, x, y, 0x18, 0xA, 0, 1, ctx, (EvtMenuHeaderFn)mnuDrawCutFlagLabel, (EvtMenuRowFn)evtCMPVALUE);
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, list);
    if (ctx->nowmenu != 0xE) {
        return 0;
    }
    packed = ctx->value;
    number = packed & 0xFFF;
    branch = (packed >> 12) & 0xF;
    field = ctx->timedActive;
    switch (field) {
    case 0:
        if (dds3PadButtonState.decOne & 2) {
            branch = branch == 0 ? 0xA : branch - 1;
        } else if (dds3PadButtonState.incOne & 2) {
            branch = branch >= 0xA ? 0 : branch + 1;
        }
        ctx->value &= 0xFFF;
        ctx->value |= branch << 12;
        break;
    case 1:
        delta = 0;
        if (dds3PadButtonState.decOne & 2) {
            delta = -1;
        } else if (dds3PadButtonState.incOne & 2) {
            delta = 1;
        }
        number += delta;
        if (number < ctx->min) {
            number = ctx->max;
        }
        if (number > ctx->max) {
            number = 0;
        }
        ctx->value = number | (branch << 12);
        break;
    }
    if ((dds3PadButtonState.decTen & 2) || (dds3PadButtonState.incTen & 2)) {
        ctx->timedActive = !field;
    }
    if (dds3PadButtonState.cancel < 0) {
        return 1;
    }
    return dds3PadButtonState.confirm >= 0 ? 0 : -1;
}

EvtMenuTableEntryAvailability evtIsMenuTableEntryEnabled(s32 *index) {
    return evtData6[*index].enabled != EVT_MENU_TABLE_ENTRY_DISABLED;
}

s32 evtLeaf10(evtViewerInfo_t *runtime) {
    return evtData6[runtime->framelist_target->type].fields[runtime->tableColumn];
}

/* NON_MATCHING: 22 of 178 words match.  */
#ifdef NON_MATCHING
void evtSOUTAI(sdfDrawTag_t *list, s32 x, s32 y, s32 hidden, evtViewerInfo_t *runtime) {
    s32 offset = 0;
    evtViewerObject_t *group = runtime->framelist_target;
    s32 i;
    s32 field;
    s32 style;

    if (hidden != 0) {
        return;
    }
    for (i = 0; i < evtData6[group->type].columns; i++) {
        field = evtData6[group->type].fields[i];
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x + offset * 192, y,
            0xFEFFFF, 14, evtSOUTAIData[field]));
        style = i == runtime->tableColumn && runtime->nowmenu == 15 ? 4 : 0;
        switch (field) {
        case 0:
            sdfDrawTagLink(list, (u32)sdfDevPrintf(x + (offset * 12 << 4),
                y + 0x80, 0xFEFFFF, style, "  %d", runtime->framelist_target->metadata.value));
            break;
        case 1:
            if (runtime->framelist_target->metadata.option < 3) {
                sdfDrawTagLink(list, (u32)sdfDevPrintf(x + (offset * 12 << 4),
                    y + 0x80, 0xFEFFFF, style, "%s",
                    evtSOUTAIData2[runtime->framelist_target->metadata.option]));
            } else {
                sdfDrawTagLink(list, (u32)sdfDevPrintf(x + (offset * 12 << 4),
                    y + 0x80, 0xFEFFFF, style, "%s",
                    runtime->nametable[runtime->framelist_target->metadata.extra2]));
            }
            break;
        case 2:
            if (runtime->framelist_target->metadata.option == 0) {
                sdfDrawTagLink(list, (u32)sdfDevPrintf(x + (offset * 12 << 4),
                    y + 0x80, 0xFEFFFF, style, "SOUTAI"));
            } else {
                sdfDrawTagLink(list, (u32)sdfDevPrintf(x + (offset * 12 << 4),
                    y + 0x80, 0xFEFFFF, style, "ZETTAI"));
            }
            break;
        }
        offset += evtSOUTAIColumnWidth[field];
    }
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSOUTAI);
#endif

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrSlightType);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrUnitAll);

s32 evtUpdateGroupPropertyDialog(s32 x, s32 y, evtViewerInfo_t *runtime) {
    sdfDrawTag_t *packets;
    evtViewerObject_t *group;
    evtFrameInfo_t *child;
    s32 column;
    s32 field;
    s32 count;

    packets = dds3FontReqAllocDrawTag();
    group = runtime->framelist_target;
    if (group == NULL) {
        return 0;
    }
    if (evtData6[group->type].columns == 0) {
        return 0;
    }
    evtDrawMenuFrame(packets, x, y, 28, 3, 0, 1, runtime, NULL, evtSOUTAI);
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, packets);
    if (runtime->nowmenu != 15) {
        return 0;
    }
    column = runtime->tableColumn;
    if (column >= evtData6[group->type].columns && evtData6[group->type].columns > 0) {
        column = runtime->tableColumn = evtData6[group->type].columns - 1;
    }
    field = evtData6[group->type].fields[column];
    if (dds3PadButtonState.incOne < 0) {
        if (column < evtData6[group->type].columns - 1) {
            runtime->tableColumn = column + 1;
        } else {
            runtime->tableColumn = 0;
        }
    } else if (dds3PadButtonState.decOne < 0) {
        if (column > 0) {
            runtime->tableColumn = column - 1;
        } else {
            runtime->tableColumn = evtData6[group->type].columns == 0 ? 0 : evtData6[group->type].columns - 1;
        }
    } else if (dds3PadButtonState.cancel < 0) {
        switch (field) {
        case 0:
            runtime->value = runtime->framelist_target->metadata.value;
            runtime->min = 0;
            runtime->max = runtime->totalframe - 1;
            evtRunFrame(runtime->nowframe, runtime);
            PushMenu(7, 180, 120, runtime);
            break;
        case 1:
            runtime->work_base = 0;
            runtime->work_y = 0;
            evtRunFrame(runtime->nowframe, runtime);
            PushMenu(17, 180, 120, runtime);
            break;
        case 2:
            runtime->select_y = runtime->framelist_target->metadata.option;
            runtime->menuItemCount = 2;
            runtime->select_header = "PATH APPLY MODE";
            runtime->itemNames = evtUpdateGroupPropertyDialogData;
            PushMenu(2, 216, 120, runtime);
            break;
        }
        return 1;
    } else if (dds3PadButtonState.frameEnd < 0) {
        if (column == 0) {
            runtime->framelist_x = 0;
        } else {
            runtime->framelist_x = 1;
        }
        count = 0;
        for (child = group->children; child != NULL; child = child->next) {
            count++;
        }
        if (count - 20 >= 0) {
            runtime->frameCursor = count - 19;
            runtime->frameFirst = 19;
        } else {
            runtime->frameFirst = count;
            runtime->frameCursor = 0;
        }
        return -1;
    } else if (dds3PadButtonState.frameStart < 0) {
        if (column == 0) {
            runtime->framelist_x = 0;
        } else {
            runtime->framelist_x = 1;
        }
        runtime->frameCursor = 0;
        runtime->frameFirst = 0;
        return -1;
    }
    return dds3PadButtonState.confirm >= 0 ? 0 : -1;
}

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

void evtCallDevPrintf(sdfDrawTag_t *list, s32 x, s32 y, s32 kind, evtViewerInfo_t *work) {
    switch (kind) {
    case 0:
        if (work->commandSecond.text != 0) {
            sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, evtFmtStr, work->commandSecond.text));
        }
        return;
    case 1:
        if (work->commandThird.text != 0) {
            sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, evtFmtStr, work->commandThird.text));
        }
        break;
    }
}

s32 evtCallMenuDisp7(s32 x, s32 y, evtViewerInfo_t *ctx) {
    sdfDrawTag_t *list = dds3FontReqAllocDrawTag();
    s32 count;

    evtDrawMenuFrame(list, x, y, 25, 2, 0, 1, ctx, NULL, evtCallDevPrintf);
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, list);
    if (ctx->nowmenu != 20) {
        return 0;
    }
    count = ctx->commandFirst;
    if (count > 0) {
        ctx->commandFirst = count - 1;
    } else if (count == 0) {
        return -1;
    }
    if (dds3PadButtonState.cancel < 0) {
        return 1;
    }
    return dds3PadButtonState.confirm >= 0 ? 0 : -1;
}

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

void evtViewerDrawShadowConfigRow(sdfDrawTag_t *list, s32 x, s32 y, s32 index, evtViewerInfo_t *ctx) {
    evtFrameInfo_t *node;
    s32 color;

    color = ctx->work_y + 2 == index ? 4 : 0;
    node = evtGetSelectFrameInfo(ctx);
    switch (index) {
    case 0:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, "SHADOW CONFIG"));
        return;
    case 2:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, "MODE   :%s", evtViewerDrawShadowConfigRowData[ctx->shadowMode]));
        return;
    case 3:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, "ALPHA  :%3d", ctx->shadowAlpha));
        return;
    case 4:
        if (node->p08.sb[0] == 3) {
            sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, "Y ZAHYO:%.1f", (double)ctx->shadowY));
        }
        break;
    }
}

s32 evtCallGetPIDfromUID6(s32 x, s32 y, evtViewerInfo_t *ctx) {
    evtFrameInfo_t *node = evtGetSelectFrameInfo(ctx);
    sdfDrawTag_t *list = dds3FontReqAllocDrawTag();
    s32 lastRow = 0;
    s32 lastMode = 0;

    evtDrawMenuFrame(list, x, y, 0x16, 6, 0, 0, ctx, 0, evtViewerDrawShadowConfigRow);
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, list);
    if (ctx->nowmenu != 0x16) {
        return 0;
    }
    switch (node->p08.sb[0]) {
    case 3:
        lastRow = 2;
        lastMode = 3;
        break;
    case 4:
        lastRow = 1;
        lastMode = 1;
        break;
    }
    if (dds3PadButtonState.incTen & 2) {
        if (ctx->work_y < lastRow) {
            ctx->work_y++;
        } else {
            ctx->work_y = 0;
        }
    } else if (dds3PadButtonState.decTen & 2) {
        if (ctx->work_y == 0) {
            ctx->work_y = lastRow;
        } else {
            ctx->work_y--;
        }
    } else if (dds3PadButtonState.incOne & 2) {
        switch (ctx->work_y) {
        case 0:
            if (ctx->shadowMode < lastMode) {
                ctx->shadowMode++;
            } else {
                ctx->shadowMode = 0;
            }
            break;
        case 1:
            ctx->shadowAlpha++;
            break;
        case 2:
            ctx->shadowY += 0.1f;
            break;
        }
    } else if (dds3PadButtonState.coarseIncrease & 2) {
        switch (ctx->work_y) {
        case 0:
            if (ctx->shadowMode < lastMode) {
                ctx->shadowMode++;
            } else {
                ctx->shadowMode = 0;
            }
            break;
        case 1:
            ctx->shadowAlpha += 10;
            break;
        case 2:
            ctx->shadowY += 1.0f;
            break;
        }
    } else if (dds3PadButtonState.decOne & 2) {
        switch (ctx->work_y) {
        case 0:
            if (ctx->shadowMode == 0) {
                ctx->shadowMode = lastMode;
            } else {
                ctx->shadowMode--;
            }
            break;
        case 1:
            ctx->shadowAlpha--;
            break;
        case 2:
            ctx->shadowY -= 0.1f;
            break;
        }
    } else if (dds3PadButtonState.coarseDecrease & 2) {
        switch (ctx->work_y) {
        case 0:
            if (ctx->shadowMode == 0) {
                ctx->shadowMode = lastMode;
            } else {
                ctx->shadowMode--;
            }
            break;
        case 1:
            ctx->shadowAlpha -= 10;
            break;
        case 2:
            ctx->shadowY -= 1.0f;
            break;
        }
    } else {
        if (dds3PadButtonState.cancel < 0) {
            return 1;
        }
        if (dds3PadButtonState.confirm < 0) {
            return -1;
        }
    }
    return 0;
}

dds3TaskUpdateResult_t evtCallGetProcessData19(dds3ProcessID_t *task) {
    evtViewerInfo_t *runtime = (evtViewerInfo_t *)dds3GetProcessData(task);
    if (effCallMemcpy3() == 0) {
        runtime->menuProcess = 0;
        return DDS3_TASK_UPDATE_FINISH_CALLBACK;
    }
    if (dds3GetProcessRunTime(task) == 0) {
        s32 selected = runtime->selectedEntry;
        evtActiveEntryFlags = selected;
        if (selected != 0) {
            evtCallGetProcessData19Data->selectedEntry = selected;
        }
    }
    if (evtCallGetProcessData19Data->selectedEntry != runtime->selectedEntry) {
        s32 selected = runtime->selectedEntry;
        if (selected != 0) {
            evtCallGetProcessData19Data->selectedEntry = selected;
        }
    }
    return DDS3_TASK_UPDATE_RETAIN_CALLBACK;
}

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

/* NON_MATCHING: 73 of 267 words match.  */
#ifdef NON_MATCHING
s32 evtCallConsPanelTagLinkZ(evtViewerInfo_t *runtime) {
    s32 x[4];
    s32 y[4];
    sdfDrawTag_t *list;
    evtFrameInfo_t *node;
    s32 component;
    s32 offset;
    s32 selectedPoint;
    u32 markerColor;
    s32 i = 0;

    list = dds3FontReqAllocDrawTag();
    dds3ConsPanelTagLinkZ(list, 120, 312, 12, 9, 0xFEFFF);
    markerColor = 0x80808080;
    node = evtGetSelectFrameInfo(runtime);
    component = runtime->curveComponent;
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7780, 0x82C0, 0xFEFFFF, 0, "P:"));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7780, 0x82C0, 0xFEFFFF, 0, "  %d", component));
    offset = component * 2;
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7780, 0x8320, 0xFEFFFF, 0, "X:"));
    selectedPoint = component + 1;
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7780, 0x8320, 0xFEFFFF, 0, "  %d", node->p08.sb[offset + 4]));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7C00, 0x8320, 0xFEFFFF, 0, "Y:"));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7C00, 0x8320, 0xFEFFFF, 0, "  %d", node->p08.sb[offset + 5]));
    x[0] = 0;
    y[0] = 0;
    x[1] = node->p0C.sb[0];
    y[1] = node->p0C.sb[1];
    x[2] = node->p0C.sb[2];
    y[2] = node->p0C.sb[3];
    x[3] = 128;
    y[3] = 128;
    for (; i < 4; i++) {
        u32 color = markerColor;
        if (selectedPoint == i) {
            color = 0x80008000;
        }
        sdfDrawTagLink(list, (u32)dds3PrimFrameRect((x[i] << 4) + 0x77E0, ((y[i] / 2) << 3) + 0x83B0, 0xFEFFFF, 0x40, 0x20, 0, color));
    }
    for (i = 0; i < 128; i++) {
        s32 y0 = mnuSolveCubicBezierYForX(i, x, y);
        s32 y1 = mnuSolveCubicBezierYForX(i + 1, x, y);
        sdfDrawTagLink(list, (u32)dds3PrimLine((i << 4) + 0x7800, ((y0 / 2) << 3) + 0x83C0, 0xFEFFFF, 0x80808080,
                                               ((i + 1) << 4) + 0x7800, ((y1 / 2) << 3) + 0x83C0, 0xFEFFFF, 0x80808080, 0));
    }
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, list);
    if (dds3PadButtonState.incTen & 2) {
        if (node->p08.sb[offset + 5] < 126) {
            node->p08.sb[offset + 5]++;
        }
    } else if (dds3PadButtonState.decTen & 2) {
        if (node->p08.sb[offset + 5] >= 2) {
            node->p08.sb[offset + 5]--;
        }
    } else if (dds3PadButtonState.decOne & 2) {
        if (node->p08.sb[offset + 4] >= 2) {
            node->p08.sb[offset + 4]--;
        }
    } else if (dds3PadButtonState.incOne & 2) {
        if (node->p08.sb[offset + 4] < 126) {
            node->p08.sb[offset + 4]++;
        }
    } else if (dds3PadButtonState.decHun & 2) {
        runtime->curveComponent ^= 1;
    } else if (dds3PadButtonState.incHun & 2) {
        runtime->curveComponent ^= 1;
    }
    if (dds3PadButtonState.confirm < 0) {
        runtime->framelist_target->parameterKeySource = 0;
        return 0;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCallConsPanelTagLinkZ);
#endif
INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrX);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrY);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrRight);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrQuick);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCMPVALUETableEntry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtSOUTAIColumnWidth);

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

