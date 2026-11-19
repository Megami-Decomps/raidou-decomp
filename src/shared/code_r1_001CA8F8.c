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
#include "evtEventViewerMenu.h"
#include "code_r1_001CC1D0.h"
#include "code_r1_001CA8F8.h"
#include "frFont.h"
#include "evtPolygonMovie.h"
#include "evtSound.h"
#include "dds3KernelDraw.h"
#include "evtUnitManager.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "itfMisc.h"
#include "sdfGsMemIO.h"
#include "sdfPrim.h"

extern u8 evtCallSetFontStringData[];

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



void evtEventViewerReleaseQueuedNode(evtViewerInfo_t *viewer, evtViewerObject_t *entry, evtFrameInfo_t *node);

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

typedef struct campFlagRow_t {
    u32 messageSet;  /* 0x00: shop message resource number */
    s16 flag[8];     /* 0x04 */
    u8 value[9];     /* 0x14: [0] default, [i + 1] for flag[i] */
    u8 pad1D[3];
} campFlagRow_t;

void mnuFxWorldDropOutOfRange(evtViewerInfo_t *scene, s32 threshold);
void mnuFindCampKeyTrackNeighbors(evtViewerObject_t *track, s32 frame, evtFrameInfo_t **selected, evtFrameInfo_t **future);
void SetDefaultTmxData(pmdTmxData_t *display);
void mnuFindCampKeyTrackNeighbors(evtViewerObject_t *track, s32 frame, evtFrameInfo_t **lowerKey, evtFrameInfo_t **upperKey);
void evtHokanTmx(f32 ratio, evtFrameInfo_t *from, evtFrameInfo_t *to, pmdTmxData_t *display);
void evtGetValueNewTMXFrame(evtViewerInfo_t *viewer, evtViewerObject_t *track, s32 frame, pmdTmxData_t *display);
void evtGetMessageDataAndTypeFromFrame(evtFrameInfo_t *key, s32 *lowBitsOut, s32 *highBitsOut);
void mnuCampCheckClockDivisor(void);

typedef struct campEntryEnableSet_t {
    s32 enabled[7];
} campEntryEnableSet_t;

/* Scan the twenty-one flag IDs in descending slot order. */

typedef struct shpSourcePriceEntry_t {
    u16 itemId;
    u8 type;
    u8 pricePercent;
} shpSourcePriceEntry_t;

typedef struct shpSourcePriceRow_t {
    u16 pricePercent;
    shpSourcePriceEntry_t entries[0x60];
} shpSourcePriceRow_t;

typedef struct campTier_t {
    u32 threshold;
    s32 value;
} campTier_t;

static inline s32 campSetHandler(mnuTerminalContext_t *context, s32 mode, void *callback) {
    return jmsSeqChange(&context->transitionWork, &context->popupState, mode, callback);
}

typedef struct shpProgressPrice_t {
    f32 percent;
    u32 unk4;
} shpProgressPrice_t;

typedef struct effItemSlot44_t {
    u32 id;
    u8 pad_04[0x40];
} effItemSlot44_t;

/* A wide reward record carries its weight, kind, item ID and optional variant. */
typedef struct campWideRewardEntry_t {
    u8 weight;
    u8 kind;
    u8 reserved02[2];
    s32 id;
    u16 variant;
    u8 reserved0A[2];
} campWideRewardEntry_t;

typedef char campWideRewardEntry_size_check[
    sizeof(campWideRewardEntry_t) == 0xC ? 1 : -1];

typedef struct campPriceTier_t {
    u8 limit;
    u8 percent;
} campPriceTier_t;

typedef struct campRateRow_t {
    u16 percent;
    u8 pad02[10];
} campRateRow_t;

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
    evtFrameInfo_t *key;

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

