#include "common.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "fileManager.h"

#include "dds3obj.h"
#include "kernel_draw.h"
#include "libsdf.h"
#include "sdf_resource.h"
#include "sdk_libc.h"
#include "include_asm.h"
#include "newdata.h"
#include "billboard.h"
#include "sdf_types.h"
#include "draw.h"
#include "sdf_movie_types.h"
#include "effect.h"
#include "sdf_lighting.h"
#include "model.h"
#include "motion.h"
#include "field.h"
#include "ps2asm.h"
#include "fpu.h"
#include "code_r1_0021CE60.h"
#include "effBattleMisc.h"
#include "fldFileResolver_00132080.h"
#include "effPCPMisc_00224D10.h"
#include "code_r1_002C8188.h"

/* Resource jobs retain a destination slot until their request completes. */
typedef struct EffectListNode {
    u32 state;
    struct EffectListNode *next;
    u32 value;
    u32 length;
    u32 kind;
    void **reference;
} EffectListNode;

typedef struct EffectList {
    u32 mode;
    s32 count;
    EffectListNode *first;
    EffectListNode *last;
    struct fileRequest *request;
} EffectList;

typedef char EffectListNode_size_must_be_0x18[(sizeof(EffectListNode) == 0x18) ? 1 : -1];

typedef char EffectList_size_must_be_0x14[(sizeof(EffectList) == 0x14) ? 1 : -1];

/* These title-specific wrappers forward an opaque stage-task owner and its
 * handled/not-handled result. */
s32 pacFileHandleFree_(void *request);

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */

enum {
    EFF_RESOURCE_SOURCE_TRANSIENT = 0,
    EFF_RESOURCE_SOURCE_RETAINED = 1
};

static inline void effSubmitSurfacePacket(sdfGrOt_t *surface, sdfDrawTag_t *list) {
    surface->regFunc(surface, list);
}

extern s32 effRequestResourceByMode(u32, u32, u32, void **);

typedef struct EffectObjectFlag {
    u32 state;
    u8 pad04[8];
    u32 flags;
} EffectObjectFlag;

/* Native resource operations add cloning before the frame callbacks.
 * The final word is the copied payload size, not another callback.
 * Factories retain this unit's existing address-word interface. */
typedef struct EffResourceOps {
    void (*initialize)();      /* 0x00 */
    u32 (*createResource)();    /* 0x04 */
    void (*destroyResource)(); /* 0x08 */
    u32 (*cloneResource)(void *); /* 0x0C */
    void (*update)();          /* 0x10 */
    void (*draw)();            /* 0x14 */
    u32 payloadSize;           /* 0x18 */
} EffResourceOps;

/* Active frame callbacks receive the parent; destruction receives its child. */
typedef struct EffActiveInstanceOps {
    void (*initialize)(EffClassWork *); /* 0x00 */
    u32 (*createResource)(); /* 0x04 */
    void (*destroyResource)(void *); /* 0x08 */
    u32 (*cloneResource)(void *); /* 0x0C */
    void (*update)(EffClassWork *); /* 0x10 */
    void (*draw)(EffClassWork *); /* 0x14 */
    u32 payloadSize; /* 0x18 */
} EffActiveInstanceOps;

typedef char EffActiveInstanceOpsSizeCheck[(sizeof(EffActiveInstanceOps) == 0x1C) ? 1 : -1];

/* Class-resource callbacks operate on the parent class-work owner. */
typedef struct EffClassResourceOps {
    void (*initialize)(EffClassWork *); /* 0x00 */
    void *(*createResource)(EffBillPointConfig *); /* 0x04 */
    void (*destroyResource)(EffClassWork *); /* 0x08 */
    void (*update)(EffClassWork *); /* 0x0C */
    void (*draw)(EffClassWork *); /* 0x10 */
    u32 payloadSize; /* 0x14 */
} EffClassResourceOps;

typedef char EffClassResourceOps_size_must_be_0x18[
    (sizeof(EffClassResourceOps) == 0x18) ? 1 : -1];

