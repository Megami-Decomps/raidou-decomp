#include "common.h"
#include "sdfMemory.h"

#include "event.h"
#include "kernel_draw.h"
#include "newdata.h"
#include "sdf_resource.h"
#include "sdk_libc.h"
#include "include_asm.h"
#include "kernel.h"
#include "draw3d.h"
#include "basic.h"
#include "evt_world.h"
#include "billboard.h"
#include "sdf_types.h"
#include "draw.h"
#include "libsdf.h"
#include "sdf_movie_types.h"
#include "field.h"
#include "code_r1_001C4B78.h"
#include "evtEventViewer.h"
#include "code_r1_001CC1D0.h"
#include "code_r1_001CA8F8.h"

/* Register selectors carried by GIF A+D writes in the two GS contexts. */
enum {
    SDF_GS_PRIM = 0x00,
    SDF_GS_RGBAQ = 0x01,
    SDF_GS_XYZ2 = 0x05,
    SDF_GS_TEX0_1 = 0x06,
    SDF_GS_TEX0_2 = 0x07,
    SDF_GS_CLAMP_1 = 0x08,
    SDF_GS_CLAMP_2 = 0x09,
    SDF_GIF_REGISTER_AD = 0x0E,
    SDF_GS_TEX1_1 = 0x14,
    SDF_GS_TEX1_2 = 0x15,
    SDF_GS_XYOFFSET_1 = 0x18,
    SDF_GS_XYOFFSET_2 = 0x19,
    SDF_GS_PRMODECONT = 0x1A,
    SDF_GS_TEXA = 0x3B,
    SDF_GS_TEXFLUSH = 0x3F,
    SDF_GS_SCISSOR_1 = 0x40,
    SDF_GS_SCISSOR_2 = 0x41,
    SDF_GS_ALPHA_1 = 0x42,
    SDF_GS_ALPHA_2 = 0x43,
    SDF_GS_DTHE = 0x45,
    SDF_GS_COLCLAMP = 0x46,
    SDF_GS_TEST_1 = 0x47,
    SDF_GS_TEST_2 = 0x48,
    SDF_GS_FRAME_1 = 0x4C,
    SDF_GS_FRAME_2 = 0x4D,
    SDF_GS_ZBUF_1 = 0x4E,
    SDF_GS_ZBUF_2 = 0x4F,
    SDF_GS_BITBLTBUF = 0x50,
    SDF_GS_TRXPOS = 0x51,
    SDF_GS_TRXREG = 0x52,
    SDF_GS_TRXDIR = 0x53
};

typedef struct pmdTmxData_t {
    s32 x;          /* 0x00 */
    s32 y;          /* 0x04 */
    u8 color[4];    /* 0x08 */
    f32 scaleY;     /* 0x0C */
    f32 scaleX;     /* 0x10 */
    s32 enabled;    /* 0x14 */
    s32 variant;    /* 0x18 */
    s32 preserveAlpha; /* 0x1C: RGB-only alpha-fail drawing */
} pmdTmxData_t;

typedef char CampDisplayDefaults_size_is_20[
    (sizeof(pmdTmxData_t) == 0x20) ? 1 : -1];

/* Zero-based selections stored in the camp viewer's primary and secondary
 * option fields. The current menus expose two primary rows and three
 * secondary rows; setters preserve the packed two-bit representation. */
typedef u32 MnuCampPrimaryOptionIndex;

typedef u32 MnuCampSecondaryOptionIndex;

void evtEventViewerReleaseQueuedNode(evtViewerInfo_t *viewer, evtViewerObject_t *entry, EvtRuntimeChild *node);

#define CAMP_TASK_NAME_BYTES 0x20

#define CAMP_TASK_DATA_BYTES 0x48

#define CAMP_TIMELINE_WRAP_FRAME 10

#define CAMP_TRACK_FIRST_OFFSET_TYPE 0x12

#define CAMP_CAMERA_COLOR_TRACK_TYPE 0x19

#define CAMP_KEY_LOW_BITS_MASK 0xFFF

#define CAMP_KEY_HIGH_BITS_SHIFT 12

