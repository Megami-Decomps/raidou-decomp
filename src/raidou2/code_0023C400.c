#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_0029A840.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "sdk_libc.h"

#include "dds/btl.h"

#include "dds/mdl.h"

#include "dds/sdf.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_chunk.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_map_position_api.h"

#include "dds/sdf_motion.h"

#include "dds/sdf_packet_alloc.h"

#include "dds/sdf_packet_builders.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_projection.h"

#include "dds/sdf_tex_create.h"

#include "dds/sdf_tex_packet.h"

#include "dds/sdf_texture_format.h"

#include "dds/sdf_texture_submit.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/sdf_vu_transform_api.h"

#include "dds/sdk_libc.h"

/* Build a look-at basis from four-float target, eye, and up vectors. */
void LookAtMatrix(const f32 *targetPoint, const f32 *eyePosition, const f32 *upVector);

/* Multiply the quaternions in vf10 and vf11, writing the product to vf10. */
void Mult(void);

/* Rigid-invert the VU matrix in vf28-vf31 in place; translation becomes
 * -(R^T * t). The rotation input is assumed to contain no scale. */
void ReverseMatrix(void);

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
/* Destroy one context. If it leaves its resource-owning group empty, group
 * teardown also releases that group's registered assets. */
void mdlFree(dds3ModelHandle_t *ctx);

u32 mdlGetColor(const dds3ModelHandle_t *ctx);

u16 mdlGetMajorNum(const dds3ModelHandle_t *ctx);

u16 mdlGetMinorNum(const dds3ModelHandle_t *ctx);

/* VU helpers transfer vectors through vf10 and basis rows through vf28-vf30. */
void mdlGetPosition(dds3ModelHandle_t *ctx);

void mdlGetScale(dds3ModelHandle_t *ctx);

void mdlLink(dds3ModelHandle_t *ctx, struct sdfGrOt_t **surfaces);

/* Allocate a distinct caller-owned context linked to the registered group.
 * Contexts share the group node and assets; release each context with mdlFree. */
dds3ModelHandle_t *mdlLoad(u32 group, u32 id);

void mdlLoopMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex);

void mdlSetColor(dds3ModelHandle_t *ctx, u32 value);

void mdlSetPosition(dds3ModelHandle_t *ctx);

void mdlSetRotate(dds3ModelHandle_t *ctx);

void mdlSetScale(dds3ModelHandle_t *ctx);

/* sdfMotionCtrlr_t-record selection and lookup. */
void mdlSingleMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex);

extern void *memcpy(void *, const void *, unsigned int);

void *memset(void *dst, s32 value, u32 size);

void *sdfChipAllocDirect(s32 size);

void *sdfChipAllocZeroDirect(s32 size);

void sdfChipFreeDirect(void *memory);

void sdfDrawTagInit(struct sdfDrawTag_t *list);

void sdfDrawTagLink(struct sdfDrawTag_t *list, u32 packetAddress);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 size);

void sdfLinkLoadImage(
    sdfDrawTag_t *list, sdfGsMemSlice_t *source, s32 destinationX,
    s32 destinationY, s32 width, s32 height, s32 sourceAddress,
    s32 (*allocatePacket)(s32));

struct sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);

void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);

void *sdfMemLock(struct sdfMemHandle_t *allocation);

u32 sdfModelGetMarkPosCount(const struct dds_sdfModelRootNode_t *model);

SdfMapPositionLookupResult sdfModelGetMarkPosMatrix(
    struct dds_sdfModelRootNode_t *model, s32 id);

void sdfMotionSeek(struct sdfMotionCtrlr_t *motion, f32 frame);

/* Allocate and register a texture. The shared registration helper initializes
 * its ordinary reference count to one; the caller releases that reference with
 * sdfTexReleaseReference or its deferred form. */
struct sdfTexHandle_t *sdfTexAlloc(s32 width, s32 height,
    SdfGsPixelStorageFormat pixelFormat, u32 maxMipLevel,
    SdfGsPixelStorageFormat clutFormat, u32 paletteCount);

