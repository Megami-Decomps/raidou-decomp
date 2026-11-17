#include "common.h"
#include "dds3ObjectBase.h"

#include "basic.h"
#include "dds3obj.h"
#include "field.h"
#include "include_asm.h"
#include "kernel_draw.h"
#include "sdf_packet_list.h"
#include "sdf_resource.h"
#include "sdk_libc.h"
#include "billboard.h"
#include "sdf_types.h"
#include "sdf_lighting.h"
#include "libsdf.h"
#include "model.h"
#include "motion.h"
#include "draw.h"
#include "sdf_movie_types.h"
#include "ps2asm.h"
#include "fpu.h"
#include "kernel.h"
#include "draw3d.h"
#include "scr.h"
#include "fldFileResolver_00132080.h"
#include "effPCPMisc_00224D10.h"
#include "fldFileResolver.h"
#include "fldProcess.h"
#include "mdlManager.h"
#include "fldDbg.h"
#include "sdfQuat.h"
#include "sdfVu0.h"
#include "fclCombineInit.h"
#include "sdk_libm.h"
#include "dds3PadManager.h"
#include "dds3Basic.h"
#include "mapData.h"
#include "sdfMath.h"
#include "dds3Randomize.h"
#include "sbtlWork.h"
#include "sBtlSound.h"
#include "sdfSound.h"
#include "fld.h"
#include "dds3KernelMain.h"
#include "fldDraw.h"
#include "sdf2D.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "sdfPrim.h"

/* The field-area loader interprets these serialized packet-header values. */
typedef enum FldPackedResourceKind {
    FLD_PACKED_RESOURCE_INFO_TABLE = 1,
    FLD_PACKED_RESOURCE_NPC_PALETTE = 2,
    FLD_PACKED_RESOURCE_SKY_BUFFER = 3,
    FLD_PACKED_RESOURCE_ACTOR_WAYPOINT_TABLE = 4,
    FLD_PACKED_RESOURCE_RETAINED_COPY = 5,
    FLD_PACKED_RESOURCE_SCENE_RECORD_CHUNK = 6
} FldPackedResourceKind;

enum {
    OBJECT_TRANSFORM_FLAG_UPDATE_PENDING = 1,
    OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID = 2,
    OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION = 8
};

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

enum SdfMotionStateValue {
    SDF_MOTION_STATE_UNINITIALIZED = 0,
    SDF_MOTION_STATE_INITIALIZED = 1,
    SDF_MOTION_STATE_SAMPLING = 4,
    SDF_MOTION_STATE_TERMINAL = 5,
    SDF_MOTION_STATE_SUSPENDED = 6,
};

/* Set or clear requested bits in the object's flag word. */

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */

typedef struct sdfGsTextureAlphaFlushPacket_t {
    sdfGrPacketHeader_t header;
    sdfGrTagStdReg_t textureAlpha;
    sdfGrTagStdReg_t textureFlush;
} sdfGsTextureAlphaFlushPacket_t;

typedef char sdfGsTextureAlphaFlushPacket_size_must_be_0x40[
    (sizeof(sdfGsTextureAlphaFlushPacket_t) == 0x40) ? 1 : -1];

struct dds3Basic_t;

struct EffNodeDescriptor;

typedef char FldSpawnBatchSize[(sizeof(fldSpawnBatch_t) == 12) ? 1 : -1];

/* Camera-setting table capacity. */
enum {
    FIELD_CAMERA_SETTING_COUNT = 8
};

typedef struct sdfDrawPacket_t sdfDrawPacket_t;

typedef struct DmaPacketHeader DmaPacketHeader;

/* Per-actor slot in the field's actor table; reset by fldResetActorSlots and
   scanned by fldReleaseActorTasksById. 0x5C is the stride retail uses. Three transition keys, signed frame
   counters and two float triples with per-frame increments. */
typedef struct {
    s32 kind;
    u32 actorId;
    u32 unk08;
    u32 firstKey;
    u32 secondaryMotion;
    u32 secondKey;
    u32 transitionKey;
    s32 firstFrame;
    s32 secondFrame;
    s32 transitionFrame;
    s32 frameCount;
    f32 firstValues[3];
    f32 firstStep[3];
    f32 secondValues[3];
    f32 secondStep[3];
} FldActorRow; /* 0x5C bytes */

typedef char FldActorRow_size_must_be_0x5C[sizeof(FldActorRow) == 0x5C ? 1 : -1];

static inline s32 fldTestBits(u32 flags, u32 mask) {
    return (flags & mask) != 0;
}

/* vu0 routine: normalize vf10 and return its original XYZ length. */
static inline f32 fldNormalizeProbeVector(void) {
    f32 length;

    SDF_VU0_GETLENGTH(length);
    SDF_VU0_NORMALIZE();
    return length;
}

extern void fldRestoreCameraModelColor(void);
extern void fldSetCameraObjectTransitionFlag(void);
extern void fldClearCameraObjectTransitionFlags(void);
extern mdlObj_t *fldPlayerModelContext;
extern dds3Basic_t *fldPlayerObject;

/* Serialized transforms leave the position W component zero. */
static inline void fldCopySpawnTransform(f32 *position, f32 *rotation, const f32 *source) {
    if (source != NULL) {
        position[0] = source[0];
        position[1] = source[1];
        position[2] = source[2];
        position[3] = 0.0f;
        rotation[0] = source[4];
        rotation[1] = source[5];
        rotation[2] = source[6];
        rotation[3] = source[7];
    } else {
        position[0] = 0.0f;
        position[1] = 0.0f;
        position[2] = 0.0f;
        position[3] = 0.0f;
        rotation[0] = 0.0f;
        rotation[1] = 0.0f;
        rotation[2] = 0.0f;
        rotation[3] = 0.0f;
    }
}

extern char D_0039FE08[], D_0039FE18[], D_0039FE28[];

extern const u8 *fldCachedRoomResourceData, *fldCachedRoomF1ResourceData, *fldCachedRoomF2ResourceData,
    *fldCachedRoomKF2ResourceData;

extern u32 fldCachedRoomResourceSize, fldCachedRoomF1ResourceSize, fldCachedRoomF2ResourceSize, fldCachedRoomKF2ResourceSize;
extern f32 D_003BAD10, D_003BAD14, D_003BAD18;

typedef union FldLookAtVector {
    u128 q;
    f32 f[4];
} FldLookAtVector;

typedef struct fldModelMatrices_t {
    u8 pad00[0xC];
    u8 **rows; /* 0x0C: slot-indexed matrix blocks */
} fldModelMatrices_t;

struct effRandState_t;

/* Signed verticalStepDirection: adjust player Y by -2/+2 and save previous XYZ. */

extern void fldDisableCameraObjectFlag(void);
void fldSetCameraNodeModeWithZero(void);
void fldEnableCameraObjectFlag(void);
void fldDisableCameraObjectFlag(void);
void fldRestoreCameraModelColor(void);
void fldSetCameraObjectTransitionFlag(void);
void fldClearCameraObjectTransitionFlags(void);
extern s32 D_003BAD88, D_003BAD8C;
extern f32 D_003BAD90, D_003BAD94;

enum {
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_1_OFFSET_32 = 1,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_2_OFFSET_32 = 2,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_05_OFFSET_32 = 3,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_1_OFFSET_48 = 4,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_2_OFFSET_48 = 5,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_05_OFFSET_48 = 6,
};