/* NON_MATCHING: 110 of 119 words match.  */
#ifdef NON_MATCHING
void mnuAdjustTimelineDurations(evtViewerInfo_t *world, s32 threshold, s32 delta) {
    evtViewerObject_t *track;
    evtFrameInfo_t *key;
    s32 length;
    s32 offset;
    s32 i;
    s32 end = 0;

    for (track = world->firstobjinfo; track != NULL; track = track->next) {
        for (key = track->children; key != NULL; key = key->next) {
            for (i = 0; i < evtData5[track->type].columns; i++) {
                switch (evtData5[track->type].fields[i]) {
                case 1:
                case 15:
                    length = key->index;
                    if (length != 0) {
                        offset = key->frame;
                        if (offset < threshold) {
                            if (offset + length >= threshold) {
                                key->index = length + delta;
                            }
                        }
                    }
                    break;
                case 9:
                    switch (track->type) {
                    case 3:
                    case 0x14:
                    case 0x15:
                    case 0x1A:
                        end = key->p08.sh[1];
                        break;
                    case 0x12:
                    case 0x21:
                        end = key->p08.sh[0];
                        break;
                    }
                    if (key->frame < threshold && end >= threshold && end != 0) {
                        switch (track->type) {
                        case 3:
                        case 0x14:
                        case 0x15:
                        case 0x1A:
                            key->p08.sh[1] += delta;
                            if (key->p08.sh[1] < 0) {
                                key->p08.sh[1] = 0;
                            }
                            break;
                        case 0x12:
                        case 0x21:
                            key->p08.sh[0] += delta;
                            if (key->p08.sh[0] < 0) {
                                key->p08.sh[0] = 0;
                            }
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_001CA8F8", mnuAdjustTimelineDurations);
#endif

void evtInitCustomSceneLightSub(f32 (*color)[4], f32 (*dir)[4], f32 *ambient, f32 *soften, f32 *bias) {
    s32 i;

    for (i = 0; i < 3; i++) {
        color[i][0] = 0.7f;
        color[i][1] = 0.7f;
        color[i][2] = 0.7f;
        color[i][3] = 0.0f;
        dir[i][0] = 0.65f;
        dir[i][1] = 0.39f;
        dir[i][2] = 0.65f;
        dir[i][3] = 0.0f;
    }
    ambient[0] = 0.2f;
    ambient[1] = 0.2f;
    ambient[2] = 0.2f;
    ambient[3] = 1.0f;
    *soften = 7.0f;
    *bias = 0.0f;
}

/* NON_MATCHING: 127 of 142 words match.  */
#ifdef NON_MATCHING
void evtGetValueNewSLightFrame(evtViewerInfo_t *pInfo, evtViewerObject_t *obj, s32 nowframe, pmdSceneLightTable_t *sl, f32 *out_soften, f32 *out_bias) {
    evtFrameInfo_t *key1;
    evtFrameInfo_t *key2;
    pmdSceneLightTable_t *light1;
    pmdSceneLightTable_t *light2;
    s32 frame1;
    s32 frame2;
    s32 hokan;
    f32 ratio;
    f32 soften1;
    f32 bias1;
    f32 soften2;
    f32 bias2;
    s32 i;

    mnuFindCampKeyTrackNeighbors(obj, nowframe, &key1, &key2);
    if (key1 != NULL) {
        frame1 = key1->frame;
    } else {
        frame1 = pInfo->firstframe;
    }
    if (key2 != NULL) {
        frame2 = key2->frame;
    } else {
        frame2 = pInfo->lastframe.word;
    }
    if (frame2 != frame1) {
        ratio = (f32)(nowframe - frame1) / (f32)(frame2 - frame1);
    } else {
        ratio = 0.0f;
    }
    if (key1 == NULL) {
        evtInitCustomSceneLightSub(sl->color, sl->direction, sl->ambient, out_soften, out_bias);
        return;
    }
    hokan = key1->nameIndex;
    light1 = key1->payload;
    soften1 = key1->p08.f;
    bias1 = key1->p0C.f;
    if (key2 != NULL) {
        bias2 = key2->p0C.f;
        light2 = key2->payload;
        soften2 = key2->p08.f;
    } else {
        for (i = 0; i < 4; i++) {
            sl->color[0][i] = light1->color[0][i];
            sl->direction[0][i] = light1->direction[0][i];
            sl->color[1][i] = light1->color[1][i];
            sl->direction[1][i] = light1->direction[1][i];
            sl->color[2][i] = light1->color[2][i];
            sl->direction[2][i] = light1->direction[2][i];
            sl->ambient[i] = light1->ambient[i];
        }
        *out_soften = soften1;
        *out_bias = bias1;
        return;
    }
    if (light1 == NULL && light2 == NULL) {
        evtInitCustomSceneLightSub(sl->color, sl->direction, sl->ambient, out_soften, out_bias);
    } else if (hokan == 0) {
        for (i = 0; i < 4; i++) {
            sl->color[0][i] = light1->color[0][i];
            sl->direction[0][i] = light1->direction[0][i];
            sl->color[1][i] = light1->color[1][i];
            sl->direction[1][i] = light1->direction[1][i];
            sl->color[2][i] = light1->color[2][i];
            sl->direction[2][i] = light1->direction[2][i];
            sl->ambient[i] = light1->ambient[i];
        }
        *out_soften = soften1;
        *out_bias = bias1;
    } else {
        evtHokanSceneLight(hokan, ratio, light1, light2, sl);
        *out_soften = evtHokanFloat(hokan, ratio, soften1, soften2);
        *out_bias = evtHokanFloat(hokan, ratio, bias1, bias2);
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtGetValueNewSLightFrame);
#endif

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
    evtFrameInfo_t *lowerKey;
    evtFrameInfo_t *upperKey;
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
    evtHokanTmx(ratio, lowerKey, upperKey, display);
}

void evtHokanTmx(f32 ratio, evtFrameInfo_t *from, evtFrameInfo_t *to, pmdTmxData_t *display) {
    s32 blend;

    blend = 1;
    if (to != NULL) {
        if (from->p1C.sb[0] == 3) {
            blend = to->p1C.sb[0] == from->p1C.sb[0];
        } else {
            blend = to->p1C.sb[0] != 3;
        }
    }
    if (from == NULL) {
        SetDefaultTmxData(display);
        return;
    }
    if (to == NULL || ratio == 0.0f || from->p08.sh[0] == 0 || !blend) {
        display->x = from->p0C.sh[0];
        display->y = from->p0C.sh[1];
        display->color[0] = from->p10.b[0];
        display->color[1] = from->p10.b[1];
        display->color[2] = from->p10.b[2];
        display->color[3] = from->p10.b[3];
        display->scaleX = from->p14.f;
        display->scaleY = from->p18.f;
        display->enabled = from->p08.sb[0];
        display->variant = from->p08.sb[1];
    } else {
        display->x = from->p0C.sh[0] + (to->p0C.sh[0] - from->p0C.sh[0]) * ratio;
        display->y = from->p0C.sh[1] + (to->p0C.sh[1] - from->p0C.sh[1]) * ratio;
        display->color[0] = (u32)(from->p10.b[0] + (to->p10.b[0] - from->p10.b[0]) * ratio);
        display->color[1] = (u32)(from->p10.b[1] + (to->p10.b[1] - from->p10.b[1]) * ratio);
        display->color[2] = (u32)(from->p10.b[2] + (to->p10.b[2] - from->p10.b[2]) * ratio);
        display->color[3] = (u32)(from->p10.b[3] + (to->p10.b[3] - from->p10.b[3]) * ratio);
        display->scaleX = from->p14.f + (to->p14.f - from->p14.f) * ratio;
        display->scaleY = from->p18.f + (to->p18.f - from->p18.f) * ratio;
        display->enabled = from->p08.sb[0];
        display->variant = from->p08.sb[1];
    }
    display->preserveAlpha = from->p1C.sb[0];
}

void mnuFindCampKeyTrackNeighbors(evtViewerObject_t *track, s32 frame, evtFrameInfo_t **lowerKey, evtFrameInfo_t **upperKey) {
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
               sndVoiceStatus((*lowerKey)->p0C.sh[0]) !=
                   EVT_VIEWER_INDEXED_CONDITION_MATCHED) {
            *lowerKey = (*lowerKey)->prev;
        }
    }
}

/* Retail never blocks the event viewer's end countdown through this hook. */
u32 evtFindInputBranchMessage(evtViewerInfo_t *pInfo) {
    return 0;
}

/* Split the key's packed halfword into its low 12 bits and upper four bits. */
void evtGetMessageDataAndTypeFromFrame(evtFrameInfo_t *key, s32 *lowBitsOut, s32 *highBitsOut) {
    *lowBitsOut = key->p08.h[0] & CAMP_KEY_LOW_BITS_MASK;
    *highBitsOut = key->p08.h[0] >> CAMP_KEY_HIGH_BITS_SHIFT;
}

s32 mnuCampFindMatchingEntryIndex(struct evtPolygonMovieWork_t *wdata, evtViewerInfo_t *pInfo, s32 pm2nametable_idx) {
    evtViewerObject_t *track = pInfo->firstobjinfo;
    while (track != NULL) {
        if (strcmp(pInfo->nametable[track->entryHeader], wdata->nameTable[pm2nametable_idx]) == 0) {
            return track->entryHeader;
        }
        track = track->next;
    }
    return -1;
}

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

void campResolvePendingValue(evtViewerInfo_t *scene, evtFrameInfo_t *cue) {
    evtFrameInfo_t *linkedKey;
    s32 entryCode;
    u16 entryCodeBits;

    if (cue == NULL) {
        return;
    }
    entryCode = cue->p10.sh[0];
    entryCodeBits = cue->p10.sh[0];
    if (entryCode == 1) {
        scene->selectedEntry = 0;
        return;
    }
    if (entryCode == 0) {
        linkedKey = cue->prev;
        scene->selectedEntry = 0;
        for (; ; linkedKey = linkedKey->prev) {
            s32 linkedEntryCode;

            if (linkedKey == NULL) {
                return;
            }
            linkedEntryCode = linkedKey->p10.sh[0];
            if (linkedEntryCode != 0) {
                if (linkedEntryCode == 1) {
                    scene->selectedEntry = 0;
                    return;
                }
                scene->selectedEntry = (s32)mnuCampFindEntryByName(scene, scene->nametable[linkedEntryCode - 2])->texture;
                return;
            }
        }
    } else {
        scene->selectedEntry = (s32)mnuCampFindEntryByName(scene, scene->nametable[(s16)entryCodeBits - 2])->texture;
    }
}

void evtDrawTexture(sdfTexHandle_t *tex, pmdTmxData_t *display, s32 mask, sdfIVector2_t *maskOfs) {
    s32 width;
    s32 height;
    s32 variant;
    sdfDrawTag_t *tag;
    u32 ot;

    width = tex->width * display->scaleX;
    height = tex->height * display->scaleY;
    evtDrawTextureData2[2] = tex->width * 16;
    evtDrawTextureData2[3] = tex->height * 16;
    if (tex->width == 1024) {
        width--;
        evtDrawTextureData2[2] -= 16;
    }
    if (tex->height == 1024) {
        height--;
        evtDrawTextureData2[3] -= 16;
    }
    evtDrawTextureData[0] = (display->x - width / 2) * 16;
    evtDrawTextureData[1] = (display->y - height / 2) * 8;
    evtDrawTextureData[2] = (display->x + width / 2) * 16;
    evtDrawTextureData[3] = (display->y + height / 2) * 8;
    evtDrawTextureData3[0] = display->color[0];
    evtDrawTextureData3[1] = display->color[1];
    evtDrawTextureData3[2] = display->color[2];
    evtDrawTextureData3[3] = display->color[3];
    variant = display->variant;
    if (display->color[3] == 0) {
        return;
    }
    tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(32);
    sdfDrawTagInit(tag);
    if (variant == 0) {
        itfMiscDrawAlphaPrim(tag, 0, 0);
    } else if (variant == 1) {
        itfMiscDrawAlphaPrim(tag, 1, 0);
    } else if (variant == 2) {
        itfMiscDrawAlphaPrim(tag, 2, 0);
    }
    if (mask == 0) {
        switch (display->preserveAlpha) {
            case 1:
                ot = EVT_TEXTURE_OT_VARIANT1;
                break;
            case 2:
                ot = EVT_TEXTURE_OT_MASKED;
                break;
            default:
                ot = EVT_TEXTURE_OT_DEFAULT;
                break;
        }
    } else {
        ot = EVT_TEXTURE_OT_MASKED;
        evtDrawTextureData[0] += maskOfs->x;
        evtDrawTextureData[1] += maskOfs->y;
        evtDrawTextureData[2] += maskOfs->x;
        evtDrawTextureData[3] += maskOfs->y;
    }
    eglSetOt(ot);
    eglLinkAlpha(0);
    eglLinkTest(1, 0, 128, 3, 0, 0, 1, 1);
    itfMiscDrawSpritePrim(evtDrawTextureData, evtDrawTextureData2, evtDrawTextureData3, 255, tex, 0, tag);
    {
        sdfGrOt_t *otEntry = &sdfGrOtTable[ot];
        otEntry->regFunc(otEntry, tag);
    }
}

void fldResetCampSceneEntries(evtViewerInfo_t *scene) {
    evtViewerObject_t *track;

    track = scene->firstobjinfo;
    if (track != 0) {
        track->parameterKeySource = 0;
        while (track = track->next, track != 0) {
            track->parameterKeySource = 0;
        }
    }
    scene->colorEditorActive = 0;
}

void evtCallSetFontString(evtViewerInfo_t *pInfo) {
    frFontHandle_t *font;

    pInfo->glyph = NULL;
    font = frSetFontString(evtCallSetFontStringData, 0, 0, 0, NULL);
    pInfo->glyph = font;
    frSetDrawPosition(font, 0x960, 0x70);
}

void mnuCampLinkFontGlyph(evtViewerInfo_t *pInfo) {
    frDestroyFontRequestAfter(pInfo->glyph);
    pInfo->glyph = NULL;
}

/* Retail keeps only the divide-by-zero check (break 7) of a division whose result is never used. */
void mnuCampCheckClockDivisor(void) {
    sdfMemInfo_t heapStats;
    s32 quotient;

    sdfMemGetInfo(&heapStats);
    quotient = 1 / heapStats.total;
}

void evtRenderTexStart(evtViewerInfo_t *scene) {
    if ((scene->renderTexInfo.stage == 0) || (scene->renderTexInfo.stage == 5)) {
        scene->renderTexInfo.stage = 1;
    }
}

void evtCallAllocRenderTexture(evtViewerInfo_t *viewer) {
    s32 stage = viewer->renderTexInfo.stage;

    switch (stage) {
        case 0:
            return;
        case 1:
            if (dds3RenderTexture == NULL) {
                dds3AllocRenderTexture(512, 224, 100.75f);
            }
            viewer->renderTexInfo.stage++;
        case 3:
            evtCallMemAllocFromTailDirect(viewer);
            viewer->renderTexInfo.stage++;
        case 4:
            evtCallPrimAllocDrawTag(viewer);
            viewer->renderTexInfo.stage++;
        case 5:
            if (viewer->renderTexInfo.draw == 1) {
                evtLeaf15(viewer);
            }
            return;
        default:
            viewer->renderTexInfo.stage = stage + 1;
            return;
    }
}

void evtCallMemAllocFromTailDirect(evtViewerInfo_t *pInfo) {
    sdfMemHandle_t *allocation;
    sdfDrawTag_t *surface;
    sdfPreDrawTag_t *context;

    if (pInfo->renderTexInfo.hImageMem == NULL) {
        allocation = sdfMemAllocFromTailDirect(0x70000);
        pInfo->renderTexInfo.hImageMem = allocation;
        pInfo->renderTexInfo.pImage = sdfMemLock(allocation);
    }
    memset(pInfo->renderTexInfo.pImage, 0x40, 0x70000);
    surface = sdfPrimAllocDrawTag(NULL);
    context = (sdfPreDrawTag_t *)sdfGrAllocPacketBuff(0x10);
    sdfPreDrawTagInit(context);
    sdfLinkStoreFrameImage(surface, context, 0, 0, 0x200, 0xE0, (s32)pInfo->renderTexInfo.pImage, 0, 0, NULL);
    sdfPreDrawChainLink(&dds3PreDrawChain, context);
    dds3Data10.regFunc(&dds3Data10, surface);
}

void evtCallPrimAllocDrawTag(evtViewerInfo_t *scene) {
    sdfDrawTag_t *drawPacket;

    if (scene->renderTexInfo.pImage != NULL) {
        drawPacket = sdfPrimAllocDrawTag(0);
        sdfLinkLoadImage(drawPacket,
                                  dds3RenderTexture->pixelGsHandle, 0, 0,
                                  512, 224,
                                  (s32)scene->renderTexInfo.pImage, 0);
        dds3Data10.regFunc(&dds3Data10, drawPacket);
    }
}

INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtCallGrAllocPacketBuff);

INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtLeaf15);

void evtLeaf16(void) {
}

void evtSetBistaMode(evtViewerInfo_t *scene, MnuCampPrimaryOptionIndex optionIndex) {
    scene->optionFlags = (scene->optionFlags & CAMP_PRIMARY_OPTION_CLEAR_MASK) | (optionIndex & CAMP_OPTION_VALUE_MASK);
}

MnuCampPrimaryOptionIndex evtMainMenuItemSub(evtViewerInfo_t *scene) {
    return scene->optionFlags & CAMP_OPTION_VALUE_MASK;
}

void evtSetSkipMode(evtViewerInfo_t *scene, MnuCampSecondaryOptionIndex optionIndex) {
    scene->optionFlags = (scene->optionFlags & CAMP_SECONDARY_OPTION_CLEAR_MASK) | ((optionIndex & CAMP_OPTION_VALUE_MASK) << CAMP_SECONDARY_OPTION_SHIFT);
}

MnuCampSecondaryOptionIndex evtEventSkipStartSub(evtViewerInfo_t *scene) {
    return (scene->optionFlags & CAMP_SECONDARY_OPTION_MASK) >> CAMP_SECONDARY_OPTION_SHIFT;
}

INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtRefDDS3GBWK);

INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtLeaf19);

INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtLeaf20);

INCLUDE_ASM(const s32, "code_r1_001CA8F8", evtLeaf21);

void evtEventRegistBGM(evtViewerInfo_t *pInfo, s32 id) {
    s32 i;

    for (i = 0; i < pInfo->registeredCount; i++) {
        if (pInfo->registeredIds[i] == id) {
            return;
        }
    }
    if (pInfo->registeredCount < 20) {
        pInfo->registeredIds[pInfo->registeredCount] = id;
        pInfo->registeredCount++;
    }
}

void evtEventFadeOutAllBgm(evtViewerInfo_t *pInfo) {
    s32 i;

    for (i = 0; i < pInfo->registeredCount; i++) {
        evtStopEventBgm(pInfo->workdata->eventNo, pInfo->registeredIds[i]);
    }
    pInfo->registeredCount = 0;
}

INCLUDE_SDATA(const s32, "code_r1_001CA8F8", evtData22);