u32 sdfTexGetCLUTAddr(sdfTexHandle_t *texture);

/* Texture resource accessors and palette helpers shared by both title providers. */
u8 sdfTexGetCLUTCount(sdfTexHandle_t *texture);

u32 sdfTexGetPixelAddr(sdfTexHandle_t *texture);

/* Store the context-one resource packet on the texture's primary buffer. */
void sdfTexMakeGIFPacket(struct sdfTexHandle_t *texture);

/* Submit a palette using dimensions selected from the texture pixel format. */
const u8 *sdfTexTransCLUT(sdfTexHandle_t *texture, u32 destination,
                    const u8 *pixels,
                    SdfTextureUploadOwnership borrowPixels);

/* Reads the source bytes and returns its cursor advanced past the submitted image. */
const u8 *sdfTexTransData(u32 destination, s32 width, s32 height, u32 format,
                    const u8 *pixels,
                    SdfTextureUploadOwnership borrowPixels);

#include "dds/sdk_ps2.h"

#include "dds/common.h"

#include "dds/sdf_map_position_api.h"

#include "dds/eff_texture_table.h"

#include "dds/sdf_vu0.h"

#include "dds/eff_object_node.h"

#include "dds/eff_model_owner.h"

#include "dds/sdf_motion.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_packet_builders.h"

#include "dds/sdf_chip.h"

#include "dds/eff_ref_obj.h"

#include "dds/sdf_heap_api.h"

#include "dds/eff_anim.h"

#include "dds/file.h"

#include "dds/ee_mmi.h"

#include "dds/pcp_vu0.h"

#include "dds/sdf_projection.h"

#include "dds/mdl.h"

#include "dds/sdf.h"

#include "dds/sdf_texture_submit.h"

#include "dds/sdf_texture_file.h"

#include "dds/sdf_tex_packet.h"

#include "dds/sdf_tex_create.h"

#include "dds/sdf_chunk.h"

#include "dds/eff_expanded_source.h"

#include "dds/eff_update_flags.h"

#include "dds/btl_runtime.h"

#include "dds/btl.h"

extern struct EffExpandedList *effCreateTextureAnimationSet(u32);

/* Battle state: resource headers for texture slots 1 and 2. */
typedef struct EffBattleTexHeaders {
    u8 pad_000[0x1F4];
    u32 statusFlags; /* 0x1F4 */
    u8 pad_1F8[0x30];
    struct EffBattleUnit *units; /* 0x228 */
    u8 pad_22C[0x350];
    u8 *slot1; // 0x57C
    u8 *slot2; // 0x580
} EffBattleTexHeaders;

extern u32 D_003BC950;

extern u32 D_003BC954;

extern EffectObjectNode *effFloorModelListHead;

extern u8 btlIsRuntimeAllocated(void);

extern u8 btlIsCurrentActorFullyMarked(void);

extern EffModelOwner *effCreateModelOwner(FileJobPayload *);

extern u32 effSharedTextureReferenceCount;

extern sdfTexHandle_t *D_003BC958;

extern void effFloorModelListRemove(EffectObjectNode *);

/* Initialize the VU transforms and the first node's float slot, if present. */
void effInitModelVUState(dds3ModelHandle_t *model);

extern u16 D_003BC944;

extern void mdlPackDecode(s32, u16, s32, const u8 *, s32);

extern void effInitModelVUState(dds3ModelHandle_t *);

dds3ModelHandle_t *effLoadViewerModelWithVUState(void *modelData, u32 modelDataSize);

/* Clear the lighting attachment before destroying its model context. */
void effDestroyModelContext(dds3ModelHandle_t *model);

EffModelOwner *effCreateModelOwner(FileJobPayload *source);

void effDestroyModelOwner(EffModelOwner *owner);

extern void effRecreateModelFromSource(EffModelOwner *, EffModelOwner *);