extern s32 D_003BADB8, D_003BADBC;
extern f32 D_003BADC0, D_003BADC4;

#include "datCalc.h"
#include "newdata.h"

/* Sparse world-node identities recovered from the paired operation tables and
 * concrete creators. Unlisted values stay unnamed; COUNT remains the full
 * directory extent rather than an enum-completeness claim. */
enum {
    EFF_WORLD_KIND_WORLD_INFO = 0,
    EFF_WORLD_KIND_EVENT_TABLE = 1,
    EFF_WORLD_KIND_WORLD_INDEX_OWNER = 2,
    EFF_WORLD_KIND_SLOT_RING = 3,
    EFF_WORLD_KIND_CAMERA = 4,
    EFF_WORLD_KIND_FOLLOW_MODEL = 5,
    EFF_WORLD_KIND_EFFECT_TRANSFORM = 6,
    EFF_WORLD_KIND_EFFECT_OBJECT = 7,
    EFF_WORLD_KIND_RESOURCE_OWNER = 8,
    EFF_WORLD_KIND_LIGHT = 9,
    EFF_WORLD_KIND_SCRIPT_OBJECT = 10,
    EFF_WORLD_KIND_ACTION_B = 11,
    EFF_WORLD_KIND_ACTION_D = 13,
    EFF_WORLD_KIND_ACTION_10 = 16,
    EFF_WORLD_KIND_TRANSFORM_SOURCE = 17,
    EFF_WORLD_KIND_COUNT = 18
};

extern void scrDestroyWorkTask(void);

#define SDF_PACKED_STATUS_BIT 0x8000

#define SDF_PACKED_CHANNEL_MASK 0x7fff

#define SDF_UNIT_ENEMY 0x20

#define SDF_FLOAT_COUNTER_FROZEN_FLAG 0x00000008

#define SDF_FLOAT_COUNTER_WRAP_FLAG 0x00000020

void sdfFirePendingCallback(void);

void fldDrawExpandedSpriteStrip(s32 alpha, s32 offset) {
    sdfDrawTag_t *list = sdfPrimAllocDrawTag(NULL);
    void *reference = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
    u64 *texturePacket;
    u64 *blendPacket;
    void *sprite;
    fldSpriteVertex_t *vertex;
    sdfGrOt_t *surface;

    sdfGrLinkDispFrameTex(list, EE_ADDR(&dds3DrawEnv[dds3GetFrameID()].stdEnv), reference);
    texturePacket = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x40));
    texturePacket[0] = 3;
    texturePacket[1] = 0x5000000310000000ULL;
    texturePacket[2] = 0x1000000000008002ULL;
    texturePacket[3] = 0xE;
    texturePacket[4] = 0x8000000080ULL;
    texturePacket[5] = 0x3B;
    texturePacket[6] = 0;
    texturePacket[7] = 0x3F;
    sdfDrawTagLink(list, EE_ADDR(texturePacket));
    blendPacket = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x40));
    blendPacket[0] = 3;
    blendPacket[1] = 0x5000000310000000ULL;
    blendPacket[2] = 0x1000000000008002ULL;
    blendPacket[3] = 0xE;
    blendPacket[4] = 0x31001;
    blendPacket[5] = 0x47;
    blendPacket[6] = 0x48;
    blendPacket[7] = 0x42;
    sdfDrawTagLink(list, EE_ADDR(blendPacket));
    sprite = sdf2DAllocSprite(1);
    vertex = EE_ADDR_AS(fldSpriteVertex_t *, sdf2DGetPrimPtr(EE_ADDR_AS(sdfGrPacketHeader_t *, sprite)));
    vertex->r = 0x80;
    vertex->g = 0x80;
    vertex->b = 0x80;
    vertex->a = alpha;
    vertex->corner[0].u = 0;
    vertex->corner[0].v = 0;
    vertex->corner[0].x = 0x6FF7 - offset;
    vertex->corner[0].y = 0x78FB - offset / 4;
    vertex->corner[0].mask = 0x3FFF;
    vertex->corner[0].flag = 0;
    vertex->corner[1].u = 0x2000;
    vertex->corner[1].v = 0xE00;
    vertex->corner[1].x = offset + offset / 4 + 0x9009;
    vertex->corner[1].y = offset / 4 + 0x8705;
    vertex->corner[1].mask = 0x3FFF;
    vertex->corner[1].flag = 0;
    sdfDrawTagLink(list, EE_ADDR(sprite));
    surface = &sdfGrOtTable[rsetFrameMaskOtOffset];
    surface->regFunc(surface, list);
}

INCLUDE_ASM(const s32, "field/fldFileResolver", fldDrawFrushOt);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldCallOtOfs2);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldCallSin2);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldGetPlayerCameraPos);

void sdfFirePendingCallback(void) {
    if (cameraBasic == 0) {
        return;
    }
    scrDestroyWorkTask();
}

INCLUDE_ASM(const s32, "field/fldFileResolver", fldAiboTaskSub);

INCLUDE_ASM(const s32, "field/fldFileResolver", mapAiboMinAlpDown);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldRefModel);

void mapAiboWakeUp(s32 mode) {
    gMapAiboWakeUpMode = mode;
    if (mode == 1 && aiboModel != NULL) {
        mdlLoopMotion(aiboModel, 0, 2);
    }
}

#ifdef VERSION_RAIDOU1
void mapAiboInit(void) {
    s32 i;

    for (i = 0; i < 128; i++) {
        map_aibo_buff[i][0] = 0.0f;
        map_aibo_buff[i][1] = 0.0f;
        map_aibo_buff[i][2] = 0.0f;
    }
    map_aibo_buff_read_idx = 0;
    map_aibo_buff_write_idx = 16;
    map_aibo_pos[0] = 0.0f;
    map_aibo_pos[1] = 0.0f;
    map_aibo_pos[2] = 0.0f;
    map_aibo_spd_max = 10.0f;
    map_aibo_rot = 0.0f;
    gMapAiboMotionBugCnt = 0;
}
#else
INCLUDE_ASM(const s32, "field/fldFileResolver", mapAiboInit);
#endif

INCLUDE_ASM(const s32, "field/fldFileResolver", fldProcMskSubBSub);

INCLUDE_ASM(const s32, "field/fldFileResolver", mapAiboAtumaru);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldNakamaBranchTaskSub);

#ifdef VERSION_RAIDOU1
void mapAiboFoceSet(f32 x, f32 y, f32 z) {
    s32 i;

    if (aiboBasic != NULL) {
        for (i = 0; i < 128; i++) {
            map_aibo_buff[i][0] = x;
            map_aibo_buff[i][1] = y;
            map_aibo_buff[i][2] = z;
        }
        map_aibo_buff_read_idx = 0;
        map_aibo_buff_write_idx = 16;
        map_aibo_pos[0] = x;
        map_aibo_pos[1] = y;
        map_aibo_pos[2] = z;
        map_aibo_spd_max = 10.0f;
        dds3GetRotPoseSet(aiboBasic);
        map_aibo_rot = (EulerAngleAxisY() * 180.0f) / 3.14f;
        gMapAiboMotionBugCnt = 0;
    }
}
#else
INCLUDE_ASM(const s32, "field/fldFileResolver", mapAiboFoceSet);
#endif