/* Block-resource callbacks receive the owning class work, not the payload. */
typedef struct EffBlockOps {
    void (*initialize)(EffClassWork *); /* 0x00 */
    u32 (*createResource)(); /* 0x04: preserve the existing generic result ABI */
    void (*destroyResource)(EffClassWork *); /* 0x08 */
    u32 (*cloneResource)(void *); /* 0x0C: preserve the existing generic result ABI */
    void (*update)(EffClassWork *); /* 0x10 */
    void (*draw)(EffClassWork *); /* 0x14 */
    u32 payloadSize; /* 0x18 */
} EffBlockOps;

typedef char EffBlockOps_size_must_be_0x1C[
    (sizeof(EffBlockOps) == 0x1C) ? 1 : -1];

struct EffModelResource;

struct EffSpanConfig;

struct EffSpanTable;

/* This table has a single populated kind with span-resource callbacks. */
typedef struct EffModelResourceOps {
    void (*initialize)(struct EffModelResource *); /* 0x00 */
    struct EffSpanTable *(*createResource)(struct EffSpanConfig *, dds3ModelHandle_t *); /* 0x04 */
    void (*destroyResource)(struct EffSpanTable *); /* 0x08 */
    void (*update)(struct EffModelResource *); /* 0x0C */
    void (*draw)(struct EffModelResource *); /* 0x10 */
    u32 payloadSize; /* 0x14 */
} EffModelResourceOps;

typedef char EffModelResourceOps_size_must_be_0x18[
    (sizeof(EffModelResourceOps) == 0x18) ? 1 : -1];

/* Shared work object for the effEditScalarTrack helpers (layout inferred). */
typedef struct BaObj {
    u8 pad_0x00[0x0C]; // 0x00
    u8 *x0C;           // 0x0C
} BaObj; // 0x10

struct EffCameraCreateRow;

/* Value holder with a float field and a u16 data pointer (layout inferred). */
typedef struct ValPtr44 {
    u8 pad_0x00[0x08]; // 0x00
    float f08;         // 0x08
    u8 pad_0x0C[0x38]; // 0x0C
    u16 *p44;          // 0x44
} ValPtr44; // 0x48

struct EffKindWork;

/* Native kind operations include handle destruction and per-frame update;
 * zero-argument notifications retain the original unprototyped callback ABI. */
typedef struct EffKindDesc {
    void *(*create)(void *); // 0x00
    void (*destroy)(void *); // 0x04: release the created handle
    void (*update)();        // 0x08: advance work through its kind callback
    void (*initialize)(struct EffKindWork *, sdfTexHandle_t *); // 0x0C
    u32 payloadSize;         // 0x10: bytes copied after the work header
} EffKindDesc; // 0x14

/* Native kind work: 0x40-byte header, followed by the copied source payload.
 * Fade callbacks consume this same object: frame supplies their signed limit,
 * handle is the renderer output, and payload holds the curve configuration. */
typedef struct EffKindWork {
    f32 position[4]; /* 0x00: vector passed to projection routines */
    s32 mode;       // 0x10
    u32 color;      // 0x14
    f32 scale;      // 0x18
    s32 kind;       // 0x1C
    u32 frame;        // 0x20, incremented by the kind callback dispatcher
    void *handle;   // 0x24
    void *payload;  // 0x28
    u32 sourceKind; // 0x2C
    sdfTexHandle_t *target;     // 0x30
    u8 pad_0x34[0xC]; // 0x34
} EffKindWork; // 0x40

/* Source payload's kind selects the initial rendering mode. */
typedef struct EffKindSource {
    u8 pad_00[0x28];
    s32 modeKind;
} EffKindSource;

typedef struct EffFadeHeaderWork {
    s32 frame;
    s32 payload[2];
} EffFadeHeaderWork;

typedef char EffFadeHeaderWorkSizeCheck[(sizeof(EffFadeHeaderWork) == 0x0C) ? 1 : -1];

typedef struct EffSelectionHeaderWork {
    s32 frame;
    s32 payload[4];
} EffSelectionHeaderWork;