EffModelOwner *effDuplicateEffectHeader(EffModelOwner *source);

void effRecreateModelFromSource(EffModelOwner *work, EffModelOwner *source);

void effModelAnimationStop(EffModelOwner *owner);

extern s32 effComputeLightDirectionVU(dds3ModelHandle_t *, sdf3DLightParallelPacket_t *);

#include "dds/sdf_packet_alloc.h"

extern sdfGrOt_t *kwlnSceneSurfaceSets[13][4];

extern u64 D_0037E5B0[];

/* Attach the owner's lighting buffer when the direction update succeeds. */
void effRefreshModelLighting(EffModelOwner *work);

/* Pass a vector to the VU0 model helpers via vf10 (gcc cannot do this from plain C). */
void effApplyModelPrimaryVector(EffModelOwner *owner, const f32 *position);

void effApplyModelVecB(EffModelOwner *owner, const f32 *rotation);

void effBroadcastModelMask(EffModelOwner *owner, u32 color);

void effApplyScaledModelTertiaryVector(EffModelOwner *owner, float scale);

void effUploadModelTextures(EffModelOwner *owner);

void effFloorModelListPush(EffModelOwner *obj);

/* Track floor models only while battle is active and the current actor is not fully marked. */
EffModelOwner *effCreateFloorModelOwner(FileJobPayload *source);

void effMarkFloorModelForDestruction(EffModelOwner *work);

extern void effRecreateModelFromSource(EffModelOwner *, EffModelOwner *);

EffModelOwner *effDuplicateFloorModelOwner(EffModelOwner *source);

/* Render the model owner with staged GS TEST_2 packets, then restore the tests. */
void effRenderModelOwnerWithGsTests(EffModelOwner *owner);

extern u32 effModelUpdateControlFlags;

void effMarkFloorModelForUpdate(EffModelOwner *work);

void effFloorModelListPush(EffModelOwner *obj);

extern char D_003B2AA0[];

#include "dds/sdf_debug_api.h"

void effFloorModelListRemove(EffectObjectNode *node);

void effSweepFloorModelList(void);

void mdlPropagateObjectFlag(void);

void mdlClearListedObjectFlag(void);

void mdlSetListedObjectFlag(void);

void mdlMarkAndProcessObjectNodes(void);

#include "dds/eff_resource_owner.h"

typedef struct EffResourceModelConfig {
    SdfColorTrack colorTrack;
    SdfAlphaTrack alphaTrack;
    u32 plainEntry;
    f32 frameStep;
} EffResourceModelConfig;

typedef char EffResourceModelConfig_size_check[
    sizeof(EffResourceModelConfig) == 0x3C ? 1 : -1];

/* Create an empty owner, or load its model and per-record queues from a file job. */
EffResourceOwner *effCreateResourceOwner(FileJobPayload *source);

void effDestroyResourceOwner(EffResourceOwner *owner);

extern EffResourceOwner *effCreateResourceOwner(FileJobPayload *);

extern void effCopyResourceOwner(EffResourceOwner *, EffResourceOwner *);

EffResourceOwner *effDuplicateModelOwner(EffResourceOwner *source);

void effCopyResourceOwner(EffResourceOwner *dst, EffResourceOwner *src);

void effResetModelMotionAndOwnerFlag(EffResourceOwner *work);

extern void effGetWorldVector(u32);

extern void sdfVuMatrixToQuaternion(const f32 [4][4]);

extern void btlDebugDrawSegment(f32 *, f32 *, u32);

extern void btlDebugDrawPoint(f32 *);

extern f32 effWorldUpVector[4];

extern f32 D_0037E110[4];

extern f32 D_0037E100[4];

extern f32 D_003B2AC0[4];

void func_0029B818(EffResourceOwner *owner);

/* Pass a vector to the VU0 model helpers via vf10 (gcc cannot do this from plain C). */
void effLoadModelPrimaryVector(EffResourceOwner *work, const f32 *position);

