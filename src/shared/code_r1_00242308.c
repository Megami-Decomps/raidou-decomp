#include "common.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "sdfQuat.h"
#include "sdfFile.h"
#include "sdf_Decomp.h"

#include "dds3obj.h"
#include "kernel_draw.h"
#include "libsdf.h"
#include "sdf_resource.h"
#include "sdk_libc.h"

#include "include_asm.h"
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
#include "code_r1_0017BBD8.h"
#include "code_r1_0021C768.h"
#include "effBattleMisc.h"
#include "fldFileResolver_00132080.h"
#include "effPCPMisc_00224D10.h"

/* Multiply the quaternions in vf10 and vf11, writing the product to vf10. */

/* Convert the quaternion in vf10 into the primary matrix bank vf28-vf31. */

/* Stable state APIs. Callback factories keep caller-specific callback types. */
sdfFileCmdHandle_t *sdfFileOpen(const char *path);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */

/* A 128-byte block the effect code copies whole (the effect file queue's saved state). */
typedef struct EffectBlock128 {
    u32 word[32];
} EffectBlock128;

/* Filters consumed by the directory scan; these are independent of entry categories. */
enum BtlResourceScanFlags {
    BTL_RESOURCE_SCAN_TMX = 0x01,
    BTL_RESOURCE_SCAN_P2A = 0x02,
    BTL_RESOURCE_SCAN_PB = 0x04,
    BTL_RESOURCE_SCAN_GENERAL = 0x08,
    BTL_RESOURCE_SCAN_EPL = 0x10,
    BTL_RESOURCE_SCAN_EP = 0x20,
    BTL_RESOURCE_SCAN_TLP = 0x40,
    BTL_RESOURCE_SCAN_F2 = 0x80,
};

typedef struct EffResourceBankSlot {
    char fullName[0xC8]; /* Formatted path or selected built-in name. */
    char name[0x34];     /* Selected resource name without its extension. */
    s32 type;            /* Resource category returned by the formatter. */
    s32 selectionStatus; /* Browser selection status. */
    s32 pathVariant;     /* Selected path variant returned by the browser. */
    u8 opaque108[8];     /* In caller-sized buffers; not written by this provider. */
} EffResourceBankSlot;

typedef char EffResourceBankSlot_size_must_be_0x110[
    sizeof(EffResourceBankSlot) == 0x110 ? 1 : -1];

typedef char EffResourceBankSlot_fullName_must_be_0xC8[
    sizeof(((EffResourceBankSlot *)0)->fullName) == 0xC8 ? 1 : -1];

/* Per-instance header shared by every resource-kind callback. payload points
 * to kind-specific bytes immediately after this 0x40-byte allocation prefix;
 * resource is the separately owned kind-specific object. */
typedef struct EffActiveResource {
    f32 position[4];
    f32 orientation[4];
    f32 scale;
    u32 color;
    u32 frame;
    union {
        u32 index;
        s32 signedIndex;
        u16 shortIndex;
    } kind;
    void *resource;
    u8 pad_34[4];
    u8 *payload;
    u8 pad_3C[4];
} EffActiveResource;

typedef char EffActiveResource_size_must_be_0x40[
    (sizeof(EffActiveResource) == 0x40) ? 1 : -1];

/* Native 0x70-byte output of the effect resource-name chooser.
 * The producer fills both 50-byte strings: nameWithExtension is the selected
 * name followed by its extension, and name is retained without the extension.
 * Callers prepend the directory path when forming the full file path. */
typedef struct EffQueueRecord {
    char nameWithExtension[0x32];
    char name[0x32];
    union {
        s32 signedState; /* +0x64 */
        u32 state;
    } completion;
    u8 pad68[8];
} EffQueueRecord;

extern void effQueueResource(const char *extension, const char *resourceName);

extern void effUpdateResourceQueue(const char *directoryPath, const char *extension, EffQueueRecord *);

typedef u32 BtlResourceWritePermissionResult;

enum {
    BTL_RESOURCE_WRITE_PERMISSION_NOT_GRANTED = 0,
    BTL_RESOURCE_WRITE_PERMISSION_GRANTED = 1,
};

/* Reports immediate permission; an active prompt or cancellation is not
 * granted. */
BtlResourceWritePermissionResult btlPollResourceNameOverwrite(
    struct BtlResourceNameRecord *record, const char *directoryPath);

static inline void effSubmitSurfacePacket(sdfGrOt_t *surface, sdfDrawTag_t *list) {
    surface->regFunc(surface, list);
}

extern char effStrToolEffectEp[];
extern char D_003B39E0[];
extern char D_003B3B88[];
extern char D_003B3BA0[];
extern char effStrToolEffect[];
extern char effStrToolEffectMat[];
extern char effStrToolEffectHlp[];
extern u32 effColorEditRgbMenuOpen;
extern u32 effColorEditSelectedKey;
extern u32 effColorEditPage;
extern u32 D_003BD158;
extern u32 D_003BD15C;
extern u32 D_003BD160;
extern u32 effColorEditSelectedAlphaKey;
extern struct BtlResourceEntryList *effResourceBankEntries;
extern struct BtlResourceDescriptor *effResourceBankDescriptor;
extern s32 effEditorCurrentMenu;
extern void *effQueuedFileObject;
extern u8 effPartsListExitRequested;
extern FileJobPayload *effTemporaryFileJob;
extern FileQueue *effAuxiliaryFileQueue;
extern FileQueue *effFileQueue;
extern u8 effCreatePolyTrackTaskData[];
extern u8 effCreatePolyTwinkleTaskData[];
extern FileJobPayload *effQueuedFileHandle;
extern s32 dds3BillboardGeneralNumber(void);

typedef struct EffectObjectFlag {
    u32 state;
    u8 pad04[8];
    u32 flags;
} EffectObjectFlag;

extern u8 btlIsRuntimeAllocated(void);
extern u32 effPartsListScroll;
extern u32 effData12[];

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
    struct EffSpanTable *(*createResource)(struct EffSpanConfig *, mdlObj_t *); /* 0x04 */
    void (*destroyResource)(struct EffSpanTable *); /* 0x08 */
    void (*update)(struct EffModelResource *); /* 0x0C */
    void (*draw)(struct EffModelResource *); /* 0x10 */
    u32 payloadSize; /* 0x14 */
} EffModelResourceOps;

typedef char EffModelResourceOps_size_must_be_0x18[
    (sizeof(EffModelResourceOps) == 0x18) ? 1 : -1];

extern EffResourceOps effRuntimeResourceOperations[];

/* Shared work object for the effEditScalarTrack helpers (layout inferred). */
typedef struct BaObj {
    u8 pad_0x00[0x0C]; // 0x00
    u8 *x0C;           // 0x0C
} BaObj; // 0x10

extern s32 effEditScalarTrack(EffScalarTrack *, u32, void *, void *);
extern u8 D_0038F9D0[];
extern u8 D_0038FAD0[];
extern u8 D_0038F8E8[];
extern u8 D_0038FA20[];
extern u8 effData17[];
extern struct BtlResourceNameRecord *effQueuedResourceNameRecord;
extern void effReleaseQueuedResourceName(void);
extern const char D_003BD088[];
extern const char D_003BD090[];
extern char effWork8[];
extern u8 effBattleOffset[];
extern u8 *D_003BD074;

struct EffCameraCreateRow;

extern u32 effSelectCameraFileJobFromRows(const char *, const struct EffCameraCreateRow *, u32);
extern char effStrPolyTrack[];
extern char D_003BCA40[];
extern char D_003BD050[];
extern char effStrEnvironment[];
extern char effStrPolyFlash[];
extern char effStrPolyRing[];
extern char effStrPolyThunder[];
extern char effStrObjParticle[];
extern char effStrPolyScaly[];
extern char effStrPolyCrack[];
extern char D_003BD000[];
extern u8 effCreatePolyTrackControlTaskData[];
extern u8 effCreateBlurTaskData[];
extern u8 effCreateFilterTaskData[];
extern u8 effCreateEnvironmentTaskData[];
extern u8 effCreatePolyFlashTaskData[];
extern u8 effCreatePolyRingTaskData[];
extern u8 effCreatePolyThunderTaskData[];
extern u8 effCreatePolyWindTaskData[];
extern u8 effCreatePolyScalyTaskData[];
extern u8 effCreatePolyCrackTaskData[];
extern u8 effCreateTwoDimensionalTaskData[];
extern u8 D_003DF840[];
extern u8 effWork9[];
extern s32 effEditColorTrack(s32, s32, s32);
extern u8 effWork6[] __attribute__((aligned(4)));

/* Value holder with a float field and a u16 data pointer (layout inferred). */
typedef struct ValPtr44 {
    u8 pad_0x00[0x08]; // 0x00
    float f08;         // 0x08
    u8 pad_0x0C[0x38]; // 0x0C
    u16 *p44;          // 0x44
} ValPtr44; // 0x48

extern s32 effPollFileRecord(char *, s32);

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

extern f32 effWorldUpVector[4];
extern f32 aViewEye[4];

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
    mdlObj_t *assetId;
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

void effResetSlots(void);

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

EffActiveResource *effAllocateResourcePayload(u16 kind, void *source);
EffActiveResource *effCreateResourceInstance(u16 kind, void *source, void *secondary, u32 param);
EffActiveResource *effCreateActiveResourceFromFile(FileJobPayload *request);
void effDestroyResourceInstance(EffActiveResource *work);
EffActiveResource *effDuplicateActiveResource(EffActiveResource *source);
void effClearCallbackFrame(EffActiveResource *work);
void effDispatchEnabledCallback(EffActiveResource *work);
void effResetFileResourceStores(void);
s32 btlUpdateEffectWork(void);
void effResetFileResources(void);
extern void effResetFileResourceManager(void);
void effSetBattleOffset(void *src);
extern void camFollowOffsetVec(u8 *, void *);
void effComputeBattleCameraPositionVU(u8 *effect);
extern void PoseRotBillboard(void *, void *);
void effMultiplyQuatWithFlag(u8 *effect);
extern u8 effSelectedJobHeader[];
extern void effComputeBattleCameraPositionVU(u8 *);
void effApplyBattleCameraToObject();
u32 effCreateBattleCameraJob(u8 *request);
FileJobPayload *effLoadFileJobPayload(EffFileJobRequest *request, u32 existingJob);
void effInvokeFileJobWithBattleCamera(u32 job);
void effFileJobQueueRelease(u32 job);
extern char D_003BD080[];
extern const char D_003BD078[];

/* Payload option (+0x0C) selects the asset identifier at +0x08. */
typedef struct EffAssetIdentifier {
    u8 pad_00[4];
    u16 type;       // 0x04
    u8 pad_06[2];
    u16 id;         // 0x08
} EffAssetIdentifier;

u32 effPollPrimaryFile(void);
void effQueueNamedResourceRequest(void);
u32 effPollNamedFile(void);
void effQueueAttachedResourceRequest(void);
u32 effPollAttachedFile(void);

/* Sentinel-terminated map from asset identifiers to their runtime objects. */
typedef struct EffectAssetLink {
    u8 *asset;
    void *object;
    u8 pad_08[0x14];
} EffectAssetLink;

u8 *effFindAssetData(FileJob *entry);
EffQueuedFileObject *effFindAssetObject(FileJob *entry);
u32 effGetSelectedFileQueueIndex(void);
extern EffectBlock128 effEditedQueueHeader;
extern u32 btlDebugMenuSetupNoOp(void *context);
extern void btlDebugMenuBeforeRecordNoOp(void);
extern void btlDebugMenuAfterRecordNoOp(void);

typedef struct EffCameraCreateRow {
    EffCameraCreateRequest request;
    u32 sourceTail;
} EffCameraCreateRow;

typedef char EffCameraCreateRowSizeCheck[(sizeof(EffCameraCreateRow) == 0x1C) ? 1 : -1];

extern void fileQueueInitTransform(void *);
extern FileJob *fileAppendJob(FileQueue *, u32);
extern s32 fileQueueCountLinkedJobs(FileQueue *);
extern FileJob *effCurrentFileQueueEntry;
extern u32 D_003BD064;
extern EffQueuedFileObject effPartsListMenu;
u32 effAppendPositionedCameraFileJob(const EffCameraCreateRequest *request);
u32 effLoadSelectedFileJob(void);
extern u32 effReinitializeFileQueueData[];
u32 effReinitializeFileQueue(void);
extern u32 effResetFileQueueStateData[];
u32 effResetFileQueueState(void);
extern u32 effFinalizeQueuedFileData[];
u32 effFinalizeQueuedFile(void);
extern FileJob *fileDuplicateAndAppendJob(FileQueue *, FileJobPayload *);
extern FileJob *fileQueueFindById(FileQueue *, u32);
u32 effCloneSelectedFileJob(void);
extern u32 effSelectLinkedFileState();
u32 effFlagFileQueueActive(void);

typedef struct EffectBlock90 {
    u32 word[36];
} EffectBlock90;

typedef struct EffectAlignedBlock128 {
    u64 word[16];
} EffectAlignedBlock128;

extern EffQueuedFileObject effBattleCameraSnapshotSelection;
extern EffEditorRow effData10[];
extern EffEditorRow effData11[];
extern u8 effPartsPreviewReload;
u32 effLoadBattleCameraSnapshot(u32 value);
u32 effStoreBattleCameraSnapshot(u32 value);

/* Present named camera/file requests, maintaining a preview job and returning selection status. */
u32 effSelectCameraFileJobFromRows(const char *nameKey, const EffCameraCreateRow *requests, u32 sourceCount);
s32 effCreatePolyTrackControlTask(void);
s32 effCreatePolyTrackTask(void);
s32 effCreateBlurTask(void);
s32 effCreateFilterTask(void);
s32 effCreateEnvironmentTask(void);
s32 effCreatePolyFlashTask(void);
s32 effCreatePolyRingTask(void);
s32 effCreatePolyThunderTask(void);
s32 effCreatePolyTwinkleTask(void);
s32 effCreatePolyWindTask(void);
s32 effCreatePolyScalyTask(void);
s32 effCreatePolyCrackTask(void);
s32 effCreateTwoDimensionalTask(void);
extern void effPollResourceBankSlot(char *, u32, EffResourceBankSlot *);
u32 effPollPartResource(void);
u32 effPollNamedFileJob(void);
extern char D_003B3A90[], D_003B3AA8[];
extern f32 D_003DFA20[4];
s32 effResetStaticState(void);

typedef struct EffectStateSnapshot {
    s128 vectors[4];
    f32 position[4]; /* 0x40: copied to the debug camera position and follow offset. */
    s128 unused50;
    u8 pad60[8];
    u32 flags; /* 0x68: transform flags tested by the camera diagnostic. */
    u8 pad6C[0x14];
} EffectStateSnapshot;