/* body: matched in raidou @ 1424963: src/shared/field/fldFileResolver.c:670, near, same function 0.93 (words 47/47, calls 0.80, shape 1.00) */
void mapAiboAtumaru2(f32 x, f32 y, f32 z) {
    sdfVector4_t rot;
    dds3PoseSet_t *pose;

    if (aiboBasic != NULL) {
        aiboBasic->poseset->pos.x = x;
        aiboBasic->poseset->pos.y = y;
        aiboBasic->poseset->pos.z = z;
        dds3OnPoseSetFlag(aiboBasic->poseset, 1);
        dds3OnPoseSetFlag(aiboBasic->poseset, 8);
        pose = aiboBasic->poseset;
        SDF_VU0_COPYVECTOR(&pose->pos2, &pose->pos);
        if (fldGlobalBase.nakamaMode == 0) {
            dds3GetRotPoseSet(playerBasic);
            SDF_VU0_STOREVECTOR(&rot);
            dds3SetRotPoseSet(aiboBasic, &rot);
        }
    }
    mapAiboFoceSet(x, y, z);
}

void mapAiboFoceSet3(f32 x, f32 y, f32 z) {
    sdfVector4_t rot;
    dds3PoseSet_t *pose;

    if (aiboBasic != NULL) {
        if (fldGlobalBase.nakamaMode == 0) {
            dds3GetRotPoseSet(playerBasic);
            SDF_VU0_STOREVECTOR(&rot);
            dds3SetRotPoseSet(aiboBasic, &rot);
        }
        map_aibo_rot = (EulerAngleAxisY() * 180.0f) / 3.14f;
        x -= sdfSin((180.0f - map_aibo_rot) * 0.017453293f) * 30.0f;
        z -= sdfCos((180.0f - map_aibo_rot) * 0.017453293f) * 30.0f;
        aiboBasic->poseset->pos.x = x;
        aiboBasic->poseset->pos.y = y;
        aiboBasic->poseset->pos.z = z;
        dds3OnPoseSetFlag(aiboBasic->poseset, 1);
        dds3OnPoseSetFlag(aiboBasic->poseset, 8);
        pose = aiboBasic->poseset;
        SDF_VU0_COPYVECTOR(&pose->pos2, &pose->pos);
        if (fldGlobalBase.nakamaMode == 0) {
            dds3GetRotPoseSet(playerBasic);
            SDF_VU0_STOREVECTOR(&rot);
            dds3SetRotPoseSet(aiboBasic, &rot);
        }
    }
    mapAiboFoceSet(x, y, z);
}

INCLUDE_ASM(const s32, "field/fldFileResolver", mapAiboAdd);

INCLUDE_ASM(const s32, "field/fldFileResolver", mapAiboGet);

#ifdef VERSION_RAIDOU1
/* NON_MATCHING: 564 of 618 words match.  */
#ifdef NON_MATCHING
void mapAiboTask(void) {
    sdfVector4_t rot;
    sdfVector4_t axis = VRF_Disable;
    f32 bx;
    f32 by;
    f32 bz;
    f32 ox;
    f32 oy;
    f32 oz;
    f32 tx;
    f32 ty;
    f32 tz;
    f32 dist;
    f32 limit;
    f32 speed;
    f32 ang;
    f32 diff;
    f32 step;
    s32 mode;
    s32 i;

    if (aiboBasic == NULL) {
        return;
    }
    fldAiboTaskSub();
    if (fldGlobalBase.aiboRunStop != 0) {
        if (aiboBasic != NULL && aiboModel->current.h.motionIndex != 2) {
            aiboModel->first->speed = 0.5f;
            mdlSmoothLoopMotion(aiboModel, 0, 2, 15.0f, 15.0f);
        }
        return;
    }
    ox = map_aibo_pos[0];
    oy = map_aibo_pos[1];
    oz = map_aibo_pos[2];
    if (mapAiboTaskData2 != 0) {
        tx = mapData56;
        ty = mapData57;
        tz = mapData58;
    } else if (fldGlobalBase.nakamaMode != 0) {
        tx = ox;
        ty = oy;
        tz = oz;
    } else {
        tx = fldGlobalBase.focusX;
        ty = fldGlobalBase.focusY;
        tz = fldGlobalBase.focusZ;
    }
    dist = fldPointDistance(ox, oy, oz, tx, ty, tz);
    if (mapAiboTaskData2 != 0) {
        for (i = 0; i < 2; i++) {
            mapAiboAdd(tx, ty, tz);
        }
    } else if (dist > 160.0f) {
        mapAiboAdd(tx, ty, tz);
    }
    mapAiboGet(&bx, &by, &bz);
    dist = fldPointDistance(map_aibo_pos[0], map_aibo_pos[1], map_aibo_pos[2], bx, by, bz);
    if (mapAiboTaskData2 != 0) {
        gMapAiboMotionBugCnt = 0;
        if (dist < 1.0f) {
            mode = 0;
            map_aibo_pos[0] = bx;
            map_aibo_pos[1] = by;
            map_aibo_pos[2] = bz;
            speed = 0.5f;
            map_aibo_spd_max = 10.0f;
        } else {
            limit = 80.0f;
            if (mapAiboTaskData2 == 2) {
                limit = 20.0f;
            }
            if (dist > limit) {
                map_aibo_pos[0] -= (map_aibo_pos[0] - bx) / dist * limit * 0.125f;
                map_aibo_pos[1] -= (map_aibo_pos[1] - by) / dist * limit * 0.125f;
                map_aibo_pos[2] -= (map_aibo_pos[2] - bz) / dist * limit * 0.125f;
            } else {
                map_aibo_pos[0] -= (map_aibo_pos[0] - bx) * 0.125f;
                map_aibo_pos[1] -= (map_aibo_pos[1] - by) * 0.125f;
                map_aibo_pos[2] -= (map_aibo_pos[2] - bz) * 0.125f;
            }
            dist = fldPointDistance(map_aibo_pos[0], map_aibo_pos[1], map_aibo_pos[2], ox, oy, oz);
            if (dist > 6.5f) {
                mode = 6;
                speed = 0.79999996f;
            } else if (dist > 6.0f) {
                mode = 6;
                speed = 0.59999997f;
            } else if (dist > 5.5f) {
                mode = 6;
                speed = 0.5f;
            } else if (dist > 5.0f) {
                mode = 1;
                speed = 1.0f;
            } else if (dist > 4.5f) {
                speed = 1.0f;
                mode = 1;
            } else if (dist > 4.0f) {
                mode = 1;
                speed = 0.79999996f;
            } else {
                mode = 1;
                speed = 0.59999997f;
            }
        }
        if (fldPointDistance(map_aibo_pos[0], map_aibo_pos[1], map_aibo_pos[2], tx, ty, tz) < 100.0f) {
            mapAiboMinAlpDown(-1);
        }
        gMapAiboWakeUpMode = 0;
    } else if (dist < 1.0f) {
        mode = 0;
        map_aibo_pos[0] = bx;
        map_aibo_pos[1] = by;
        map_aibo_pos[2] = bz;
        speed = 0.5f;
        map_aibo_spd_max = 10.0f;
        gMapAiboMotionBugCnt = 0;
    } else {
        if (dist >= map_aibo_spd_max) {
            if (dist < map_aibo_spd_max + 5.0f) {
                map_aibo_spd_max = dist;
            } else {
                map_aibo_spd_max += 5.0f;
            }
            if (map_aibo_spd_max > 80.0f) {
                map_aibo_spd_max = 80.0f;
            }
        } else {
            if (map_aibo_spd_max - 5.0f < dist) {
                map_aibo_spd_max = dist;
            } else {
                map_aibo_spd_max -= 5.0f;
            }
            if (map_aibo_spd_max < 10.0f) {
                map_aibo_spd_max = 10.0f;
            }
        }
        if (dist > map_aibo_spd_max) {
            map_aibo_pos[0] -= (map_aibo_pos[0] - bx) / dist * map_aibo_spd_max / 10.0f;
            map_aibo_pos[1] -= (map_aibo_pos[1] - by) / dist * map_aibo_spd_max / 10.0f;
            map_aibo_pos[2] -= (map_aibo_pos[2] - bz) / dist * map_aibo_spd_max / 10.0f;
        } else {
            map_aibo_pos[0] -= (map_aibo_pos[0] - bx) / 10.0f;
            map_aibo_pos[1] -= (map_aibo_pos[1] - by) / 10.0f;
            map_aibo_pos[2] -= (map_aibo_pos[2] - bz) / 10.0f;
        }
        dist = fldPointDistance(map_aibo_pos[0], map_aibo_pos[1], map_aibo_pos[2], ox, oy, oz);
        if (dist > 6.5f) {
            speed = 0.79999996f;
            gMapAiboMotionBugCnt = 3;
            mode = 6;
        } else if (dist > 6.0f) {
            speed = 0.59999997f;
            gMapAiboMotionBugCnt = 3;
            mode = 6;
        } else if (dist > 5.5f) {
            speed = 0.5f;
            gMapAiboMotionBugCnt = 3;
            mode = 6;
        } else if (dist > 5.0f) {
            speed = 1.0f;
            mode = 1;
            if (gMapAiboMotionBugCnt > 0) {
                mode = 6;
                gMapAiboMotionBugCnt--;
            }
        } else if (dist > 4.5f) {
            mode = 1;
            speed = 1.0f;
            gMapAiboMotionBugCnt = 0;
        } else if (dist > 4.0f) {
            mode = 1;
            speed = 0.79999996f;
            gMapAiboMotionBugCnt = 0;
        } else {
            mode = 1;
            speed = 0.59999997f;
            gMapAiboMotionBugCnt = 0;
        }
        gMapAiboWakeUpMode = 0;
    }
    aiboBasic->poseset->pos.x = map_aibo_pos[0];
    aiboBasic->poseset->pos.y = map_aibo_pos[1];
    aiboBasic->poseset->pos.z = map_aibo_pos[2];
    dds3OnPoseSetFlag(aiboBasic->poseset, 1);
    dds3OnPoseSetFlag(aiboBasic->poseset, 8);
    SDF_VU0_COPYVECTOR(&aiboBasic->poseset->pos2, &aiboBasic->poseset->pos);
    bx = ox - map_aibo_pos[0];
    bz = oz - map_aibo_pos[2];
    if (bx > 0.099999995f || bx < -0.099999995f || bz > 0.099999995f || bz < -0.099999995f) {
        ang = sdfAtan2(bx, bz) * 57.324841f;
        if (ang < 0.0f) {
            ang += 360.0f;
        }
        if (ang > 360.0f) {
            ang -= 360.0f;
        }
        SDF_VU0_LOADVECTOR(&axis);
        diff = R360_LR(map_aibo_rot, -ang);
        step = diff / 5.0f;
        if (step < 0.0f) {
            step = -step;
        }
        if (step < 2.0f) {
            step = 2.0f;
        }
        if ((diff >= 0.0f && diff <= step) || (diff <= 0.0f && diff >= -step)) {
            diff = -ang;
        } else if (diff < 0.0f) {
            diff = map_aibo_rot - step;
        } else {
            diff = map_aibo_rot + step;
        }
        map_aibo_rot = diff;
        FromRotation(diff * 3.14f / 180.0f);
        SDF_VU0_STOREVECTOR(&rot);
        dds3SetRotPoseSet(aiboBasic, &rot);
    }
    if (gMapAiboWakeUpMode == 0) {
        if (aiboModel->current.h.motionIndex != mode) {
            mdlSmoothLoopMotion(aiboModel, 0, mode, 8.0f, 8.0f);
        }
        aiboModel->first->speed = speed;
    }
    mapUnitSetLight(aiboBasic, mapGetLight(map_aibo_pos[0], map_aibo_pos[2]), 10);
}
#else
INCLUDE_ASM(const s32, "field/fldFileResolver", mapAiboTask);
#endif
#else
INCLUDE_ASM(const s32, "field/fldFileResolver", mapAiboTask);