typedef char EffSelectionHeaderWorkSizeCheck[(sizeof(EffSelectionHeaderWork) == 0x14) ? 1 : -1];

typedef struct EffFadeConfig {
    /* Color and alpha tracks at 0x00/0x24, followed by three scalar
     * tracks at 0x34, 0x60 and 0x8C. The scalar extents differ. */
    SdfColorTrack blendA;   /* 0x00 */
    SdfAlphaTrack blendB2;  /* 0x24 */
    EffScalarCurve blendB;   /* 0x34 */
    u8 pad58[8];
    EffScalarTrack rateA;   /* 0x60 */
    EffScalarTrack rateB;   /* 0x8C */
    s32 duration;         /* 0xB8 */
    u8 padBC[4];
    dds3Effect_Filter_t out; /* 0xC0 */
} EffFadeConfig;

typedef struct EffRateConfig {
    /* Color and alpha tracks at 0x00/0x24, followed by three scalar
     * tracks at 0x34, 0x60 and 0x8C. The scalar extents differ. */
    SdfColorTrack blendA;   /* 0x00 */
    SdfAlphaTrack blendB2;  /* 0x24 */
    EffScalarCurve blendB;   /* 0x34 */
    u8 pad58[8];
    EffScalarTrack rateA;   /* 0x60 */
    EffScalarTrack rateB;   /* 0x8C */
    s32 duration;         /* 0xB8 */
    u8 fixedMode;         /* 0xBC */
    u8 padBD[3];
    dds3Effect_Blur_t out;       /* 0xC0 */
} EffRateConfig;

typedef struct EffBillFrameEntry {
    s32 age;
    f32 spin;
    f32 angle;
    f32 radius;
    f32 innerWidth;
    f32 outerWidth;
} EffBillFrameEntry;

typedef char EffBillFrameEntrySizeCheck[(sizeof(EffBillFrameEntry) == 0x18) ? 1 : -1];

/* Kind-4 animated frame entry: the radius grows by radiusStep each frame
 * for the config's drawProgress frames. */
typedef struct EffBillAnimatedEntry {
    s32 age;
    f32 spin;
    f32 angle;
    f32 radius;
    f32 radiusStep;
    f32 width;
} EffBillAnimatedEntry;

/* Four-byte owner header followed by its class-work pointer array. */
typedef struct EffClassWorkList {
    EffClassWork **entries;
} EffClassWorkList;

/* The ring source selects a minimum of three segments and repeats its three colors. */
typedef struct EffRingSource {
    u8 pad_00[0x38];
    u32 segments;       // 0x38
    u8 pad_3C[8];
    u32 firstColor;     // 0x44
    u32 middleColor;    // 0x48
    u32 lastColor;      // 0x4C
} EffRingSource;

/* Class kind 3 copies this complete 0x68-byte serialized parameter record.
 * The color/alpha prefix is the existing interpolation provider's layout;
 * alphaTrack.surfaceIndex at 0x28 also selects the point-set draw type. */
typedef struct EffRadialRingParams {
    SdfColorTrack colorTrack; /* 0x00 */
    SdfAlphaTrack alphaTrack; /* 0x24 */
    u32 duration;            /* 0x34 */
    u32 segments;            /* 0x38 */
    u8 flag;                 /* 0x3C */
    u8 pad3D[3];
    f32 baseRadius;          /* 0x40 */
    u32 firstColor;          /* 0x44 */
    u32 middleColor;         /* 0x48 */
    u32 lastColor;           /* 0x4C */
    f32 widths[3];           /* 0x50 */
    f32 speed;               /* 0x5C */
    f32 acceleration;        /* 0x60 */
    u8 reverseTime;          /* 0x64 */
    u8 pad65[3];
} EffRadialRingParams;

typedef char EffRadialRingParams_size_must_be_0x68[(sizeof(EffRadialRingParams) == 0x68) ? 1 : -1];

struct sdfMat_t;

typedef struct EffCounterHeader {
    u32 unk00;
    u32 frame;
} EffCounterHeader;