typedef char EffectStateSnapshotSizeCheck[(sizeof(EffectStateSnapshot) == 0x80) ? 1 : -1];

extern s32 effUpdateCameraDiagnostics(EffectStateSnapshot *, s8);
extern u8 effUpdateCameraDiagnosticsData[];
extern s8 effData22[];
extern u32 btlDebugMenuSetupNoOp(void *camera);
extern void camFollowOffsetVec(u8 *camera, void *destination);
extern void effComputeBattleCameraPositionVU(u8 *snapshot);
s32 effUpdateCameraDiagnostics(EffectStateSnapshot *snapshot, s8 mode);

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

extern EffectCameraDebugState effUpdateCameraSnapshotGuidesData;

typedef struct EffectCameraDebugScratch {
    s128 basePosition;
    s128 segmentStart;
    s128 segmentEnd;
    s128 rotationMatrix[4];
} EffectCameraDebugScratch;

u32 effUpdateCameraSnapshotGuides(EffectCameraSnapshotView *state, s8 useBattlePosition);
u32 effLoadFileSlotAndPoll(void);
s32 effRunWithStateBackup(void);
extern u32 effUpdateCameraSnapshotGuides(EffectCameraSnapshotView *, s8);
u32 effProcessQueuedFileRecordAndPersistChanges(void);
void effBattleResourceDescriptorRelease(void);
void effPollResourceBankSlot(char *path, u32 flags, EffResourceBankSlot *selection);
extern struct BtlResourceNameRecord *effFileQueueNameRecord;
extern const char D_003BCF30[];
void effInitializeResourceQueue(void);
u32 effPollResourceQueue(void);
void effReleaseQueuedResourceName(void);
void effReleaseResourceBankDescriptors(void);

typedef struct EffectPollRecord {
    char fullName[0xC8]; /* Formatted path or selected built-in name. */
    s32 type;   // 0xC8
    s32 selectionStatus;  // 0xCC
    s32 pathVariant; // 0xD0
} EffectPollRecord;

void effPollResourceBank(s32 flags, EffectPollRecord *record);
extern EffQueuedFileObject effMappingState;
void effResetMappingFlags(void);
extern EffEditorRow D_0038F7D8[];
extern EffEditorRow effData14[];
extern EffEditorRow D_0038F658[];

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
    u8 pad_F8[0x10];
    s32 value108;      // 0x108
} EffMappingObject;

s32 effMapObjectWithTemporaryMappingTable(s32 request);
s32 effEditGkColorTrack(s32 request);
s32 effEditBfColorTrack(s32 request);
s32 effEditHlpColorTrack(void);
s32 effEditTBoardColorTrack(s32 request);
s32 effEditRainColorTrack(s32 request);
s32 effEditFlareColorTrack(s32 request);
s32 effEditSColorTrack(s32 request);
s32 effEditRColorTrack(s32 request);
s32 effEditRTrackColorTrack(s32 request);
s32 effEditDoColorTrack(s32 request);
s32 effResolveMappingWithTemporaryTable(s32 request);
s32 effEditNColorTrack(s32 request);
s32 effEditNPolygonColorTrack(s32 request);
s32 effEditUCylinderColorTrack(s32 request);
s32 effEditUCylinderPolygonColorTrack(s32 request);
s32 effEditUSmokeSpiralColorTrack(s32 request);
s32 effEditUSphereColorTrack(s32 request);
s32 effEditCrackPolygonColorTrack(s32 request);
s32 effEditFlashPolygonColorTrack(s32 request);
s32 effEditTwoDimensionalColorTrack(s32 request);
void btlResetEffectWork(void);
s32 effEditScalarTrack(EffScalarTrack *track, u32 frames, void *panelWork, void *subPanelWork);
s32 effEditPScaleTrack(BaObj *p);
s32 effEditPRotationTrack(BaObj *p);
s32 effEditGkScaleTrack(BaObj *p);
s32 effEditKRotationTrack(BaObj *p);
s32 effEditBScaleTrack(BaObj *p);
s32 effEditBRotationTrack(BaObj *p);
s32 effEditBfAreaTrack(BaObj *p);
s32 effEditTBoardScaleTrack(BaObj *p);

/* Pass the requested T-BOARD ROTATE track and its frame bound to the editor. */
s32 effEditTBoardRotationTrack(BaObj *p);
s32 effEditWScaleTrack(BaObj *p);
s32 effEditWRotationTrack(BaObj *p);
s32 effEditUCylinderPolygonUvScaleTrack(BaObj *p);
s32 effEditUSmokeSpiralScaleTrack(BaObj *p);
s32 effEditUSmokeSpiralRotationTrack(BaObj *p);
s32 effEditUSmokeSpiralPolygonUvScaleTrack(BaObj *p);
s32 effEditUSphereScaleTrack(BaObj *p);
s32 effEditTwoDimensionalScaleTrack(BaObj *p);
s32 effEditTwoDimensionalRotationTrack(BaObj *p);
extern char effColorEditFrameCountFormat[];

/* Edit the alpha fade interval and its four-breakpoint graph. */
s32 func_002BB188(SdfAlphaTrack *range, s32 frames);
s32 effEditSPolygonAlphaTrack(s32 work);
s32 effEditDoPolygonFadeTrack(s32 work);
s32 effEditNPolygonFadeTrack(s32 work);
s32 effEditUCylinderPolygonFadeTrack(s32 work);
s32 effEditUSpherePolygonFadeTrack(s32 work);

/* Poll record from the resource-bank queue; the path variant selects its entry. */
extern void effPollResourceBank(s32, EffectPollRecord *);
s32 effPollFileQueueRecord(s32 request);
s32 effPollResourceQueueWithTlp(void);
s32 effPollAlternateResourceQueueWithTlp(void);
s32 effPollResourceQueueWithoutTlp(void);
extern u16 Start_sbtl(s32);

typedef struct EffFileResourceRecord {
    char *name;
    u8 pad4[4];
    u16 mode;
    u8 padA[2];
    u8 *buffer;
    u32 size;
    struct sdfMemHandle_t *allocation;
} EffFileResourceRecord;

s32 effPollFileRecord(char *path, s32 flags);
s32 effPollTextureResourceFile(void);
s32 effPollAlternateTextureResourceFile(void);
s32 effPollEpResourceFile(void);
s32 effPollEplResourceFile(void);
s32 effPollAlternateEpResourceFile(void);
s32 effPollBfMaterialTmxFile(void);
s32 effPollNMaterialTmxFile(void);
s32 effPollSTMaterialTmxFile(void);
u32 effFileJobSecondaryDataSet(void);
s32 effPollHlpPbResourceFile(void);

typedef struct EffectFileHeader {
    u8 unk_00[8];
    u16 mode;
    u8 unk_0A[2];
    u32 start;
    u32 length;
} EffectFileHeader;

extern EffectFileHeader effLoadMaterialFileData;
extern EffectFileHeader fileLoadEffectSlotAData;
extern EffectFileHeader fileLoadEffectSlotBData;
u32 fileLoadEffectSlotA(void);
extern u8 fileLoadEffectSlotHelpData[] __attribute__((aligned(4)));
u32 fileLoadEffectSlotHelp(void);
u32 fileLoadEffectSlotB(void);
extern EffectFileHeader effLoadFileSlotF2Data;
u32 effLoadFileSlotF2(void);
u32 effLoadMaterialFile(void);
void effResetFileResourceManager(void);
u16 Start_sbtl(s32 flags);

#include "newdata.h"

/* Return the descriptor's payload base as a byte address, not the descriptor. */

#define SCR_FLAG_ID_MASK 0xFFFF

#define SCR_FLAG_SLOT_INDEX_MASK 7

#define SCR_FLAG_WORD_INDEX_SHIFT 3

#define SCR_FLAG_SLOT_BIT_SHIFT 2

#define SCR_FLAG_PRIMARY_BIT 1U

#define SCR_SKILL_STATE_TWO_BIT 2U

#define SCR_FLAG_SECONDARY_BIT 4U

#define SCR_SECONDARY_FLAG_COUNT 0x260

#define SCR_GLOBAL_FLAG_FIRST_ID 0x1AB

#define SCR_GLOBAL_FLAG_END_ID 0x200

#define SCR_GLOBAL_FLAG_WRAP_BIAS 0xFE55

#define SCR_GLOBAL_FLAG_WORD_SHIFT 5

#define SCR_GLOBAL_FLAG_BIT_MASK 31

#define PTY_SKILL_SLOT_COUNT 24

#define PRF_SKILL_LIST_FLAGGED 4

#define PTY_PRESET_SKILL_SLOT_COUNT 8

#define PTY_PRESET_POOL_SKILL_COUNT 40

#define PTY_PRESET_POOL_EXTRA_GATE 0xB90

#define PTY_PROFILE_UNIT_ID_COUNT 0x10

#define PTY_PROFILE_UNIT_OCCUPIED_BIT 1

#define PRF_REQUIRED_PROFILE_COUNT 8

#define PRF_FALLBACK_GROUP_COUNT 4

#define PRF_FALLBACK_FLAG_COUNT 2

#define PRF_REQUIREMENT_RULES_BIT 4

#define PRF_REQUIREMENT_EXCLUSION_BIT 1

#define SCR_PAIRED_OUTPUT_BYTES 8

#define SDF_FLAG_LIST_VALUE_SHIFT 3

#define SDF_FLAG_LIST_RESET_MARK 0xFFFFFFFF

#define SDF_FLAG_LIST_ENTRY_VERTEX_BYTES 32

#define SDF_FLAG_LIST_VERTICES_PER_ENTRY 2

#define SDF_FLAG_LIST_PACKET_BYTES 0x20

#define SDF_FLAG_SLOT_TABLE_SHIFT 7

#define SDF_FLAG_SLOT_COUNT 16

extern u32 D_003BD2C8;

typedef struct Entry24W {
    u32 v0;            // 0x00
    u8 pad_0x04[0x14]; // 0x04
} Entry24W; 

typedef struct PrfFallbackGroup {
    u8 id;
    u8 pad01[3];
    u32 flags[2];
} PrfFallbackGroup;

extern sdfGrOt_t *sdfDrawFlagListVerticesData[];
void sdfSampleFlagListStartColor(SdfFlagListWork *work);

void effLeaf384(void) {
    effResetSlots();
}

INCLUDE_ASM(const s32, "code_r1_00242308", effCallPadVibeFadeIN);

void effLeaf385(void) {
}

void effLeaf386(void) {
}

void effLeaf387(void) {
}

EffActiveResource *effAllocateResourcePayload(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = effRuntimeResourceOperations[kind].payloadSize;
    EffActiveResource *effect = sdfChipAllocDirect(size + headerSize);
    effect->payload = (u8 *)effect + headerSize;
    effect->color = 0x80808080;
    effect->scale = 1.0f;
    effect->kind.index = kind;
    effect->frame = 0;
    VU0_STORE_VF($vf0, effect);
    VU0_STORE_VF($vf0, (u8 *)effect + 0x10);
    memcpy(effect->payload, source, size);
    return effect;
}

EffActiveResource *effCreateResourceInstance(u16 kind, void *source, void *secondary, u32 param) {
    EffActiveResource *effect = effAllocateResourcePayload(kind, source);

    if (btlIsRuntimeAllocated() != 0) {
        if (effRuntimeResourceOperations[kind].createResource != NULL) {
            effect->resource = (void *)effRuntimeResourceOperations[kind].createResource(source, secondary, param);
        }
        if (effRuntimeResourceOperations[kind].initialize != NULL) {
            effRuntimeResourceOperations[kind].initialize(effect);
        }
    }
    return effect;
}

EffActiveResource *effCreateActiveResourceFromFile(FileJobPayload *request) {
    void *first = fileResolvePrimaryBuffer(request);
    void *second = fileResolveSecondaryBuffer(request);
    return effCreateResourceInstance(request->option, first, second, request->secondary.size);
}

void effDestroyResourceInstance(EffActiveResource *work) {
    if (btlIsRuntimeAllocated() != 0) {
        void (*callback)(void *) = effRuntimeResourceOperations[work->kind.signedIndex].destroyResource;
        if (callback != NULL) {
            callback(work->resource);
        }
    }
    sdfChipFreeDirect(work);
}

EffActiveResource *effDuplicateActiveResource(EffActiveResource *source) {
    EffActiveResource *effect;
    u32 kind = source->kind.index;

    if (effRuntimeResourceOperations[kind].cloneResource == NULL) {
        effect = effCreateResourceInstance(source->kind.shortIndex, source->payload, 0, 0);
    } else {
        void *resource;
        u32 activeKind;
        effect = effAllocateResourcePayload(source->kind.shortIndex, source->payload);
        resource = (void *)effRuntimeResourceOperations[source->kind.signedIndex].cloneResource(source);
        activeKind = source->kind.index;
        effect->resource = resource;
        if (effRuntimeResourceOperations[activeKind].initialize != NULL) {
            effRuntimeResourceOperations[activeKind].initialize(effect);
        }
    }
    return effect;
}

void effClearCallbackFrame(EffActiveResource *work) {
    if (btlIsRuntimeAllocated() != 0) {
        void (*callback)(void *) = effRuntimeResourceOperations[work->kind.signedIndex].initialize;
        if (callback != NULL) {
            callback(work);
        }
        work->frame = 0;
    }
}

INCLUDE_ASM(const s32, "code_r1_00242308", effCallProcSBtl);

void effDispatchEnabledCallback(EffActiveResource *work) {
    if (btlIsRuntimeAllocated() != 0) {
        void (*callback)(void *) = effRuntimeResourceOperations[work->kind.signedIndex].draw;
        if (callback != NULL) {
            callback(work);
        }
    }
}

void effCallProcSBtl(EffActiveResource *work);

void effFunc18(EffActiveResource *work) {
    effCallProcSBtl(work);
    effDispatchEnabledCallback(work);
}

INCLUDE_ASM(const s32, "code_r1_00242308", effLeaf388);

INCLUDE_ASM(const s32, "code_r1_00242308", effLeaf389);

INCLUDE_ASM(const s32, "code_r1_00242308", effLeaf390);

INCLUDE_ASM(const s32, "code_r1_00242308", effLeaf391);

/* Reset each entry's timer to free, then clear both colour words per entry. */
void sdfResetFlagListEntries(SdfFlagListWork *work) {
    u32 entryCount;
    SdfFlagListMark *markCursor;
    u32 entryIndex;

    entryIndex = 0;
    entryCount = work->params.count;
    markCursor = work->marks;
    if (entryCount != 0) {
        do {
            entryIndex = entryIndex + 1;
            markCursor->timer = SDF_FLAG_LIST_RESET_MARK;
            markCursor++;
        } while (entryIndex < entryCount);
    }
    memset(work->colors, 0, entryCount << SDF_FLAG_LIST_VALUE_SHIFT);
}

