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

typedef struct SdfGsTextureAlphaFlushPacket {
    SdfGsPacketHeader header;
    SdfGsRegisterWrite textureAlpha;
    SdfGsRegisterWrite textureFlush;
} SdfGsTextureAlphaFlushPacket;

typedef char SdfGsTextureAlphaFlushPacket_size_must_be_0x40[
    (sizeof(SdfGsTextureAlphaFlushPacket) == 0x40) ? 1 : -1];

struct dds3Basic_t;

struct EffNodeDescriptor;

typedef char FldSpawnBatchSize[(sizeof(FldSpawnBatch) == 12) ? 1 : -1];

/* Camera-setting table capacity. */
enum {
    FIELD_CAMERA_SETTING_COUNT = 8
};

typedef struct SdfDrawPacket SdfDrawPacket;

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

typedef struct FldModelMatrices {
    u8 pad00[0xC];
    u8 **rows; /* 0x0C: slot-indexed matrix blocks */
} FldModelMatrices;

struct EffRandState;

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
extern scrHeader_t *cameraBasic;

#define SDF_PACKED_STATUS_BIT 0x8000

#define SDF_PACKED_CHANNEL_MASK 0x7fff

#define SDF_UNIT_ENEMY 0x20

#define SDF_FLOAT_COUNTER_FROZEN_FLAG 0x00000008

#define SDF_FLOAT_COUNTER_WRAP_FLAG 0x00000020

void sdfFirePendingCallback(void);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldDrawExpandedSpriteStrip);

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

INCLUDE_ASM(const s32, "field/fldFileResolver", mapAiboWakeUp);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldGbInitSystemSub);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldProcMskSubBSub);

INCLUDE_ASM(const s32, "field/fldFileResolver", mapAiboAtumaru);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldNakamaBranchTaskSub);

INCLUDE_ASM(const s32, "field/fldFileResolver", mapAiboFoceSet);

INCLUDE_ASM(const s32, "field/fldFileResolver", mapAiboAtumaru2);

INCLUDE_ASM(const s32, "field/fldFileResolver", mapAiboFoceSet3);

INCLUDE_ASM(const s32, "field/fldFileResolver", mapAiboAdd);

INCLUDE_ASM(const s32, "field/fldFileResolver", mapAiboGet);

INCLUDE_ASM(const s32, "field/fldFileResolver", mapAiboTask);

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "field/fldFileResolver", mapEnemy_SetFAtk_PlayerDir);

#endif
INCLUDE_ASM(const s32, "field/fldFileResolver", mapSummonDvFoceSet_NotRot);

INCLUDE_ASM(const s32, "field/fldFileResolver", mapSummonDvAtumaru);

INCLUDE_ASM(const s32, "field/fldFileResolver", mapSummonDvFoceSet);

INCLUDE_ASM(const s32, "field/fldFileResolver", mapNakamaWachLock);

INCLUDE_ASM(const s32, "field/fldFileResolver", mapSummonDvFoceSet_DeadReturn);

INCLUDE_ASM(const s32, "field/fldFileResolver", mapSummonDvFoceSet2);

INCLUDE_ASM(const s32, "field/fldFileResolver", mapSummonDvAdd);

INCLUDE_ASM(const s32, "field/fldFileResolver", mapSummonDvGet);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldCatGetNakamaSub);

INCLUDE_ASM(const s32, "field/fldFileResolver", sndSeStatus);

INCLUDE_ASM(const s32, "field/fldFileResolver", mapSummonDvTask);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldLeaf18);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldLeaf19);

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "field/fldFileResolver", fldPlayerCalcNmlFldSub);

#endif
INCLUDE_ASM(const s32, "field/fldFileResolver", rstDrawEvoBgFade);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldSetUnitSub3);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldLeaf20);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "field/fldFileResolver", NakamaMotion);
#else
INCLUDE_ASM(const s32, "field/fldFileResolver", fldCallBtlGetMotionVisualDataSBtlVi);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "field/fldFileResolver", fldCallPlaySEVol);
#else
INCLUDE_ASM(const s32, "field/fldFileResolver", fldPlayerCalcNmlFldSub2);

INCLUDE_RODATA(const s32, "field/fldFileResolver", VRF_Disable);

INCLUDE_RODATA(const s32, "field/fldFileResolver", mapEnemy_SetFAtk_PlayerDirTable);

INCLUDE_RODATA(const s32, "field/fldFileResolver", mapSummonDvTaskTable2);

INCLUDE_ASM(const s32, "field/fldFileResolver", PlayerMotionSe2);
#endif

INCLUDE_ASM(const s32, "field/fldFileResolver", PlayerMotionSe);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldCallNakamaMotion3DSE);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldPlayer_MoveStart);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldPlayer_MoveCalc);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldProcMskSubBSub2);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldPlayer_MoveGet);

INCLUDE_ASM(const s32, "field/fldFileResolver", PlayerCalc_Nml);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "field/fldFileResolver", fldCallEncountTestFlag);
#else
INCLUDE_RODATA(const s32, "field/fldFileResolver", fldPlayer_MoveCalcTable);

INCLUDE_RODATA(const s32, "field/fldFileResolver", PlayerCalc_Nml_fldTable);

INCLUDE_RODATA(const s32, "field/fldFileResolver", fldCallEncountTestFlagTable2);

INCLUDE_ASM(const s32, "field/fldFileResolver", PlayerCalc_Nml_fld);
#endif

INCLUDE_ASM(const s32, "field/fldFileResolver", fldPlayerCalc);

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
INCLUDE_SDATA(const s32, "field/fldFileResolver", mapAiboAddData);

INCLUDE_SDATA(const s32, "field/fldFileResolver", mapAiboAddData2);

INCLUDE_SDATA(const s32, "field/fldFileResolver", mapAiboTaskData);

INCLUDE_SDATA(const s32, "field/fldFileResolver", mapData55);

INCLUDE_SDATA(const s32, "field/fldFileResolver", mapAiboTaskData2);

INCLUDE_SDATA(const s32, "field/fldFileResolver", mapSummonDvAddData);

INCLUDE_SDATA(const s32, "field/fldFileResolver", mapSummonDvAddData2);

INCLUDE_SDATA(const s32, "field/fldFileResolver", mapSummonDvTaskData2);

INCLUDE_SDATA(const s32, "field/fldFileResolver", map_summondv_rot);

INCLUDE_SDATA(const s32, "field/fldFileResolver", gMapNakamaRunAtumaru);

INCLUDE_SDATA(const s32, "field/fldFileResolver", mapAiboMinAlpDownData);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldAiboTaskSubData);

INCLUDE_SDATA(const s32, "field/fldFileResolver", mapAiboTaskData3);

INCLUDE_SDATA(const s32, "field/fldFileResolver", mapAiboMinAlpDownData2);

INCLUDE_SDATA(const s32, "field/fldFileResolver", mapData56);

INCLUDE_SDATA(const s32, "field/fldFileResolver", mapData57);

INCLUDE_SDATA(const s32, "field/fldFileResolver", mapData58);

INCLUDE_SDATA(const s32, "field/fldFileResolver", mapAiboTaskData4);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCallPlaySEVolData);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldData18);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldData19);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCallEncountTestFlagData2);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCallEncountTestFlagData3);