INCLUDE_ASM(const s32, "field/fldFileResolver", mapEnemy_SetFAtk_PlayerDir);
#endif

void mapSummonDvInit(void) {
    s32 i;

    for (i = 0; i < 256; i++) {
        map_summondv_buff[i][0] = 0.0f;
        map_summondv_buff[i][1] = 0.0f;
        map_summondv_buff[i][2] = 0.0f;
        mapWork3[i] = 0;
    }
    map_summondv_buff_read_idx = 0;
#ifdef VERSION_RAIDOU1
    map_summondv_buff_add_idx = 32;
#else
    map_summondv_buff_add_idx = 16;
#endif
    map_summondv_pos[0] = 0.0f;
    map_summondv_pos[1] = 0.0f;
    map_summondv_pos[2] = 0.0f;
    mapSummonDvTaskData2 = 10.0f;
    map_summondv_rot = 0.0f;
}

INCLUDE_ASM(const s32, "field/fldFileResolver", mapSummonDvAtumaru);

#ifdef VERSION_RAIDOU1
void mapSummonDvFoceSet(f32 x, f32 y, f32 z) {
    s32 i;

    for (i = 0; i < 256; i++) {
        map_summondv_buff[i][0] = x;
        map_summondv_buff[i][1] = y;
        map_summondv_buff[i][2] = z;
        mapWork3[i] = fldGlobalBase.nameU;
    }
    map_summondv_buff_read_idx = 0;
    map_summondv_buff_add_idx = 32;
    map_summondv_pos[0] = x;
    map_summondv_pos[1] = y;
    map_summondv_pos[2] = z;
    mapSummonDvTaskData2 = 6.0f;
    if (DDS3_GBWK->mapSummonID > 0 && summonDvBasic != NULL) {
        dds3GetRotPoseSet(summonDvBasic);
        map_summondv_rot = (EulerAngleAxisY() * 180.0f) / 3.14f;
    }
}
#else
INCLUDE_ASM(const s32, "field/fldFileResolver", mapSummonDvFoceSet);
#endif

INCLUDE_ASM(const s32, "field/fldFileResolver", mapNakamaWachLock);

INCLUDE_ASM(const s32, "field/fldFileResolver", mapSummonDvFoceSet_DeadReturn);

void mapSummonDvFoceSet2(f32 x, f32 y, f32 z) {
    sdfVector4_t rot;
    dds3PoseSet_t *pose;

    if (summonDvBasic != NULL) {
        summonDvBasic->poseset->pos.x = x;
        summonDvBasic->poseset->pos.y = y;
        summonDvBasic->poseset->pos.z = z;
        dds3OnPoseSetFlag(summonDvBasic->poseset, 1);
        dds3OnPoseSetFlag(summonDvBasic->poseset, 8);
        pose = summonDvBasic->poseset;
        SDF_VU0_COPYVECTOR(&pose->pos2, &pose->pos);
        if (fldGlobalBase.nakamaMode == 0) {
            dds3GetRotPoseSet(playerBasic);
            SDF_VU0_STOREVECTOR(&rot);
            dds3SetRotPoseSet(summonDvBasic, &rot);
        }
    }
    mapSummonDvFoceSet(x, y, z);
}