SdfFlagListWork *sdfCreateFlagListWork(const SdfFlagListParams *source) {
    u32 count;
    u32 arrayBytes;
    sdfMemHandle_t *allocation;
    u8 *buffer;
    SdfFlagListWork *work;

    count = source->count;
    arrayBytes = count * (sizeof(f32[2][4]) + sizeof(u32[2]) + sizeof(SdfFlagListMark));
    allocation = sdfMemAllocDirect(arrayBytes + sizeof(SdfFlagListWork));
    buffer = (u8 *)sdfMemLock(allocation);
    work = (SdfFlagListWork *)(buffer + arrayBytes);
    work->vertices = (f32 (*)[4])buffer;
    buffer += count * sizeof(f32[2][4]);
    work->colors = (u32 *)buffer;
    buffer += count * sizeof(u32[2]);
    work->marks = (SdfFlagListMark *)buffer;
    work->unk04 = 0x80808080;
    work->allocation = allocation;
    work->frame = 0;
    memcpy(&work->params, source, sizeof(work->params));
    sdfResetFlagListEntries(work);
    return work;
}

SdfFlagListWork *sdfInitializeFlagListFromResource(void *file) {
    return effCreateSelectionFlagListFromWork(fileResolvePrimaryBuffer(file));
}

void sdfReleaseFlagListResource(SdfFlagListWork *work) {
    sdfMemFreeDirect(work->allocation);
}

INCLUDE_ASM(const s32, "code_r1_00242308", sdfUpdateFlagListEntries);

/* Copy vertex pairs, optionally add the view target, and submit the packet list.
 * count is reused first as an entry count and then as a vertex count. */
void sdfDrawFlagListVertices(SdfFlagListWork *work) {
    f32 (*sourceVertices)[4];
    f32 (*copiedVertices)[4];
    u32 count;
    u32 vertexIndex;
    struct sdfMemHandle_t *vertexAllocation;
    sdfDrawTag_t *packetList;
    sdfGrOt_t *surface;

    if (work->params.maxFrames != 0 && work->frame >= work->params.maxFrames) {
        return;
    }
    count = work->params.count;
    sourceVertices = work->vertices;
    vertexAllocation = sdfMemAllocDirect(count * SDF_FLAG_LIST_ENTRY_VERTEX_BYTES);
    count *= SDF_FLAG_LIST_VERTICES_PER_ENTRY;
    copiedVertices = (f32 (*)[4])sdfMemGetPtr(vertexAllocation);
    for (vertexIndex = 0; vertexIndex < count; vertexIndex++) {
        SDF_VU0_LOADVECTOR(&sourceVertices[vertexIndex]);
        if (D_003BD2C8 != 0) {
            SDF_VU0_LOADVECTOR_B(aViewEye);
            SDF_VU0_ADDVECTOR();
        }
        SDF_VU0_STOREVECTOR_NOMEM(&copiedVertices[vertexIndex]);
    }
    packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(SDF_FLAG_LIST_PACKET_BYTES);
    sdfDrawTagInit(packetList);
    sdfDrawTagLink(packetList, (u32)effDebugDrawWireModel(copiedVertices, work->colors, work->params.count * SDF_FLAG_LIST_VERTICES_PER_ENTRY, 0x40));
    surface = sdfDrawFlagListVerticesData[work->params.alpha.surfaceIndex];
    surface->regFunc(surface, packetList);
    sdfMemFreeDirect(vertexAllocation);
}

INCLUDE_ASM(const s32, "code_r1_00242308", effLeaf392);

INCLUDE_ASM(const s32, "code_r1_00242308", effLeaf393);

/* Sample color/alpha at frame zero with duration zero; discard the return value. */
void sdfSampleFlagListStartColor(SdfFlagListWork *work) {
    effColorAnimation(&work->params.color, &work->params.alpha, 0, 0);
}

INCLUDE_ASM(const s32, "code_r1_00242308", effLeaf394);

INCLUDE_ASM(const s32, "code_r1_00242308", effLeaf395);

INCLUDE_ASM(const s32, "code_r1_00242308", effLeaf396);

INCLUDE_ASM(const s32, "code_r1_00242308", effLeaf397);

extern s32 sdfUpdateFlagListEntriesData;

void effLeaf398(void) {
    sdfUpdateFlagListEntriesData = 1;
}

void effLeaf399(void) {
    sdfUpdateFlagListEntriesData = 0;
}

INCLUDE_ASM(const s32, "code_r1_00242308", effBtlDebugSetStatusflgSub);

s32 effUpdatePartsListEditor(s32 mode);

s32 btlUpdateEffectWork(void) {
    if (D_003BD074 != NULL) {
        if ((effUpdatePartsListEditor(0) & 1) == 0) {
            effResetFileResourceStores();
            return 0;
        }
    }
    return 1;
}

void effLeaf400(void) {
}

void effLeaf341(void);

void effFunc19(void) {
    effResetFileResources();
    effResetFileResourceManager();
    effLeaf341();
}

void effSetBattleOffset(void *src) {
    SDF_VU0_COPYVECTOR(effBattleOffset, src);
}

void effComputeBattleCameraPositionVU(u8 *effect) {
    u128 direction;
    u32 flags = ((FileJob *)effect)->xformFlags;

    if ((flags & 0x18) != 0) {
        camFollowOffsetVec(effect, &direction);
        VU0_LOAD_VF($vf10, &direction);
        flags = ((FileJob *)effect)->xformFlags;
    } else {
        VU0_LOAD_VF($vf10, ((FileJob *)effect)->offset);
    }
    if ((flags & 0x80) != 0) {
        VU0_SCALAR_OP_CLOBBER(effFileQueue->scaleMultiplier, "vmulx.xyzw vf10, vf10, vf2x");
    }
    VU0_LOAD_VF($vf11, effBattleOffset);
    VU0_ADD($vf10, $vf10, $vf11);
    VU0_LOAD_VF($vf11, effFileQueue);
    VU0_ADD($vf10, $vf10, $vf11);
    if ((flags & 4) != 0) {
        VU0_SCALAR_OP_CLOBBER(-5.0f, "vaddx.y vf10, vf0, vf2x");
    }
}

void effMultiplyQuatWithFlag(u8 *effect) {
    if ((((FileJob *)effect)->xformFlags & 0x60) != 0) {
        u128 quaternion;
        PoseRotBillboard(effect, &quaternion);
        VU0_LOAD_VF($vf10, effFileQueue->quat);
        VU0_LOAD_VF($vf11, &quaternion);
        Mult();
    } else {
        VU0_LOAD_VF($vf10, effFileQueue->quat);
        VU0_LOAD_VF($vf11, ((FileJob *)effect)->quat);
        Mult();
    }
}

INCLUDE_ASM(const s32, "code_r1_00242308", effApplyBattleCameraToObject);

u32 effCreateBattleCameraJob(u8 *request) {
    u8 *outputBuffer = ((EffFileJobRequest *)request)->outputBuffer;
    u32 job;

    if (outputBuffer != NULL) {
        if (((EffFileJobRequest *)request)->fileKind == 6) {
            memcpy(outputBuffer, effWork6, 0x3C);
            outputBuffer = ((EffFileJobRequest *)request)->outputBuffer;
        }
        fileJobSetPrimaryData(effQueuedFileHandle, outputBuffer, ((EffFileJobRequest *)request)->size, ((EffFileJobRequest *)request)->transferMode);
    }
    job = (u32)Create_effParts(effQueuedFileHandle);
    effApplyBattleCameraToObject(job);
    return job;
}

FileJobPayload *effLoadFileJobPayload(EffFileJobRequest *request, u32 existingJob) {
    FileJobPayload *job;
    if (existingJob != 0) {
        void *source;
        job = fileJobCreateFromCommandState((const char *)existingJob);
        source = fileResolvePrimaryBuffer(job);
        memcpy(request->outputBuffer, source, request->size);
    } else {
        job = fileCreateJob(request->fileKind);
        if (request->outputBuffer != NULL) {
            fileJobSetPrimaryData(job, request->outputBuffer, request->size, request->transferMode);
        }
        if (request->secondaryCommandPath != 0) {
            fileJobCopyCommandIntoSecondaryData(job, request->secondaryCommandPath, request->resourceMode);
        } else {
            u32 value = 0;
            fileJobSetSecondaryData(job, &value, 4, 4);
        }
    }
    effResetFileResourceManager();
    return job;
}

void effInvokeFileJobWithBattleCamera(u32 job) {
    if (effAuxiliaryFileQueue == 0) {
        effApplyBattleCameraToObject((void *)job);
        fileJobInvokeTypeCallback((FileJobPayload *)job);
        return;
    }
}

void effFileJobQueueRelease(u32 job) {
    if (effAuxiliaryFileQueue != 0) {
        effPartsDelete(effAuxiliaryFileQueue);
        effAuxiliaryFileQueue = 0;
    }
    if (effTemporaryFileJob != 0) {
        fileJobDestroy(effTemporaryFileJob);
        effTemporaryFileJob = 0;
    }
    fileJobDestroy((FileJobPayload *)job);
}

u32 effPollPrimaryFile(void) {
    EffQueueRecord record;
    char path[0x70];
    u32 state;
    u32 result = 0x400001;

    effUpdateResourceQueue(effStrToolEffectEp, D_003BD078, &record);
    state = record.completion.state;
    if (state == BTL_RESOURCE_SELECTION_CANCELED) {
        result = 0x400000;
    } else if (state == BTL_RESOURCE_SELECTION_ACCEPTED) {
        if (effQueuedFileHandle != 0) {
            sprintf(path, D_003BD080, effStrToolEffectEp, record.nameWithExtension);
            fileWriteToPfs(effQueuedFileHandle, path);
            result = 0x400002;
        }
    }
    return result;
}

void effQueueNamedResourceRequest(void) {
    effReleaseQueuedResourceName();
    effQueueResource(D_003BD088, effWork8);
}

u32 effPollNamedFile(void) {
    EffQueueRecord record;
    char path[0x70];
    u32 state;
    u32 result = 0x400001;

    effUpdateResourceQueue(D_003B39E0, D_003BD088, &record);
    state = record.completion.state;
    if (state == BTL_RESOURCE_SELECTION_CANCELED) {
        result = 0x400000;
    } else if (state == BTL_RESOURCE_SELECTION_ACCEPTED) {
        if (effFileQueue != 0) {
            strcpy(effWork8, record.name);
            sprintf(path, D_003BD080, D_003B39E0, record.nameWithExtension);
            fileQueueSaveImage(effFileQueue, path);
            result = 0x400002;
        }
    }
    return result;
}

void effQueueAttachedResourceRequest(void) {
    effReleaseQueuedResourceName();
    effQueueResource(D_003BD090, effWork8);
}

u32 effPollAttachedFile(void) {
    EffQueueRecord record;
    char path[0x70];
    u32 state;
    u32 result = 0x400001;

    effUpdateResourceQueue(D_003B39E0, D_003BD090, &record);
    state = record.completion.state;
    if (state == BTL_RESOURCE_SELECTION_CANCELED) {
        result = 0x400000;
    } else if (state == BTL_RESOURCE_SELECTION_ACCEPTED) {
        if (effFileQueue != 0) {
            strcpy(effWork8, record.name);
            sprintf(path, D_003BD080, D_003B39E0, record.nameWithExtension);
            fileQueueSaveVersionedImage(effFileQueue, path);
            result = 0x400002;
        }
    }
    return result;
}

extern EffectAssetLink *effData9[24];

u8 * effFindAssetData(FileJob *entry) {
    FileJobPayload *requested = (FileJobPayload *)entry->id;
    u16 type = requested->type;
    u16 option = requested->option;
    u16 i;
    for (i = 0; i < 24; i++) {
        EffectAssetLink *links = effData9[i];
        if (links != NULL) {
            EffectAssetLink *current = links;
            if (current->asset != NULL) {
                do {
                    u8 *asset = current->asset;
                    if (((EffAssetIdentifier *)asset)->type == type && ((EffAssetIdentifier *)asset)->id == option) {
                        return asset;
                    }
                    current++;
                } while (current->asset != NULL);
            }
        }
    }
    return NULL;
}

EffQueuedFileObject * effFindAssetObject(FileJob *entry) {
    FileJobPayload *requested = (FileJobPayload *)entry->id;
    u16 type = requested->type;
    u16 option = requested->option;
    u16 i;
    for (i = 0; i < 24; i++) {
        EffectAssetLink *links = effData9[i];
        if (links != NULL) {
            EffectAssetLink *current = links;
            if (current->asset != NULL) {
                do {
                    u8 *asset = current->asset;
                    if (((EffAssetIdentifier *)asset)->type == type && ((EffAssetIdentifier *)asset)->id == option) {
                        return current->object;
                    }
                    current++;
                } while (current->asset != NULL);
            }
        }
    }
    return NULL;
}

/* Return the selected queue row plus the offset of its visible window. */
u32 effGetSelectedFileQueueIndex(void) {
    return effData12[0] + effPartsListScroll;
}

INCLUDE_RODATA(const s32, "code_r1_00242308", effStrToolEffect);

INCLUDE_RODATA(const s32, "code_r1_00242308", effStrPartsList20);

INCLUDE_RODATA(const s32, "code_r1_00242308", effStrPosition2);

INCLUDE_ASM(const s32, "code_r1_00242308", effUpdatePartsListEditor);

u32 effAppendPositionedCameraFileJob(const EffCameraCreateRequest *request) {
    FileJob *entry;
    u32 count;

    fileQueueInitTransform(effSelectedJobHeader);
    entry = fileAppendJob(effFileQueue, (u32)effLoadFileJobPayload(request->resource, request->existingJob));
    effCurrentFileQueueEntry = entry;
    strcpy(entry->name, request->resource->name);
    entry->offset[0] = request->position[0];
    entry->offset[1] = request->position[1];
    entry->offset[2] = request->position[2];
    entry->unk88[0] = 8;
    entry->unk88[1] = 0;
    entry->unk88[2] = 0;
    memcpy(effSelectedJobHeader, entry, 0x80);
    effQueuedFileHandle = (FileJobPayload *)entry->id;
    D_003BD064 = effCreateBattleCameraJob((u8 *)request->resource);
    effQueuedFileObject = request->object;
    count = fileQueueCountLinkedJobs(effFileQueue);
    if (count > 21) {
        effPartsListMenu.cursor = 20;
        effPartsListScroll = count - 21;
    } else {
        effPartsListScroll = 0;
        effPartsListMenu.cursor = count - 1;
    }
    request->object->linkedState = (u8 *)&effPartsListMenu;
    effResetFileResourceManager();
    return 0x800000;
}

