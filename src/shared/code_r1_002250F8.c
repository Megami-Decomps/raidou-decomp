#include "common.h"
#include "raidou_alloc.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "sdfDraw.h"
#include "sdfTex.h"
#include "sdfPacketBuff.h"
#include "sdfImageSet.h"
#include "sdfGsMemIO.h"
#include "sdfModel.h"
#include "mdlManager.h"

#include "sdk_libc.h"
#include "include_asm.h"
#include "newdata.h"
#include "billboard.h"
#include "sdf_types.h"
#include "sdf_lighting.h"
#include "libsdf.h"
#include "model.h"
#include "motion.h"
#include "ps2asm.h"
#include "sdf_resource.h"
#include "draw.h"
#include "sdf_movie_types.h"
#include "effect.h"
#include "code_r1_0021C768.h"
#include "code_r1_002250F8.h"
#include "code_r1_00226EC8.h"
#include "effPCPMisc_00224D10.h"

enum {
    SDF_PSMCT32 = 0x00,
    SDF_PSMCT24 = 0x01,
    SDF_PSMCT16 = 0x02,
    SDF_PSMCT16S = 0x0A,
    SDF_PSMT8 = 0x13,
    SDF_PSMT4 = 0x14,
    SDF_PSMT8H = 0x1B,
};

/* Model-context lifetime, per-frame update, and broadcast accessors. */

/* Allocate a distinct caller-owned context linked to the registered group.
 * Contexts share the group node and assets; release each context with mdlFree. */
mdlObj_t *mdlLoad(u32 group, u32 id);
void mdlLoopMotion(mdlObj_t *ctx, s32 searchId, s32 motionIndex);
void mdlSetColor(mdlObj_t *ctx, u32 value);
void mdlSetPosition(mdlObj_t *ctx);
void mdlSetRotate(mdlObj_t *ctx);
void mdlSetScale(mdlObj_t *ctx);

/* sdfMotionCtrlr_t-record selection and lookup. */
void mdlSingleMotion(mdlObj_t *ctx, s32 searchId, s32 motionIndex);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */

/* Allocate and register a texture. The shared registration helper initializes
 * its ordinary reference count to one; the caller releases that reference with
 * sdfTexReleaseReference or its deferred form. */

/* Store the context-one resource packet on the texture's primary buffer. */

/* Intrusive model-object list node; the object payload stays opaque here. */
typedef struct effObjectNode_t {
    void *object;
    struct effObjectNode_t *prev;
    struct effObjectNode_t *next;
} effObjectNode_t;

typedef char effObjectNode_size_must_be_0xC[
    (sizeof(effObjectNode_t) == 0xC) ? 1 : -1];

/* Floor-list lifecycle and deferred-refresh state shared by both providers. */
enum {
    EFF_MODEL_OWNER_FLAG_UPDATE_PENDING = 0x01,
    EFF_MODEL_OWNER_FLAG_DESTROY_REQUESTED = 0x02,
    EFF_MODEL_OWNER_FLAG_FLOOR_LISTED = 0x04,
    EFF_MODEL_OWNER_FLAG_DESTROY_READY = 0x08,
    EFF_MODEL_OWNER_FLAG_LISTED_OBJECT = 0x10,
    EFF_MODEL_OWNER_FLAG_UPDATE_DEFERRED = 0x20
};

/* Kind-7 reference animation modes; other flag bits are uninterpreted. */
enum EffReferenceFlag {
    EFF_ANIM_SET_LOOP = 0x1,
    EFF_ANIM_SET_DOUBLE_Y_SCALE = 0x4,
};


#define SDF_TEXTURE_PAYLOAD_EXTRA_OFFSET_MASK 0xF0

/* Serialized texture-animation list header, count entry records, then payloads.
 * flags is copied to ctexlHandle_t and tested by animation sampling. */
typedef struct effExpandedSource_t {
    u8 pad00[4];
    u32 count;
    u32 flags;
    u8 pad0C[4];
    effExpandedEntry_t entries[1];
} effExpandedSource_t;

typedef char effExpandedSource_size_check[
    sizeof(effExpandedSource_t) == 0x20 ? 1 : -1];

#define EFF_MODEL_UPDATE_DEFER_FLOOR_REFRESH 0x1

extern struct ctexlHandle_t *effCacheTexCreate(u32);