#define CAMP_ENTRY_VALUE_FOLLOW_ALT 0

#define CAMP_ENTRY_VALUE_CLEAR 1

#define CAMP_ENTRY_NAME_CODE_BASE 2

#define CAMP_FONT_CONTEXT_WIDTH 0x960

#define CAMP_FONT_CONTEXT_HEIGHT 0x70

#define CAMP_DESCRIPTOR_WIDTH 0x200

#define CAMP_DESCRIPTOR_HEIGHT 0xE0

#define CAMP_OPTION_VALUE_MASK 3

#define CAMP_PRIMARY_OPTION_CLEAR_MASK 0xfffffffc

#define CAMP_SECONDARY_OPTION_CLEAR_MASK 0xfffffff3

#define CAMP_SECONDARY_OPTION_SHIFT 2

#define CAMP_SECONDARY_OPTION_MASK 0xc

#define CAMP_TRANSFORM_COMPONENT_COUNT 4

#define CAMP_TRANSFORM_MIDDLE_START 4

#define CAMP_TRANSFORM_LAST_START 8

#define CAMP_REGISTERED_ID_LIMIT 20

#define CAMP_TIMELINE_SCRIPT_REGISTER_BASE 200

#define CAMP_TIMELINE_SCRIPT_SLOT_COUNT 10

#define CAMP_STATUS_BATCH_COUNT 2

#define CAMP_STATUS_INITIAL_PARAMETER 15

#define CAMP_SLOT_LAST_ROW_INDEX 0x14

#define CAMP_PARTY_SCAN_LAST 4

#define CAMP_PARTY_ACTIVE_FLAG 1

typedef struct CampFlagRow {
    u32 messageSet;  /* 0x00: shop message resource number */
    s16 flag[8];     /* 0x04 */
    u8 value[9];     /* 0x14: [0] default, [i + 1] for flag[i] */
    u8 pad1D[3];
} CampFlagRow;

void mnuFxWorldDropOutOfRange(evtViewerInfo_t *scene, s32 threshold);
void mnuFindCampKeyTrackNeighbors(evtViewerObject_t *track, s32 frame, EvtRuntimeChild **selected, EvtRuntimeChild **future);
void SetDefaultTmxData(pmdTmxData_t *display);
void mnuFindCampKeyTrackNeighbors(evtViewerObject_t *track, s32 frame, EvtRuntimeChild **lowerKey, EvtRuntimeChild **upperKey);
void evtHokanTmx(EvtRuntimeChild *from, EvtRuntimeChild *to, pmdTmxData_t *display, f32 ratio);
void evtGetValueNewTMXFrame(evtViewerInfo_t *viewer, evtViewerObject_t *track, s32 frame, pmdTmxData_t *display);
u32 evtFindInputBranchMessage(void);
void mnuUnpackNibbleFields(EvtRuntimeChild *key, s32 *lowBitsOut, s32 *highBitsOut);
void mnuCampCheckClockDivisor(void);

typedef struct CampEntryEnableSet {
    s32 enabled[7];
} CampEntryEnableSet;

/* Scan the twenty-one flag IDs in descending slot order. */

typedef struct ShopSourcePriceEntry {
    u16 itemId;
    u8 type;
    u8 pricePercent;
} ShopSourcePriceEntry;

typedef struct ShopSourcePriceRow {
    u16 pricePercent;
    ShopSourcePriceEntry entries[0x60];
} ShopSourcePriceRow;

typedef struct CampTier {
    u32 threshold;
    s32 value;
} CampTier;

static inline s32 campSetHandler(mnuTerminalContext_t *context, s32 mode, void *callback) {
    return jmsSeqChange(&context->transitionWork, &context->popupState, mode, callback);
}

typedef struct ShopProgressPrice {
    f32 percent;
    u32 unk4;
} ShopProgressPrice;

typedef struct effItemSlot44_t {
    u32 id;
    u8 pad_04[0x40];
} effItemSlot44_t;

/* A wide reward record carries its weight, kind, item ID and optional variant. */
typedef struct CampWideRewardEntry {
    u8 weight;
    u8 kind;
    u8 reserved02[2];
    s32 id;
    u16 variant;
    u8 reserved0A[2];
} CampWideRewardEntry;