/* Load the selected job's payload and camera snapshots; return the refresh status. */
u32 effLoadSelectedFileJob(void) {
    FileJob *entry = fileQueueGetAt(effFileQueue, effGetSelectedFileQueueIndex());
    EffFileJobRequest *request;
    u8 *fileData;
    u32 job;

    effCurrentFileQueueEntry = entry;

    /* These independent snapshots cover the queue header and camera transform. */
    memcpy(D_003DF840, entry, 0x90);
    memcpy(effSelectedJobHeader, entry, 0x80);

    effQueuedFileHandle = (FileJobPayload *)entry->id;
    request = (EffFileJobRequest *)effFindAssetData(entry);
    if (request->fileKind != 6) {
        fileData = (u8 *)fileResolvePrimaryBuffer(effQueuedFileHandle);
        memcpy(request->outputBuffer, fileData, request->size);
    } else {
        FileJobPayload *payload;
        u32 transferMode;
        u32 payloadSize;

        if (request->allocation != NULL) {
            sdfMemFreeDirect(request->allocation);
        }
        request->allocation = sdfMemAllocDirect(
            effQueuedFileHandle->primary.size);
        {
            void *output = sdfMemLock(request->allocation);
            payload = effQueuedFileHandle;
            /* Publish the retained pointer through its object representation. */
            memcpy(&request->outputBuffer, &output, sizeof(request->outputBuffer));
        }
        payloadSize = payload->primary.size;
        transferMode = payload->option;
        request->transferMode = transferMode;
        request->size = payloadSize;
        fileData = (u8 *)fileResolvePrimaryBuffer(payload);
        memcpy(request->outputBuffer, fileData,
               effQueuedFileHandle->primary.size);
        memcpy(effWork6, fileData, 0x3C);
    }

    job = effCreateBattleCameraJob((u8 *)request);
    D_003BD064 = job;
    effQueuedFileObject = effFindAssetObject(entry);
    ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = (u8 *)&effPartsListMenu;
    effResetFileResourceManager();
    if (effTemporaryFileJob != 0) {
        fileJobDestroy(effTemporaryFileJob);
        effTemporaryFileJob = 0;
    }
    return 0x800000;
}

u32 effReinitializeFileQueue(void) {
    if (effTemporaryFileJob != 0) {
        effPartsDelete(effAuxiliaryFileQueue);
        effAuxiliaryFileQueue = 0;
    }
    if (effTemporaryFileJob != 0) {
        fileJobDestroy(effTemporaryFileJob);
        effTemporaryFileJob = 0;
    }
    if (effFileQueue != 0) {
        effPartsDelete(effFileQueue);
    }
    effFileQueue = fileQueueCreate();
    effReinitializeFileQueueData[0] = 0;
    effWork8[0] = 0;
    effPartsListScroll = 0;
    effPartsListMenu.cursor = 0;
    effQueuedFileObject = &effPartsListMenu;
    return 0;
}

u32 effResetFileQueueState(void) {
    fileQueueRemoveAndDestroyJob(effFileQueue, fileQueueGetAt(effFileQueue, effGetSelectedFileQueueIndex()));
    effResetFileQueueStateData[0] = 0;
    effPartsListScroll = 0;
    effPartsListMenu.cursor = 0;
    effQueuedFileObject = &effPartsListMenu;
    return 0;
}

u32 effFinalizeQueuedFile(void) {
    s32 index = effGetSelectedFileQueueIndex();
    FileJob *record = fileQueueGetAt(effFileQueue, index);
    FileJob *copy = fileJobDuplicateAfter(effFileQueue, record);
    effFinalizeQueuedFileData[0] = 0;
    fileJobCopyHeader(copy, record);
    effQueuedFileObject = &effPartsListMenu;
    return 0;
}

/* Clone the selected job, preserve its sector leader, and refresh the selected asset. */
u32 effCloneSelectedFileJob(void) {
    FileJob *source;
    FileJob *entry;
    FileJob *leader;
    EffFileJobRequest *resource;
    void *fileData;
    s32 linkedCount;

    source = fileQueueGetAt(effFileQueue, effGetSelectedFileQueueIndex());
    entry = fileDuplicateAndAppendJob(effFileQueue,
                                      (FileJobPayload *)source->id);
    effCurrentFileQueueEntry = entry;
    fileJobCopyHeader(entry, source);

    if ((source->flags & 2) != 0) {
        leader = fileQueueFindById(effFileQueue, source->sector);
    } else {
        leader = source;
    }
    fileQueueLinkJobToSectorLeader(effFileQueue, entry, leader);

    memcpy(D_003DF840, entry, 0x90);
    memcpy(effSelectedJobHeader, entry, 0x80);

    effQueuedFileHandle = (FileJobPayload *)entry->id;
    resource = (EffFileJobRequest *)effFindAssetData(entry);
    strcpy(entry->name, resource->name);

    if (resource->fileKind != 6) {
        fileData = fileResolvePrimaryBuffer(effQueuedFileHandle);
        memcpy(resource->outputBuffer, fileData, resource->size);
    } else {
        if (resource->allocation != 0) {
            sdfMemFreeDirect(resource->allocation);
        }
        resource->allocation = sdfMemAllocDirect(
            effQueuedFileHandle->primary.size);
        {
            void *buffer = sdfMemLock(resource->allocation);
            FileJobPayload *payload = effQueuedFileHandle;
            s32 option;
            u32 primarySize;
            /* Publish the retained pointer through its object representation. */
            memcpy(&resource->outputBuffer, &buffer, sizeof(resource->outputBuffer));
            primarySize = payload->primary.size;
            option = payload->option;
            resource->transferMode = option;
            resource->size = primarySize;
            fileData = fileResolvePrimaryBuffer(payload);
        }
        memcpy(resource->outputBuffer, fileData,
               effQueuedFileHandle->primary.size);
        memcpy(effWork6, fileData, 0x3C);
    }

    D_003BD064 = effCreateBattleCameraJob((u8 *)resource);
    effQueuedFileObject = effFindAssetObject(entry);
    ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = (u8 *)&effPartsListMenu;

    linkedCount = fileQueueCountLinkedJobs(effFileQueue);
    if ((u32)linkedCount >= 22) {
        effPartsListMenu.cursor = 20;
        effPartsListScroll = linkedCount - 21;
    } else {
        effPartsListScroll = 0;
        effPartsListMenu.cursor = linkedCount - 1;
    }

    effResetFileResourceManager();
    if (effTemporaryFileJob != 0) {
        fileJobDestroy(effTemporaryFileJob);
        effTemporaryFileJob = 0;
    }
    return 0x800000;
}

u32 effFlagFileQueueActive(void) {
    effPartsListExitRequested = 1;
    effSelectLinkedFileState(0);
    return 0;
}

/* Select the queued file object's linked state when present; return zero in either case. */
u32 effSelectLinkedFileState(void) {
    if (((EffQueuedFileObject *)effEditorCurrentMenu)->linkedState != NULL) {
        effQueuedFileObject = ((EffQueuedFileObject *)effEditorCurrentMenu)->linkedState;
    }
    return 0;
}

u32 effLoadBattleCameraSnapshot(u32 value) {
    FileJob *record = fileQueueGetAt(effFileQueue, effGetSelectedFileQueueIndex());

    *(EffectBlock90 *)D_003DF840 = *(EffectBlock90 *)record;
    *(EffectAlignedBlock128 *)D_003DF840 = *(EffectAlignedBlock128 *)effSelectedJobHeader;
    if (((EffectBlock90 *)D_003DF840)->word[26] & 2) {
        effBattleCameraSnapshotSelection.rows = effData10;
        effBattleCameraSnapshotSelection.count = 3;
        if (effBattleCameraSnapshotSelection.cursor > effBattleCameraSnapshotSelection.count) {
            effBattleCameraSnapshotSelection.cursor = 0;
        }
    } else {
        effBattleCameraSnapshotSelection.rows = effData11;
        effBattleCameraSnapshotSelection.count = 9;
        if (effBattleCameraSnapshotSelection.cursor == 0) {
            effBattleCameraSnapshotSelection.cursor = 1;
        }
    }
    return value;
}

u32 effStoreBattleCameraSnapshot(u32 value) {
    FileJob *record = fileQueueGetAt(effFileQueue, effGetSelectedFileQueueIndex());

    *(EffectBlock90 *)record = *(EffectBlock90 *)D_003DF840;
    *(EffectAlignedBlock128 *)effSelectedJobHeader = *(EffectAlignedBlock128 *)D_003DF840;
    effPartsPreviewReload = 1;
    if (((EffectBlock90 *)D_003DF840)->word[26] & 2) {
        effBattleCameraSnapshotSelection.rows = effData10;
        effBattleCameraSnapshotSelection.count = 3;
        if (effBattleCameraSnapshotSelection.cursor > effBattleCameraSnapshotSelection.count) {
            effBattleCameraSnapshotSelection.cursor = 0;
        }
    } else {
        effBattleCameraSnapshotSelection.rows = effData11;
        effBattleCameraSnapshotSelection.count = 9;
        if (effBattleCameraSnapshotSelection.cursor == 0) {
            effBattleCameraSnapshotSelection.cursor = 1;
        }
    }
    return value;
}

INCLUDE_ASM(const s32, "code_r1_00242308", effSelectCameraFileJobFromRows);

extern char sysStrParticle[];
extern char sysPARTICLEData[];
s32 sysPARTICLE(void) {
    return effSelectCameraFileJobFromRows(sysStrParticle, sysPARTICLEData, 0xF);
}

s32 effCreatePolyTrackControlTask(void) {
    return effSelectCameraFileJobFromRows(effStrPolyTrack, effCreatePolyTrackControlTaskData, 6);
}

s32 effCreatePolyTrackTask(void) {
    return effSelectCameraFileJobFromRows("POLY TRACK S", effCreatePolyTrackTaskData, 6);
}

s32 effCreateBlurTask(void) {
    return effSelectCameraFileJobFromRows(D_003BCA40, effCreateBlurTaskData, 6);
}

s32 effCreateFilterTask(void) {
    return effSelectCameraFileJobFromRows(D_003BD050, effCreateFilterTaskData, 3);
}

s32 effCreateEnvironmentTask(void) {
    return effSelectCameraFileJobFromRows(effStrEnvironment, effCreateEnvironmentTaskData, 5);
}

extern char sysStrPolyTex[];
extern char sysPOLYTEXData[];
s32 sysPOLYTEX(void) {
    return effSelectCameraFileJobFromRows(sysStrPolyTex, sysPOLYTEXData, 0x3);
}

s32 effCreatePolyFlashTask(void) {
    return effSelectCameraFileJobFromRows(effStrPolyFlash, effCreatePolyFlashTaskData, 0xB);
}

s32 effCreatePolyRingTask(void) {
    return effSelectCameraFileJobFromRows(effStrPolyRing, effCreatePolyRingTaskData, 6);
}

s32 effCreatePolyThunderTask(void) {
    return effSelectCameraFileJobFromRows(effStrPolyThunder, effCreatePolyThunderTaskData, 4);
}

s32 effCreatePolyTwinkleTask(void) {
    return effSelectCameraFileJobFromRows("POLY TWINKLE", effCreatePolyTwinkleTaskData, 6);
}

extern char sysStrPolyWind[];
extern char sysPOLYWINDData[];
s32 sysPOLYWIND(void) {
    return effSelectCameraFileJobFromRows(sysStrPolyWind, sysPOLYWINDData, 0x8);
}

s32 effCreatePolyScalyTask(void) {
    return effSelectCameraFileJobFromRows(effStrPolyScaly, effCreatePolyScalyTaskData, 5);
}

s32 effCreatePolyCrackTask(void) {
    return effSelectCameraFileJobFromRows(effStrPolyCrack, effCreatePolyCrackTaskData, 2);
}

extern char sysStrBattleOnly[];
extern char sysBATTLEONLYData[];
s32 sysBATTLEONLY(void) {
    return effSelectCameraFileJobFromRows(sysStrBattleOnly, sysBATTLEONLYData, 0xD);
}

s32 effCreateTwoDimensionalTask(void) {
    return effSelectCameraFileJobFromRows(D_003BD000, effCreateTwoDimensionalTaskData, 2);
}

s32 effCreatePolyWindTask(void) {
    return effSelectCameraFileJobFromRows(effStrObjParticle, effCreatePolyWindTaskData, 7);
}

u32 effPollPartResource(void) {
    EffResourceBankSlot record;
    u32 state;
    u32 result = 0x400001;

    effPollResourceBankSlot(effStrToolEffectEp, BTL_RESOURCE_SCAN_EP, &record);
    state = record.selectionStatus;
    if (state == BTL_RESOURCE_SELECTION_CANCELED) {
        result = 0x400000;
    } else if (state == BTL_RESOURCE_SELECTION_ACCEPTED) {
        if (effFileQueue != 0) {
            FileJob *job = fileAppendJobFromCommandPath(effFileQueue, (const char *)&record);
            u8 *asset = effFindAssetData(job);
            strcpy(job->name, *(char **)asset);
        }
        result = 0x400002;
    }
    return result;
}

u32 effPollNamedFileJob(void) {
    EffResourceBankSlot record;
    u32 state;
    u32 result = 0x400001;

    effPollResourceBankSlot(D_003B39E0, BTL_RESOURCE_SCAN_EPL, &record);
    state = record.selectionStatus;
    if (state == BTL_RESOURCE_SELECTION_CANCELED) {
        result = 0x400000;
    } else if (state == BTL_RESOURCE_SELECTION_ACCEPTED) {
        if (effFileQueue != 0) {
            effPartsDelete(effFileQueue);
        }
        strcpy(effWork8, record.name);
        effFileQueue = fileQueueCreateFromCommandState((const char *)&record);
        effEditedQueueHeader = *(EffectBlock128 *)effFileQueue;
        effPartsListScroll = 0;
        effData12[0] = 0;
        result = 0x400002;
    }
    return result;
}

INCLUDE_RODATA(const s32, "code_r1_00242308", effStrUpDownHeight);

INCLUDE_RODATA(const s32, "code_r1_00242308", effStrLeftRightDir);

INCLUDE_ASM(const s32, "code_r1_00242308", effHandleDebugOrbitInput);

extern u32 effHandleDebugOrbitInputWork[];
extern u8 effWork7[];