void effSetModelRotationQuaternion(EffResourceOwner *work, const f32 *rotationQuaternion);

void effPropagateResourceModelMask(EffResourceOwner *work, u32 color);

void effScaleModelVec(EffResourceOwner *work, float scale);

void effObjectListCountersReset(void);

extern s32 sdfImageDataSize(u32 format, s32 width, s32 height);

RefObj *effCreateSharedTextureReference(SdfTextureFileHeader *source);

RefObj *effCloneSharedReferenceWithValue(SdfTextureFileHeader *source, u32 textureIndex);

void effReleaseSharedReference(RefObj *obj);

RefObj *effRetainSharedReference(RefObj *obj);

/* Native GS dimension SDK calls: 0029C16C/0029C17C.
 * Address normalization before TEX0 packing is also traced in
 * texture_renew_secondary_epoch_probe (parity OK, local-alloc q14/q16). */
sdfTexHandle_t *sdfGetOrCreateTextureForReference(sdfGrOt_t *surface, RefObj *source);

s32 effCountExpandedEntries(const EffExpandedList *list);

/* Serialized expanded list: 0x10-byte header, the entry table, then each entry's payload. */
/* Copy a serialized list into one allocation and retain a shared texture reference per entry. */
struct EffExpandedList *effCreateTextureAnimationSet(u32 sourceAddress);

void effReleaseReferenceHolder(EffExpandedList *holder);

EffExpandedList *effReferenceObjectRetain(EffExpandedList *obj);

void effSampleAnimSet(const EffExpandedList *set, u32 frame, EffAnimSample *out);

sdfTexHandle_t *effAssignSampledSegmentReference(const EffExpandedList *set, sdfGrOt_t *target,
                                                 const EffAnimSample *sample);
/* End of declarations from src/dds1/game/code_0029A840.c */

void effRecreateModelFromSource(EffModelOwner *work, EffModelOwner *source) {
    EffModelOwner *owner = work;
    EffModelOwner *original = source;
    dds3ModelHandle_t *model;

    if (owner->model != 0) {
        effDestroyModelContext(owner->model);
    }
    model = mdlLoad(mdlGetMajorNum(original->model), mdlGetMinorNum(original->model));
    effInitModelVUState(model);
    SDF_VU0_SETSCALE1VECTOR();
    VU0_SCALE_VF_MFC1(vf10, owner->scale);
    mdlSetScale(model);
    owner->model = model;
}

INCLUDE_ASM(const s32, "code_0023C400", func_r1_00225190);

INCLUDE_ASM(const s32, "code_0023C400", func_r1_002251E8);

/* Pass a vector to the VU0 model helpers via vf10 (gcc cannot do this from plain C). */
void effApplyModelPrimaryVector(EffModelOwner *owner, const f32 *position) {
    SDF_VU0_LOADVECTOR(position);
    mdlSetPosition(owner->model);
}

void effApplyModelVecB(EffModelOwner *owner, const f32 *rotation) {
    SDF_VU0_LOADVECTOR(rotation);
    mdlSetRotate(owner->model);
}

void effBroadcastModelMask(EffModelOwner *owner, u32 color) {
    mdlSetColor(owner->model, color);
}

void effApplyScaledModelTertiaryVector(EffModelOwner *owner, float scale) {
    float v[4];
    float t;

    t = owner->scale * scale;
    v[0] = v[1] = v[2] = t;
    SDF_VU0_LOADVECTOR(v);
    mdlSetScale(owner->model);
}

void func_r1_00225308(void) {
}

INCLUDE_ASM(const s32, "code_0023C400", func_r1_00225310);

void effMarkFloorModelForDestruction(EffModelOwner *work) {
    u32 flags = work->flags | EFF_MODEL_OWNER_FLAG_DESTROY_REQUESTED;
    work->flags = flags;
    if ((flags & EFF_MODEL_OWNER_FLAG_FLOOR_LISTED) == 0) {
        effDestroyModelOwner(work);
    }
}

