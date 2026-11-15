#include "common.h"
#include "sdf_resource.h"
#include "basic.h"
#include "dds3obj.h"
#include "field.h"
#include "fileManager.h"

enum fldPackedResourceKind
{
    FLD_PACKED_RESOURCE_INFO_TABLE = 1,
    FLD_PACKED_RESOURCE_NPC_PALETTE = 2,
    FLD_PACKED_RESOURCE_SKY_BUFFER = 3,
    FLD_PACKED_RESOURCE_ACTOR_WAYPOINT_TABLE = 4,
    FLD_PACKED_RESOURCE_RETAINED_COPY = 5,
    FLD_PACKED_RESOURCE_SCENE_RECORD_CHUNK = 6
};

typedef struct objectTransform_t objectTransform_t;

typedef s32 (*dds3MoverUpdate_t)(struct objectTransform_t *, dds3Basic_t *);

typedef struct worldIndexNode_t worldIndexNode_t;

typedef struct motion_t motion_t;

typedef struct fldMoverWork_t fldMoverWork_t;

typedef struct fldTransferChunk_t fldTransferChunk_t;

typedef struct fldNpcPalette_t fldNpcPalette_t;

typedef struct fldSkyBuffer_t fldSkyBuffer_t;

/* Kind 3 allocates 0x10 bytes in dds3AllocateClearedObjectWork. */
struct fldMoverWork_t {
    dds3Basic_t *target; /* 0x00: object whose transform is updated. */
    dds3PathBasicControl_t *path; /* 0x04: retained curve work. */
    dds3MoverUpdate_t update; /* 0x08: used when no curve work is retained. */
    dds3Basic_t *sourceObject; /* 0x0C: kind-16 curve source. */
};

/* Packed-resource transfer descriptor; the first word's role is unknown. */
struct fldTransferChunk_t {
    u32 unk00; /* 0x00: opaque descriptor word */
    s32 offset; /* 0x04: signed byte offset from the resource base */
    u32 size; /* 0x08: byte length of the transferred payload */
};

#include "sdk_libc.h"

typedef struct fldModelData_t {
    u8 unk00[16]; /* 0x00 */
    u8 modelDef[16]; /* 0x10: the model definition dds3AddModelBasic2 takes */
    void *playBook; /* 0x20: motion play book */
} fldModelData_t;

typedef struct fldResourceAreaRef_t {
    u32 word00; /* 0x00 */
    const u8 *name; /* 0x04 */
} fldResourceAreaRef_t;

void sdfAddressResolverReset(u32 *aDataBody, u32 aOffset, const u8 *aAddrInfoPtr, s32 aAddrInfoLeng);
extern void *memcpy(void *a, const void *b, size_t c);
extern u32 fldNextTbSize, fldNextF1Size, fldNextF1Size, fldNextF2Size;
extern u32 fldNextTbData, fldNextF1Data, fldNextF1Data, fldNextF2Data;

typedef struct fldFileAreaDataFL1_t {
    u32 fileID;
    u32 SkyTblIdx;
    char *filename0;
    char *filename1;
    char *filename2;
    char *filename3;
} fldFileAreaDataFL1_t;

typedef struct fldFileInfoHeaderFL1_t {
    u32 nResourceTypeTable;
    void *pResourceTypeTable;
    fldFileAreaDataFL1_t *pAreaData;
} fldFileInfoHeaderFL1_t;

extern addr_t fldNextTbHandle, fldFileBufF1, fldFileBuf2;
extern u8 *FildMaskObj, *mat_black, *mat_white, *mat_line;

typedef struct fldConstruntAreaFL2_t {
    u32 flag; /* 0x00 */
    const char *areaName; /* 0x04 */
    u32 areaID; /* 0x08 */
} fldConstruntAreaFL2_t;

typedef struct fldFileAreaDataFL2_t {
    u32 word00;
    s32 label; /* 0x04: the script entry recorded for a field retry. */
} fldFileAreaDataFL2_t;

typedef struct fldFileInfoHeaderFL2_t {
    u32 nResourceTypeTable; /* 0x00 */
    void *pResourceTypeTable; /* 0x04 */
    fldFileAreaDataFL2_t *pAreaData; /* 0x08 */
    u32 nArea; /* 0x0C */
    fldConstruntAreaFL2_t *pAreaTable; /* 0x10 */
} fldFileInfoHeaderFL2_t;

/* FL1 and FL2 use the same 0x0C-byte resource-type table entries. */
typedef struct fldSpawnBatch_t {
    u32 kind;
    u32 count;
    fldResHead_t *entries;
} fldSpawnBatch_t;

typedef struct fldBillConfigView_t {
    u32 flags;
    u32 kind;
    u32 resourceIndex;
    f32 width;
} fldBillConfigView_t;

typedef struct fldLightDataView_t {
    u8 pad00[0x64];
    u32 flags;
} fldLightDataView_t;

typedef struct fldCollisionDescriptor_t fldCollisionDescriptor_t;

typedef struct fldSpawnCollisionView_t {
    u32 kind;
    u32 word04;
    fldCollisionDescriptor_t *header;
} fldSpawnCollisionView_t;