/* Class draw modes own either a counter header or per-segment scales. */
typedef struct EffClassDrawState {
    union {
        EffCounterHeader *ring;
        f32 *scales;
    };
    EffClassWork *effect;
    EffTrackSet *trackSet;
    struct sdfMemHandle_t *allocation;
} EffClassDrawState;

/* Class 1 ring parameters have a distinct 0x6C-byte copied layout. */
typedef struct EffExpandingRingParams {
    SdfColorTrack colorTrack;
    SdfAlphaTrack alphaTrack;
    s32 duration;
    u32 segments;
    u8 flag;
    u8 pad3D[3];
    f32 baseRadius;
    u32 colors[3];
    f32 widths[3];
    u32 unk5C;
    f32 speed;
    f32 acceleration;
    u8 reverseTime;
    u8 pad69[3];
} EffExpandingRingParams;

typedef char EffExpandingRingParams_size[(sizeof(EffExpandingRingParams) == 0x6C) ? 1 : -1];

/* The cached class payload is copied from the 0x5C-byte source prefix. */
typedef struct EffRingClassConfig {
    EffRingSource ring;
    u8 pad50[0xC];
    u8 classConfig[0x5C];
    u8 padB8[0x10];
    f32 scale;
    f32 scaleRand;
} EffRingClassConfig;

/* Point history of a grid-surface trail: a ring of 16-byte points with packed colors,
 * drawn as triangle strips into the current render packet, then a fixed end cap. */
typedef struct EffSurfaceTrailHistory {
    u8 pad00[0x10];
    s32 pointCapacity;        /* 0x10 */
    s32 activePointCount;     /* 0x14 */
    s32 nextPointWriteIndex;  /* 0x18 */
    u8 pad1C[0x08];
    u128 *points;             /* 0x24 */
    u32 *colors;              /* 0x28 */
    s32 resourceHandle;       /* 0x2C */
    u128 *endPoints;          /* 0x30 */
    u32 *endColors;           /* 0x34 */
} EffSurfaceTrailHistory;

typedef struct EffDrawEntry {
    u32 handle;
    s32 frame;
    u8 pad08[8];
    void (*draw)(u8 *, struct sdfDrawTag_t *);
} EffDrawEntry;

typedef struct Matrix4 Matrix4;

typedef struct EffScaleRange {
    EffScaleRangeEntry *entries;
    f32 start;
    f32 delta;
    struct sdfMemHandle_t *allocation;
} EffScaleRange;

typedef struct EffSpiralParticle {
    s32 timer;      /* -1 free, -2 retired */
    u8 pad04[4];
    f32 tiltX;      /* 0x08 */
    f32 tiltY;      /* 0x0C */
    f32 radius;     /* 0x10 */
    f32 radiusStep; /* 0x14 */
    f32 spin;       /* 0x18 */
    f32 phase;      /* 0x1C */
    f32 width;      /* 0x20 */
    f32 sweep;      /* 0x24 */
    f32 height;     /* 0x28 */
} EffSpiralParticle;

static inline void unpackNormalizedColor(u32 color) {
    u32 rgba[4];
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK(rgba, 1.0f / 128.0f);
}

static inline u32 packNormalizedColor(void) {
    u32 rgba[4];
    EE_MMI_RGBA_PACK_UNIT(rgba[0], 128.0f);
    return rgba[0];
}

typedef char EffModelResource_size_must_be_0x40[
    (sizeof(EffModelResource) == 0x40) ? 1 : -1];

typedef struct EffModelCreateRequest {
    u8 pad0[0x2C];
    u16 kind;
    u8 pad2E[2];
    dds3ModelHandle_t *assetId;
    u32 attributes;
    u8 pad38[4];
    void *source;
} EffModelCreateRequest;

#define EFF_COLLECT_ACTORS_VARIANT_SELECTOR 0

#define EFF_COLLECT_ACTORS_ORIGINAL_SELECTOR 1

#define EFF_COLLECT_ACTORS_VARIANT_SELECTOR_ALIAS 2