void mapSummonDvAdd(f32 x, f32 y, f32 z) {
    s32 i = map_summondv_buff_add_idx;
    s32 prev = i - 1;
    f32 dist;

    if (prev < 0) {
        prev = 255;
    }
    dist = fldPointDistance(map_summondv_buff[prev][0], map_summondv_buff[prev][1], map_summondv_buff[prev][2], x, y, z);
    if (gMapNakamaRunAtumaru == 0 && dist < 10.0f) {
        return;
    }
    map_summondv_buff[i][0] = x;
    map_summondv_buff[i][1] = y;
    mapWork3[i] = fldGlobalBase.nameU;
    map_summondv_buff[i][2] = z;
    map_summondv_buff_read_idx = (map_summondv_buff_read_idx + 1) % 256;
    map_summondv_buff_add_idx = (map_summondv_buff_add_idx + 1) % 256;
}

/* NON_MATCHING: 2 of 16 words match.  */
#ifdef NON_MATCHING
void mapSummonDvGet(f32 *x, f32 *y, f32 *z) {
    f32 *pos = map_summondv_buff[map_summondv_buff_read_idx];

    *x = pos[0];
    *y = pos[1];
    *z = pos[2];
}
#else
INCLUDE_ASM(const s32, "field/fldFileResolver", mapSummonDvGet);
#endif

INCLUDE_ASM(const s32, "field/fldFileResolver", fldCatGetNakamaSub);

INCLUDE_ASM(const s32, "field/fldFileResolver", sndSeStatus);

#ifdef VERSION_RAIDOU1
/* NON_MATCHING: 587 of 593 words match.  */
#ifdef NON_MATCHING
void mapSummonDvTask(void) {
    sdfVector4_t rot;
    sdfVector4_t axis = mapSummonDvTaskTable;
    f32 bx;
    f32 by;
    f32 bz;
    f32 ox;
    f32 oy;
    f32 oz;
    f32 px;
    f32 py;
    f32 pz;
    f32 dist;
    f32 speed;
    f32 ang;
    f32 diff;
    f32 step;
    f32 r;
    SBTL_UNIT_SUB *sub;
    s32 follow = 0;
    s32 mode;
    s32 se;
    s16 motion;
    s32 i;

    if (!fldEncountTestFlag(0x40)) {
        follow = gMapNakamaRunAtumaru == 0;
    }
    ox = map_summondv_pos[0];
    oy = map_summondv_pos[1];
    oz = map_summondv_pos[2];
    px = fldGlobalBase.focusX;
    py = fldGlobalBase.focusY;
    pz = fldGlobalBase.focusZ;
    dist = fldPointDistance(ox, oy, oz, px, py, pz);
    if (gMapNakamaRunAtumaru != 0) {
        for (i = 0; i < 2; i++) {
            mapSummonDvAdd(px, py, pz);
        }
    } else if (dist > 200.0f) {
        mapSummonDvAdd(px, py, pz);
    }
    mapSummonDvGet(&bx, &by, &bz);
    dist = fldPointDistance(map_summondv_pos[0], map_summondv_pos[1], map_summondv_pos[2], bx, by, bz);
    if (follow != 0) {
        if (dist < 4.0f) {
            mode = 0;
            map_summondv_pos[0] = bx;
            map_summondv_pos[1] = by;
            map_summondv_pos[2] = bz;
            mapSummonDvTaskData2 = 4.0f;
        } else {
            if (dist > 80.0f) {
                map_summondv_pos[0] -= (map_summondv_pos[0] - bx) / dist * 80.0f * 0.125f;
                map_summondv_pos[1] -= (map_summondv_pos[1] - by) / dist * 80.0f * 0.125f;
                map_summondv_pos[2] -= (map_summondv_pos[2] - bz) / dist * 80.0f * 0.125f;
            } else {
                map_summondv_pos[0] -= (map_summondv_pos[0] - bx) * 0.125f;
                map_summondv_pos[1] -= (map_summondv_pos[1] - by) * 0.125f;
                map_summondv_pos[2] -= (map_summondv_pos[2] - bz) * 0.125f;
            }
            mode = 1;
        }
    } else if (gMapNakamaRunAtumaru != 0) {
        if (dist < 1.0f) {
            mode = 0;
            map_summondv_pos[0] = bx;
            map_summondv_pos[1] = by;
            map_summondv_pos[2] = bz;
            mapSummonDvTaskData2 = 4.0f;
        } else {
            if (dist > 80.0f) {
                map_summondv_pos[0] -= (map_summondv_pos[0] - bx) / dist * 80.0f * 0.125f;
                map_summondv_pos[1] -= (map_summondv_pos[1] - by) / dist * 80.0f * 0.125f;
                map_summondv_pos[2] -= (map_summondv_pos[2] - bz) / dist * 80.0f * 0.125f;
            } else {
                map_summondv_pos[0] -= (map_summondv_pos[0] - bx) * 0.125f;
                map_summondv_pos[1] -= (map_summondv_pos[1] - by) * 0.125f;
                map_summondv_pos[2] -= (map_summondv_pos[2] - bz) * 0.125f;
            }
            mode = 1;
        }
        if (fldPointDistance(map_summondv_pos[0], map_summondv_pos[1], map_summondv_pos[2], px, py, pz) < 100.0f) {
            mapNakamaMinAlpDown(-1);
        }
    } else {
        if (dist < 1.0f) {
            mode = 0;
            map_summondv_pos[0] = bx;
            map_summondv_pos[1] = by;
            map_summondv_pos[2] = bz;
            mapSummonDvTaskData2 = 4.0f;
        } else {
            if (dist >= mapSummonDvTaskData2) {
                r = mapSummonDvTaskData2 + 3.0f;
                if (dist < r) {
                    mapSummonDvTaskData2 = dist;
                } else {
                    mapSummonDvTaskData2 = r;
                }
                if (mapSummonDvTaskData2 > 70.0f) {
                    mapSummonDvTaskData2 = 70.0f;
                }
            } else {
                r = mapSummonDvTaskData2 - 4.0f;
                if (r < dist) {
                    mapSummonDvTaskData2 = dist;
                } else {
                    mapSummonDvTaskData2 = r;
                }
                if (mapSummonDvTaskData2 < 6.0f) {
                    mapSummonDvTaskData2 = 6.0f;
                }
            }
            if (dist > mapSummonDvTaskData2) {
                map_summondv_pos[0] -= (map_summondv_pos[0] - bx) / dist * mapSummonDvTaskData2 * 0.125f;
                map_summondv_pos[1] -= (map_summondv_pos[1] - by) / dist * mapSummonDvTaskData2 * 0.125f;
                map_summondv_pos[2] -= (map_summondv_pos[2] - bz) / dist * mapSummonDvTaskData2 * 0.125f;
            } else {
                map_summondv_pos[0] -= (map_summondv_pos[0] - bx) * 0.125f;
                map_summondv_pos[1] -= (map_summondv_pos[1] - by) * 0.125f;
                map_summondv_pos[2] -= (map_summondv_pos[2] - bz) * 0.125f;
            }
            mode = 1;
            fldPointDistance(map_summondv_pos[0], map_summondv_pos[1], map_summondv_pos[2], ox, oy, oz);
        }
    }
    if (summonDvBasic == NULL) {
        return;
    }
    if (mode == 0 && mapCheckDying(1) != 0) {
        mode = 2;
    }
    sub = sbtlGetMotionVisualData(1, DDS3_GBWK->mapSummonID, mode);
    if (sub->flags & 1) {
        speed = sub->field10;
    } else {
        speed = sbtlGetVisualData(1, DDS3_GBWK->mapSummonID)->field0C;
    }
    summonDvBasic->poseset->pos.x = map_summondv_pos[0];
    summonDvBasic->poseset->pos.y = map_summondv_pos[1];
    summonDvBasic->poseset->pos.z = map_summondv_pos[2];
    dds3OnPoseSetFlag(summonDvBasic->poseset, 1);
    dds3OnPoseSetFlag(summonDvBasic->poseset, 8);
    SDF_VU0_COPYVECTOR(&summonDvBasic->poseset->pos2, &summonDvBasic->poseset->pos);
    bx = ox - map_summondv_pos[0];
    bz = oz - map_summondv_pos[2];
    if (bx > 0.099999995f || bx < -0.099999995f || bz > 0.099999995f || bz < -0.099999995f) {
        ang = sdfAtan2(bx, bz) * 57.324841f;
        if (ang < 0.0f) {
            ang += 360.0f;
        }
        if (ang > 360.0f) {
            ang -= 360.0f;
        }
        SDF_VU0_LOADVECTOR(&axis);
        diff = R360_LR(map_summondv_rot, -ang);
        step = diff * 0.125f;
        if (step < 0.0f) {
            step = -step;
        }
        if (step < 2.0f) {
            step = 2.0f;
        }
        if ((diff >= 0.0f && diff <= step) || (diff <= 0.0f && diff >= -step)) {
            diff = -ang;
        } else if (diff < 0.0f) {
            diff = map_summondv_rot - step;
        } else {
            diff = map_summondv_rot + step;
        }
        map_summondv_rot = diff;
        FromRotation(diff * 3.14f / 180.0f);
        SDF_VU0_STOREVECTOR(&rot);
        dds3SetRotPoseSet(summonDvBasic, &rot);
    } else {
        SDF_VU0_LOADVECTOR(&axis);
        FromRotation(map_summondv_rot * 3.14f / 180.0f);
        SDF_VU0_STOREVECTOR(&rot);
        dds3SetRotPoseSet(summonDvBasic, &rot);
    }
    motion = summonDvModel->current.h.motionIndex;
    if (motion >= 3 && (mode == 0 || mode == 2)) {
        return;
    }
    if (mode == 0 && motion == 13 && summonDvModel->first->stat != 5) {
        return;
    }
    if (motion != mode) {
        if (mode != 1) {
            sbtlSummonDvTaskSub(&mapSummonDvTaskData);
        }
        mdlSmoothLoopMotion(summonDvModel, 0, mode, 8.0f, 8.0f);
    } else if (motion == 1) {
        se = NakamaMotion3DSE(&mapSummonDvTaskData, DDS3_GBWK->mapSummonID, summonDvModel->first->frame);
        if (se != -1) {
            p2sdPlaySE(se, 127, 63);
        }
    }
    summonDvModel->first->speed = speed;
    mapUnitSetLight(summonDvBasic, mapGetLight(map_summondv_pos[0], map_summondv_pos[2]), 10);
}
#else
INCLUDE_ASM(const s32, "field/fldFileResolver", mapSummonDvTask);
#endif
#else
INCLUDE_ASM(const s32, "field/fldFileResolver", mapSummonDvTask);
#endif