/* Battle state: resource headers for texture slots 1 and 2. */
typedef struct effBattleTexHeaders_t {
    u8 pad_000[0x1F4];
    u32 statusFlags; /* 0x1F4 */
    u8 pad_1F8[0x30];
    struct EffBattleUnit *units; /* 0x228 */
    u8 pad_22C[0x350];
    u8 *slot1; // 0x57C
    u8 *slot2; // 0x580
} effBattleTexHeaders_t;

extern u32 D_003BC950;
extern u32 D_003BC954;
extern effObjectNode_t *effFloorModelListHead;
extern u32 effSharedTextureReferenceCount;
extern sdfTexHandle_t *D_003BC958;
extern void effFloorModelListRemove(effObjectNode_t *);

/* Initialize the VU transforms and the first node's float slot, if present. */
void Init_effModel(mdlObj_t *model);
extern void Init_effModel(mdlObj_t *);
mdlObj_t *effLoadViewerModelWithVUState(void *modelData, u32 modelDataSize);

/* Clear the lighting attachment before destroying its model context. */
void effDestroyModelContext(mdlObj_t *model);
void effDestroyModelOwner(effModelOwner_t *owner);
void effRecreateModelFromSource(effModelOwner_t *work, effModelOwner_t *source);
void effApplyModelPrimaryVector(effModelOwner_t *owner, const f32 *position);
void effApplyModelVecB(effModelOwner_t *owner, const f32 *rotation);
void effBroadcastModelMask(effModelOwner_t *owner, u32 color);
void effApplyScaledModelTertiaryVector(effModelOwner_t *owner, float scale);
void effFloorModelListPush(effModelOwner_t *obj);
void effMarkFloorModelForDestruction(effModelOwner_t *work);

/* Render the model owner with staged GS TEST_2 packets, then restore the tests. */
void effRenderModelOwnerWithGsTests(effModelOwner_t *owner);
extern u32 effModelUpdateControlFlags;
extern u64 effRenderModelOwnerWithGsTestsData2[];
void effMarkFloorModelForUpdate(effModelOwner_t *work);
extern char D_003B2AA0[];

void effFloorModelListRemove(effObjectNode_t *node);
void effSweepFloorModelList(void);
void mdlMarkAndProcessObjectNodes(void);

typedef struct effResourceOwner_t {
    u32 count;
    u32 currentFrame;
    effBehavior_RGBParams_t colorTrack; /* 0x08 */
    effBehavior_AlphaParams_t alphaTrack; /* 0x2C */
    u32 plainEntry; // 0x3C: add the model entry plain instead of flagged
    f32 frameStep; /* 0x40 */
    struct plistHandle_t **entries; /* 0x44: retained queue-pointer array */
    struct sdfMemHandle_t *queueAllocation; /* 0x48: queue-pointer array descriptor */
    mdlObj_t *model;
} effResourceOwner_t;

typedef char effResourceOwner_size_check[(sizeof(effResourceOwner_t) == 0x50) ? 1 : -1];

typedef struct effResourceModelConfig_t {
    effBehavior_RGBParams_t colorTrack;
    effBehavior_AlphaParams_t alphaTrack;
    u32 plainEntry;
    f32 frameStep;
} effResourceModelConfig_t;

typedef char effResourceModelConfig_size_check[
    sizeof(effResourceModelConfig_t) == 0x3C ? 1 : -1];

effResourceOwner_t *Create_effHelper(fileJobPayload_t *source);
void Delete_effHelper(effResourceOwner_t *owner);
extern effResourceOwner_t *Create_effHelper(fileJobPayload_t *);
extern void effResourceCopy(effResourceOwner_t *, effResourceOwner_t *);
effResourceOwner_t *Copy_effHelper(effResourceOwner_t *source);
void effResourceCopy(effResourceOwner_t *dst, effResourceOwner_t *src);
void effLoadModelPrimaryVector(effResourceOwner_t *work, const f32 *position);
void effSetModelRotationQuaternion(effResourceOwner_t *work, const f32 *rotationQuaternion);
void effPropagateResourceModelMask(effResourceOwner_t *work, u32 color);
void effScaleModelVec(effResourceOwner_t *work, float scale);
void effObjectListCountersReset(void);
sdfTexHandle_t *sdfGetOrCreateTextureForReference(sdfGrOt_t *surface, effRefObj_t *source);