s32 effResetStaticState(void) {
    effHandleDebugOrbitInputWork[0] = 0;
    effHandleDebugOrbitInputWork[1] = 0;
    effHandleDebugOrbitInputWork[2] = 0;
    effHandleDebugOrbitInputWork[3] = 0;
    effWork7[0x88] = 8;
    effWork7[0x89] = 0;
    effWork7[0x8A] = 0;
    return 0;
}

/* Synchronize the diagnostic camera position and calculate its positional guides.
 * Return input flags; the native diagnostic drawing providers are retail stubs. */
s32 effUpdateCameraDiagnostics(EffectStateSnapshot *snapshot, s8 mode) {
    u8 *camera = effUpdateCameraDiagnosticsData;
    f32 vectors[6][4] __attribute__((aligned(16)));
    f32 length;
    f32 deltaY;
    s32 result = 1;

    if ((*(u32 *)(camera + 0x10) & 2) == 0) {
        if (effData22[0] < 0) {
            result = 0;
        }
    }

    SDF_VU0_COPYVECTOR(D_003DFA20, snapshot->position);
    btlDebugMenuSetupNoOp(camera);
    btlDebugMenuBeforeRecordNoOp();
    btlDebugMenuRecordNoOp(camera);
    btlDebugMenuAfterRecordNoOp();
    SDF_VU0_COPYVECTOR(snapshot->position, D_003DFA20);

    if ((*(u32 *)(camera + 0x10) & 8) != 0) {
        result |= 2;
    }
    if ((result & 2) != 0 && (snapshot->flags & 0x18) != 0) {
        camFollowOffsetVec((u8 *)snapshot, vectors[0]);
        SDF_VU0_COPYVECTOR(snapshot->position, vectors[0]);
    }

    if (mode != 0) {
        effComputeBattleCameraPositionVU((u8 *)snapshot);
        SDF_VU0_LOADVECTOR_B(&effEditedQueueHeader);
        SDF_VU0_SUBVECTOR();
    } else {
        SDF_VU0_LOADVECTOR(snapshot->position);
    }
    SDF_VU0_STOREVECTOR(vectors[1]);
    VU0_SET_VF10_COMPONENT(y, 0.0f);
    SDF_VU0_GETLENGTH(length);
    SDF_VU0_LOADVECTOR(effBattleOffset);
    if (mode != 0) {
        SDF_VU0_LOADVECTOR_B(&effEditedQueueHeader);
        SDF_VU0_ADDVECTOR();
    }
    SDF_VU0_STOREVECTOR(vectors[5]);
    SDF_VU0_STOREVECTOR(vectors[3]);
    deltaY = vectors[1][1];
    vectors[3][1] += deltaY;
    if (vectors[3][1] == 0.0f) {
        vectors[3][1] = vectors[5][1] + (-2.0f);
    }
    if (length > 0.0f) {
        btlDebugDrawRadiusGuide(vectors[3], length, 0x80006060, 0);
        deltaY = vectors[1][1];
    }

    PCP_COPY_VECTOR_F32(vectors[2], vectors[3]);
    vectors[2][1] = vectors[5][1] + (-2.0f);
    if (deltaY != vectors[5][1]) {
        btlDebugDrawRadiusGuide(vectors[2], length, 0x80404040, 0);
        btlDebugDrawSegment(vectors[2], vectors[3], 0x80006060);
    }

    SDF_VU0_LOADVECTOR(vectors[1]);
    SDF_VU0_LOADVECTOR_B(vectors[5]);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(vectors[4]);
    btlDebugDrawSegment(vectors[3], vectors[4], 0x80006060);
    btlDebugDrawSegment(vectors[2], vectors[4], 0x80006060);
    btlDebugDrawPoint(vectors[4]);
    return result;
}

extern u8 sysUPDOWNHEIGHTData;
extern u8 sysUPDOWNHEIGHTData2;
extern u8 sysUPDOWNHEIGHTData3;

void sysLeaf45(void) {
    sysUPDOWNHEIGHTData = effWork7[0x88];
    sysUPDOWNHEIGHTData2 = effWork7[0x89];
    sysUPDOWNHEIGHTData3 = effWork7[0x8A];
}

INCLUDE_ASM(const s32, "code_r1_00242308", sysUPDOWNHEIGHT);

INCLUDE_ASM(const s32, "code_r1_00242308", effResetCameraSnapshotSelection);

u32 effUpdateCameraSnapshotGuides(EffectCameraSnapshotView *state, s8 useBattlePosition) {
    EffectCameraDebugScratch scratch;
    u32 result = 1;

    if ((effUpdateCameraSnapshotGuidesData.flags & 2) == 0) {
        if (effData22[0] < 0) {
            result = 0;
        }
    }

    SDF_VU0_COPYVECTOR(effWork9, state->camera.quat);
    if (useBattlePosition != 0) {
        effComputeBattleCameraPositionVU((u8 *)state);
    } else {
        SDF_VU0_LOADVECTOR(state->camera.offset);
    }
    SDF_VU0_STOREVECTOR_NOMEM(&scratch.basePosition);
    SDF_VU0_STOREVECTOR(D_003DFA20);

    btlDebugMenuSetupNoOp(&effUpdateCameraSnapshotGuidesData);
    btlDebugMenuBeforeRecordNoOp();
    btlDebugMenuRecordNoOp(&effUpdateCameraSnapshotGuidesData);
    btlDebugMenuAfterRecordNoOp();
    SDF_VU0_COPYVECTOR(state->camera.quat, effWork9);
    if (effUpdateCameraSnapshotGuidesData.flags & 8) {
        result |= 2;
    }
    SDF_VU0_LOADVECTOR(&scratch.basePosition);
    SDF_VU0_LOADVECTOR_B(effBattleOffset);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&scratch.segmentStart);

    SDF_VU0_LOADVECTOR(state->camera.quat);
    ToMatrix();
    VU0_STORE_MATRIX_UNCLOBBERED(scratch.rotationMatrix);

    SDF_VU0_LOADVECTOR(effWorldUpVector);
    SDF_VU0_APPLYMATRIX();
    VU0_SCALE_VF(vf10, 250.0f);
    SDF_VU0_LOADVECTOR_B(&scratch.segmentStart);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&scratch.segmentEnd);

    btlDebugDrawOrientedExtentGuide((f32 *)&scratch.segmentStart, 30.0f, -50.0f, 0x80006060, (f32 (*)[4])scratch.rotationMatrix);
    btlDebugDrawSegment((f32 *)&scratch.segmentStart, (f32 *)&scratch.segmentEnd, 0x80000080);
    btlDebugDrawPoint((f32 *)&scratch.segmentStart);

    SDF_VU0_LOADVECTOR_B(effBattleOffset);
    VU0_LOAD_MATRIX(scratch.rotationMatrix);
    SDF_VU0_LOAD_ROW3(&scratch.basePosition);
    SDF_VU0_LOADIDENTITYVECTOR();
    VU0_SET_VF10_COMPONENT(x, 100.0f);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&scratch.segmentStart);
    SDF_VU0_LOADIDENTITYVECTOR();
    VU0_SET_VF10_COMPONENT(x, -100.0f);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&scratch.segmentEnd);
    btlDebugDrawSegment((f32 *)&scratch.segmentStart, (f32 *)&scratch.segmentEnd, 0x80006060);

    SDF_VU0_LOADVECTOR_B(effBattleOffset);
    VU0_LOAD_MATRIX(scratch.rotationMatrix);
    SDF_VU0_LOAD_ROW3(&scratch.basePosition);
    SDF_VU0_LOADIDENTITYVECTOR();
    VU0_SET_VF10_COMPONENT(z, 25.0f);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&scratch.segmentStart);
    SDF_VU0_LOADIDENTITYVECTOR();
    VU0_SET_VF10_COMPONENT(z, -25.0f);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&scratch.segmentEnd);
    btlDebugDrawSegment((f32 *)&scratch.segmentStart, (f32 *)&scratch.segmentEnd, 0x80006060);

    return result;
}

u32 effLoadFileSlotAndPoll(void) {
    FileJob *file = fileQueueGetAt(effFileQueue, effGetSelectedFileQueueIndex());
    u32 result;

    memcpy(D_003DF840, file, 0x90);
    result = effUpdateCameraDiagnostics((EffectStateSnapshot *)D_003DF840, 1);
    memcpy(file, D_003DF840, 0x90);
    if (result & 1) {
        result |= 0x800000;
    }
    return result;
}

s32 effRunWithStateBackup(void) {
    EffectStateSnapshot snapshot = *(EffectStateSnapshot *)&effEditedQueueHeader;
    f32 *backup = snapshot.position;
    s32 result;

    SDF_VU0_COPYVECTOR(backup, &effEditedQueueHeader);
    result = effUpdateCameraDiagnostics(&snapshot, 0);
    SDF_VU0_COPYVECTOR(&effEditedQueueHeader, backup);
    if (result & 1) {
        result |= 0x800000;
    }
    return result;
}

u32 effProcessQueuedFileRecordAndPersistChanges(void) {
    FileJob *file = fileQueueGetAt(effFileQueue, effGetSelectedFileQueueIndex());
    u32 result;

    memcpy(D_003DF840, file, 0x90);
    result = effUpdateCameraSnapshotGuides((EffectCameraSnapshotView *)D_003DF840, 1);
    memcpy(file, D_003DF840, 0x90);
    if (result & 1) {
        result |= 0x800000;
    }
    return result;
}

void effResetFileResources(void) {
    effData12[0] = 0;
    effPartsPreviewReload = 0;
    effPartsListScroll = 0;
    if (effFileQueue != 0) {
        effPartsDelete(effFileQueue);
        effFileQueue = 0;
    }
    if (effAuxiliaryFileQueue != 0) {
        effPartsDelete(effAuxiliaryFileQueue);
        effAuxiliaryFileQueue = 0;
    }
    if (effTemporaryFileJob != 0) {
        fileJobDestroy(effTemporaryFileJob);
        effTemporaryFileJob = 0;
    }
}

void effBattleResourceDescriptorRelease(void) {
    if (effResourceBankDescriptor != 0) {
        btlDestroyResourceDescriptor(effResourceBankDescriptor);
        effResourceBankDescriptor = 0;
    }
    if (effResourceBankEntries != 0) {
        btlDestroyEntryList(effResourceBankEntries);
        effResourceBankEntries = 0;
    }
}

void effPollResourceBankSlot(char *path, u32 flags, EffResourceBankSlot *selection) {

    if (effResourceBankEntries == 0) {
        effResourceBankEntries = btlScanDirectory(path, flags);
        effResourceBankDescriptor = btlCreateResourceDescriptor(effResourceBankEntries);
        btlSetResourceBrowserPosition(effResourceBankDescriptor, 0xBA, 0x1C);
        return;
    }
    Draw_effLensFlare(effResourceBankDescriptor);
    selection->selectionStatus = btlGetResourceBrowserSelectionStatus(effResourceBankDescriptor);
    selection->type = btlFormatSelectedResourceName(effResourceBankDescriptor, selection->fullName);
    selection->pathVariant = btlGetResourcePathVariant(effResourceBankDescriptor);
    btlTrimResourceName(effResourceBankDescriptor, selection->name);
    if (selection->selectionStatus == BTL_RESOURCE_SELECTION_ACCEPTED) {
        btlDestroyResourceDescriptor(effResourceBankDescriptor);
        effResourceBankDescriptor = 0;
        btlDestroyEntryList(effResourceBankEntries);
        effResourceBankEntries = 0;
    }
}

void effInitializeResourceQueue(void) {
    FileJob *record;

    if (effFileQueueNameRecord != 0) {
        btlDestroyResourceNameRecord(effFileQueueNameRecord);
    }
    effFileQueueNameRecord = btlCreateResourceNameRecord(D_003BCF30);
    btlSetResourceNamePosition(effFileQueueNameRecord, 0xC2, 0xC8);
    btlSetResourceNameLengthLimit(effFileQueueNameRecord, 9);
    record = fileQueueGetAt(effFileQueue, effGetSelectedFileQueueIndex());
    btlResourceRecordSetName(effFileQueueNameRecord, record->name);
}

u32 effPollResourceQueue(void) {
    u32 state;

    btlUpdateResourceNameEditor(effFileQueueNameRecord);
    btlFormatResourceNameWithoutExtension(effFileQueueNameRecord, fileQueueGetAt(effFileQueue, effGetSelectedFileQueueIndex())->name);
    state = btlGetResourceNameSelectionStatus(effFileQueueNameRecord);
    if ((u32)(state - BTL_RESOURCE_SELECTION_ACCEPTED) < 2) {
        btlDestroyResourceNameRecord(effFileQueueNameRecord);
        effFileQueueNameRecord = 0;
        return 0;
    }
    return 0x200001;
}

void effReleaseQueuedResourceName(void) {
    if (effQueuedResourceNameRecord != 0) {
        btlDestroyResourceNameRecord(effQueuedResourceNameRecord);
        effQueuedResourceNameRecord = 0;
    }
}

void effQueueResource(const char *extension, const char *resourceName) {
    if (effQueuedResourceNameRecord == 0) {
        effQueuedResourceNameRecord = btlCreateResourceNameRecord(extension);
        btlSetResourceNamePosition(effQueuedResourceNameRecord, 0xC2, 0xC8);
    }
    btlResourceRecordSetName(effQueuedResourceNameRecord, resourceName);
}

void effUpdateResourceQueue(const char *directoryPath, const char *extension, EffQueueRecord *record) {
    u32 state;

    if (effQueuedResourceNameRecord == 0) {
        effQueuedResourceNameRecord = btlCreateResourceNameRecord(extension);
        btlSetResourceNamePosition(effQueuedResourceNameRecord, 0xC2, 0xC8);
        return;
    }
    btlUpdateResourceNameEditor(effQueuedResourceNameRecord);
    btlFormatResourceNameWithExtension(effQueuedResourceNameRecord, record->nameWithExtension);
    btlFormatResourceNameWithoutExtension(effQueuedResourceNameRecord, record->name);
    state = btlGetResourceNameSelectionStatus(effQueuedResourceNameRecord);
    record->completion.state = state;
    if (state == BTL_RESOURCE_SELECTION_ACCEPTED) {
        if (btlPollResourceNameOverwrite(effQueuedResourceNameRecord, directoryPath) !=
            BTL_RESOURCE_WRITE_PERMISSION_NOT_GRANTED) {
            btlDestroyResourceNameRecord(effQueuedResourceNameRecord);
            effQueuedResourceNameRecord = 0;
        } else {
            record->completion.state = BTL_RESOURCE_SELECTION_PENDING;
        }
    }
}