void fldLeaf18(s32 hisMotion, s32 upMotion, f32 hokan) {
    if (hisMotion != upMotion) {
        playerModel->first->speed = 0.5f;
        mdlSmoothLoopMotion(playerModel, 0, upMotion, hokan, hokan);
    }
}

void fldLeaf19(s32 hisMotion, s32 upMotion, f32 hokan) {
    if (hisMotion != upMotion) {
        playerModel->first->speed = 0.5f;
        mdlSmoothSingleMotion(playerModel, 0, upMotion, hokan, hokan);
    }
}

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "field/fldFileResolver", fldPlayerCalcNmlFldSub);

#endif
void rstDrawEvoBgFade(s32 Motion, f32 spd) {
    fldPlayerModelContext->first->speed = spd;
    mdlSingleMotion(fldPlayerModelContext, 0, Motion);
}

void fldSetUnitSub3(s32 motion, f32 speed) {
    playerModel->first->speed = speed;
    mdlLoopMotion(playerModel, 0, motion);
}

INCLUDE_ASM(const s32, "field/fldFileResolver", fldLeaf20);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "field/fldFileResolver", NakamaMotion);
#else
INCLUDE_ASM(const s32, "field/fldFileResolver", fldCallBtlGetMotionVisualDataSBtlVi);
#endif

#ifdef VERSION_RAIDOU1
void PlayerMotionSe2(s32 se, f32 x, f32 z) {
    s32 id = 0;
    f32 prev = fldGlobalBase.PlayerMotionFrame;
    f32 frame = playerModel->first->frame;
    f32 next;

    fldGlobalBase.PlayerMotionFrame = frame;
    switch (mapCallSetPlayerTargetData5(se, playerModel, x, z)) {
    case 0:
        break;
    case 1:
        id = 4;
        break;
    case 2:
        id = 8;
        break;
    case 3:
        id = 12;
        break;
    }
    id += PlayerMotionSe2Data % 4;
    if (se == 1) {
        if (prev < 5.0f && frame >= 5.0f) {
            goto play;
        }
        next = 15.0f;
    } else if (se == 6) {
        if (prev < 16.0f && frame >= 16.0f) {
            goto play;
        }
        next = 36.0f;
    } else {
        return;
    }
    if (!(prev < next && frame >= next)) {
        return;
    }
play:
    p2sdPlaySE_Vol(id, 127);
    PlayerMotionSe2Data += ((dds3GetRandA(NULL) >> 4) & 1) + 1;
}
#else
INCLUDE_ASM(const s32, "field/fldFileResolver", fldPlayerCalcNmlFldSub2);

INCLUDE_RODATA(const s32, "field/fldFileResolver", VRF_Disable);

INCLUDE_RODATA(const s32, "field/fldFileResolver", mapEnemy_SetFAtk_PlayerDirTable);

INCLUDE_RODATA(const s32, "field/fldFileResolver", mapSummonDvTaskTable2);

INCLUDE_ASM(const s32, "field/fldFileResolver", PlayerMotionSe2);
#endif

void PlayerMotionSe(s32 hisMotion, s32 upMotion) {
    PlayerMotionSe2(upMotion, fldGlobalBase.focusX, fldGlobalBase.focusZ);
}

INCLUDE_ASM(const s32, "field/fldFileResolver", fldCallNakamaMotion3DSE);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldPlayer_MoveStart);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldPlayer_MoveCalc);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldProcMskSubBSub2);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldPlayer_MoveGet);

INCLUDE_ASM(const s32, "field/fldFileResolver", PlayerCalc_Nml);

#ifdef VERSION_RAIDOU1
/* NON_MATCHING: 329 of 733 words match.  */
#ifdef NON_MATCHING