/* Serialized expanded list: 0x10-byte header, the entry table, then each entry's payload. */
struct ctexlHandle_t *effCacheTexCreate(u32 sourceAddress);

void effRecreateModelFromSource(effModelOwner_t *work, effModelOwner_t *source) {
    effModelOwner_t *owner = work;
    effModelOwner_t *original = source;
    mdlObj_t *model;

    if (owner->model != 0) {
        effDestroyModelContext(owner->model);
    }
    model = mdlLoad(mdlGetMajorNum(original->model), mdlGetMinorNum(original->model));
    Init_effModel(model);
    SDF_VU0_SETSCALE1VECTOR();
    VU0_SCALE_VF_MFC1(vf10, owner->scale);
    mdlSetScale(model);
    owner->model = model;
}

INCLUDE_ASM(const s32, "code_r1_002250F8", effCallMotionSeek);

INCLUDE_ASM(const s32, "code_r1_002250F8", effCallSRAnimation2);

/* Pass a vector to the VU0 model helpers via vf10 (gcc cannot do this from plain C). */
void effApplyModelPrimaryVector(effModelOwner_t *owner, const f32 *position) {
    SDF_VU0_LOADVECTOR(position);
    mdlSetPosition(owner->model);
}

void effApplyModelVecB(effModelOwner_t *owner, const f32 *rotation) {
    SDF_VU0_LOADVECTOR(rotation);
    mdlSetRotate(owner->model);
}

void effBroadcastModelMask(effModelOwner_t *owner, u32 color) {
    mdlSetColor(owner->model, color);
}

void effApplyScaledModelTertiaryVector(effModelOwner_t *owner, float scale) {
    float v[4];
    float t;

    t = owner->scale * scale;
    v[0] = v[1] = v[2] = t;
    SDF_VU0_LOADVECTOR(v);
    mdlSetScale(owner->model);
}

void effLeaf350(void) {
}

INCLUDE_ASM(const s32, "code_r1_002250F8", effFunc13);

void effMarkFloorModelForDestruction(effModelOwner_t *work) {
    u32 flags = work->flags | EFF_MODEL_OWNER_FLAG_DESTROY_REQUESTED;
    work->flags = flags;
    if ((flags & EFF_MODEL_OWNER_FLAG_FLOOR_LISTED) == 0) {
        effDestroyModelOwner(work);
    }
}

INCLUDE_ASM(const s32, "code_r1_002250F8", effFunc14);

void effRenderModelOwnerWithGsTests(effModelOwner_t *owner) {
    sdfDrawTag_t *list;
    u64 *packet;
    s32 index;

    mdlSetColor(owner->model, (mdlGetColor(owner->model) & 0xFFFFFF) | 0x80000000);

    for (index = 0; index != 4; index++) {
        list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(list);
        packet = (u64 *)sdfGrAllocPacketBuff(0x30);
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        if (index == 0) {
            packet[4] = 0x7000D;
        } else {
            packet[4] = 0x71007;
        }
        packet[5] = 0x47;
        sdfDrawTagLink(list, (u32)packet);
        ModelOt[2].ot[index]->regFunc(ModelOt[2].ot[index], list);
    }

    if (effComputeLightDirectionVU(owner->model, owner->ownedBuffer)) {
        owner->model->inner->light = owner->ownedBuffer;
    }

    mdlLink(owner->model, &ModelOt[2]);

    for (index = 1; index != 4; index++) {
        list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(list);
        packet = (u64 *)sdfGrAllocPacketBuff(0x30);
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[4] = effRenderModelOwnerWithGsTestsData2[index];
        packet[5] = 0x47;
        sdfDrawTagLink(list, (u32)packet);
        ModelOt[2].ot[index]->regFunc(ModelOt[2].ot[index], list);
    }
}

void effMarkFloorModelForUpdate(effModelOwner_t *work) {
    u32 previous = work->flags;
    u32 flags = previous | EFF_MODEL_OWNER_FLAG_UPDATE_PENDING;
    work->flags = flags;
    if ((flags & EFF_MODEL_OWNER_FLAG_FLOOR_LISTED) == 0) {
        if ((effModelUpdateControlFlags & EFF_MODEL_UPDATE_DEFER_FLOOR_REFRESH) == 0) {
            effRenderModelOwnerWithGsTests(work);
        }
    } else if ((effModelUpdateControlFlags & EFF_MODEL_UPDATE_DEFER_FLOOR_REFRESH) != 0) {
        work->flags = previous | EFF_MODEL_OWNER_FLAG_UPDATE_PENDING |
                      EFF_MODEL_OWNER_FLAG_LISTED_OBJECT | EFF_MODEL_OWNER_FLAG_UPDATE_DEFERRED;
    }
}