enum {
    FLD_SPAWN_BATCH_KIND_SCRIPT_RESOURCE = 2,
    FLD_SPAWN_BATCH_KIND_COLLISION = 3,
    FLD_SPAWN_BATCH_KIND_CAMERA = 4,
    FLD_SPAWN_BATCH_KIND_EVENT = 6,
    FLD_SPAWN_BATCH_KIND_UNSUPPORTED = 7,
    FLD_SPAWN_BATCH_KIND_PATH = 9,
    FLD_SPAWN_BATCH_KIND_POINT = 10,
    FLD_SPAWN_BATCH_KIND_BILLBOARD_EFFECT = 11,
    FLD_SPAWN_BATCH_KIND_LIGHT = 12
};

typedef struct fldResourceEffect_t fldResourceEffect_t;

typedef struct fldResourceLight_t {
    u32 resv, Flag, AnimeType;
    f32 InsideR, OutsideR, Soften, Bias;
    f32 DiffuseR, DiffuseG, DiffuseB;
    f32 AmbientR, AmbientG, AmbientB;
} fldResourceLight_t;

/* Resolved FL2 event and placement payloads, selected by the batch kind. */
typedef struct fldSpawnEventDataFL2_t {
    u32 kind;
    void *data;
} fldSpawnEventDataFL2_t;

typedef struct fldSpawnSoundDataFL2_t {
    f32 radius;
    s32 soundId;
} fldSpawnSoundDataFL2_t;

typedef struct fldEventDataHeader_t fldEventDataHeader_t;

extern u8 *FildMaskObj;
extern u8 *mat_black;
extern u8 *mat_white;
extern addr_t fldNextTbHandle;
extern addr_t fldFileBufF1;
extern u32 fldNextTbData;
extern u32 fldNextF1Data;
extern u32 fldNextTbSize;
extern u32 fldNextF1Size;

#include "kernel_draw.h"
#include "sdf_packet_list.h"
#include "include_asm.h"
#include "kernel.h"
#include "draw3d.h"
#include "evt_world.h"
#include "font.h"
#include "billboard.h"
#include "sdf_types.h"
#include "sdf_lighting.h"
#include "libsdf.h"
#include "model.h"
#include "motion.h"
#include "draw.h"
#include "sdf_movie_types.h"
#include "effect.h"
#include "ps2asm.h"
#include "fldWap.h"
#include "fpu.h"
#include "scr.h"
#include "newdata.h"
#include "fldFileResolver_00132080.h"
#include "dds3WorldBasic.h"
#include "dds3MatterObjectBasic.h"

/* Variable-sized packet header initialized by sdf2DMakePrimPacketHeader. */
typedef struct SdfDrawPacket {
    u16 quadwords;
    u8 pad02[6];
    u32 pad08;
    u32 command;
    u64 gifTag;
    u64 payloadHeader;
} SdfDrawPacket;

typedef char SdfDrawPacket_size_must_be_0x20[
    (sizeof(SdfDrawPacket) == 0x20) ? 1 : -1];

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

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */

typedef s32 FldPlacementBatchMode;

enum {
    FLD_PLACEMENT_BATCH_MODE_INITIAL = 0,
    FLD_PLACEMENT_BATCH_MODE_APPEND = 1
};

/* Scene-placement chunks use the same wire size but store a fallback script
 * name at +0x04 rather than a file-resolver index. */
typedef struct FldSceneResourceRecord {
    u32 unk00;
    const char *fallbackResourceName;
    const char *resourceA;
    const char *resourceB;
    const char *resourceC;
    const char *resourceD;
} FldSceneResourceRecord;

/* Object-spawn records carry a name pointer and signed world-node key. */
typedef struct FldActionSpawn {
    u32 unk00;
    const char *objectName;
    s32 key;
} FldActionSpawn;

typedef char FldSceneResourceRecord_size_check[
    (sizeof(FldSceneResourceRecord) == 0x18) ? 1 : -1];

typedef char FldActionSpawn_size_check[
    (sizeof(FldActionSpawn) == 0x0C) ? 1 : -1];

/* Counted placement-batch input consumed by both field request paths. */
typedef struct FldSceneRequest {
    u32 spawnBatchCount;
    FldSpawnBatch *spawnBatches;
    const FldSceneResourceRecord *record;
    u32 spawnCount;
    struct FldActionSpawn *spawnList;
} FldSceneRequest;

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

extern void fldSpawnPlacementObjectsFromBatches(FldSpawnBatch *spawnBatches, u32 batchCount,
                                                FldPlacementBatchMode mode);

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

void fldSpawnPlacementObjectsFromBatches(FldSpawnBatch *spawnBatch, u32 batchCount, FldPlacementBatchMode mode);
extern char D_0039FE08[], D_0039FE18[], D_0039FE28[];
void fldProcessFieldRequestAlternate(const FldSceneRequest *request);

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

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldMakeBlockBasicFL2);

typedef struct fldObjectPlacement_t {
    s32 serialID;
    u8 unk04[4];
    char *name; /* 0x08 */
    u8 unk0C[4];
    f32 *transform; /* 0x10: position at 0, rotation at 0x10 */
    fldResourceAreaRef_t *area; /* 0x14 */
    u8 unk18[8];
    fldModelData_t *model; /* 0x20 */
} fldObjectPlacement_t;