s32 PlayerCalc_Nml_fld(void) {
    s32 dir = -1;
    sdfVector4_t pos;
    sdfVector4_t rot;
    sdfVector4_t old;
    sdfVector4_t axis = { 0.0f, 1.0f, 0.0f, 1.0f };
    f32 dirAngle[32] = {
        180.0f, 168.0f, 157.5f, 146.0f, 135.0f, 123.0f, 112.5f, 101.0f,
        90.0f, 78.0f, 67.5f, 46.0f, 45.0f, 33.0f, 22.5f, 11.0f,
        0.0f, -11.0f, -22.5f, -33.0f, -45.0f, -56.0f, -67.5f, -78.0f,
        -90.0f, -101.0f, -112.5f, -123.0f, -135.0f, -146.0f, -157.5f, -168.0f,
    };
    s32 motion;
    s32 dyingMotion;
    s32 motionIndex;
    f32 x;
    f32 y;
    f32 angle;
    f32 speed;
    f32 diff;
    f32 step;
    f32 rad;
    f32 stick;

    if (fldEncountTestFlag(0x40) == 0) {
        return 0;
    }
    if (fldGlobalBase.noInpPlCnt != 0) {
        return 0;
    }
    if (dds3PadStickState[2] < 0x40 || dds3PadStickState[2] > 0xC0 || dds3PadStickState[3] < 0x40 ||
        dds3PadStickState[3] > 0xC0) {
        x = dds3PadStickState[2] - 0x80;
        y = -(f32)(dds3PadStickState[3] - 0x80);
        if (x > -64.0f && x < 64.0f && y > -64.0f && y < 64.0f) {
            y = 0.0f;
            x = y;
        }
        stick = sdfAtan2(x, y) * 57.324841f + 5.625f;
        if (stick < 0.0f) {
            stick += 360.0f;
        }
        if (stick > 360.0f) {
            stick -= 360.0f;
        }
        dir = stick / 11.25f;
    }
    if (dir == -1) {
        if (dds3PadButtonState[0x24] && dds3PadButtonState[0x26]) {
            dir = 28;
        } else if (dds3PadButtonState[0x24] && dds3PadButtonState[0x27]) {
            dir = 20;
        } else if (dds3PadButtonState[0x25] && dds3PadButtonState[0x26]) {
            dir = 4;
        } else if (dds3PadButtonState[0x25] && dds3PadButtonState[0x27]) {
            dir = 12;
        } else if (dds3PadButtonState[0x24]) {
            dir = 24;
        } else if (dds3PadButtonState[0x25]) {
            dir = 8;
        } else if (dds3PadButtonState[0x26]) {
            dir = 0;
        } else if (dds3PadButtonState[0x27]) {
            dir = 16;
        }
    }
    dds3GetPosPoseSet(fldPlayerObject);
    SDF_VU0_STOREVECTOR(&pos);
    fldGlobalBase.floatA8 = fldGlobalBase.focusX;
    fldGlobalBase.floatAC = fldGlobalBase.focusY;
    fldGlobalBase.floatB0 = fldGlobalBase.focusZ;
    fldGlobalBase.focusX = pos.x;
    fldGlobalBase.focusY = pos.y;
    fldGlobalBase.focusZ = pos.z;
    while (fldGlobalBase.float28 >= 360.0f) {
        fldGlobalBase.float28 -= 360.0f;
    }
    while (fldGlobalBase.float28 < 0.0f) {
        fldGlobalBase.float28 += 360.0f;
    }
    while (fldGlobalBase.floatC0 >= 360.0f) {
        fldGlobalBase.floatC0 -= 360.0f;
    }
    while (fldGlobalBase.floatC0 < 0.0f) {
        fldGlobalBase.floatC0 += 360.0f;
    }
    dds3GetRotPoseSet(fldPlayerObject);
    SDF_VU0_STOREVECTOR(&rot);
    SDF_VU0_LOADMATRIX(&sdfViewMatrix);
    motion = 0;
    InversMatrix();
    dds3OnPoseSetFlag(fldPlayerObject->poseset, 8);
    if (fldGlobalBase.nakamaMode == 0) {
        if (mapCheckDying(0) != 0) {
            motion = 2;
        }
    } else {
        motion = 0;
        if (mapCheckDying(1) != 0) {
            motion = 2;
        }
    }
    dyingMotion = motion;
    motionIndex = fldPlayerModelContext->current.h.motionIndex;
    dds3GetPosPoseSet(fldPlayerObject);
    angle = fldGlobalBase.floatC4;
    if (fldGlobalBase.cutState == 1) {
        return 0;
    }
    if (fldGlobalBase.cutState == 0) {
        fldCallEncountTestFlagData3 = dir;
    } else if (dir == -1 || fldCallEncountTestFlagData3 != dir) {
        fldGlobalBase.cutState = 0;
    }
    if (dir != -1) {
        if (fldGlobalBase.cutState == 2) {
            angle = fldCallEncountTestFlagData2;
        } else {
            angle = mapGetScreenDir(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ, 0.0f) +
                    dirAngle[dir];
            fldCallEncountTestFlagData2 = angle;
        }
        speed = 7.0f;
        motion = 1;
        if (fldGlobalBase.nakamaMode != 0) {
            speed = mapLeaf26(DDS3_GBWK->mapSummonID);
        }
        diff = R360_LR(fldGlobalBase.floatC0, angle);
        if (diff < 0.0f) {
            diff = -diff;
        }
        if (diff > 90.0f) {
            speed = 0.0f;
        } else {
            speed = speed / 90.0f * (90.0f - diff);
        }
        rad = -angle;
        fldGlobalBase.focusY = mapGetY(fldGlobalBase.focusX, fldGlobalBase.focusZ);
        pos.x = fldGlobalBase.focusX - sdfSin(rad * 0.017453293f) * speed;
        pos.y = fldGlobalBase.focusY;
        pos.z = fldGlobalBase.focusZ - sdfCos(rad * 0.017453293f) * speed;
        dds3SetPosPoseSet_dds3Basic(fldPlayerObject, &pos);
    } else {
        pos.x = fldGlobalBase.focusX;
        pos.y = fldGlobalBase.focusY;
        pos.z = fldGlobalBase.focusZ;
        dds3SetPosPoseSet_dds3Basic(fldPlayerObject, &pos);
    }
    if (fldDbg.hitOn != 0 && fldEncountTestFlag(0x40) != 0) {
        fldGlobalBase.focusX = pos.x;
        fldGlobalBase.focusY = pos.y;
        fldGlobalBase.focusZ = pos.z;
        mapHamidasiCheck();
        pos.x = fldGlobalBase.focusX;
        pos.y = fldGlobalBase.focusY;
        pos.z = fldGlobalBase.focusZ;
        dds3SetPosPoseSet_dds3Basic(fldPlayerObject, &pos);
    }
    SDF_VU0_COPYVECTOR_F32(&old, &fldPlayerObject->poseset->pos2);
    if (fabsf(old.x - pos.x) < 1.0f) {
        fldPlayerObject->poseset->pos2.x = pos.x;
    } else {
        fldPlayerObject->poseset->pos2.x = old.x + (pos.x - old.x) / 3.0f;
    }
    if (fabsf(old.y - pos.y) < 1.0f) {
        fldPlayerObject->poseset->pos2.y = pos.y;
    } else {
        fldPlayerObject->poseset->pos2.y = old.y + (pos.y - old.y) * 0.25f;
    }
    if (fabsf(old.z - pos.z) < 1.0f) {
        fldPlayerObject->poseset->pos2.z = pos.z;
    } else {
        fldPlayerObject->poseset->pos2.z = old.z + (pos.z - old.z) / 3.0f;
    }
    fldPlayerObject->poseset->pos2.w = 1.0f;
    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    fldGlobalBase.floatC4 = fldSnapAngleToCompassPoint(angle);
    diff = R360_LR(fldGlobalBase.floatC0, angle);
    step = diff / (fldGlobalBase.nakamaMode == 0 ? 5.0f : (f32)fldGlobalBase.word174);
    if (step < 0.0f) {
        step = -step;
    }
    if (step < 2.0f) {
        step = 2.0f;
    }
    if (diff > 45.0f || diff < -45.0f) {
        motion = dyingMotion;
    }
    if ((0.0f <= diff && diff <= step) || (diff <= 0.0f && -step <= diff)) {
        diff = angle;
    } else if (diff < 0.0f) {
        diff = fldGlobalBase.floatC0 - step;
    } else {
        diff = fldGlobalBase.floatC0 + step;
    }
    fldGlobalBase.floatC0 = diff;
    SDF_VU0_LOADVECTOR(&axis);
    FromRotation(fldGlobalBase.floatC0 * 3.14f / 180.0f);
    SDF_VU0_STOREVECTOR(&rot);
    dds3SetRotPoseSet(fldPlayerObject, &rot);
    fldRestoreCameraModelColor();
    fldDisableCameraObjectFlag();
    if (fldGlobalBase.nakamaMode == 0) {
        if (fldGlobalBase.amSBlock == 3 && fldGlobalBase.nameU == 2) {
            fldLeaf18(motionIndex, motion, 6.0f);
        } else if (motionIndex == 1 && motion == 0 && fldGlobalBase.wordDC >= 11) {
            motion = 10;
            fldLeaf19(1, 10, 6.0f);
        } else if (motionIndex == 10 && motion == 0) {
            if (fldPlayerModelContext->first->stat == 5) {
                fldLeaf18(10, 0, 6.0f);
            }
        } else {
            fldLeaf18(motionIndex, motion, 6.0f);
        }
        PlayerMotionSe(motionIndex, motion);
    } else {
        NakamaMotion(motionIndex, motion, 6.0f);
        fldCallNakamaMotion3DSE(motionIndex, motion);
    }
    fldGlobalBase.focusX = pos.x;
    fldGlobalBase.focusY = pos.y;
    fldGlobalBase.focusZ = pos.z;
    ppos01Point.pos.x = fldPlayerObject->poseset->pos2.x;
    ppos01Point.pos.y = fldPlayerObject->poseset->pos2.y;
    ppos01Point.pos.z = fldPlayerObject->poseset->pos2.z;
    ppos01Point.pos.w = fldPlayerObject->poseset->pos2.w;
    ppos01Point.rot.x = fldPlayerObject->poseset->rot.x;
    ppos01Point.rot.y = fldPlayerObject->poseset->rot.y;
    ppos01Point.rot.z = fldPlayerObject->poseset->rot.z;
    ppos01Point.rot.w = fldPlayerObject->poseset->rot.w;
    return 0;
}
#else
INCLUDE_ASM(const s32, "field/fldFileResolver", PlayerCalc_Nml_fld);
#endif
#else
INCLUDE_RODATA(const s32, "field/fldFileResolver", fldPlayer_MoveCalcTable);