typedef char CampWideRewardEntry_size_check[
    sizeof(CampWideRewardEntry) == 0xC ? 1 : -1];

typedef struct CampPriceTier {
    u8 limit;
    u8 percent;
} CampPriceTier;

typedef struct CampRateRow {
    u16 percent;
    u8 pad02[10];
} CampRateRow;

#define EVT_DISPATCH_OPERATION_POLL 0

#define EVT_DISPATCH_OPERATION_PRIMARY 1

#define EVT_DISPATCH_OPERATION_SECONDARY 2

#define EVT_PROGRESS_SLOT_COUNT 8

#define EVT_PROGRESS_RECORD_WORDS 3

#define EVT_ALLOWED_ITEM_COUNT 3

#define EVT_PROGRESS_UNSIGNED_LIMIT 999999U

#define EVT_PROGRESS_FLAG_GATE_COUNT 1

typedef struct evtFlagGate_t {
    s8 threshold;
    u8 pad1;
    u16 cue;
    u32 flag;
} evtFlagGate_t;

#define MNU_INSTALL_STATE_TABLE(ctx, table, entry) \
    do { \
        (ctx)->popupTable = (table); \
        jmsSeqJump(&(ctx)->popupState, (entry)); \
    } while (0)

struct mnuList_t;

struct dds3ProcessID_t;

INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtCallMessageDelete);

/* Process keys at or beyond threshold, restarting at the head after each call. */
void mnuFxWorldDropOutOfRange(evtViewerInfo_t *scene, s32 threshold) {
    evtViewerObject_t *track;
    EvtRuntimeChild *key;

    if (scene->objcnt <= 0) {
        return;
    }
    for (track = scene->firstobjinfo; track != NULL; track = track->next) {
        key = track->children;
        while (key != NULL) {
            if (key->frame + track->metadata.value < threshold) {
                key = key->next;
            } else {
                evtEventViewerReleaseQueuedNode(scene, track, key);
                key = track->children;
            }
        }
    }
}

INCLUDE_ASM(const s32, "code_r1_001CA8F8", mnuAdjustTimelineDurations);

INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtInitCustomSceneLightSub);

INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtGetValueNewSLightFrame);

void SetDefaultTmxData(pmdTmxData_t *display) {
    display->x = 0x100;
    display->y = 0xE0;
    display->color[0] = 0x80;
    display->color[1] = 0x80;
    display->color[2] = 0x80;
    display->color[3] = 0x80;
    display->scaleX = 1.0f;
    display->scaleY = 1.0f;
    display->enabled = 1;
    display->variant = 0;
    display->preserveAlpha = 0;
}

/* Use defaults before the first key; otherwise blend the bounding keys. */
void evtGetValueNewTMXFrame(evtViewerInfo_t *viewer, evtViewerObject_t *track, s32 frame, pmdTmxData_t *display) {
    EvtRuntimeChild *lowerKey;
    EvtRuntimeChild *upperKey;
    s32 startFrame;
    s32 endFrame;
    f32 ratio;

    mnuFindCampKeyTrackNeighbors(track, frame, &lowerKey, &upperKey);
    if (lowerKey != 0) {
        startFrame = lowerKey->frame;
    } else {
        SetDefaultTmxData(display);
        return;
    }
    if (upperKey != 0) {
        endFrame = upperKey->frame;
    } else {
        endFrame = lowerKey->frame;
    }
    if (endFrame != startFrame) {
        ratio = (f32)(frame - startFrame) / (f32)(endFrame - startFrame);
    } else {
        ratio = 0.0f;
    }
    evtHokanTmx(lowerKey, upperKey, display, ratio);
}

INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtHokanTmx);