void effReleaseResourceBankDescriptors(void) {
    if (effResourceBankDescriptor != 0) {
        btlDestroyResourceDescriptor(effResourceBankDescriptor);
        effResourceBankDescriptor = 0;
    }
    if (effResourceBankEntries != 0) {
        btlDestroyEntryList(effResourceBankEntries);
        effResourceBankEntries = 0;
    }
}

void effPollResourceBank(s32 flags, EffectPollRecord *record) {
    if (effResourceBankEntries == 0) {
        effResourceBankEntries = btlScanDirectory(0, flags);
        if (flags & BTL_RESOURCE_SCAN_GENERAL) {
            u32 i = 0;
            u32 count = dds3BillboardGeneralNumber();
            if (count != 0) {
                do {
                    char name[0x70];
                    sprintf(name, "GENERAL %d", i);
                    btlAppendEntry(effResourceBankEntries, name, BTL_RESOURCE_ENTRY_CATEGORY_GENERAL, i, 0);
                    i++;
                } while (i < count);
            }
        }
        effResourceBankDescriptor = btlCreateResourceDescriptor(effResourceBankEntries);
        btlSetResourceBrowserPosition(effResourceBankDescriptor, 0xBA, 0x1C);
    } else {
        Draw_effLensFlare(effResourceBankDescriptor);
        record->selectionStatus = btlGetResourceBrowserSelectionStatus(effResourceBankDescriptor);
        record->type = btlFormatSelectedResourceName(effResourceBankDescriptor, record->fullName);
        record->pathVariant = btlGetResourcePathVariant(effResourceBankDescriptor);
        if (record->selectionStatus == BTL_RESOURCE_SELECTION_ACCEPTED) {
            btlDestroyResourceDescriptor(effResourceBankDescriptor);
            effResourceBankDescriptor = 0;
            btlDestroyEntryList(effResourceBankEntries);
            effResourceBankEntries = 0;
        }
    }
}

void effResetMappingFlags(void) {
    effColorEditRgbMenuOpen = 0;
    effColorEditPage = 0;
    effColorEditSelectedKey = 0;
    effColorEditSelectedAlphaKey = 1;
    effMappingState.cursor = 1;
    effMappingState.flags |= 0x10;
}

INCLUDE_ASM(const s32, "code_r1_00242308", effEditColorTrack);

s32 effMapObjectWithTemporaryMappingTable(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.rows = D_0038F7D8;
    effMappingState.count = 8;
    result = effEditColorTrack(object + 0x2c, object + 0x50, ((EffMappingObject *)object)->valueB8);
    effMappingState.rows = D_0038F658;
    effMappingState.count = 8;
    return result;
}

/* Edit the request's G-/K-family COLOR tracks; return the native editor status. */
s32 effEditGkColorTrack(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.rows = D_0038F7D8;
    effMappingState.count = 8;
    result = effEditColorTrack(object + 0x48, object + 0x6c, ((EffMappingObject *)object)->valueD4);
    effMappingState.rows = D_0038F658;
    effMappingState.count = 8;
    return result;
}

/* B-/F-family COLOR callback: edit the request's color and adjacent alpha tracks. */
s32 effEditBfColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->valueB8);
}

/* HELPER's COLOR callback edits the shared default color/alpha data. */
s32 effEditHlpColorTrack(void) {
    return effEditColorTrack((s32)effWork6, (s32)effWork6 + 0x24, ((EffMappingObject *)effWork6)->value34);
}

/* Edit T-BOARD color/alpha tracks using the alternate property-option table. */
s32 effEditTBoardColorTrack(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.rows = D_0038F7D8;
    effMappingState.count = 8;
    result = effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value8C);
    effMappingState.rows = D_0038F658;
    effMappingState.count = 8;
    return result;
}

/* Pass the E-RAIN request's color/alpha tracks and frame bound to the editor. */
s32 effEditRainColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value34);
}

/* Pass the E-FLARE request's color/alpha tracks and frame bound to the editor. */
s32 effEditFlareColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value38);
}

/* COLOR property callback shared by the native S-family descriptors. */
s32 effEditSColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value34);
}

/* Edit R-NOMAL/R-CRCLE COLOR tracks, then restore the default option table. */
s32 effEditRColorTrack(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.rows = effData14;
    effMappingState.count = 8;
    result = effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value34);
    effMappingState.rows = D_0038F658;
    effMappingState.count = 8;
    return result;
}

/* COLOR property callback for the native R-TRACK descriptor. */
s32 effEditRTrackColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value34);
}

/* D-family COLOR callback. */
s32 effEditDoColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value34);
}

s32 effResolveMappingWithTemporaryTable(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.rows = D_0038F7D8;
    effMappingState.count = 8;
    result = effEditColorTrack(object + 0x4c, object + 0x70, ((EffMappingObject *)object)->valueD8);
    effMappingState.rows = D_0038F658;
    effMappingState.count = 8;
    return result;
}

/* COLOR callback shared by the native N-SPIRL/N-CYLND/N-SPHRE descriptors. */
s32 effEditNColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value34);
}

/* Edit COLOR in the N-family descriptors' nested POLYGON data. */
s32 effEditNPolygonColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object + 0x3c, object + 0x60, ((EffMappingObject *)object)->value80);
}

/* Edit the root U-CYLND color and alpha tracks selected by the request. */
s32 effEditUCylinderColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value34);
}

/* Edit COLOR in U-CYLND's nested POLYGON data. */
s32 effEditUCylinderPolygonColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object + 0x3c, object + 0x60, ((EffMappingObject *)object)->value80);
}

/* U-SMOKE/U-SPIRL COLOR uses its own property options; forward editor status. */
s32 effEditUSmokeSpiralColorTrack(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.rows = D_0038F7D8;
    effMappingState.count = 8;
    result = effEditColorTrack(object + 0x68, object + 0x8c, ((EffMappingObject *)object)->valueF4);
    effMappingState.rows = D_0038F658;
    effMappingState.count = 8;
    return result;
}

/* Edit U-SPHRE's root COLOR tracks using its frame bound. */
s32 effEditUSphereColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value70);
}

/* COLOR callback for the CRACK polygon descriptor. */
s32 effEditCrackPolygonColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value34);
}

/* COLOR callback for the FLASH polygon descriptor. */
s32 effEditFlashPolygonColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object + 0x50, object + 0x74, ((EffMappingObject *)object)->value84);
}

/* 2D's COLOR callback restores its property options and forwards editor status. */
s32 effEditTwoDimensionalColorTrack(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.rows = effData14;
    effMappingState.count = 8;
    result = effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value8C);
    effMappingState.rows = D_0038F658;
    effMappingState.count = 8;
    return result;
}

extern EffEditorRow effData15[];
extern EffEditorRow effData13[];

/* FUNC effEditMColorTrack */
#ifdef NON_MATCHING
/* 7 of 25 words differ, first at +0x20: retail loads the rows address before p->x0C and stores count and rows in a different order. */
void effEditMColorTrack(BaObj *p) {
    s32 object;

    effMappingState.rows = effData15;
    effMappingState.count = 8;
    object = p->x0C;
    effEditColorTrack(object + 0x7C, object + 0xA0, ((EffMappingObject *)object)->value108);
    effMappingState.rows = effData13;
    effMappingState.count = 8;
}
#else
INCLUDE_ASM(const s32, "code_r1_00242308", effEditMColorTrack);
#endif

extern EffQueuedFileObject effData18;
extern EffQueuedFileObject effData20;
extern u32 effEditScalarTrackData;
extern u32 effEditScalarTrackData2;
extern u32 effEditScalarTrackData3;

void btlResetEffectWork(void) {
    effData18.flags |= 0x10;
    effData18.cursor = 0;
    effData20.flags |= 0x10;
    effData20.cursor = 0;
    effEditScalarTrackData = 0;
    effEditScalarTrackData2 = 0;
    effEditScalarTrackData3 = 0;
}

INCLUDE_ASM(const s32, "code_r1_00242308", effEditScalarTrack);

/* Edit the requested P-family SCALE track; return the native editor status. */
s32 effEditPScaleTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x60), ((EffMappingObject *)p->x0C)->valueB8, D_0038F9D0, D_0038F8E8);
}

/* Edit the requested P-family ROTATE track with its independent editor state. */
s32 effEditPRotationTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x8C), ((EffMappingObject *)p->x0C)->valueB8, D_0038FAD0, D_0038FA20);
}

/* SCALE property callback shared by native G-/K-family descriptors. */
s32 effEditGkScaleTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x7C), ((EffMappingObject *)p->x0C)->valueD4, D_0038F9D0, D_0038F8E8);
}

/* ROTATE property callback for native K-family descriptors. */
s32 effEditKRotationTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0xA8), ((EffMappingObject *)p->x0C)->valueD4, D_0038FAD0, D_0038FA20);
}

/* SCALE property callback for native B-family descriptors. */
s32 effEditBScaleTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x34), ((EffMappingObject *)p->x0C)->valueB8, D_0038F9D0, D_0038F8E8);
}

/* B-family ROTATE callback; identical forwarding code does not imply P-family SCALE. */
s32 effEditBRotationTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x60), ((EffMappingObject *)p->x0C)->valueB8, D_0038F9D0, D_0038F8E8);
}

/* AREA property callback shared by B-/F-family descriptors. */
s32 effEditBfAreaTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x8C), ((EffMappingObject *)p->x0C)->valueB8, D_0038F9D0, D_0038F8E8);
}

/* Pass the requested T-BOARD SCALE track and its frame bound to the editor. */
s32 effEditTBoardScaleTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x34), ((EffMappingObject *)p->x0C)->value8C, D_0038F9D0, D_0038F8E8);
}

extern char effData19[];

s32 effEditTBoardRotationTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x60), ((EffMappingObject *)p->x0C)->value8C, &effData20, effData19);
}

/* SCALE property callback for native W-family descriptors. */
s32 effEditWScaleTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x80), ((EffMappingObject *)p->x0C)->valueD8, D_0038F9D0, D_0038F8E8);
}

/* ROTATE property callback for native W-family descriptors. */
s32 effEditWRotationTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0xAC), ((EffMappingObject *)p->x0C)->valueD8, D_0038FAD0, D_0038FA20);
}

/* Edit the SCALE_UV curve in U-CYLND's POLYGON child; forward editor status. */
s32 effEditUCylinderPolygonUvScaleTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x90), ((EffMappingObject *)p->x0C)->value80, D_0038F9D0, effData17);
}

/* Edit the shared U-SMOKE/U-SPIRL root SCALE curve; forward editor status. */
s32 effEditUSmokeSpiralScaleTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x9C), ((EffMappingObject *)p->x0C)->valueF4, D_0038F9D0, D_0038F8E8);
}

/* Edit the shared U-SMOKE/U-SPIRL root ROTATE curve; forward editor status. */
s32 effEditUSmokeSpiralRotationTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0xC8), ((EffMappingObject *)p->x0C)->valueF4, D_0038FAD0, D_0038FA20);
}

/* Edit SCALE_UV in their POLYGON children; forward the numeric editor's status. */
s32 effEditUSmokeSpiralPolygonUvScaleTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C), ((EffMappingObject *)p->x0C)->valueF4, D_0038F9D0, effData17);
}

/* Edit the native U-SPHRE root SCALE curve; forward editor status. */
s32 effEditUSphereScaleTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x34), ((EffMappingObject *)p->x0C)->value70, D_0038F9D0, effData17);
}

/* Edit the native 2D root SCALE curve; forward editor status. */
s32 effEditTwoDimensionalScaleTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x34), ((EffMappingObject *)p->x0C)->value8C, D_0038F9D0, D_0038F8E8);
}

/* Edit the native 2D root ROTATE curve; forward editor status. */
s32 effEditTwoDimensionalRotationTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x60), ((EffMappingObject *)p->x0C)->value8C, D_0038F9D0, D_0038F8E8);
}

extern char effData16[];

s32 effEditMScaleTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0xB0), ((EffMappingObject *)p->x0C)->value108, &effData18, &effData16);
}

void effLeaf401(void) {
    effColorEditSelectedAlphaKey = 1;
}

INCLUDE_ASM(const s32, "code_r1_00242308", effFunc20);

/* ALPHA callback for POLYGON children of the native S-family descriptors. */
s32 effEditSPolygonAlphaTrack(s32 work) {
    return func_002BB188((SdfAlphaTrack *)(((BaObj *)work)->x0C + 0x3c), ((EffMappingObject *)((BaObj *)work)->x0C)->value4C);
}

/* D-family POLYGON FADE callback; retain bound plus one. */
s32 effEditDoPolygonFadeTrack(s32 work) {
    return func_002BB188((SdfAlphaTrack *)(((BaObj *)work)->x0C + 0x60), ((EffMappingObject *)((BaObj *)work)->x0C)->value3C + 1);
}

/* FADE callback for N-family POLYGON children; preserve bound plus one. */
s32 effEditNPolygonFadeTrack(s32 work) {
    return func_002BB188((SdfAlphaTrack *)(((BaObj *)work)->x0C + 0x70), ((EffMappingObject *)((BaObj *)work)->x0C)->value8C + 1);
}

/* FADE callback for the native U-CYLND POLYGON child. */
s32 effEditUCylinderPolygonFadeTrack(s32 work) {
    return func_002BB188((SdfAlphaTrack *)(((BaObj *)work)->x0C + 0x70), ((EffMappingObject *)((BaObj *)work)->x0C)->value8C + 1);
}

/* FADE callback for the native U-SPHRE POLYGON child. */
s32 effEditUSpherePolygonFadeTrack(s32 work) {
    return func_002BB188((SdfAlphaTrack *)(((BaObj *)work)->x0C + 0x60), ((EffMappingObject *)((BaObj *)work)->x0C)->value74 + 1);
}

s32 effPollFileQueueRecord(s32 request) {
    u8 record[0xE0];
    EffectPollRecord *status = (EffectPollRecord *)record;
    s32 selectionStatus;
    s32 result = 0x600001;

    effPollResourceBank(request, status);
    selectionStatus = status->selectionStatus;
    if (selectionStatus == BTL_RESOURCE_SELECTION_CANCELED) {
        result = 0x400000;
    } else if (selectionStatus == BTL_RESOURCE_SELECTION_ACCEPTED) {
        if (status->type != 8) {
            FileJob *entry = fileQueueGetAt(effFileQueue, -status->pathVariant);
            if (effCurrentFileQueueEntry != entry) {
                fileQueueDetachSectorFollower(effFileQueue, effCurrentFileQueueEntry);
                fileQueueLinkJobToSectorLeader(effFileQueue, effCurrentFileQueueEntry, entry);
            }
        } else {
            fileQueueDetachSectorFollower(effFileQueue, effCurrentFileQueueEntry);
            fileJobSetSecondaryData(effQueuedFileHandle, &status->pathVariant, 4, 4);
        }
        result = 0x400002;
    }
    return result;
}