INCLUDE_ASM(const s32, "code_0023C400", func_r1_00225390);

INCLUDE_ASM(const s32, "code_0023C400", effRenderModelOwnerWithGsTests);

void effMarkFloorModelForUpdate(EffModelOwner *work) {
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

void effFloorModelListPush(EffModelOwner *obj) {
    EffectObjectNode *entry = sdfChipAllocZeroDirect(sizeof(EffectObjectNode));

    entry->object = obj;
    entry->prev = NULL;
    if (effFloorModelListHead != NULL) {
        effFloorModelListHead->prev = entry;
        entry->next = effFloorModelListHead;
    } else {
        entry->next = NULL;
    }
    effFloorModelListHead = entry;
    ((EffModelOwner *)entry->object)->flags |= EFF_MODEL_OWNER_FLAG_FLOOR_LISTED;
}

void effFloorModelListRemove(EffectObjectNode *node) {
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
    EffectObjectNode *node = effFloorModelListHead;
    EffectObjectNode *next;

    if (node != NULL) {
        do {
            next = node->next;
            if (!(((EffModelOwner *)node->object)->flags & EFF_MODEL_OWNER_FLAG_DESTROY_READY)) {
                if ((((EffModelOwner *)node->object)->flags &
                     (EFF_MODEL_OWNER_FLAG_UPDATE_PENDING | EFF_MODEL_OWNER_FLAG_LISTED_OBJECT |
                      EFF_MODEL_OWNER_FLAG_UPDATE_DEFERRED)) == EFF_MODEL_OWNER_FLAG_UPDATE_PENDING) {
                    effRenderModelOwnerWithGsTests(node->object);
                }
                ((EffModelOwner *)node->object)->flags &= ~EFF_MODEL_OWNER_FLAG_UPDATE_DEFERRED;
            } else {
                effFloorModelListRemove(node);
            }
            node = next;
        } while (node != NULL);
    }
}

INCLUDE_ASM(const s32, "code_0023C400", mdlPropagateObjectFlag);

INCLUDE_ASM(const s32, "code_0023C400", mdlClearListedObjectFlag);

INCLUDE_ASM(const s32, "code_0023C400", mdlSetListedObjectFlag);

void mdlMarkAndProcessObjectNodes(void) {
    EffectObjectNode *node = effFloorModelListHead;
    EffModelOwner *object;
    EffectObjectNode *next;
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

INCLUDE_ASM(const s32, "code_0023C400", func_r1_002258D0);

INCLUDE_ASM(const s32, "code_0023C400", func_r1_00225998);

INCLUDE_ASM(const s32, "code_0023C400", func_r1_002259E8);

INCLUDE_ASM(const s32, "code_0023C400", func_r1_00225A40);

INCLUDE_ASM(const s32, "code_0023C400", func_r1_00225B00);

INCLUDE_ASM(const s32, "code_0023C400", func_r1_00225B78);

INCLUDE_ASM(const s32, "code_0023C400", func_r1_00225C08);

INCLUDE_ASM(const s32, "code_0023C400", func_r1_00225C28);

INCLUDE_ASM(const s32, "code_0023C400", func_r1_00225C48);

INCLUDE_ASM(const s32, "code_0023C400", func_r1_00225C60);

/* Create an empty owner, or load its model and per-record queues from a file job. */
EffResourceOwner *effCreateResourceOwner(FileJobPayload *source) {
    EffResourceOwner *owner = sdfChipAllocDirect(sizeof(EffResourceOwner));
    u8 *data;
    u32 i;

    memset(owner, 0, sizeof(EffResourceOwner));
    if (source != NULL) {
        data = fileResolvePrimaryBuffer(source);
        memcpy((u8 *)owner + 8, data, sizeof(EffResourceModelConfig));
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
            owner->entries[0] = fileCloneQueueEntries((struct FileQueue *)data);
            for (i = 1; i < owner->count; i++) {
                owner->entries[i] = fileQueueClone(owner->entries[0]);
            }
        }
    }
    return owner;
}

void effDestroyResourceOwner(EffResourceOwner *owner) {
    u32 i;

    if (owner->model != 0) {
        effDestroyModelContext(owner->model);
    }
    if (owner->queueAllocation != 0) {
        for (i = 0; i < owner->count; i++) {
            fileQueueDestroy(owner->entries[i]);
        }
        sdfMemFreeDirect(owner->queueAllocation);
    }
    sdfChipFreeDirect(owner);
}

EffResourceOwner *effDuplicateModelOwner(EffResourceOwner *source) {
    EffResourceOwner *effect = effCreateResourceOwner(0);

    *(EffResourceModelConfig *)((u8 *)effect + 8) =
        *(EffResourceModelConfig *)((u8 *)source + 8);
    effCopyResourceOwner(effect, source);
    return effect;
}

void effCopyResourceOwner(EffResourceOwner *dst, EffResourceOwner *src) {
    u32 i;

    if (dst->model != 0) {
        effDestroyModelContext(dst->model);
    }
    dst->model = mdlLoad(mdlGetMajorNum(src->model), mdlGetMinorNum(src->model));
    effInitModelVUState(dst->model);
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
                fileQueueDestroy(dst->entries[i]);
            }
            sdfMemFreeDirect(dst->queueAllocation);
        }
        dst->queueAllocation = sdfMemAllocDirect(dst->count * 4);
        dst->entries = sdfMemLock(dst->queueAllocation);
        for (i = 0; i < dst->count; i++) {
            dst->entries[i] = fileQueueClone(*src->entries);
        }
    }
}