INCLUDE_RODATA(const s32, "field/fldFileResolver", PlayerCalc_Nml_fldTable);

INCLUDE_RODATA(const s32, "field/fldFileResolver", fldCallEncountTestFlagTable2);

INCLUDE_ASM(const s32, "field/fldFileResolver", PlayerCalc_Nml_fld);
#endif

s32 fldPlayerCalc(void) {
    if (fldDbg.menuOn != 0) {
        return 0;
    }
    if (fldEncountTestFlag(64) == 0 && map_player_move_mode != 0) {
        fldPlayer_MoveCalc();
        return 0;
    }
    if (fldEncountTestFlag(64) != 0) {
        PlayerCalc_Nml_fld();
        return 0;
    }
    return 0;
}

void fldEnableCameraObjectFlag(void) {
    dds3ObjectBaseOnFlag(fldPlayerObject, 1);
    fldSetCameraNodeModeWithZero();
}

void fldDisableCameraObjectFlag(void) {
    dds3ObjectBaseOffFlag(fldPlayerObject, 1);
}

INCLUDE_ASM(const s32, "field/fldFileResolver", fldLeaf21);

void fldRestoreCameraModelColor(void) {
    fldPlayerModelContext->inner->color = 0x80808080;
}

void fldSetCameraObjectTransitionFlag(void) {
    dds3ObjectBaseOnFlag(fldPlayerObject, 0x200);
}

void fldClearCameraObjectTransitionFlags(void) {
    dds3ObjectBaseOffFlag(fldPlayerObject, 0x400);
    dds3ObjectBaseOffFlag(fldPlayerObject, 0x200);
}
#ifdef VERSION_RAIDOU1

INCLUDE_RODATA(const s32, "field/fldFileResolver", VRF_Disable);

INCLUDE_RODATA(const s32, "field/fldFileResolver", mapSummonDvTaskTable);

INCLUDE_RODATA(const s32, "field/fldFileResolver", fldPlayer_MoveCalcTable);

INCLUDE_RODATA(const s32, "field/fldFileResolver", fldCallEncountTestFlagTable);

INCLUDE_RODATA(const s32, "field/fldFileResolver", fldCallEncountTestFlagTable2);

#endif
INCLUDE_SDATA(const s32, "field/fldFileResolver", map_aibo_buff_read_idx);

INCLUDE_SDATA(const s32, "field/fldFileResolver", map_aibo_buff_write_idx);

INCLUDE_SDATA(const s32, "field/fldFileResolver", map_aibo_spd_max);

INCLUDE_SDATA(const s32, "field/fldFileResolver", map_aibo_rot);

INCLUDE_SDATA(const s32, "field/fldFileResolver", mapAiboTaskData2);

INCLUDE_SDATA(const s32, "field/fldFileResolver", map_summondv_buff_read_idx);

INCLUDE_SDATA(const s32, "field/fldFileResolver", map_summondv_buff_add_idx);

INCLUDE_SDATA(const s32, "field/fldFileResolver", mapSummonDvTaskData2);

INCLUDE_SDATA(const s32, "field/fldFileResolver", map_summondv_rot);

INCLUDE_SDATA(const s32, "field/fldFileResolver", gMapNakamaRunAtumaru);

INCLUDE_SDATA(const s32, "field/fldFileResolver", mapAiboMinAlpDownData);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldAiboTaskSubData);

INCLUDE_SDATA(const s32, "field/fldFileResolver", gMapAiboWakeUpMode);

INCLUDE_SDATA(const s32, "field/fldFileResolver", mapAiboMinAlpDownData2);

INCLUDE_SDATA(const s32, "field/fldFileResolver", mapData56);

INCLUDE_SDATA(const s32, "field/fldFileResolver", mapData57);

INCLUDE_SDATA(const s32, "field/fldFileResolver", mapData58);

INCLUDE_SDATA(const s32, "field/fldFileResolver", gMapAiboMotionBugCnt);

INCLUDE_SDATA(const s32, "field/fldFileResolver", PlayerMotionSe2Data);

INCLUDE_SDATA(const s32, "field/fldFileResolver", map_player_move_mode);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldData19);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCallEncountTestFlagData2);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCallEncountTestFlagData3);