#define EFF_COLLECT_ACTORS_ORIGINAL_GROUP 3

#define EFF_COLLECT_ACTORS_VARIANT_GROUP 4

#define EFF_COLLECT_ACTORS_EITHER_GROUP 5

#define EFF_COLLECT_ACTORS_OVERRIDE_SELECTOR 6

#define EFF_COLLECT_ACTORS_FINAL_SELECTOR 7

/* Payload of resource kind 2: two colours plus the position request that feeds the shared light vector. */
typedef struct EffFieldColorConfig {
    u32 firstColor;   /* 0x00 */
    u32 secondColor;  /* 0x04 */
    EffectVectorRequest direction; /* 0x08 */
} EffFieldColorConfig;

typedef struct EffCopiedSpawnList {
    u32 count;
    FldSpawnBatch *groups;
} EffCopiedSpawnList;

extern u128 D_0037EE90, D_0037EEA0, D_0037EEB0, D_0037EEC0, D_0037EED0;

typedef struct EffAnimInfo {
    u16 id;
    u16 flags;
    u8 actorSelection;
    u8 unk05;
    u16 loop;
} EffAnimInfo;

/* The final timer word also supplies the default tint's halfword ID. */
typedef struct EffTintTransitionParams {
    u32 duration;
    u32 resourceArgument;
    u16 resourceKind;
    u16 unk0A;
    union {
        u32 delay;
        struct {
            u16 defaultKind;
            u16 unk0E;
        } ids;
    } tail;
} EffTintTransitionParams;

/* Payload option (+0x0C) selects the asset identifier at +0x08. */
typedef struct EffAssetIdentifier {
    u8 pad_00[4];
    u16 type;       // 0x04
    u8 pad_06[2];
    u16 id;         // 0x08
} EffAssetIdentifier;

typedef struct EffCameraCreateRow {
    EffCameraCreateRequest request;
    u32 sourceTail;
} EffCameraCreateRow;

typedef char EffCameraCreateRowSizeCheck[(sizeof(EffCameraCreateRow) == 0x1C) ? 1 : -1];

typedef struct EffectBlock90 {
    u32 word[36];
} EffectBlock90;

typedef struct EffectAlignedBlock128 {
    u64 word[16];
} EffectAlignedBlock128;

extern char D_003B3A90[], D_003B3AA8[];

typedef struct EffectStateSnapshot {
    s128 vectors[4];
    f32 position[4]; /* 0x40: copied to the debug camera position and follow offset. */
    s128 unused50;
    u8 pad60[8];
    u32 flags; /* 0x68: transform flags tested by the camera diagnostic. */
    u8 pad6C[0x14];
} EffectStateSnapshot;

typedef char EffectStateSnapshotSizeCheck[(sizeof(EffectStateSnapshot) == 0x80) ? 1 : -1];

/* The camera snapshot payload is the 0x90-byte record consumed by this updater. */
typedef union EffectCameraSnapshotView {
    u32 word[36];
    struct {
        u8 pad00[0x40];
        f32 offset[4];
        f32 quat[4];
        u8 pad60[0x30];
    } camera;
} EffectCameraSnapshotView;

typedef struct EffectCameraDebugState {
    u32 unknown00[4];
    u32 flags;
    u32 unknown14[9];
} EffectCameraDebugState;

typedef struct EffectCameraDebugScratch {
    s128 basePosition;
    s128 segmentStart;
    s128 segmentEnd;
    s128 rotationMatrix[4];
} EffectCameraDebugScratch;

typedef struct EffectPollRecord {
    char fullName[0xC8]; /* Formatted path or selected built-in name. */
    s32 type;   // 0xC8
    s32 selectionStatus;  // 0xCC
    s32 pathVariant; // 0xD0
} EffectPollRecord;

/* The effect mapping dispatcher stores its data pointer at +0x0C. */
typedef struct EffMappingRequest {
    u8 pad_00[0x0C];
    s32 object;          // 0x0C
} EffMappingRequest;