INCLUDE_ASM(const s32, "code_0023C400", func_r1_00226110);

INCLUDE_ASM(const s32, "code_0023C400", func_r1_00226188);

/* Pass a vector to the VU0 model helpers via vf10 (gcc cannot do this from plain C). */
void effLoadModelPrimaryVector(EffResourceOwner *work, const f32 *position) {
    SDF_VU0_LOADVECTOR(position);
    mdlSetPosition(work->model);
}

void effSetModelRotationQuaternion(EffResourceOwner *work, const f32 *rotationQuaternion) {
    SDF_VU0_LOADVECTOR(rotationQuaternion);
    mdlSetRotate(work->model);
}

void effPropagateResourceModelMask(EffResourceOwner *work, u32 color) {
    mdlSetColor(work->model, color);
}

void effScaleModelVec(EffResourceOwner *work, float scale) {
    u32 bits;

    SDF_VU0_SETSCALE1VECTOR();
    VU0_SCALAR_OP_TMP(bits, scale, "vmulx.xyzw vf10, vf10, vf2x");
    mdlSetScale(work->model);
}

void effObjectListCountersReset(void) {
    D_003BC954 = 0;
    D_003BC950 = 0xffffffff;
}

RefObj *effCreateSharedTextureReference(SdfTextureFileHeader *source) {
    u32 paletteWidth;
    u32 paletteHeight;
    s32 paletteBytes;
    s32 imageBytes;
    s32 payloadBytes;
    s32 textureOffset;
    struct sdfMemHandle_t *allocationHandle;
    u8 *cursor;
    RefObj *texture;

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
    texture = (RefObj *)(cursor + textureOffset);
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

RefObj *effCloneSharedReferenceWithValue(SdfTextureFileHeader *source, u32 textureIndex) {
    RefObj *copy;

    copy = effCreateSharedTextureReference(source);
    copy->index = textureIndex;
    return copy;
}

void effReleaseSharedReference(RefObj *obj) {
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

RefObj *effRetainSharedReference(RefObj *obj) {
    obj->refCount++;
    effSharedTextureReferenceCount++;
    return obj;
}

/* Native GS dimension SDK calls: 0029C16C/0029C17C.
 * Address normalization before TEX0 packing is also traced in
 * texture_renew_secondary_epoch_probe (parity OK, local-alloc q14/q16). */
sdfTexHandle_t *sdfGetOrCreateTextureForReference(sdfGrOt_t *surface, RefObj *source) {
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
    sdfLinkLoadImage(list, D_003BC958->pixelGsHandle, 0, 0, ((SdfTextureFileHeader *)source->base)->width, ((SdfTextureFileHeader *)source->base)->height, (s32)source->pixels, 0);
    surface->regFunc(surface, list);
    texture = D_003BC958;
    width = ((SdfTextureFileHeader *)source->base)->width;
    texture->width = width;
    widthLog2 = (s16)width - 1;
    primary = texture->pixelGsHandle->addr >> 6;
    height = ((SdfTextureFileHeader *)source->base)->height;
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

s32 effCountExpandedEntries(const EffExpandedList *list) {
    u32 count = list->count;
    u32 i = 0;
    s32 total = 0;

    if (count != 0) {
        const EffExpandedEntry *entries = list->entries;
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
struct EffExpandedList *effCreateTextureAnimationSet(u32 sourceAddress) {
    const EffExpandedSource *source = (const EffExpandedSource *)sourceAddress;
    u32 count = source->count;
    const EffExpandedEntry *sourceEntry = source->entries;
    u32 i = 0;
    struct sdfMemHandle_t *allocation;
    EffExpandedList *list;
    u8 *payload;

    allocation = sdfMemAllocDirect(count * (sizeof(EffExpandedEntry) + sizeof(RefObj *)) + sizeof(EffExpandedList));
    list = (EffExpandedList *)sdfMemLock(allocation);
    list->entries = (EffExpandedEntry *)(list + 1);
    list->handles = (RefObj **)(list->entries + count);
    list->unk20 = -1;
    list->allocation = allocation;
    list->refCount = 0;
    memcpy(list, source, 0x10);
    payload = (u8 *)(sourceEntry + count);
    if (count != 0) {
        do {
            list->entries[i] = *sourceEntry;
            list->handles[i] = effCreateSharedTextureReference((SdfTextureFileHeader *)payload);
            payload += sourceEntry->payloadSize;
            sourceEntry++;
            i++;
        } while (i < count);
    }
    list->totalEntryCount = effCountExpandedEntries(list);
    list->refCount++;
    return list;
}

void effReleaseReferenceHolder(EffExpandedList *holder) {
    u32 i;
    if (--holder->refCount != 0) {
        return;
    }
    for (i = 0; i < holder->count; i++) {
        effReleaseSharedReference(holder->handles[i]);
    }
    sdfMemFreeDirect(holder->allocation);
}

EffExpandedList *effReferenceObjectRetain(EffExpandedList *obj) {
    obj->refCount++;
    return obj;
}

void effSampleAnimSet(const EffExpandedList *set, u32 frame, EffAnimSample *out) {
    u32 count = set->count;
    u32 local = 0;
    s32 segment = -1;
    const EffExpandedEntry *seg;
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

sdfTexHandle_t *effAssignSampledSegmentReference(const EffExpandedList *set, sdfGrOt_t *target,
                                                 const EffAnimSample *sample) {
    return sdfGetOrCreateTextureForReference(target, set->handles[sample->segment]);
}

INCLUDE_RODATA(const s32, "code_0023C400", D_r1_00455D00);

INCLUDE_RODATA(const s32, "code_0023C400", D_r1_00455D20);

INCLUDE_SDATA(const s32, "code_0023C400", D_r1_0046D100);

INCLUDE_SDATA(const s32, "code_0023C400", D_r1_0046D104);

INCLUDE_SDATA(const s32, "code_0023C400", D_r1_0046D108);

INCLUDE_SDATA(const s32, "code_0023C400", D_r1_0046D10C);

INCLUDE_SDATA(const s32, "code_0023C400", D_r1_0046D110);