/* Poll queued TMX/P2A/general resources with the TLP format enabled. */
s32 effPollResourceQueueWithTlp(void) {
    return effPollFileQueueRecord(BTL_RESOURCE_SCAN_TMX | BTL_RESOURCE_SCAN_P2A | BTL_RESOURCE_SCAN_GENERAL | BTL_RESOURCE_SCAN_TLP);
}

/* Separate native descriptor callback with the same TLP-enabled queue filter. */
s32 effPollAlternateResourceQueueWithTlp(void) {
    return effPollFileQueueRecord(BTL_RESOURCE_SCAN_TMX | BTL_RESOURCE_SCAN_P2A | BTL_RESOURCE_SCAN_GENERAL | BTL_RESOURCE_SCAN_TLP);
}

/* Poll the queue without adding the TLP flag to its format mask. */
s32 effPollResourceQueueWithoutTlp(void) {
    return effPollFileQueueRecord(BTL_RESOURCE_SCAN_TMX | BTL_RESOURCE_SCAN_P2A | BTL_RESOURCE_SCAN_GENERAL);
}

s32 effPollFileRecord(char *path, s32 flags) {
    EffResourceBankSlot record;
    s32 kind;
    s32 result = 0x600001;

    effPollResourceBankSlot(path, flags, &record);
    kind = record.selectionStatus;
    if (kind == BTL_RESOURCE_SELECTION_CANCELED) {
        result = 0x400000;
    } else if (kind == BTL_RESOURCE_SELECTION_ACCEPTED) {
        s32 type;
        fileQueueDetachSectorFollower(effFileQueue, effCurrentFileQueueEntry);
        type = record.type;
        if (type != 8) {
            fileJobCopyCommandIntoSecondaryData(effQueuedFileHandle,
                                                (const char *)&record, Start_sbtl(type));
        } else {
            fileJobSetSecondaryData(effQueuedFileHandle, &record.pathVariant, 4, 4);
        }
        result = 0x400002;
    }
    return result;
}

/* Poll the TEXTURE property chooser in /tool/effect/mat/ and return its status. */
s32 effPollTextureResourceFile(void) {
    return effPollFileRecord(D_003B3B88, BTL_RESOURCE_SCAN_TMX | BTL_RESOURCE_SCAN_P2A | BTL_RESOURCE_SCAN_TLP);
}

/* Alternate descriptor family's TEXTURE callback; retain its separate entrypoint. */
s32 effPollAlternateTextureResourceFile(void) {
    return effPollFileRecord(D_003B3B88, BTL_RESOURCE_SCAN_TMX | BTL_RESOURCE_SCAN_P2A | BTL_RESOURCE_SCAN_TLP);
}

/* Poll EP resources from /tool/effect/ep/ and return the chooser status. */
s32 effPollEpResourceFile(void) {
    return effPollFileRecord(effStrToolEffectEp, BTL_RESOURCE_SCAN_EP);
}

/* Poll EPL resources from /tool/effect/ and return the chooser status. */
s32 effPollEplResourceFile(void) {
    return effPollFileRecord(D_003B39E0, BTL_RESOURCE_SCAN_EPL);
}

/* Separate native EP descriptor callback using the same directory and format. */
s32 effPollAlternateEpResourceFile(void) {
    return effPollFileRecord(effStrToolEffectEp, BTL_RESOURCE_SCAN_EP);
}

/* TMX-only TEXTURE chooser for B-/F-family MATERIAL groups. */
s32 effPollBfMaterialTmxFile(void) {
    return effPollFileRecord(D_003B3B88, BTL_RESOURCE_SCAN_TMX);
}

/* TMX-only TEXTURE chooser for N-family MATERIAL groups. */
s32 effPollNMaterialTmxFile(void) {
    return effPollFileRecord(D_003B3B88, BTL_RESOURCE_SCAN_TMX);
}

/* TMX-only TEXTURE chooser for S-RAD_T/SPK_T/RNG_T/SCT_T MATERIAL groups. */
s32 effPollSTMaterialTmxFile(void) {
    return effPollFileRecord(D_003B3B88, BTL_RESOURCE_SCAN_TMX);
}

u32 effFileJobSecondaryDataSet(void) {
    u32 zero = 0;
    fileJobSetSecondaryData(effQueuedFileHandle, &zero, 4, 4);
    return 0x400002;
}

/* Poll PB resources from /tool/effect/hlp/ and return the chooser status. */
s32 effPollHlpPbResourceFile(void) {
    return effPollFileRecord(D_003B3BA0, BTL_RESOURCE_SCAN_PB);
}

s32 effPollMMaterialPbFile(void) {
    return effPollFileRecord(effStrToolEffectMat, BTL_RESOURCE_SCAN_PB);
}

s32 effPollMMaterialEplFile(void) {
    return effPollFileRecord(effStrToolEffect, BTL_RESOURCE_SCAN_EPL);
}

s32 effPollOMaterialPbFile(void) {
    return effPollFileRecord(effStrToolEffectHlp, BTL_RESOURCE_SCAN_PB);
}

u32 fileLoadEffectSlotA(void) {
    EffResourceBankSlot fileInfo;
    FileJobPayload *job;
    FileJob *entry;
    EffFileResourceRecord *resource;
    void *fileData;
    s32 status;
    u32 result;

    effPollResourceBankSlot(D_003B3B88, BTL_RESOURCE_SCAN_PB, &fileInfo);
    status = fileInfo.selectionStatus;
    result = 0x600001;
    if (status == BTL_RESOURCE_SELECTION_CANCELED) {
        result = 0x400000;
    } else if (status == BTL_RESOURCE_SELECTION_ACCEPTED) {
        job = fileCreateJob(3);
        fileJobSetPrimaryData(job, fileLoadEffectSlotAData.start, fileLoadEffectSlotAData.length,
                      fileLoadEffectSlotAData.mode);
        fileJobCopyCommandIntoSecondaryData(job, (const char *)&fileInfo,
                      Start_sbtl(fileInfo.type));
        entry = fileAppendJob(effFileQueue, (u32)job);
        effCurrentFileQueueEntry = entry;
        memcpy(effSelectedJobHeader, entry, 0x80);
        effQueuedFileHandle = (FileJobPayload *)entry->id;
        resource = (EffFileResourceRecord *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        fileData = fileResolvePrimaryBuffer(effQueuedFileHandle);
        memcpy(resource->buffer, fileData, resource->size);
        D_003BD064 = effCreateBattleCameraJob((u8 *)resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = (u8 *)&effPartsListMenu;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy(effTemporaryFileJob);
            effTemporaryFileJob = 0;
        }
        result = 0x800002;
    }
    return result;
}

INCLUDE_ASM(const s32, "code_r1_00242308", effPollAndQueueCopiedFileResource);

u32 fileLoadEffectSlotHelp(void) {
    EffResourceBankSlot fileInfo;
    u32 result;
    s32 status;
    FileJobPayload *job;
    FileJob *entry;
    EffFileResourceRecord *resource;
    u8 *buffer;
    sdfFileCmdHandle_t *command;
    u32 totalLength;
    u32 dataLength;
    struct sdfMemHandle_t *allocation;

    effPollResourceBankSlot(D_003B3BA0, BTL_RESOURCE_SCAN_PB, &fileInfo);
    status = fileInfo.selectionStatus;
    result = 0x600001;
    if (status == BTL_RESOURCE_SELECTION_CANCELED) {
        result = 0x400000;
    } else if (status == BTL_RESOURCE_SELECTION_ACCEPTED) {
        u32 headerBytes = 0x40;
        struct sdfMemHandle_t *oldAllocation;
        u32 queuedFile;
        job = fileCreateJob(6);
        command = sdfFileOpen(&fileInfo);
        dataLength = sdfFileLength(command);
        totalLength = dataLength + headerBytes;
        allocation = sdfMemAllocDirect(totalLength);
        buffer = (u8 *)sdfMemLock(allocation);
        memset(buffer, 0, headerBytes);
        sdfFileRead(command, buffer + headerBytes, dataLength);
        sdfFileClose(command);
        fileJobSetPrimaryData(job, buffer, totalLength, 0);
        entry = fileAppendJob(effFileQueue, (u32)job);
        effCurrentFileQueueEntry = entry;
        resource = (EffFileResourceRecord *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        memcpy(effSelectedJobHeader, entry, 0x80);
        queuedFile = entry->id;
        oldAllocation = resource->allocation;
        effQueuedFileHandle = (FileJobPayload *)queuedFile;
        if (oldAllocation != 0) {
            sdfMemFreeDirect(oldAllocation);
        }
        resource->allocation = allocation;
        resource->buffer = buffer;
        resource->size = dataLength + headerBytes;
        resource->mode = 0;
        memcpy(effWork6, fileLoadEffectSlotHelpData, 0x3C);
        D_003BD064 = effCreateBattleCameraJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = (u8 *)&effPartsListMenu;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy(effTemporaryFileJob);
            effTemporaryFileJob = 0;
        }
        result = 0x800002;
    }
    return result;
}

u32 fileLoadEffectSlotB(void) {
    EffResourceBankSlot fileInfo;
    FileJobPayload *job;
    FileJob *entry;
    EffFileResourceRecord *resource;
    void *fileData;
    s32 status;
    u32 result;

    effPollResourceBankSlot(D_003B3B88, BTL_RESOURCE_SCAN_PB, &fileInfo);
    status = fileInfo.selectionStatus;
    result = 0x600001;
    if (status == BTL_RESOURCE_SELECTION_CANCELED) {
        result = 0x400000;
    } else if (status == BTL_RESOURCE_SELECTION_ACCEPTED) {
        job = fileCreateJob(18);
        fileJobSetPrimaryData(job, fileLoadEffectSlotBData.start, fileLoadEffectSlotBData.length,
                      fileLoadEffectSlotBData.mode);
        fileJobCopyCommandIntoSecondaryData(job, (const char *)&fileInfo,
                      Start_sbtl(fileInfo.type));
        entry = fileAppendJob(effFileQueue, (u32)job);
        effCurrentFileQueueEntry = entry;
        memcpy(effSelectedJobHeader, entry, 0x80);
        effQueuedFileHandle = (FileJobPayload *)entry->id;
        resource = (EffFileResourceRecord *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        fileData = fileResolvePrimaryBuffer(effQueuedFileHandle);
        memcpy(resource->buffer, fileData, resource->size);
        D_003BD064 = effCreateBattleCameraJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = (u8 *)&effPartsListMenu;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy(effTemporaryFileJob);
            effTemporaryFileJob = 0;
        }
        result = 0x800002;
    }
    return result;
}

INCLUDE_RODATA(const s32, "code_r1_00242308", effStrFadeOut);

INCLUDE_RODATA(const s32, "code_r1_00242308", effStrFadeIn);

INCLUDE_RODATA(const s32, "code_r1_00242308", effStrBlend);

INCLUDE_RODATA(const s32, "code_r1_00242308", effStrAlpha);

INCLUDE_RODATA(const s32, "code_r1_00242308", effStrTransparency);

INCLUDE_RODATA(const s32, "code_r1_00242308", effStrFrame);

INCLUDE_RODATA(const s32, "code_r1_00242308", effStrPoint);

INCLUDE_RODATA(const s32, "code_r1_00242308", effStrToolEffectMat);

INCLUDE_RODATA(const s32, "code_r1_00242308", effStrToolEffectHlp);

u32 effLoadFileSlotF2(void) {
    EffResourceBankSlot fileInfo;
    FileJobPayload *job;
    FileJob *entry;
    EffFileResourceRecord *resource;
    void *fileData;
    s32 status;
    u32 result;

    effPollResourceBankSlot("/tool/effect/f2/", BTL_RESOURCE_SCAN_F2, &fileInfo);
    status = fileInfo.selectionStatus;
    result = 0x600001;
    if (status == BTL_RESOURCE_SELECTION_CANCELED) {
        result = 0x400000;
    } else if (status == BTL_RESOURCE_SELECTION_ACCEPTED) {
        job = fileCreateJob(0x14);
        fileJobSetPrimaryData(job, effLoadFileSlotF2Data.start, effLoadFileSlotF2Data.length,
                      effLoadFileSlotF2Data.mode);
        fileJobCopyCommandIntoSecondaryData(job, (const char *)&fileInfo,
                      Start_sbtl(fileInfo.type));
        entry = fileAppendJob(effFileQueue, (u32)job);
        effCurrentFileQueueEntry = entry;
        memcpy(effSelectedJobHeader, entry, 0x80);
        effQueuedFileHandle = (FileJobPayload *)entry->id;
        resource = (EffFileResourceRecord *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        fileData = fileResolvePrimaryBuffer(effQueuedFileHandle);
        memcpy(resource->buffer, fileData, resource->size);
        D_003BD064 = effCreateBattleCameraJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = (u8 *)&effPartsListMenu;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy(effTemporaryFileJob);
            effTemporaryFileJob = 0;
        }
        result = 0x800002;
    }
    return result;
}

u32 effLoadMaterialFile(void) {
    EffResourceBankSlot fileInfo;
    FileJobPayload *job;
    FileJob *entry;
    EffFileResourceRecord *resource;
    void *fileData;
    s32 status;
    u32 result;

    effPollResourceBankSlot(D_003B3B88, BTL_RESOURCE_SCAN_P2A, &fileInfo);
    status = fileInfo.selectionStatus;
    result = 0x600001;
    if (status == BTL_RESOURCE_SELECTION_CANCELED) {
        result = 0x400000;
    } else if (status == BTL_RESOURCE_SELECTION_ACCEPTED) {
        job = fileCreateJob(0x16);
        fileJobSetPrimaryData(job, effLoadMaterialFileData.start, effLoadMaterialFileData.length,
                      effLoadMaterialFileData.mode);
        fileJobCopyCommandIntoSecondaryData(job, (const char *)&fileInfo,
                      Start_sbtl(fileInfo.type));
        entry = fileAppendJob(effFileQueue, (u32)job);
        effCurrentFileQueueEntry = entry;
        memcpy(effSelectedJobHeader, entry, 0x80);
        effQueuedFileHandle = (FileJobPayload *)entry->id;
        resource = (EffFileResourceRecord *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        fileData = fileResolvePrimaryBuffer(effQueuedFileHandle);
        memcpy(resource->buffer, fileData, resource->size);
        D_003BD064 = effCreateBattleCameraJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = (u8 *)&effPartsListMenu;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy(effTemporaryFileJob);
            effTemporaryFileJob = 0;
        }
        result = 0x800002;
    }
    return result;
}

void effResetFileResourceManager(void) {
    effColorEditRgbMenuOpen = 0;
    effColorEditSelectedAlphaKey = 1;
    effColorEditSelectedKey = 0;
    effColorEditPage = 0;
    D_003BD158 = 0;
    D_003BD15C = 0;
    D_003BD160 = 0;
}

u16 Start_sbtl(s32 flags) {
    switch (flags) {
        case 1:   return 1;
        case 2:   return 2;
        case 4:   return 3;
        case 8:   return 4;
        case 16:  return 6;
        case 32:  return 5;
        case 64:  return 7;
        case 128: return 8;
        default:  return 0;
    }
}
INCLUDE_SDATA(const s32, "code_r1_00242308", sdfUpdateFlagListEntriesData);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPARTICLEDataEntry10);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPARTICLEDataEntry110);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPARTICLEDataEntry111);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPARTICLEDataEntry112);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTrackControlTaskDataEntry76);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTrackControlTaskDataEntry77);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTrackControlTaskDataEntry78);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTrackTaskDataEntry14);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTrackTaskDataEntry15);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTrackTaskDataEntry16);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyWindTaskDataEntry107);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyWindTaskDataEntry108);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyWindTaskDataEntry109);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyWindTaskDataEntry110);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTrackTaskDataEntry17);