typedef struct fldResourceNameEntry_t {
    u32 word00;
    const char *name; /* 0x04 */
    u32 word08;
} fldResourceNameEntry_t;

typedef struct fldResourceNameTable_t {
    u32 count;
    fldResourceNameEntry_t entries[1];
} fldResourceNameTable_t;

typedef struct fldResourceSet_t {
    u8 unk00[0x18];
    fldResourceNameTable_t *names; /* 0x18 */
    u8 unk1C[8];
} fldResourceSet_t;

extern fldObjectPlacement_t *SetBattleLightWork;
extern u32 SetBattleLightWork2;
extern fldResourceSet_t *MakeResouceBasicFL2Work;
extern u32 MakeResouceBasicFL2Work2;
extern void *fldData15;
extern void dds3SetUnitObjectId(dds3Basic_t *basic, char *name);
extern void dds3ObjectBaseMakeMatter(dds3Basic_t *basic);
extern void dds3ObjectBaseRegistArea(dds3Basic_t *basic);

void MakeObjectBasic2(void)
{
    fldObjectPlacement_t *item = SetBattleLightWork;
    u32 count = SetBattleLightWork2;
    dds3Basic_t *scene;
    dds3Basic_t *matter;
    f32 position[4];
    f32 rotation[4];
    fldResourceSet_t *set;
    fldResourceAreaRef_t *areaRef;
    u32 i;
    u32 j;
    u32 k;

    if (item == NULL) {
        return;
    }
    scene = dds3GetControlScene();
    for (i = 0; i < count; i++, item++) {
        dds3AddModelBasic2(item->serialID, (sdfModelDef_t *)item->model->modelDef, item->model->playBook,
                           fldData15, item->name);
        fldCopySpawnTransform(position, rotation, item->transform);
        matter = dds3AddMatterObjectBasic(item->serialID, position, rotation);
        dds3SetUnitObjectId(matter, item->name);
        dds3MatterObjectBasicSetOt(matter, 9);
        dds3MatterObjectBasicSetHojiID(matter, fldCommand_GetHojiID(item->name));
        dds3ObjectBaseSetControlBasic(matter, dds3SearchSceneBasic(scene, item->serialID, 10));
        dds3ObjectBaseMakeMatter(matter);
        for (j = 0, set = MakeResouceBasicFL2Work; j < MakeResouceBasicFL2Work2; j++, set++) {
            if (set->names != NULL) {
                fldResourceNameEntry_t *entry = set->names->entries;

                for (k = 0; k < set->names->count; k++, entry++) {
                    if (strcmp(item->name, entry->name) == 0) {
                        break;
                    }
                }
            }
        }
        areaRef = item->area;
        if (matter != NULL) {
            dds3ObjectBaseSetControlBasic(matter,
                dds3SceneBasicSearchNameAndType(scene, 2, (char *)areaRef->name));
            dds3ObjectBaseRegistArea(matter);
        }
    }
}

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldFileResolverFL1);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldFileResolverFL2);

void fldProcessFieldRequestAlternate(const FldSceneRequest *request) {
    fldSpawnPlacementObjectsFromBatches(request->spawnBatches, request->spawnBatchCount,
                                        FLD_PLACEMENT_BATCH_MODE_APPEND);
}

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldGetInfoHeader);

void sdfAddressResolverReset(u32 *aDataBody, u32 aOffset, const u8 *aAddrInfoPtr, s32 aAddrInfoLeng) {
    const u8 *p = aAddrInfoPtr;
    s32 code;
    s32 i;

    while (p - aAddrInfoPtr < aAddrInfoLeng) {
        code = *p++;
        if ((code & 1) == 0) {
            code >>= 1;
        } else if ((code & 2) == 0) {
            code = (code | (*p++ << 8)) >> 2;
        } else if ((code & 4) == 0) {
            code = (code | (p[0] << 8) | (p[1] << 16)) >> 3;
            p += 2;
        } else {
            code = (code >> 3) + 2;
            for (i = 0; i < code; i++) {
                aDataBody++;
                *aDataBody -= aOffset;
            }
            continue;
        }
        aDataBody += code;
        *aDataBody -= aOffset;
    }
}

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldAddressResolverReset);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldAddressResolver);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldLeaf9);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldLeaf10);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldLeaf11);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldLeaf12);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldLeaf13);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldLeaf14);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldLeaf15);

void fldLeaf16(void) {
}

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldLoadFile);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldLoadFile_FromMemory);
INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldData15);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldCallTexListFreeData);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldCallTexListFreeData2);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldCallTexListFreeData3);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldCallTexListFreeData4);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldCallTexListFreeData5);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldCallTexListFreeData6);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldData16);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldCallTexListFreeData7);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldCallTexListFreeData8);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldData17);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldCallTexListFreeData9);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldCallTexListFreeData10);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", rsetFrameMaskOtOffset);