void mnuFindCampKeyTrackNeighbors(evtViewerObject_t *track, s32 frame, EvtRuntimeChild **lowerKey, EvtRuntimeChild **upperKey) {
    s32 frameOffset;

    *lowerKey = 0;
    *upperKey = 0;
    if (track == 0) {
        return;
    }
    frameOffset = track->metadata.value;
    *upperKey = track->children;
    while (*upperKey != 0) {
        if (frame < (*upperKey)->frame + frameOffset) {
            break;
        }
        *upperKey = (*upperKey)->next;
    }
    if (*upperKey != 0) {
        *lowerKey = (*upperKey)->prev;
    } else {
        *lowerKey = track->lastChild;
    }
    if (track->type == 2) {
        while (*lowerKey != 0 &&
               evtViewerTestIndexedCondition((*lowerKey)->p0C.sh[0]) !=
                   EVT_VIEWER_INDEXED_CONDITION_MATCHED) {
            *lowerKey = (*lowerKey)->prev;
        }
    }
}

/* Retail never blocks the event viewer's end countdown through this hook. */
u32 evtFindInputBranchMessage(void) {
    return 0;
}

/* Split the key's packed halfword into its low 12 bits and upper four bits. */
void mnuUnpackNibbleFields(EvtRuntimeChild *key, s32 *lowBitsOut, s32 *highBitsOut) {
    *lowBitsOut = key->p08.h[0] & CAMP_KEY_LOW_BITS_MASK;
    *highBitsOut = key->p08.h[0] >> CAMP_KEY_HIGH_BITS_SHIFT;
}

INCLUDE_ASM(const s32, "code_r1_001CA8F8", mnuCampFindMatchingEntryIndex);

/* Return the first track with this fixed-width name, or NULL when absent. */
evtViewerObject_t *mnuCampFindEntryByName(evtViewerInfo_t *scene, const char *name) {
    evtViewerObject_t *track = scene->firstobjinfo;
    while (track != NULL) {
        if (strcmp(scene->nametable[track->entryHeader], name) == 0) {
            return track;
        }
        track = track->next;
    }
    return NULL;
}

INCLUDE_ASM(const s32, "code_r1_001CA8F8", campResolvePendingValue);

INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtDrawTexture);

INCLUDE_ASM(const s32, "code_r1_001CA8F8", fldResetCampSceneEntries);

INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtCallSetFontString);

INCLUDE_ASM(const s32, "code_r1_001CA8F8", mnuCampLinkFontGlyph);

/* Retail keeps only the divide-by-zero check (break 7) of a division whose result is never used. */
void mnuCampCheckClockDivisor(void) {
    sdfMemInfo_t heapStats;
    s32 quotient;

    sdfMemGetInfo(&heapStats);
    quotient = 1 / heapStats.total;
}

INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtRenderTexStart);

INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtCallAllocRenderTexture);

INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtCallMemAllocFromTailDirect);

INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtCallPrimAllocDrawTag);

INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtCallGrAllocPacketBuff);

INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtLeaf15);

void evtLeaf16(void) {
}

void evtLeaf17(evtViewerInfo_t *scene, MnuCampPrimaryOptionIndex optionIndex) {
    scene->optionFlags = (scene->optionFlags & CAMP_PRIMARY_OPTION_CLEAR_MASK) | (optionIndex & CAMP_OPTION_VALUE_MASK);
}

MnuCampPrimaryOptionIndex evtMainMenuItemSub(evtViewerInfo_t *scene) {
    return scene->optionFlags & CAMP_OPTION_VALUE_MASK;
}

void evtLeaf18(evtViewerInfo_t *scene, MnuCampSecondaryOptionIndex optionIndex) {
    scene->optionFlags = (scene->optionFlags & CAMP_SECONDARY_OPTION_CLEAR_MASK) | ((optionIndex & CAMP_OPTION_VALUE_MASK) << CAMP_SECONDARY_OPTION_SHIFT);
}

MnuCampSecondaryOptionIndex evtEventSkipStartSub(evtViewerInfo_t *scene) {
    return (scene->optionFlags & CAMP_SECONDARY_OPTION_MASK) >> CAMP_SECONDARY_OPTION_SHIFT;
}

INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtRefDDS3GBWK);

INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtLeaf19);

INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtLeaf20);

INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtLeaf21);

INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtLeaf22);

INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtEventFadeOutAllBgm);

INCLUDE_SDATA(const s32, "code_r1_001CA8F8", evtData22);