INCLUDE_SDATA(const s32, "code_r1_00242308", effLabelBlur);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPOLYTEXDataEntry8);

INCLUDE_SDATA(const s32, "code_r1_00242308", effUpdatePartsListEditorData6Entry2);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPOLYTEXDataEntry9);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData20Entry2);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTrackTaskDataEntry18);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTrackTaskDataEntry19);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPARTICLEDataEntry11);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTwinkleTaskDataEntry83);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyScalyTaskDataEntry58);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPARTICLEDataEntry12);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyThunderTaskDataEntry27);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyThunderTaskDataEntry28);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyThunderTaskDataEntry29);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyThunderTaskDataEntry30);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTrackControlTaskDataEntry79);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyScalyTaskDataEntry59);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyScalyTaskDataEntry11);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPARTICLEDataEntry13);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPARTICLEDataEntry14);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyWindTaskDataEntry111);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyWindTaskDataEntry112);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPARTICLEDataEntry15);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPARTICLEDataEntry113);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTrackControlTaskDataEntry20);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTwinkleTaskDataEntry84);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTwinkleTaskDataEntry85);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTwinkleTaskDataEntry86);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTwinkleTaskDataEntry87);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyScalyTaskDataEntry60);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPOLYWINDDataEntry87);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTrackTaskDataEntry20);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTrackControlTaskDataEntry8);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTrackControlTaskDataEntry9);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTrackControlTaskDataEntry10);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTrackControlTaskDataEntry11);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTrackTaskDataEntry6);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTrackTaskDataEntry87);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTrackTaskDataEntry88);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTrackTaskDataEntry89);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTrackTaskDataEntry7);

INCLUDE_SDATA(const s32, "code_r1_00242308", fileLoadEffectSlotADataEntry);

INCLUDE_SDATA(const s32, "code_r1_00242308", fileLoadEffectSlotADataEntry2);

INCLUDE_SDATA(const s32, "code_r1_00242308", effPollAndQueueCopiedFileResourceDataEntry);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData9Entry20);

INCLUDE_SDATA(const s32, "code_r1_00242308", effPollAndQueueCopiedFileResourceDataEntry2);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData9Entry21);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreateBlurTaskDataEntry18);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreateBlurTaskDataEntry49);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPOLYTEXDataEntry10);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreateBlurTaskDataEntry19);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreateBlurTaskDataEntry7);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTwinkleTaskDataEntry88);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreateBlurTaskDataEntry20);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreateEnvironmentTaskDataEntry17);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreateBlurTaskDataEntry21);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreateBlurTaskDataEntry22);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreateBlurTaskDataEntry8);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreateFilterTaskDataEntry3);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreateFilterTaskDataEntry4);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyCrackTaskDataEntry14);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData9Entry22);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPOLYTEXDataEntry3);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPOLYTEXDataEntry11);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreateEnvironmentTaskDataEntry6);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreateEnvironmentTaskDataEntry7);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreateEnvironmentTaskDataEntry18);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyFlashTaskDataEntry9);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyFlashTaskDataEntry151);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyFlashTaskDataEntry152);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyFlashTaskDataEntry153);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyRingTaskDataEntry9);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData48);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData49);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData50);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyFlashTaskDataEntry22);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyFlashTaskDataEntry23);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyFlashTaskDataEntry24);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyFlashTaskDataEntry154);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyFlashTaskDataEntry155);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyFlashTaskDataEntry156);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyFlashTaskDataEntry10);

INCLUDE_SDATA(const s32, "code_r1_00242308", effUpdateCameraSnapshotGuidesDataEntry2);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData51);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyFlashTaskDataEntry157);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyFlashTaskDataEntry25);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyFlashTaskDataEntry158);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyFlashTaskDataEntry159);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyFlashTaskDataEntry160);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyFlashTaskDataEntry161);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyFlashTaskDataEntry162);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyFlashTaskDataEntry163);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyFlashTaskDataEntry164);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyFlashTaskDataEntry165);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyFlashTaskDataEntry11);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyFlashTaskDataEntry166);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyFlashTaskDataEntry167);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreateEnvironmentTaskDataEntry8);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreateEnvironmentTaskDataEntry19);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry49);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry50);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry51);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry52);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyRingTaskDataEntry33);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTwinkleTaskDataEntry89);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyRingTaskDataEntry34);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyRingTaskDataEntry10);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyRingTaskDataEntry4);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTwinkleTaskDataEntry90);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyThunderTaskDataEntry31);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyRingTaskDataEntry35);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyThunderTaskDataEntry4);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyScalyTaskDataEntry61);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyThunderTaskDataEntry32);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyThunderTaskDataEntry33);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyThunderTaskDataEntry8);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyThunderTaskDataEntry34);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTwinkleTaskDataEntry7);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTwinkleTaskDataEntry91);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTwinkleTaskDataEntry92);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTwinkleTaskDataEntry8);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTwinkleTaskDataEntry9);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTwinkleTaskDataEntry10);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyTwinkleTaskDataEntry11);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPOLYWINDDataEntry6);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyScalyTaskDataEntry62);

INCLUDE_SDATA(const s32, "code_r1_00242308", sndInitDataEntry2);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyScalyTaskDataEntry63);

INCLUDE_SDATA(const s32, "code_r1_00242308", sndInitDataEntry3);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyScalyTaskDataEntry64);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPOLYWINDDataEntry88);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPOLYWINDDataEntry89);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPOLYWINDDataEntry90);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPOLYWINDDataEntry91);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPOLYWINDDataEntry92);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPOLYWINDDataEntry93);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPOLYWINDDataEntry94);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPOLYWINDDataEntry95);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPOLYWINDDataEntry7);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPOLYWINDDataEntry96);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysPOLYWINDDataEntry97);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyScalyTaskDataEntry5);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyScalyTaskDataEntry65);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyScalyTaskDataEntry66);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyScalyTaskDataEntry67);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyScalyTaskDataEntry6);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyScalyTaskDataEntry68);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyScalyTaskDataEntry69);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyScalyTaskDataEntry70);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyScalyTaskDataEntry71);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyScalyTaskDataEntry72);

INCLUDE_SDATA(const s32, "code_r1_00242308", fileLoadEffectSlotBDataEntry);

INCLUDE_SDATA(const s32, "code_r1_00242308", fileLoadEffectSlotBDataEntry2);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyCrackTaskDataEntry15);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyCrackTaskDataEntry16);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyCrackTaskDataEntry17);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyCrackTaskDataEntry3);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyCrackTaskDataEntry4);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyWindTaskDataEntry7);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyWindTaskDataEntry113);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyWindTaskDataEntry114);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyWindTaskDataEntry115);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyWindTaskDataEntry116);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyWindTaskDataEntry117);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyWindTaskDataEntry118);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyWindTaskDataEntry119);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyWindTaskDataEntry120);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyWindTaskDataEntry121);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyWindTaskDataEntry20);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyWindTaskDataEntry122);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyWindTaskDataEntry8);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyWindTaskDataEntry9);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyWindTaskDataEntry10);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreatePolyWindTaskDataEntry11);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry22);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry53);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry54);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry55);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry56);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry57);

INCLUDE_SDATA(const s32, "code_r1_00242308", effInitializeResourceQueueData);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry58);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry23);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry24);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData9Entry23);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData9Entry24);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData9Entry25);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry25);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry59);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry60);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry61);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry26);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry62);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry27);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry63);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry28);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry64);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry65);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry29);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry66);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry30);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry67);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry68);

INCLUDE_SDATA(const s32, "code_r1_00242308", effLoadMaterialFileDataEntry);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData9Entry26);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData9Entry27);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreateTwoDimensionalTaskData2);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry69);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry70);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry71);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry72);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry73);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry74);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry75);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysBATTLEONLYDataEntry76);

INCLUDE_SDATA(const s32, "code_r1_00242308", effResetFileQueueStateDataEntry2);

INCLUDE_SDATA(const s32, "code_r1_00242308", effResetFileQueueStateDataEntry3);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData10Entry);

INCLUDE_SDATA(const s32, "code_r1_00242308", effUpdatePartsListEditorData3Entry2);

INCLUDE_SDATA(const s32, "code_r1_00242308", effUpdatePartsListEditorData4Entry2);

INCLUDE_SDATA(const s32, "code_r1_00242308", effUpdatePartsListEditorData4Entry3);

INCLUDE_SDATA(const s32, "code_r1_00242308", effUpdatePartsListEditorData3Entry3);

INCLUDE_SDATA(const s32, "code_r1_00242308", effUpdatePartsListEditorData5Entry2);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCreateFilterTaskData2);

INCLUDE_SDATA(const s32, "code_r1_00242308", effPartsListScroll);

INCLUDE_SDATA(const s32, "code_r1_00242308", fldTitleMiniTex4);

INCLUDE_SDATA(const s32, "code_r1_00242308", effFileQueue);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData38);

INCLUDE_SDATA(const s32, "code_r1_00242308", effQueuedFileHandle);

INCLUDE_SDATA(const s32, "code_r1_00242308", fldTitleMiniWork3);

INCLUDE_SDATA(const s32, "code_r1_00242308", effCurrentFileQueueEntry);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData39);

INCLUDE_SDATA(const s32, "code_r1_00242308", effPollPrimaryFileData);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData40);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData41);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData42);

INCLUDE_SDATA(const s32, "code_r1_00242308", effEditorCurrentMenu);

INCLUDE_SDATA(const s32, "code_r1_00242308", effQueuedFileObject);

INCLUDE_SDATA(const s32, "code_r1_00242308", effUpdatePartsListEditorData14);

INCLUDE_SDATA(const s32, "code_r1_00242308", effUpdatePartsListEditorData15);

INCLUDE_SDATA(const s32, "code_r1_00242308", effUpdatePartsListEditorData16);

INCLUDE_SDATA(const s32, "code_r1_00242308", effUpdatePartsListEditorData17);

INCLUDE_SDATA(const s32, "code_r1_00242308", effUpdatePartsListEditorData18);

INCLUDE_SDATA(const s32, "code_r1_00242308", effUpdatePartsListEditorData19);

INCLUDE_SDATA(const s32, "code_r1_00242308", effUpdatePartsListEditorData20);

INCLUDE_SDATA(const s32, "code_r1_00242308", effUpdatePartsListEditorData21);

INCLUDE_SDATA(const s32, "code_r1_00242308", effUpdatePartsListEditorData22);

INCLUDE_SDATA(const s32, "code_r1_00242308", effSelectCameraFileJobFromRowsData2);

INCLUDE_SDATA(const s32, "code_r1_00242308", effUpdateCameraDiagnosticsDataEntry2);

INCLUDE_SDATA(const s32, "code_r1_00242308", effUpdateCameraSnapshotGuidesDataEntry3);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysUPDOWNHEIGHTData);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysUPDOWNHEIGHTData2);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysUPDOWNHEIGHTData3);

INCLUDE_SDATA(const s32, "code_r1_00242308", sysUPDOWNHEIGHTData4);

INCLUDE_SDATA(const s32, "code_r1_00242308", fldTitleMiniWork);

INCLUDE_SDATA(const s32, "code_r1_00242308", fldTitleMiniTex);

INCLUDE_SDATA(const s32, "code_r1_00242308", effFileQueueNameRecord);

INCLUDE_SDATA(const s32, "code_r1_00242308", effQueuedResourceNameRecord);

INCLUDE_SDATA(const s32, "code_r1_00242308", effColorEditRgbMenuOpen);

INCLUDE_SDATA(const s32, "code_r1_00242308", effColorEditSelectedKey);

INCLUDE_SDATA(const s32, "code_r1_00242308", effColorEditSelectedAlphaKey);

INCLUDE_SDATA(const s32, "code_r1_00242308", effColorEditPage);

INCLUDE_SDATA(const s32, "code_r1_00242308", effColorEditKeyColor);

INCLUDE_SDATA(const s32, "code_r1_00242308", effEditColorTrackDataEntry2);

INCLUDE_SDATA(const s32, "code_r1_00242308", effEditColorTrackDataEntry3);

INCLUDE_SDATA(const s32, "code_r1_00242308", effEditColorTrackDataEntry4);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData15Entry);

INCLUDE_SDATA(const s32, "code_r1_00242308", effColorEditFrameCountFormat);

INCLUDE_SDATA(const s32, "code_r1_00242308", effEditScalarTrackData);

INCLUDE_SDATA(const s32, "code_r1_00242308", effEditScalarTrackData2);

INCLUDE_SDATA(const s32, "code_r1_00242308", effEditScalarTrackData3);

INCLUDE_SDATA(const s32, "code_r1_00242308", effEditScalarTrackData4);

INCLUDE_SDATA(const s32, "code_r1_00242308", effEditScalarTrackData5);

INCLUDE_SDATA(const s32, "code_r1_00242308", effEditScalarTrackData6);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData20Entry3);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData18Entry2);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData20Entry4);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData19Entry2);

INCLUDE_SDATA(const s32, "code_r1_00242308", effData20Entry5);

INCLUDE_SDATA(const s32, "code_r1_00242308", effEditScalarTrackData7);

INCLUDE_SDATA(const s32, "code_r1_00242308", wk_sbtl);