void effFloorModelListPush(effModelOwner_t *obj) {
    effObjectNode_t *entry = sdfChipAllocZeroDirect(sizeof(effObjectNode_t));

    entry->object = obj;
    entry->prev = NULL;
    if (effFloorModelListHead != NULL) {
        effFloorModelListHead->prev = entry;
        entry->next = effFloorModelListHead;
    } else {
        entry->next = NULL;
    }
    effFloorModelListHead = entry;
    ((effModelOwner_t *)entry->object)->flags |= EFF_MODEL_OWNER_FLAG_FLOOR_LISTED;
}

void effFloorModelListRemove(effObjectNode_t *node) {
    if (node->object != NULL) {
        printf(D_003B2AA0, node->object);
        effDestroyModelOwner(node->object);
        node->object = NULL;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        effFloorModelListHead = node->next;
    }
    sdfChipFreeDirect(node);
}

void effSweepFloorModelList(void) {
    effObjectNode_t *node = effFloorModelListHead;
    effObjectNode_t *next;

    if (node != NULL) {
        do {
            next = node->next;
            if (!(((effModelOwner_t *)node->object)->flags & EFF_MODEL_OWNER_FLAG_DESTROY_READY)) {
                if ((((effModelOwner_t *)node->object)->flags &
                     (EFF_MODEL_OWNER_FLAG_UPDATE_PENDING | EFF_MODEL_OWNER_FLAG_LISTED_OBJECT |
                      EFF_MODEL_OWNER_FLAG_UPDATE_DEFERRED)) == EFF_MODEL_OWNER_FLAG_UPDATE_PENDING) {
                    effRenderModelOwnerWithGsTests(node->object);
                }
                ((effModelOwner_t *)node->object)->flags &= ~EFF_MODEL_OWNER_FLAG_UPDATE_DEFERRED;
            } else {
                effFloorModelListRemove(node);
            }
            node = next;
        } while (node != NULL);
    }
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_0029A840.c:416, rework, same function 1.00 (words 15/15, calls 1.00, shape 1.00) */
void mdlPropagateObjectFlag(void) {
    effObjectNode_t *node = effFloorModelListHead;
    effModelOwner_t *object;
    s32 flags;

    if (node != NULL) {
        do {
            object = node->object;
            flags = object->flags;
            if ((flags & EFF_MODEL_OWNER_FLAG_DESTROY_REQUESTED) != 0) {
                object->flags = flags | EFF_MODEL_OWNER_FLAG_DESTROY_READY;
            }
            node = node->next;
        } while (node != NULL);
    }
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_0029A840.c:433, rework, same function 1.00 (words 13/13, calls 1.00, shape 1.00) */
void mdlClearListedObjectFlag(void) {
    effModelOwner_t *object;
    effObjectNode_t *node;

    node = effFloorModelListHead;
    while (node != NULL) {
        object = node->object;
        node = node->next;
        object->flags = object->flags & ~EFF_MODEL_OWNER_FLAG_LISTED_OBJECT;
    }
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_0029A840.c:445, rework, same function 1.00 (words 13/13, calls 1.00, shape 1.00) */
void mdlSetListedObjectFlag(void) {
    effModelOwner_t *object;
    effObjectNode_t *node;

    node = effFloorModelListHead;
    while (node != NULL) {
        object = node->object;
        node = node->next;
        object->flags = object->flags | EFF_MODEL_OWNER_FLAG_LISTED_OBJECT;
    }
}

void mdlMarkAndProcessObjectNodes(void) {
    effObjectNode_t *node = effFloorModelListHead;
    effModelOwner_t *object;
    effObjectNode_t *next;
    s32 flags;

    if (node == NULL) {
        return;
    }
    do {
        object = node->object;
        next = node->next;
        flags = object->flags | EFF_MODEL_OWNER_FLAG_DESTROY_REQUESTED |
                EFF_MODEL_OWNER_FLAG_DESTROY_READY;
        object->flags = flags;
        effFloorModelListRemove(node);
        node = next;
    } while (node != NULL);
}

emdlHandle_t *Create_effModel(fileJobPayload_t *job) {
    emdlHandle_t *handle = sdfChipAllocZeroDirect(sizeof(emdlHandle_t));
    const u8 *data;

    handle->pLightPacket = sdfChipAllocZeroDirect(0xE0);
    if (job != NULL) {
        handle->param = *(effModel_Params_t *)fileResolvePrimaryBuffer(job);
        data = fileResolveSecondaryBuffer(job);
        if (data != NULL) {
            handle->handle = effHandleCreate(data, job->secondary.size);
            SDF_VU0_SETSCALE1VECTOR();
            VU0_SCALE_VF_MFC1(vf10, handle->param.scale);
            mdlSetScale(handle->handle);
            if (handle->handle->first != NULL && !handle->param.flg) {
                mdlLoopMotion(handle->handle, 0, 0);
            }
        }
    }
    return handle;
}

INCLUDE_ASM(const s32, "code_r1_002250F8", Delete_effModel);

INCLUDE_ASM(const s32, "code_r1_002250F8", Copy_effModel);

INCLUDE_ASM(const s32, "code_r1_002250F8", ResourceCopy_effModel);

INCLUDE_ASM(const s32, "code_r1_002250F8", effCallMotionSeek2);

INCLUDE_ASM(const s32, "code_r1_002250F8", effCallSRAnimation3);

void PositionSet_effModel(emdlHandle_t *handle, sdfVector4_t *pos) {
    SDF_VU0_LOADVECTOR(pos);
    mdlSetPosition(handle->handle);
}

void RotateSet_effModel(emdlHandle_t *handle, sdfVector4_t *rot) {
    SDF_VU0_LOADVECTOR(rot);
    mdlSetRotate(handle->handle);
}

void ColorSet_effModel(emdlHandle_t *handle, u32 color) {
    mdlSetColor(handle->handle, color);
}

void ScaleSet_effModel(emdlHandle_t *handle, f32 scale) {
    f32 v[4];
    f32 t;

    t = handle->param.scale * scale;
    v[0] = v[1] = v[2] = t;
    SDF_VU0_LOADVECTOR(v);
    mdlSetScale(handle->handle);
}

/* Create an empty owner, or load its model and per-record queues from a file job. */
effResourceOwner_t *Create_effHelper(fileJobPayload_t *source) {
    effResourceOwner_t *owner = sdfChipAllocDirect(sizeof(effResourceOwner_t));
    u8 *data;
    u32 i;

    memset(owner, 0, sizeof(effResourceOwner_t));
    if (source != NULL) {
        data = fileResolvePrimaryBuffer(source);
        memcpy((u8 *)owner + 8, data, sizeof(effResourceModelConfig_t));
        owner->model = effLoadViewerModelWithVUState(data + 0x40,
                                                   source->primary.size - 0x40);
        if (owner->model->first != NULL) {
            if (owner->plainEntry != 0) {
                mdlSingleMotion(owner->model, 0, 0);
            } else {
                mdlLoopMotion(owner->model, 0, 0);
            }
        }
        owner->count = sdfModelGetMarkPosCount(owner->model->inner);
        if (owner->count == 0) {
            return owner;
        }
        data = (u8 *)fileResolveSecondaryBuffer(source);
        if (data != NULL) {
            owner->queueAllocation = sdfMemAllocDirect(owner->count * 4);
            owner->entries = sdfMemLock(owner->queueAllocation);
            owner->entries[0] = Create_effPartsList((struct plistHandle_t *)data);
            for (i = 1; i < owner->count; i++) {
                owner->entries[i] = effPartsCreateFinish(owner->entries[0]);
            }
        }
    }
    return owner;
}

void Delete_effHelper(effResourceOwner_t *owner) {
    u32 i;

    if (owner->model != 0) {
        effDestroyModelContext(owner->model);
    }
    if (owner->queueAllocation != 0) {
        for (i = 0; i < owner->count; i++) {
            effPartsDelete(owner->entries[i]);
        }
        sdfMemFreeDirect(owner->queueAllocation);
    }
    sdfChipFreeDirect(owner);
}

effResourceOwner_t *Copy_effHelper(effResourceOwner_t *source) {
    effResourceOwner_t *effect = Create_effHelper(0);

    *(effResourceModelConfig_t *)((u8 *)effect + 8) =
        *(effResourceModelConfig_t *)((u8 *)source + 8);
    effResourceCopy(effect, source);
    return effect;
}

void effResourceCopy(effResourceOwner_t *dst, effResourceOwner_t *src) {
    u32 i;

    if (dst->model != 0) {
        effDestroyModelContext(dst->model);
    }
    dst->model = mdlLoad(mdlGetMajorNum(src->model), mdlGetMinorNum(src->model));
    Init_effModel(dst->model);
    if (dst->model->first != NULL) {
        if (dst->plainEntry != 0) {
            mdlSingleMotion(dst->model, 0, 0);
        } else {
            mdlLoopMotion(dst->model, 0, 0);
        }
    }
    dst->count = sdfModelGetMarkPosCount(dst->model->inner);
    if (src->queueAllocation != 0) {
        if (dst->queueAllocation != 0) {
            for (i = 0; i < dst->count; i++) {
                effPartsDelete(dst->entries[i]);
            }
            sdfMemFreeDirect(dst->queueAllocation);
        }
        dst->queueAllocation = sdfMemAllocDirect(dst->count * 4);
        dst->entries = sdfMemLock(dst->queueAllocation);
        for (i = 0; i < dst->count; i++) {
            dst->entries[i] = effPartsCreateFinish(*src->entries);
        }
    }
}

INCLUDE_ASM(const s32, "code_r1_002250F8", effReleaseReferenceHolder);

INCLUDE_ASM(const s32, "code_r1_002250F8", Draw_effHelper);

/* Pass a vector to the VU0 model helpers via vf10 (gcc cannot do this from plain C). */
void effLoadModelPrimaryVector(effResourceOwner_t *work, const f32 *position) {
    SDF_VU0_LOADVECTOR(position);
    mdlSetPosition(work->model);
}

void effSetModelRotationQuaternion(effResourceOwner_t *work, const f32 *rotationQuaternion) {
    SDF_VU0_LOADVECTOR(rotationQuaternion);
    mdlSetRotate(work->model);
}

void effPropagateResourceModelMask(effResourceOwner_t *work, u32 color) {
    mdlSetColor(work->model, color);
}

void effScaleModelVec(effResourceOwner_t *work, float scale) {
    u32 bits;

    SDF_VU0_SETSCALE1VECTOR();
    VU0_SCALAR_OP_TMP(bits, scale, "vmulx.xyzw vf10, vf10, vf2x");
    mdlSetScale(work->model);
}

void effObjectListCountersReset(void) {
    D_003BC954 = 0;
    D_003BC950 = 0xffffffff;
}

effRefObj_t *effCreateSharedTextureReference(sdfTextureFileHeader_t *source) {
    u32 paletteWidth;
    u32 paletteHeight;
    s32 paletteBytes;
    s32 imageBytes;
    s32 payloadBytes;
    s32 textureOffset;
    struct sdfMemHandle_t *allocationHandle;
    u8 *cursor;
    effRefObj_t *texture;

    if ((source->pixelFormat == 0x13) || (source->pixelFormat == 0x1B)) {
        paletteWidth = 0x10;
        paletteHeight = 0x10;
    } else {
        paletteWidth = 8;
        paletteHeight = 2;
    }

    paletteBytes = sdfImageDataSize(source->clutFormat, paletteWidth, paletteHeight) << 4;
    imageBytes = sdfImageDataSize(source->pixelFormat, source->width, source->height) << 4;
    payloadBytes = imageBytes + paletteBytes;
    textureOffset = payloadBytes + 0x40;
    allocationHandle = sdfMemAllocDirect(payloadBytes + 0x60);
    cursor = (u8 *)sdfMemLock(allocationHandle);
    texture = (effRefObj_t *)(cursor + textureOffset);
    texture->base = cursor;
    cursor += 0x40;
    texture->palette = cursor;
    cursor += paletteBytes;
    texture->pixels = cursor;
    texture->paletteWidth = paletteWidth;
    texture->paletteHeight = paletteHeight;
    texture->refCount = 0;
    texture->index = -1;
    texture->allocationHandle = allocationHandle;

    memcpy(texture->base, source, 0x40);
    cursor = (u8 *)source + 0x40 + (source->payloadOffsetFlags & SDF_TEXTURE_PAYLOAD_EXTRA_OFFSET_MASK);
    memcpy(texture->palette, cursor, paletteBytes);
    cursor += paletteBytes;
    memcpy(texture->pixels, cursor, imageBytes);

    if (effSharedTextureReferenceCount == 0) {
        D_003BC958 = sdfTexAlloc(0x100, 0x100, SDF_PSMT8, 0, 0, 1);
        sdfTexMakeGIFPacket(D_003BC958);
    }
    effSharedTextureReferenceCount++;
    texture->refCount++;
    return texture;
}

effRefObj_t *effCloneSharedReferenceWithValue(sdfTextureFileHeader_t *source, u32 textureIndex) {
    effRefObj_t *copy;

    copy = effCreateSharedTextureReference(source);
    copy->index = textureIndex;
    return copy;
}

void effReleaseSharedReference(effRefObj_t *obj) {
    effSharedTextureReferenceCount--;
    if (effSharedTextureReferenceCount == 0) {
        sdfTexHandle_t *graphics = D_003BC958;
        graphics->width = 0x100;
        graphics->height = 0x100;
        D_003BC950 = -1;
        sdfTexReleaseReference(graphics);
    }
    obj->refCount--;
    if (obj->refCount == 0) {
        sdfMemFreeDirect(obj->allocationHandle);
    }
}

effRefObj_t *effRetainSharedReference(effRefObj_t *obj) {
    obj->refCount++;
    effSharedTextureReferenceCount++;
    return obj;
}

/* Native GS dimension SDK calls: 0029C16C/0029C17C.
 * Address normalization before TEX0 packing is also traced in
 * texture_renew_secondary_epoch_probe (parity OK, local-alloc q14/q16). */
sdfTexHandle_t *sdfGetOrCreateTextureForReference(sdfGrOt_t *surface, effRefObj_t *source) {
    sdfDrawTag_t *list;
    u16 width;
    u16 height;
    s32 widthLog2;
    s32 heightLog2;
    sdfTexHandle_t *texture;
    u32 primary;
    u32 secondary;
    u8 clutFormat;
    u8 pixelFormat;

    if (source->index != -1 && D_003BC950 == source->index && D_003BC954 == (u32)surface) {
        return D_003BC958;
    }
    D_003BC950 = source->index;
    D_003BC954 = (u32)surface;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    sdfLinkLoadImage(list, D_003BC958->clutGsHandle, 0, 0, source->paletteWidth, source->paletteHeight, (s32)source->palette, 0);
    sdfLinkLoadImage(list, D_003BC958->pixelGsHandle, 0, 0, ((sdfTextureFileHeader_t *)source->base)->width, ((sdfTextureFileHeader_t *)source->base)->height, (s32)source->pixels, 0);
    surface->regFunc(surface, list);
    texture = D_003BC958;
    width = ((sdfTextureFileHeader_t *)source->base)->width;
    texture->width = width;
    widthLog2 = (s16)width - 1;
    primary = texture->pixelGsHandle->addr >> 6;
    height = ((sdfTextureFileHeader_t *)source->base)->height;
    texture->height = height;
    clutFormat = texture->CLUTFormat;
    secondary = texture->clutGsHandle->addr;
    pixelFormat = texture->pixelFormat;
    heightLog2 = (s16)height - 1;
    EE_MMI_BIT_LENGTH(heightLog2);
    EE_MMI_BIT_LENGTH(widthLog2);
    secondary >>= 6;
    texture->packet->tex0.data =
        ((u64)2 << 61) |
        ((u64)clutFormat << 51) |
        ((u64)secondary << 37) |
        ((u64)1 << 34) |
        ((u64)heightLog2 << 30) |
        (widthLog2 << 26) |
        ((u64)pixelFormat << 20) |
        (4 << 14) |
        (u64)primary;
    return D_003BC958;
}

s32 effCountExpandedEntries(const ctexlHandle_t *list) {
    u32 count = list->count;
    u32 i = 0;
    s32 total = 0;

    if (count != 0) {
        const effExpandedEntry_t *entries = list->entries;
        do {
            s32 additionalCount = entries->additionalCount;
            entries++;
            i++;
            total++;
            total += additionalCount;
        } while (i < count);
    }
    return total;
}

/* Serialized expanded list: 0x10-byte header, the entry table, then each entry's payload. */
/* Copy a serialized list into one allocation and retain a shared texture reference per entry. */
struct ctexlHandle_t *effCacheTexCreate(u32 sourceAddress) {
    const effExpandedSource_t *source = (const effExpandedSource_t *)sourceAddress;
    u32 count = source->count;
    const effExpandedEntry_t *sourceEntry = source->entries;
    u32 i = 0;
    struct sdfMemHandle_t *allocation;
    ctexlHandle_t *list;
    u8 *payload;

    allocation = sdfMemAllocDirect(count * (sizeof(effExpandedEntry_t) + sizeof(effRefObj_t *)) + sizeof(ctexlHandle_t));
    list = (ctexlHandle_t *)sdfMemLock(allocation);
    list->entries = (effExpandedEntry_t *)(list + 1);
    list->handles = (effRefObj_t **)(list->entries + count);
    list->unk20 = -1;
    list->allocation = allocation;
    list->refCount = 0;
    memcpy(list, source, 0x10);
    payload = (u8 *)(sourceEntry + count);
    if (count != 0) {
        do {
            list->entries[i] = *sourceEntry;
            list->handles[i] = effCreateSharedTextureReference((sdfTextureFileHeader_t *)payload);
            payload += sourceEntry->payloadSize;
            sourceEntry++;
            i++;
        } while (i < count);
    }
    list->totalEntryCount = effCountExpandedEntries(list);
    list->refCount++;
    return list;
}

void effCacheTexDelete(ctexlHandle_t *holder) {
    u32 i;
    if (--holder->refCount != 0) {
        return;
    }
    for (i = 0; i < holder->count; i++) {
        effReleaseSharedReference(holder->handles[i]);
    }
    sdfMemFreeDirect(holder->allocation);
}

ctexlHandle_t *Copy_effCacheTexList(ctexlHandle_t *obj) {
    obj->refCount++;
    return obj;
}

void effCacheTexAnimGet(const ctexlHandle_t *set, u32 frame, effCacheTexList_Anim_t *out) {
    u32 count = set->count;
    u32 local = 0;
    s32 segment = -1;
    const effExpandedEntry_t *seg;
    u32 acc;
    u32 i;

    if (count == 1) {
        segment = 0;
    } else {
        if (set->flags & EFF_ANIM_SET_LOOP) {
            local = frame % set->totalEntryCount;
        } else if (frame >= set->totalEntryCount) {
            segment = count - 1;
        } else {
            local = frame;
        }
        if (segment == -1) {
            seg = set->entries;
            acc = 0;
            for (i = 0; i < count; i++) {
                acc += seg->additionalCount;
                if (acc >= local) {
                    segment = i;
                    break;
                }
                acc++;
                seg++;
            }
        }
    }
    out->scaleX = 1.0f;
    {
        f32 scaleY = 2.0f;

        if (!(set->flags & EFF_ANIM_SET_DOUBLE_Y_SCALE)) {
            scaleY = 1.0f;
        }
        out->segment = segment;
        out->scaleY = scaleY;
    }
    out->angle = 0.0f;
}

sdfTexHandle_t *effAssignSampledSegmentReference(const ctexlHandle_t *set, sdfGrOt_t *target,
                                                 const effCacheTexList_Anim_t *sample) {
    return sdfGetOrCreateTextureForReference(target, set->handles[sample->segment]);
}

INCLUDE_RODATA(const s32, "code_r1_002250F8", effStrEffFloorModelDelete);

INCLUDE_RODATA(const s32, "code_r1_002250F8", Draw_effHelperTable);

INCLUDE_SDATA(const s32, "code_r1_002250F8", effFloorModelListHead);

INCLUDE_SDATA(const s32, "code_r1_002250F8", effSharedTextureReferenceCount);

INCLUDE_SDATA(const s32, "code_r1_002250F8", sdfGetOrCreateTextureForReferenceData);

INCLUDE_SDATA(const s32, "code_r1_002250F8", sdfGetOrCreateTextureForReferenceData2);

INCLUDE_SDATA(const s32, "code_r1_002250F8", sdfGetOrCreateTextureForReferenceData3);