/* Parameters passed to the mapping dispatcher from distinct object channels. */
typedef struct EffMappingObject {
    u8 pad_00[0x34];
    s32 value34;       // 0x34
    u32 value38;       // 0x38
    s32 value3C;
    u8 pad40[0xC];
    u32 value4C;
    u8 pad50[0x20];
    u32 value70;       // 0x70
    s32 value74;
    u8 pad78[8];
    u32 value80;       // 0x80
    u32 value84;       // 0x84
    u8 pad_88[4];
    s32 value8C;       // 0x8C
    u8 pad_90[0x28];
    u32 valueB8;       // 0xB8
    u8 pad_BC[0x18];
    s32 valueD4;       // 0xD4
    s32 valueD8;       // 0xD8
    u8 pad_DC[0x18];
    s32 valueF4;       // 0xF4
} EffMappingObject;

typedef struct EffFileResourceRecord {
    char *name;
    u8 pad4[4];
    u16 mode;
    u8 padA[2];
    u8 *buffer;
    u32 size;
    struct sdfMemHandle_t *allocation;
} EffFileResourceRecord;

typedef struct EffectFileHeader {
    u8 unk_00[8];
    u16 mode;
    u8 unk_0A[2];
    u32 start;
    u32 length;
} EffectFileHeader;

s32 effAppendListEntry(EffectList *list, u32 value, u32 length,
                          u32 kind, u32 reference);

s32 effRemoveListEntry(EffectList *list);
s32 effPollResourceList(EffectList *list);

s32 effAppendListEntry(EffectList *list, u32 value, u32 length,
                          u32 kind, u32 reference) {
    EffectListNode *node = sdfChipAllocDirect(sizeof(EffectListNode));
    memset(node, 0, sizeof(EffectListNode));
    node->next = NULL;
    node->kind = kind;
    node->value = value;
    node->length = length;
    node->reference = (void **)reference;
    if (list->last == NULL) {
        list->first = node;
        list->last = node;
    } else {
        list->last->next = node;
        list->last = node;
    }
    return ++list->count;
}

s32 effRemoveListEntry(EffectList *list) {
    EffectListNode *node = list->first;
    EffectListNode *next = node->next;
    sdfChipFreeDirect(node);
    list->first = next;
    if (--list->count == 0) {
        list->last = NULL;
    }
    return list->count;
}

/* Drives the head request of the list: mode 0 asks for a resource by name, modes 1 and 2 stream a package and hand each
 * finished job's buffer to the entry's destination. Returns the remaining entry count. */
s32 effPollResourceList(EffectList *list) {
    EffectListNode *node = list->first;
    sdfPacDecodeResrc_t *item;
    struct sdfMemHandle_t *allocation;

    if (node != NULL) {
        switch (list->mode) {
        case 0:
            if (node->state == 0) {
                effRequestResourceByMode(node->value, node->length, node->kind, node->reference);
                node->state = 1;
            } else if (*node->reference != NULL) {
                effRemoveListEntry(list);
            }
            break;
        case 1:
        case 2:
            if (node->state == 0 && node->length != 0) {
                if (list->request != NULL) {
                    pacFileHandleFree_(list->request);
                }
                list->request = pacLoad((const char *)node->length);
                if (list->mode == 2) {
                    pacSync(list->request);
                }
                node->state = 1;
            } else if (pacCheckEnd(list->request) != 0) {
                for (item = list->request->pac.resrcHead; item != NULL; item = item->next) {
                    if (((sdfPacStdBlockHeader_t *)item->header)->fileType == 1) {
                        node = list->first;
                        allocation = item->resrc;
                        *node->reference = effCreateResourceSlotSetFromAllocation(allocation, node->kind);
                        if (node->kind == EFF_RESOURCE_SOURCE_TRANSIENT) {
                            sdfMemFreeDirect(allocation);
                        }
                        effRemoveListEntry(list);
                    }
                }
            }
            if (list->count == 0) {
                pacFileHandleFree_(list->request);
                list->request = NULL;
            }
            break;
        }
    }
    return list->count;
}
