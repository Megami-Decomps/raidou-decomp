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
void effComputeBattleCameraPositionVU(effPartsListPose_t *effect);
void effMultiplyQuatWithFlag(effPartsListPose_t *effect);
extern u8 effSelectedJobHeader[];
void effApplyBattleCameraToObject();
u32 effCreateBattleCameraJob(u8 *request);
FileJobPayload *effLoadFileJobPayload(EffFileJobRequest *request, const char *path);
void effInvokeFileJobWithBattleCamera(u32 job);
void effFileJobQueueRelease(u32 job);
extern char D_003BD080[];
extern const char D_003BD078[];

/* Payload option (+0x0C) selects the asset identifier at +0x08. */
u32 effPollPrimaryFile(void);
void effQueueNamedResourceRequest(void);
u32 effPollNamedFile(void);
void effQueueAttachedResourceRequest(void);
u32 effPollAttachedFile(void);

u8 *effFindAssetData(FileJob *entry);
effDebugMenu_t *effFindAssetObject(FileJob *entry);
u32 effGetSelectedFileQueueIndex(void);
extern EffectBlock128 effEditedQueueHeader;
extern u32 btlDebugMenuSetupNoOp(void *context);
extern void btlDebugMenuBeforeRecordNoOp(void);
extern void btlDebugMenuAfterRecordNoOp(void);

/* A row of an effect kind's sample list; a list ends with a NULL resource. */
typedef struct EffCameraCreateRow {
    EffCameraCreateRequest request;
    u32 sourceTail;
} EffCameraCreateRow;

typedef char EffCameraCreateRowSizeCheck[(sizeof(EffCameraCreateRow) == 0x1C) ? 1 : -1];

extern EffCameraCreateRow sysPARTICLEData[];
extern EffCameraCreateRow effCreatePolyTrackControlTaskData[];
extern EffCameraCreateRow effCreatePolyTrackTaskData[];
extern EffCameraCreateRow effCreateBlurTaskData[];
extern EffCameraCreateRow effCreateFilterTaskData[];
extern EffCameraCreateRow sysPOLYTEXData[];
extern EffCameraCreateRow effCreateEnvironmentTaskData[];
extern EffCameraCreateRow effCreatePolyFlashTaskData[];
extern EffCameraCreateRow effCreatePolyRingTaskData[];
extern EffCameraCreateRow effCreatePolyThunderTaskData[];
extern EffCameraCreateRow effCreatePolyTwinkleTaskData[];
extern EffCameraCreateRow sysPOLYWINDData[];
extern EffCameraCreateRow effCreatePolyScalyTaskData[];
extern EffCameraCreateRow effCreatePolyCrackTaskData[];
extern EffCameraCreateRow sysBATTLEONLYData[];
extern EffCameraCreateRow effCreateTwoDimensionalTaskData[];
extern EffCameraCreateRow effCreatePolyWindTaskData[];

extern void fileQueueInitTransform(void *);
extern FileJob *fileAppendJob(FileQueue *, u32);
extern s32 fileQueueCountLinkedJobs(FileQueue *);
extern FileJob *effCurrentFileQueueEntry;
extern u32 D_003BD064;
extern effDebugMenu_t effPartsListMenu;
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

extern effDebugMenu_t effBattleCameraSnapshotSelection;
extern effDebugMenuItem_t effData10[];
extern effDebugMenuItem_t effData11[];
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
    effPartsListPose_t pose; /* pos is copied to the debug camera position and follow offset */
    u8 pad70[0x10];
} __attribute__((aligned(16))) EffectStateSnapshot;

typedef char EffectStateSnapshotSizeCheck[(sizeof(EffectStateSnapshot) == 0x80) ? 1 : -1];

extern s32 effUpdateCameraDiagnostics(EffectStateSnapshot *, s8);
extern u8 effUpdateCameraDiagnosticsData[];
extern s8 effData22[];
extern u32 btlDebugMenuSetupNoOp(void *camera);
s32 effUpdateCameraDiagnostics(EffectStateSnapshot *snapshot, s8 mode);

/* The camera snapshot payload is the 0x90-byte record consumed by this updater. */
typedef union EffectCameraSnapshotView {
    u32 word[36];
    struct {
        effPartsListPose_t pose;
        u8 pad70[0x20];
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
extern effDebugMenu_t effMappingState;
void effResetMappingFlags(void);
extern effDebugMenuItem_t D_0038F7D8[];
extern effDebugMenuItem_t effData14[];
extern effDebugMenuItem_t D_0038F658[];

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
void effEditMColorTrack(BaObj *p);
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

extern EffFileJobRequest effLoadMaterialFileData;
extern EffFileJobRequest fileLoadEffectSlotAData;
extern EffFileJobRequest fileLoadEffectSlotBData;
u32 fileLoadEffectSlotA(void);
extern effHelper_Params_t fileLoadEffectSlotHelpData;
u32 fileLoadEffectSlotHelp(void);
u32 fileLoadEffectSlotB(void);
extern EffFileJobRequest effLoadFileSlotF2Data;
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

void effComputeBattleCameraPositionVU(effPartsListPose_t *effect) {
    u128 direction;
    u32 flags = effect->flag;

    if ((flags & 0x18) != 0) {
        camFollowOffsetVec(effect, &direction);
        VU0_LOAD_VF($vf10, &direction);
        flags = effect->flag;
    } else {
        VU0_LOAD_VF($vf10, effect->pos);
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

void effMultiplyQuatWithFlag(effPartsListPose_t *effect) {
    if ((effect->flag & 0x60) != 0) {
        u128 quaternion;
        PoseRotBillboard(effect, &quaternion);
        VU0_LOAD_VF($vf10, effFileQueue->pose.rot);
        VU0_LOAD_VF($vf11, &quaternion);
        Mult();
    } else {
        VU0_LOAD_VF($vf10, effFileQueue->pose.rot);
        VU0_LOAD_VF($vf11, effect->rot);
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

FileJobPayload *effLoadFileJobPayload(EffFileJobRequest *request, const char *path) {
    FileJobPayload *job;
    if (path != NULL) {
        void *source;
        job = fileJobCreateFromCommandState(path);
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

extern EffCameraCreateRow *effData9[24];

u8 * effFindAssetData(FileJob *entry) {
    FileJobPayload *requested = (FileJobPayload *)entry->id;
    u16 type = requested->type;
    u16 option = requested->option;
    u16 i;
    for (i = 0; i < 24; i++) {
        EffCameraCreateRow *links = effData9[i];
        if (links != NULL) {
            EffCameraCreateRow *current = links;
            if (current->request.resource != NULL) {
                do {
                    EffFileJobRequest *asset = current->request.resource;
                    if (asset->fileKind == type && asset->transferMode == option) {
                        return (u8 *)asset;
                    }
                    current++;
                } while (current->request.resource != NULL);
            }
        }
    }
    return NULL;
}

effDebugMenu_t * effFindAssetObject(FileJob *entry) {
    FileJobPayload *requested = (FileJobPayload *)entry->id;
    u16 type = requested->type;
    u16 option = requested->option;
    u16 i;
    for (i = 0; i < 24; i++) {
        EffCameraCreateRow *links = effData9[i];
        if (links != NULL) {
            EffCameraCreateRow *current = links;
            if (current->request.resource != NULL) {
                do {
                    EffFileJobRequest *asset = current->request.resource;
                    if (asset->fileKind == type && asset->transferMode == option) {
                        return current->request.object;
                    }
                    current++;
                } while (current->request.resource != NULL);
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
    entry = fileAppendJob(effFileQueue, (u32)effLoadFileJobPayload(request->resource, request->path));
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
        effPartsListMenu.selected = 20;
        effPartsListScroll = count - 21;
    } else {
        effPartsListScroll = 0;
        effPartsListMenu.selected = count - 1;
    }
    request->object->parent = &effPartsListMenu;
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
    ((effDebugMenu_t *)effQueuedFileObject)->parent = &effPartsListMenu;
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
    effPartsListMenu.selected = 0;
    effQueuedFileObject = &effPartsListMenu;
    return 0;
}

u32 effResetFileQueueState(void) {
    fileQueueRemoveAndDestroyJob(effFileQueue, fileQueueGetAt(effFileQueue, effGetSelectedFileQueueIndex()));
    effResetFileQueueStateData[0] = 0;
    effPartsListScroll = 0;
    effPartsListMenu.selected = 0;
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
    ((effDebugMenu_t *)effQueuedFileObject)->parent = &effPartsListMenu;

    linkedCount = fileQueueCountLinkedJobs(effFileQueue);
    if ((u32)linkedCount >= 22) {
        effPartsListMenu.selected = 20;
        effPartsListScroll = linkedCount - 21;
    } else {
        effPartsListScroll = 0;
        effPartsListMenu.selected = linkedCount - 1;
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
    if (((effDebugMenu_t *)effEditorCurrentMenu)->parent != NULL) {
        effQueuedFileObject = ((effDebugMenu_t *)effEditorCurrentMenu)->parent;
    }
    return 0;
}

u32 effLoadBattleCameraSnapshot(u32 value) {
    FileJob *record = fileQueueGetAt(effFileQueue, effGetSelectedFileQueueIndex());

    *(EffectBlock90 *)D_003DF840 = *(EffectBlock90 *)record;
    *(EffectAlignedBlock128 *)D_003DF840 = *(EffectAlignedBlock128 *)effSelectedJobHeader;
    if (((EffectBlock90 *)D_003DF840)->word[26] & 2) {
        effBattleCameraSnapshotSelection.items = effData10;
        effBattleCameraSnapshotSelection.itemCount = 3;
        if (effBattleCameraSnapshotSelection.selected > effBattleCameraSnapshotSelection.itemCount) {
            effBattleCameraSnapshotSelection.selected = 0;
        }
    } else {
        effBattleCameraSnapshotSelection.items = effData11;
        effBattleCameraSnapshotSelection.itemCount = 9;
        if (effBattleCameraSnapshotSelection.selected == 0) {
            effBattleCameraSnapshotSelection.selected = 1;
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
        effBattleCameraSnapshotSelection.items = effData10;
        effBattleCameraSnapshotSelection.itemCount = 3;
        if (effBattleCameraSnapshotSelection.selected > effBattleCameraSnapshotSelection.itemCount) {
            effBattleCameraSnapshotSelection.selected = 0;
        }
    } else {
        effBattleCameraSnapshotSelection.items = effData11;
        effBattleCameraSnapshotSelection.itemCount = 9;
        if (effBattleCameraSnapshotSelection.selected == 0) {
            effBattleCameraSnapshotSelection.selected = 1;
        }
    }
    return value;
}

INCLUDE_ASM(const s32, "code_r1_00242308", effSelectCameraFileJobFromRows);

extern char sysStrParticle[];
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

    SDF_VU0_COPYVECTOR(D_003DFA20, snapshot->pose.pos);
    btlDebugMenuSetupNoOp(camera);
    btlDebugMenuBeforeRecordNoOp();
    btlDebugMenuRecordNoOp(camera);
    btlDebugMenuAfterRecordNoOp();
    SDF_VU0_COPYVECTOR(snapshot->pose.pos, D_003DFA20);

    if ((*(u32 *)(camera + 0x10) & 8) != 0) {
        result |= 2;
    }
    if ((result & 2) != 0 && (snapshot->pose.flag & 0x18) != 0) {
        camFollowOffsetVec(&snapshot->pose, vectors[0]);
        SDF_VU0_COPYVECTOR(snapshot->pose.pos, vectors[0]);
    }

    if (mode != 0) {
        effComputeBattleCameraPositionVU(&snapshot->pose);
        SDF_VU0_LOADVECTOR_B(&effEditedQueueHeader);
        SDF_VU0_SUBVECTOR();
    } else {
        SDF_VU0_LOADVECTOR(snapshot->pose.pos);
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

    SDF_VU0_COPYVECTOR(effWork9, state->camera.pose.rot);
    if (useBattlePosition != 0) {
        effComputeBattleCameraPositionVU(&state->camera.pose);
    } else {
        SDF_VU0_LOADVECTOR(state->camera.pose.pos);
    }
    SDF_VU0_STOREVECTOR_NOMEM(&scratch.basePosition);
    SDF_VU0_STOREVECTOR(D_003DFA20);

    btlDebugMenuSetupNoOp(&effUpdateCameraSnapshotGuidesData);
    btlDebugMenuBeforeRecordNoOp();
    btlDebugMenuRecordNoOp(&effUpdateCameraSnapshotGuidesData);
    btlDebugMenuAfterRecordNoOp();
    SDF_VU0_COPYVECTOR(state->camera.pose.rot, effWork9);
    if (effUpdateCameraSnapshotGuidesData.flags & 8) {
        result |= 2;
    }
    SDF_VU0_LOADVECTOR(&scratch.basePosition);
    SDF_VU0_LOADVECTOR_B(effBattleOffset);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&scratch.segmentStart);

    SDF_VU0_LOADVECTOR(state->camera.pose.rot);
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
    f32 *backup = snapshot.pose.pos;
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
    effMappingState.selected = 1;
    effMappingState.flags |= 0x10;
}

INCLUDE_ASM(const s32, "code_r1_00242308", effEditColorTrack);

s32 effMapObjectWithTemporaryMappingTable(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.items = D_0038F7D8;
    effMappingState.itemCount = 8;
    result = effEditColorTrack(object + 0x2c, object + 0x50, ((EffMappingObject *)object)->valueB8);
    effMappingState.items = D_0038F658;
    effMappingState.itemCount = 8;
    return result;
}

/* Edit the request's G-/K-family COLOR tracks; return the native editor status. */
s32 effEditGkColorTrack(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.items = D_0038F7D8;
    effMappingState.itemCount = 8;
    result = effEditColorTrack(object + 0x48, object + 0x6c, ((EffMappingObject *)object)->valueD4);
    effMappingState.items = D_0038F658;
    effMappingState.itemCount = 8;
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

    effMappingState.items = D_0038F7D8;
    effMappingState.itemCount = 8;
    result = effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value8C);
    effMappingState.items = D_0038F658;
    effMappingState.itemCount = 8;
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

    effMappingState.items = effData14;
    effMappingState.itemCount = 8;
    result = effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value34);
    effMappingState.items = D_0038F658;
    effMappingState.itemCount = 8;
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

    effMappingState.items = D_0038F7D8;
    effMappingState.itemCount = 8;
    result = effEditColorTrack(object + 0x4c, object + 0x70, ((EffMappingObject *)object)->valueD8);
    effMappingState.items = D_0038F658;
    effMappingState.itemCount = 8;
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

    effMappingState.items = D_0038F7D8;
    effMappingState.itemCount = 8;
    result = effEditColorTrack(object + 0x68, object + 0x8c, ((EffMappingObject *)object)->valueF4);
    effMappingState.items = D_0038F658;
    effMappingState.itemCount = 8;
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

    effMappingState.items = effData14;
    effMappingState.itemCount = 8;
    result = effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value8C);
    effMappingState.items = D_0038F658;
    effMappingState.itemCount = 8;
    return result;
}

extern effDebugMenuItem_t effData15[];
extern effDebugMenuItem_t effData13[];

/* FUNC effEditMColorTrack */
#ifdef NON_MATCHING
/* 7 of 25 words differ, first at +0x20: retail loads the rows address before p->x0C and stores count and rows in a different order. */
void effEditMColorTrack(BaObj *p) {
    s32 object;

    effMappingState.items = effData15;
    effMappingState.itemCount = 8;
    object = p->x0C;
    effEditColorTrack(object + 0x7C, object + 0xA0, ((EffMappingObject *)object)->value108);
    effMappingState.items = effData13;
    effMappingState.itemCount = 8;
}
#else
INCLUDE_ASM(const s32, "code_r1_00242308", effEditMColorTrack);
#endif

extern effDebugMenu_t effData18;
extern effDebugMenu_t effData20;
extern u32 effEditScalarTrackData;
extern u32 effEditScalarTrackData2;
extern u32 effEditScalarTrackData3;

void btlResetEffectWork(void) {
    effData18.flags |= 0x10;
    effData18.selected = 0;
    effData20.flags |= 0x10;
    effData20.selected = 0;
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
        fileJobSetPrimaryData(job, fileLoadEffectSlotAData.outputBuffer, fileLoadEffectSlotAData.size,
                      fileLoadEffectSlotAData.transferMode);
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
        ((effDebugMenu_t *)effQueuedFileObject)->parent = &effPartsListMenu;
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
        memcpy(effWork6, &fileLoadEffectSlotHelpData, sizeof(effHelper_Params_t));
        D_003BD064 = effCreateBattleCameraJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((effDebugMenu_t *)effQueuedFileObject)->parent = &effPartsListMenu;
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
        fileJobSetPrimaryData(job, fileLoadEffectSlotBData.outputBuffer, fileLoadEffectSlotBData.size,
                      fileLoadEffectSlotBData.transferMode);
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
        ((effDebugMenu_t *)effQueuedFileObject)->parent = &effPartsListMenu;
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
        fileJobSetPrimaryData(job, effLoadFileSlotF2Data.outputBuffer, effLoadFileSlotF2Data.size,
                      effLoadFileSlotF2Data.transferMode);
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
        ((effDebugMenu_t *)effQueuedFileObject)->parent = &effPartsListMenu;
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
        fileJobSetPrimaryData(job, effLoadMaterialFileData.outputBuffer, effLoadMaterialFileData.size,
                      effLoadMaterialFileData.transferMode);
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
        ((effDebugMenu_t *)effQueuedFileObject)->parent = &effPartsListMenu;
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

#ifdef VERSION_RAIDOU1
struct EffCopiedPayload;

void sbtlCallBtlSetLightMap();
void sbtlLeaf49(void);
void sbtlCallSetUnitColorRGB();
void sbtlCallSetUnitColorRGB2();
void effResetDefaultColorTables();
void sbtlLeaf50();
void effLeaf376(void);
u32 effLeaf377();
void effDestroyCopiedEffectPayload(struct EffCopiedPayload *payload);
u32 effLeaf378(void *);
void effLeaf379(void);
void effLeaf380(void);
void effLeaf381(void);
void effLeaf382(void);
void effLeaf383(void);
void effCallPadVibeFadeIN();
void sbtlCallBtlEffCreateUnitList();
void sbtlLeaf51();
void sbtlCallBtlSetLightMap2();
void sbtlCallBtlEffCreateUnitList2();
void effCallSetUnitColorRGB();
void effCallBtlEffCreateUnitList();

extern sdfGrOt_t dds3Data7;
extern sdfGrOt_t sdfDrawFlagListVerticesDataEntry;
extern sdfGrOt_t effData2;
extern sdfGrOt_t dds3Data8;

EffResourceOps effRuntimeResourceOperations[14] = {
    { NULL, NULL, NULL, NULL, NULL, NULL, 0x0 },
    { NULL, NULL, sbtlCallBtlSetLightMap, NULL, sbtlLeaf49, NULL, 0x28 },
    { NULL, NULL, sbtlCallSetUnitColorRGB, NULL, sbtlCallSetUnitColorRGB2, NULL, 0x14 },
    { NULL, NULL, effResetDefaultColorTables, NULL, sbtlLeaf50, NULL, 0x10 },
    { effLeaf376, effLeaf377, effDestroyCopiedEffectPayload, effLeaf378, effLeaf379, NULL, 0xC },
    { NULL, NULL, NULL, NULL, effLeaf380, NULL, 0x8 },
    { NULL, NULL, NULL, NULL, effLeaf381, NULL, 0x4 },
    { NULL, NULL, effLeaf382, NULL, effLeaf383, NULL, 0xC },
    { NULL, NULL, effLeaf384, NULL, effCallPadVibeFadeIN, NULL, 0x1C },
    { NULL, NULL, effLeaf385, NULL, effLeaf386, NULL, 0x10 },
    { NULL, NULL, NULL, NULL, effLeaf387, NULL, 0xC },
    { NULL, NULL, NULL, NULL, sbtlCallBtlEffCreateUnitList, NULL, 0xC },
    { sbtlLeaf51, NULL, sbtlCallBtlSetLightMap2, NULL, sbtlCallBtlEffCreateUnitList2, NULL, 0x20 },
    { NULL, NULL, effCallSetUnitColorRGB, NULL, effCallBtlEffCreateUnitList, NULL, 0x14 },
};

mapLightParam_t sbtlCallBtlEffCreateUnitList2Data __attribute__((aligned(16))) = { { 0.0f } };

sdfGrOt_t *sdfDrawFlagListVerticesData[8] = {
    &dds3Data7, &dds3Data7, &effData2, &dds3Data8, &sdfDrawFlagListVerticesDataEntry, NULL, NULL, NULL,
};

/* Effect editor pages for the effect kinds; see effDebugMenu_t. */
s32 effEditMScaleTrack(BaObj *p);
extern f32 bssWork66;
extern f32 bssWork67;
extern u8 bssWork68;
extern u32 bssWork72;
extern u32 bssWork73;
extern f32 bssWork74;
extern f32 bssWork75;
extern f32 bssWork76;
extern f32 bssWork77;
extern f32 bssWork78;
extern s32 bssWork79;
extern char effCreateBlurTaskDataEntry18[];
extern char effCreateBlurTaskDataEntry19[];
extern char effCreateBlurTaskDataEntry20[];
extern char effCreateBlurTaskDataEntry21[];
extern char effCreateBlurTaskDataEntry22[];
extern u8 effCreateBlurTaskDataEntry23[];
extern s32 effCreateBlurTaskDataEntry24;
extern s32 effCreateBlurTaskDataEntry25;
extern s32 effCreateBlurTaskDataEntry26;
extern s32 effCreateBlurTaskDataEntry27;
extern s32 effCreateBlurTaskDataEntry28;
extern s32 effCreateBlurTaskDataEntry29;
extern s32 effCreateBlurTaskDataEntry30;
extern f32 effCreateBlurTaskDataEntry31;
extern s32 effCreateBlurTaskDataEntry32;
extern u8 effCreateBlurTaskDataEntry33[];
extern s32 effCreateBlurTaskDataEntry34;
extern s32 effCreateBlurTaskDataEntry35;
extern s32 effCreateBlurTaskDataEntry36;
extern f32 effCreateBlurTaskDataEntry37;
extern f32 effCreateBlurTaskDataEntry38;
extern f32 effCreateBlurTaskDataEntry39;
extern u8 effCreateBlurTaskDataEntry40[];
extern s32 effCreateBlurTaskDataEntry41;
extern char effCreateBlurTaskDataEntry49[];
extern u8 effCreateBlurTaskDataEntry50[];
extern char effCreateBlurTaskDataEntry7[];
extern char effCreateBlurTaskDataEntry8[];
extern u8 effCreateBlurTaskDataEntry9[];
extern char effCreateEnvironmentTaskDataEntry17[];
extern char effCreateEnvironmentTaskDataEntry18[];
extern char effCreateEnvironmentTaskDataEntry19[];
extern s32 effCreateEnvironmentTaskDataEntry20;
extern s32 effCreateEnvironmentTaskDataEntry21;
extern u8 effCreateEnvironmentTaskDataEntry22[];
extern s32 effCreateEnvironmentTaskDataEntry23;
extern s32 effCreateEnvironmentTaskDataEntry24;
extern f32 effCreateEnvironmentTaskDataEntry25;
extern f32 effCreateEnvironmentTaskDataEntry26;
extern s32 effCreateEnvironmentTaskDataEntry27;
extern u8 effCreateEnvironmentTaskDataEntry28;
extern s32 effCreateEnvironmentTaskDataEntry29;
extern s32 effCreateEnvironmentTaskDataEntry30;
extern s32 effCreateEnvironmentTaskDataEntry31;
extern s32 effCreateEnvironmentTaskDataEntry32;
extern char effCreateEnvironmentTaskDataEntry6[];
extern char effCreateEnvironmentTaskDataEntry7[];
extern char effCreateEnvironmentTaskDataEntry8[];
extern u8 effCreateEnvironmentTaskDataEntry9[];
extern s32 effCreateFilterTaskDataEntry10;
extern u8 effCreateFilterTaskDataEntry11[];
extern s32 effCreateFilterTaskDataEntry12;
extern s32 effCreateFilterTaskDataEntry13;
extern char effCreateFilterTaskDataEntry3[];
extern char effCreateFilterTaskDataEntry4[];
extern u8 effCreateFilterTaskDataEntry9[];
extern char effCreatePolyCrackTaskDataEntry14[];
extern char effCreatePolyCrackTaskDataEntry15[];
extern char effCreatePolyCrackTaskDataEntry16[];
extern char effCreatePolyCrackTaskDataEntry17[];
extern u8 effCreatePolyCrackTaskDataEntry18[];
extern s32 effCreatePolyCrackTaskDataEntry19;
extern u8 effCreatePolyCrackTaskDataEntry20;
extern u32 effCreatePolyCrackTaskDataEntry21;
extern u32 effCreatePolyCrackTaskDataEntry22;
extern f32 effCreatePolyCrackTaskDataEntry23;
extern s32 effCreatePolyCrackTaskDataEntry24;
extern u8 effCreatePolyCrackTaskDataEntry25;
extern u8 effCreatePolyCrackTaskDataEntry26;
extern s32 effCreatePolyCrackTaskDataEntry27;
extern s32 effCreatePolyCrackTaskDataEntry28;
extern f32 effCreatePolyCrackTaskDataEntry29;
extern f32 effCreatePolyCrackTaskDataEntry30;
extern char effCreatePolyCrackTaskDataEntry3[];
extern char effCreatePolyCrackTaskDataEntry4[];
extern f32 effCreatePolyCrackTaskDataEntry5;
extern u8 effCreatePolyCrackTaskDataEntry6;
extern f32 effCreatePolyFlashTaskDataEntry100;
extern f32 effCreatePolyFlashTaskDataEntry101;
extern s32 effCreatePolyFlashTaskDataEntry102;
extern char effCreatePolyFlashTaskDataEntry10[];
extern char effCreatePolyFlashTaskDataEntry11[];
extern char effCreatePolyFlashTaskDataEntry151[];
extern char effCreatePolyFlashTaskDataEntry152[];
extern char effCreatePolyFlashTaskDataEntry153[];
extern char effCreatePolyFlashTaskDataEntry154[];
extern char effCreatePolyFlashTaskDataEntry155[];
extern char effCreatePolyFlashTaskDataEntry156[];
extern char effCreatePolyFlashTaskDataEntry157[];
extern char effCreatePolyFlashTaskDataEntry158[];
extern char effCreatePolyFlashTaskDataEntry159[];
extern char effCreatePolyFlashTaskDataEntry160[];
extern char effCreatePolyFlashTaskDataEntry161[];
extern char effCreatePolyFlashTaskDataEntry162[];
extern char effCreatePolyFlashTaskDataEntry163[];
extern char effCreatePolyFlashTaskDataEntry164[];
extern char effCreatePolyFlashTaskDataEntry165[];
extern char effCreatePolyFlashTaskDataEntry166[];
extern char effCreatePolyFlashTaskDataEntry167[];
extern u8 effCreatePolyFlashTaskDataEntry168;
extern u32 effCreatePolyFlashTaskDataEntry169;
extern u32 effCreatePolyFlashTaskDataEntry170;
extern f32 effCreatePolyFlashTaskDataEntry171;
extern f32 effCreatePolyFlashTaskDataEntry172;
extern f32 effCreatePolyFlashTaskDataEntry173;
extern f32 effCreatePolyFlashTaskDataEntry174;
extern f32 effCreatePolyFlashTaskDataEntry175;
extern u8 effCreatePolyFlashTaskDataEntry176[];
extern u8 effCreatePolyFlashTaskDataEntry177;
extern u32 effCreatePolyFlashTaskDataEntry178;
extern u32 effCreatePolyFlashTaskDataEntry179;
extern f32 effCreatePolyFlashTaskDataEntry180;
extern f32 effCreatePolyFlashTaskDataEntry181;
extern f32 effCreatePolyFlashTaskDataEntry182;
extern f32 effCreatePolyFlashTaskDataEntry183;
extern f32 effCreatePolyFlashTaskDataEntry184;
extern f32 effCreatePolyFlashTaskDataEntry185;
extern u8 effCreatePolyFlashTaskDataEntry186[];
extern u8 effCreatePolyFlashTaskDataEntry187;
extern u32 effCreatePolyFlashTaskDataEntry188;
extern u32 effCreatePolyFlashTaskDataEntry189;
extern f32 effCreatePolyFlashTaskDataEntry190;
extern f32 effCreatePolyFlashTaskDataEntry191;
extern f32 effCreatePolyFlashTaskDataEntry192;
extern f32 effCreatePolyFlashTaskDataEntry193;
extern f32 effCreatePolyFlashTaskDataEntry194;
extern f32 effCreatePolyFlashTaskDataEntry195;
extern f32 effCreatePolyFlashTaskDataEntry196;
extern f32 effCreatePolyFlashTaskDataEntry197;
extern f32 effCreatePolyFlashTaskDataEntry198;
extern f32 effCreatePolyFlashTaskDataEntry199;
extern f32 effCreatePolyFlashTaskDataEntry200;
extern u8 effCreatePolyFlashTaskDataEntry201[];
extern u8 effCreatePolyFlashTaskDataEntry202;
extern u32 effCreatePolyFlashTaskDataEntry203;
extern u32 effCreatePolyFlashTaskDataEntry204;
extern f32 effCreatePolyFlashTaskDataEntry205;
extern f32 effCreatePolyFlashTaskDataEntry206;
extern f32 effCreatePolyFlashTaskDataEntry207;
extern f32 effCreatePolyFlashTaskDataEntry208;
extern s32 effCreatePolyFlashTaskDataEntry209;
extern u8 effCreatePolyFlashTaskDataEntry210[];
extern u8 effCreatePolyFlashTaskDataEntry211;
extern u32 effCreatePolyFlashTaskDataEntry212;
extern u32 effCreatePolyFlashTaskDataEntry213;
extern f32 effCreatePolyFlashTaskDataEntry214;
extern f32 effCreatePolyFlashTaskDataEntry215;
extern f32 effCreatePolyFlashTaskDataEntry216;
extern f32 effCreatePolyFlashTaskDataEntry217;
extern f32 effCreatePolyFlashTaskDataEntry218;
extern f32 effCreatePolyFlashTaskDataEntry219;
extern u8 effCreatePolyFlashTaskDataEntry220;
extern u32 effCreatePolyFlashTaskDataEntry221;
extern u32 effCreatePolyFlashTaskDataEntry222;
extern f32 effCreatePolyFlashTaskDataEntry223;
extern f32 effCreatePolyFlashTaskDataEntry224;
extern f32 effCreatePolyFlashTaskDataEntry225;
extern f32 effCreatePolyFlashTaskDataEntry226;
extern f32 effCreatePolyFlashTaskDataEntry227;
extern f32 effCreatePolyFlashTaskDataEntry228;
extern f32 effCreatePolyFlashTaskDataEntry229;
extern char effCreatePolyFlashTaskDataEntry22[];
extern f32 effCreatePolyFlashTaskDataEntry230;
extern f32 effCreatePolyFlashTaskDataEntry231;
extern f32 effCreatePolyFlashTaskDataEntry232;
extern f32 effCreatePolyFlashTaskDataEntry233;
extern f32 effCreatePolyFlashTaskDataEntry234;
extern f32 effCreatePolyFlashTaskDataEntry235;
extern f32 effCreatePolyFlashTaskDataEntry236;
extern u8 effCreatePolyFlashTaskDataEntry237[];
extern u8 effCreatePolyFlashTaskDataEntry238;
extern u32 effCreatePolyFlashTaskDataEntry239;
extern char effCreatePolyFlashTaskDataEntry23[];
extern u32 effCreatePolyFlashTaskDataEntry240;
extern f32 effCreatePolyFlashTaskDataEntry241;
extern f32 effCreatePolyFlashTaskDataEntry242;
extern f32 effCreatePolyFlashTaskDataEntry243;
extern f32 effCreatePolyFlashTaskDataEntry244;
extern f32 effCreatePolyFlashTaskDataEntry245;
extern f32 effCreatePolyFlashTaskDataEntry246;
extern f32 effCreatePolyFlashTaskDataEntry247;
extern f32 effCreatePolyFlashTaskDataEntry248;
extern f32 effCreatePolyFlashTaskDataEntry249;
extern char effCreatePolyFlashTaskDataEntry24[];
extern f32 effCreatePolyFlashTaskDataEntry250;
extern f32 effCreatePolyFlashTaskDataEntry251;
extern f32 effCreatePolyFlashTaskDataEntry252;
extern f32 effCreatePolyFlashTaskDataEntry253;
extern f32 effCreatePolyFlashTaskDataEntry254;
extern f32 effCreatePolyFlashTaskDataEntry255;
extern u8 effCreatePolyFlashTaskDataEntry256[];
extern u8 effCreatePolyFlashTaskDataEntry257;
extern u32 effCreatePolyFlashTaskDataEntry258;
extern u32 effCreatePolyFlashTaskDataEntry259;
extern char effCreatePolyFlashTaskDataEntry25[];
extern f32 effCreatePolyFlashTaskDataEntry260;
extern f32 effCreatePolyFlashTaskDataEntry261;
extern f32 effCreatePolyFlashTaskDataEntry262;
extern f32 effCreatePolyFlashTaskDataEntry263;
extern f32 effCreatePolyFlashTaskDataEntry264;
extern f32 effCreatePolyFlashTaskDataEntry265;
extern f32 effCreatePolyFlashTaskDataEntry266;
extern f32 effCreatePolyFlashTaskDataEntry267;
extern f32 effCreatePolyFlashTaskDataEntry268;
extern f32 effCreatePolyFlashTaskDataEntry269;
extern u8 effCreatePolyFlashTaskDataEntry26[];
extern s32 effCreatePolyFlashTaskDataEntry27;
extern s32 effCreatePolyFlashTaskDataEntry28;
extern s32 effCreatePolyFlashTaskDataEntry29;
extern s32 effCreatePolyFlashTaskDataEntry30;
extern u8 effCreatePolyFlashTaskDataEntry31;
extern u8 effCreatePolyFlashTaskDataEntry32;
extern f32 effCreatePolyFlashTaskDataEntry33;
extern f32 effCreatePolyFlashTaskDataEntry34;
extern f32 effCreatePolyFlashTaskDataEntry35;
extern s32 effCreatePolyFlashTaskDataEntry36;
extern s32 effCreatePolyFlashTaskDataEntry37;
extern s32 effCreatePolyFlashTaskDataEntry38;
extern s32 effCreatePolyFlashTaskDataEntry39;
extern u8 effCreatePolyFlashTaskDataEntry40;
extern u8 effCreatePolyFlashTaskDataEntry41;
extern f32 effCreatePolyFlashTaskDataEntry42;
extern f32 effCreatePolyFlashTaskDataEntry43;
extern f32 effCreatePolyFlashTaskDataEntry44;
extern s32 effCreatePolyFlashTaskDataEntry45;
extern s32 effCreatePolyFlashTaskDataEntry46;
extern s32 effCreatePolyFlashTaskDataEntry47;
extern s32 effCreatePolyFlashTaskDataEntry48;
extern s32 effCreatePolyFlashTaskDataEntry49;
extern u8 effCreatePolyFlashTaskDataEntry50;
extern u8 effCreatePolyFlashTaskDataEntry51;
extern f32 effCreatePolyFlashTaskDataEntry52;
extern f32 effCreatePolyFlashTaskDataEntry53;
extern f32 effCreatePolyFlashTaskDataEntry54;
extern s32 effCreatePolyFlashTaskDataEntry55;
extern s32 effCreatePolyFlashTaskDataEntry56;
extern s32 effCreatePolyFlashTaskDataEntry57;
extern s32 effCreatePolyFlashTaskDataEntry58;
extern u8 effCreatePolyFlashTaskDataEntry59;
extern u8 effCreatePolyFlashTaskDataEntry60;
extern f32 effCreatePolyFlashTaskDataEntry61;
extern f32 effCreatePolyFlashTaskDataEntry62;
extern f32 effCreatePolyFlashTaskDataEntry63;
extern s32 effCreatePolyFlashTaskDataEntry64;
extern s32 effCreatePolyFlashTaskDataEntry65;
extern s32 effCreatePolyFlashTaskDataEntry66;
extern s32 effCreatePolyFlashTaskDataEntry67;
extern u8 effCreatePolyFlashTaskDataEntry68;
extern u8 effCreatePolyFlashTaskDataEntry69;
extern f32 effCreatePolyFlashTaskDataEntry70;
extern f32 effCreatePolyFlashTaskDataEntry71;
extern f32 effCreatePolyFlashTaskDataEntry72;
extern s32 effCreatePolyFlashTaskDataEntry73;
extern u8 effCreatePolyFlashTaskDataEntry74[];
extern s32 effCreatePolyFlashTaskDataEntry75;
extern s32 effCreatePolyFlashTaskDataEntry76;
extern s32 effCreatePolyFlashTaskDataEntry77;
extern s32 effCreatePolyFlashTaskDataEntry78;
extern u8 effCreatePolyFlashTaskDataEntry79;
extern u8 effCreatePolyFlashTaskDataEntry80;
extern f32 effCreatePolyFlashTaskDataEntry81;
extern f32 effCreatePolyFlashTaskDataEntry82;
extern f32 effCreatePolyFlashTaskDataEntry83;
extern s32 effCreatePolyFlashTaskDataEntry84;
extern s32 effCreatePolyFlashTaskDataEntry85;
extern s32 effCreatePolyFlashTaskDataEntry86;
extern s32 effCreatePolyFlashTaskDataEntry87;
extern u8 effCreatePolyFlashTaskDataEntry88;
extern u8 effCreatePolyFlashTaskDataEntry89;
extern f32 effCreatePolyFlashTaskDataEntry90;
extern f32 effCreatePolyFlashTaskDataEntry91;
extern f32 effCreatePolyFlashTaskDataEntry92;
extern s32 effCreatePolyFlashTaskDataEntry93;
extern s32 effCreatePolyFlashTaskDataEntry94;
extern s32 effCreatePolyFlashTaskDataEntry95;
extern s32 effCreatePolyFlashTaskDataEntry96;
extern u8 effCreatePolyFlashTaskDataEntry97;
extern u8 effCreatePolyFlashTaskDataEntry98;
extern f32 effCreatePolyFlashTaskDataEntry99;
extern char effCreatePolyFlashTaskDataEntry9[];
extern char effCreatePolyRingTaskDataEntry10[];
extern s32 effCreatePolyRingTaskDataEntry11;
extern f32 effCreatePolyRingTaskDataEntry12;
extern f32 effCreatePolyRingTaskDataEntry13;
extern s32 effCreatePolyRingTaskDataEntry14;
extern u8 effCreatePolyRingTaskDataEntry15[];
extern s32 effCreatePolyRingTaskDataEntry16;
extern f32 effCreatePolyRingTaskDataEntry17;
extern f32 effCreatePolyRingTaskDataEntry18;
extern f32 effCreatePolyRingTaskDataEntry19;
extern f32 effCreatePolyRingTaskDataEntry20;
extern s32 effCreatePolyRingTaskDataEntry21;
extern f32 effCreatePolyRingTaskDataEntry22;
extern f32 effCreatePolyRingTaskDataEntry23;
extern s32 effCreatePolyRingTaskDataEntry24;
extern char effCreatePolyRingTaskDataEntry33[];
extern char effCreatePolyRingTaskDataEntry34[];
extern char effCreatePolyRingTaskDataEntry35[];
extern u8 effCreatePolyRingTaskDataEntry36[];
extern s32 effCreatePolyRingTaskDataEntry37;
extern u8 effCreatePolyRingTaskDataEntry38;
extern f32 effCreatePolyRingTaskDataEntry39;
extern u32 effCreatePolyRingTaskDataEntry40;
extern u32 effCreatePolyRingTaskDataEntry41;
extern u32 effCreatePolyRingTaskDataEntry42;
extern f32 effCreatePolyRingTaskDataEntry43;
extern f32 effCreatePolyRingTaskDataEntry44;
extern f32 effCreatePolyRingTaskDataEntry45;
extern s32 effCreatePolyRingTaskDataEntry46;
extern u8 effCreatePolyRingTaskDataEntry47;
extern f32 effCreatePolyRingTaskDataEntry48;
extern u32 effCreatePolyRingTaskDataEntry49;
extern char effCreatePolyRingTaskDataEntry4[];
extern u32 effCreatePolyRingTaskDataEntry50;
extern u32 effCreatePolyRingTaskDataEntry51;
extern f32 effCreatePolyRingTaskDataEntry52;
extern f32 effCreatePolyRingTaskDataEntry53;
extern f32 effCreatePolyRingTaskDataEntry54;
extern u8 effCreatePolyRingTaskDataEntry55[];
extern s32 effCreatePolyRingTaskDataEntry56;
extern u8 effCreatePolyRingTaskDataEntry57;
extern f32 effCreatePolyRingTaskDataEntry58;
extern u32 effCreatePolyRingTaskDataEntry59;
extern u32 effCreatePolyRingTaskDataEntry60;
extern u32 effCreatePolyRingTaskDataEntry61;
extern f32 effCreatePolyRingTaskDataEntry62;
extern char effCreatePolyRingTaskDataEntry9[];
extern f32 effCreatePolyScalyTaskDataEntry100;
extern f32 effCreatePolyScalyTaskDataEntry101;
extern f32 effCreatePolyScalyTaskDataEntry102;
extern f32 effCreatePolyScalyTaskDataEntry103;
extern f32 effCreatePolyScalyTaskDataEntry104;
extern f32 effCreatePolyScalyTaskDataEntry105;
extern u8 effCreatePolyScalyTaskDataEntry106[];
extern f32 effCreatePolyScalyTaskDataEntry107;
extern f32 effCreatePolyScalyTaskDataEntry108;
extern f32 effCreatePolyScalyTaskDataEntry109;
extern f32 effCreatePolyScalyTaskDataEntry110;
extern s32 effCreatePolyScalyTaskDataEntry111;
extern s32 effCreatePolyScalyTaskDataEntry112;
extern f32 effCreatePolyScalyTaskDataEntry113;
extern f32 effCreatePolyScalyTaskDataEntry114;
extern f32 effCreatePolyScalyTaskDataEntry115;
extern f32 effCreatePolyScalyTaskDataEntry116;
extern f32 effCreatePolyScalyTaskDataEntry117;
extern f32 effCreatePolyScalyTaskDataEntry118;
extern f32 effCreatePolyScalyTaskDataEntry119;
extern char effCreatePolyScalyTaskDataEntry11[];
extern f32 effCreatePolyScalyTaskDataEntry120;
extern f32 effCreatePolyScalyTaskDataEntry121;
extern u8 effCreatePolyScalyTaskDataEntry122[];
extern s32 effCreatePolyScalyTaskDataEntry123;
extern f32 effCreatePolyScalyTaskDataEntry124;
extern f32 effCreatePolyScalyTaskDataEntry125;
extern f32 effCreatePolyScalyTaskDataEntry126;
extern f32 effCreatePolyScalyTaskDataEntry127;
extern f32 effCreatePolyScalyTaskDataEntry128;
extern f32 effCreatePolyScalyTaskDataEntry129;
extern s32 effCreatePolyScalyTaskDataEntry12;
extern f32 effCreatePolyScalyTaskDataEntry130;
extern s32 effCreatePolyScalyTaskDataEntry13;
extern s32 effCreatePolyScalyTaskDataEntry14;
extern s32 effCreatePolyScalyTaskDataEntry15;
extern u8 effCreatePolyScalyTaskDataEntry16;
extern u8 effCreatePolyScalyTaskDataEntry17;
extern f32 effCreatePolyScalyTaskDataEntry18;
extern f32 effCreatePolyScalyTaskDataEntry19;
extern f32 effCreatePolyScalyTaskDataEntry20;
extern s32 effCreatePolyScalyTaskDataEntry21;
extern u8 effCreatePolyScalyTaskDataEntry22;
extern u8 effCreatePolyScalyTaskDataEntry23;
extern f32 effCreatePolyScalyTaskDataEntry24;
extern f32 effCreatePolyScalyTaskDataEntry25;
extern f32 effCreatePolyScalyTaskDataEntry26;
extern s32 effCreatePolyScalyTaskDataEntry27;
extern u8 effCreatePolyScalyTaskDataEntry28;
extern u8 effCreatePolyScalyTaskDataEntry29;
extern f32 effCreatePolyScalyTaskDataEntry30;
extern f32 effCreatePolyScalyTaskDataEntry31;
extern f32 effCreatePolyScalyTaskDataEntry32;
extern s32 effCreatePolyScalyTaskDataEntry33;
extern char effCreatePolyScalyTaskDataEntry58[];
extern char effCreatePolyScalyTaskDataEntry59[];
extern char effCreatePolyScalyTaskDataEntry5[];
extern char effCreatePolyScalyTaskDataEntry60[];
extern char effCreatePolyScalyTaskDataEntry61[];
extern char effCreatePolyScalyTaskDataEntry62[];
extern char effCreatePolyScalyTaskDataEntry63[];
extern char effCreatePolyScalyTaskDataEntry64[];
extern char effCreatePolyScalyTaskDataEntry65[];
extern char effCreatePolyScalyTaskDataEntry66[];
extern char effCreatePolyScalyTaskDataEntry67[];
extern char effCreatePolyScalyTaskDataEntry68[];
extern char effCreatePolyScalyTaskDataEntry69[];
extern char effCreatePolyScalyTaskDataEntry6[];
extern char effCreatePolyScalyTaskDataEntry70[];
extern char effCreatePolyScalyTaskDataEntry71[];
extern char effCreatePolyScalyTaskDataEntry72[];
extern u8 effCreatePolyScalyTaskDataEntry73[];
extern s32 effCreatePolyScalyTaskDataEntry74;
extern f32 effCreatePolyScalyTaskDataEntry75;
extern f32 effCreatePolyScalyTaskDataEntry76;
extern f32 effCreatePolyScalyTaskDataEntry77;
extern f32 effCreatePolyScalyTaskDataEntry78;
extern f32 effCreatePolyScalyTaskDataEntry79;
extern f32 effCreatePolyScalyTaskDataEntry80;
extern f32 effCreatePolyScalyTaskDataEntry81;
extern f32 effCreatePolyScalyTaskDataEntry82;
extern f32 effCreatePolyScalyTaskDataEntry83;
extern f32 effCreatePolyScalyTaskDataEntry84;
extern f32 effCreatePolyScalyTaskDataEntry85;
extern f32 effCreatePolyScalyTaskDataEntry86;
extern f32 effCreatePolyScalyTaskDataEntry87;
extern f32 effCreatePolyScalyTaskDataEntry88;
extern f32 effCreatePolyScalyTaskDataEntry89;
extern f32 effCreatePolyScalyTaskDataEntry90;
extern u8 effCreatePolyScalyTaskDataEntry91[];
extern f32 effCreatePolyScalyTaskDataEntry92;
extern f32 effCreatePolyScalyTaskDataEntry93;
extern f32 effCreatePolyScalyTaskDataEntry94;
extern f32 effCreatePolyScalyTaskDataEntry95;
extern s32 effCreatePolyScalyTaskDataEntry96;
extern s32 effCreatePolyScalyTaskDataEntry97;
extern f32 effCreatePolyScalyTaskDataEntry98;
extern f32 effCreatePolyScalyTaskDataEntry99;
extern s32 effCreatePolyThunderTaskDataEntry10;
extern s32 effCreatePolyThunderTaskDataEntry11;
extern f32 effCreatePolyThunderTaskDataEntry12;
extern s32 effCreatePolyThunderTaskDataEntry13;
extern s32 effCreatePolyThunderTaskDataEntry14;
extern s32 effCreatePolyThunderTaskDataEntry15;
extern f32 effCreatePolyThunderTaskDataEntry16;
extern f32 effCreatePolyThunderTaskDataEntry17;
extern f32 effCreatePolyThunderTaskDataEntry18;
extern f32 effCreatePolyThunderTaskDataEntry19;
extern char effCreatePolyThunderTaskDataEntry27[];
extern char effCreatePolyThunderTaskDataEntry28[];
extern char effCreatePolyThunderTaskDataEntry29[];
extern char effCreatePolyThunderTaskDataEntry30[];
extern char effCreatePolyThunderTaskDataEntry31[];
extern char effCreatePolyThunderTaskDataEntry32[];
extern char effCreatePolyThunderTaskDataEntry33[];
extern char effCreatePolyThunderTaskDataEntry34[];
extern s32 effCreatePolyThunderTaskDataEntry35;
extern s32 effCreatePolyThunderTaskDataEntry36;
extern f32 effCreatePolyThunderTaskDataEntry37;
extern u8 effCreatePolyThunderTaskDataEntry38;
extern u32 effCreatePolyThunderTaskDataEntry39;
extern u32 effCreatePolyThunderTaskDataEntry40;
extern u32 effCreatePolyThunderTaskDataEntry41;
extern f32 effCreatePolyThunderTaskDataEntry42;
extern f32 effCreatePolyThunderTaskDataEntry43;
extern u8 effCreatePolyThunderTaskDataEntry44[];
extern s32 effCreatePolyThunderTaskDataEntry45;
extern s32 effCreatePolyThunderTaskDataEntry46;
extern f32 effCreatePolyThunderTaskDataEntry47;
extern u8 effCreatePolyThunderTaskDataEntry48;
extern u32 effCreatePolyThunderTaskDataEntry49;
extern char effCreatePolyThunderTaskDataEntry4[];
extern u32 effCreatePolyThunderTaskDataEntry50;
extern u32 effCreatePolyThunderTaskDataEntry51;
extern f32 effCreatePolyThunderTaskDataEntry52;
extern f32 effCreatePolyThunderTaskDataEntry53;
extern s32 effCreatePolyThunderTaskDataEntry54;
extern f32 effCreatePolyThunderTaskDataEntry55;
extern f32 effCreatePolyThunderTaskDataEntry56;
extern f32 effCreatePolyThunderTaskDataEntry57;
extern f32 effCreatePolyThunderTaskDataEntry58;
extern f32 effCreatePolyThunderTaskDataEntry59;
extern u8 effCreatePolyThunderTaskDataEntry5[];
extern char effCreatePolyThunderTaskDataEntry8[];
extern s32 effCreatePolyThunderTaskDataEntry9;
extern s32 effCreatePolyTrackControlTaskDataEntry100;
extern s32 effCreatePolyTrackControlTaskDataEntry101;
extern f32 effCreatePolyTrackControlTaskDataEntry102;
extern f32 effCreatePolyTrackControlTaskDataEntry103;
extern f32 effCreatePolyTrackControlTaskDataEntry104;
extern s32 effCreatePolyTrackControlTaskDataEntry105;
extern s32 effCreatePolyTrackControlTaskDataEntry106;
extern u32 effCreatePolyTrackControlTaskDataEntry107;
extern u32 effCreatePolyTrackControlTaskDataEntry108;
extern u32 effCreatePolyTrackControlTaskDataEntry109;
extern char effCreatePolyTrackControlTaskDataEntry10[];
extern u32 effCreatePolyTrackControlTaskDataEntry110;
extern s32 effCreatePolyTrackControlTaskDataEntry111;
extern s32 effCreatePolyTrackControlTaskDataEntry112;
extern f32 effCreatePolyTrackControlTaskDataEntry113;
extern f32 effCreatePolyTrackControlTaskDataEntry114;
extern f32 effCreatePolyTrackControlTaskDataEntry115;
extern f32 effCreatePolyTrackControlTaskDataEntry116;
extern f32 effCreatePolyTrackControlTaskDataEntry117;
extern f32 effCreatePolyTrackControlTaskDataEntry118;
extern f32 effCreatePolyTrackControlTaskDataEntry119;
extern char effCreatePolyTrackControlTaskDataEntry11[];
extern f32 effCreatePolyTrackControlTaskDataEntry120;
extern f32 effCreatePolyTrackControlTaskDataEntry121;
extern s32 effCreatePolyTrackControlTaskDataEntry122;
extern u32 effCreatePolyTrackControlTaskDataEntry123;
extern u32 effCreatePolyTrackControlTaskDataEntry124;
extern u32 effCreatePolyTrackControlTaskDataEntry125;
extern u32 effCreatePolyTrackControlTaskDataEntry126;
extern s32 effCreatePolyTrackControlTaskDataEntry127;
extern s32 effCreatePolyTrackControlTaskDataEntry128;
extern f32 effCreatePolyTrackControlTaskDataEntry129;
extern s32 effCreatePolyTrackControlTaskDataEntry12;
extern f32 effCreatePolyTrackControlTaskDataEntry130;
extern f32 effCreatePolyTrackControlTaskDataEntry131;
extern f32 effCreatePolyTrackControlTaskDataEntry132;
extern f32 effCreatePolyTrackControlTaskDataEntry133;
extern s32 effCreatePolyTrackControlTaskDataEntry134;
extern s32 effCreatePolyTrackControlTaskDataEntry135;
extern u32 effCreatePolyTrackControlTaskDataEntry136;
extern u32 effCreatePolyTrackControlTaskDataEntry137;
extern u32 effCreatePolyTrackControlTaskDataEntry138;
extern u32 effCreatePolyTrackControlTaskDataEntry139;
extern s32 effCreatePolyTrackControlTaskDataEntry140;
extern s32 effCreatePolyTrackControlTaskDataEntry141;
extern f32 effCreatePolyTrackControlTaskDataEntry142;
extern f32 effCreatePolyTrackControlTaskDataEntry143;
extern f32 effCreatePolyTrackControlTaskDataEntry144;
extern f32 effCreatePolyTrackControlTaskDataEntry145;
extern f32 effCreatePolyTrackControlTaskDataEntry146;
extern char effCreatePolyTrackControlTaskDataEntry20[];
extern s32 effCreatePolyTrackControlTaskDataEntry21;
extern s32 effCreatePolyTrackControlTaskDataEntry22;
extern u8 effCreatePolyTrackControlTaskDataEntry23;
extern f32 effCreatePolyTrackControlTaskDataEntry24;
extern f32 effCreatePolyTrackControlTaskDataEntry25;
extern f32 effCreatePolyTrackControlTaskDataEntry26;
extern s32 effCreatePolyTrackControlTaskDataEntry27;
extern u8 effCreatePolyTrackControlTaskDataEntry28;
extern f32 effCreatePolyTrackControlTaskDataEntry29;
extern f32 effCreatePolyTrackControlTaskDataEntry30;
extern f32 effCreatePolyTrackControlTaskDataEntry31;
extern f32 effCreatePolyTrackControlTaskDataEntry32;
extern s32 effCreatePolyTrackControlTaskDataEntry33;
extern u8 effCreatePolyTrackControlTaskDataEntry34;
extern f32 effCreatePolyTrackControlTaskDataEntry35;
extern f32 effCreatePolyTrackControlTaskDataEntry36;
extern f32 effCreatePolyTrackControlTaskDataEntry37;
extern s32 effCreatePolyTrackControlTaskDataEntry38;
extern s32 effCreatePolyTrackControlTaskDataEntry39;
extern u8 effCreatePolyTrackControlTaskDataEntry40;
extern f32 effCreatePolyTrackControlTaskDataEntry41;
extern f32 effCreatePolyTrackControlTaskDataEntry42;
extern f32 effCreatePolyTrackControlTaskDataEntry43;
extern f32 effCreatePolyTrackControlTaskDataEntry44;
extern s32 effCreatePolyTrackControlTaskDataEntry45;
extern u8 effCreatePolyTrackControlTaskDataEntry46;
extern f32 effCreatePolyTrackControlTaskDataEntry47;
extern f32 effCreatePolyTrackControlTaskDataEntry48;
extern f32 effCreatePolyTrackControlTaskDataEntry49;
extern f32 effCreatePolyTrackControlTaskDataEntry50;
extern char effCreatePolyTrackControlTaskDataEntry76[];
extern char effCreatePolyTrackControlTaskDataEntry77[];
extern char effCreatePolyTrackControlTaskDataEntry78[];
extern char effCreatePolyTrackControlTaskDataEntry79[];
extern s32 effCreatePolyTrackControlTaskDataEntry80;
extern u32 effCreatePolyTrackControlTaskDataEntry81;
extern u32 effCreatePolyTrackControlTaskDataEntry82;
extern u32 effCreatePolyTrackControlTaskDataEntry83;
extern u32 effCreatePolyTrackControlTaskDataEntry84;
extern s32 effCreatePolyTrackControlTaskDataEntry85;
extern s32 effCreatePolyTrackControlTaskDataEntry86;
extern f32 effCreatePolyTrackControlTaskDataEntry87;
extern f32 effCreatePolyTrackControlTaskDataEntry88;
extern f32 effCreatePolyTrackControlTaskDataEntry89;
extern char effCreatePolyTrackControlTaskDataEntry8[];
extern f32 effCreatePolyTrackControlTaskDataEntry90;
extern f32 effCreatePolyTrackControlTaskDataEntry91;
extern f32 effCreatePolyTrackControlTaskDataEntry92;
extern f32 effCreatePolyTrackControlTaskDataEntry93;
extern f32 effCreatePolyTrackControlTaskDataEntry94;
extern s32 effCreatePolyTrackControlTaskDataEntry95;
extern u32 effCreatePolyTrackControlTaskDataEntry96;
extern u32 effCreatePolyTrackControlTaskDataEntry97;
extern u32 effCreatePolyTrackControlTaskDataEntry98;
extern u32 effCreatePolyTrackControlTaskDataEntry99;
extern char effCreatePolyTrackControlTaskDataEntry9[];
extern f32 effCreatePolyTrackTaskDataEntry100;
extern f32 effCreatePolyTrackTaskDataEntry101;
extern f32 effCreatePolyTrackTaskDataEntry102;
extern f32 effCreatePolyTrackTaskDataEntry103;
extern f32 effCreatePolyTrackTaskDataEntry104;
extern f32 effCreatePolyTrackTaskDataEntry105;
extern f32 effCreatePolyTrackTaskDataEntry106;
extern s32 effCreatePolyTrackTaskDataEntry107;
extern s32 effCreatePolyTrackTaskDataEntry108;
extern u32 effCreatePolyTrackTaskDataEntry109;
extern u32 effCreatePolyTrackTaskDataEntry110;
extern u32 effCreatePolyTrackTaskDataEntry111;
extern u32 effCreatePolyTrackTaskDataEntry112;
extern f32 effCreatePolyTrackTaskDataEntry113;
extern s32 effCreatePolyTrackTaskDataEntry114;
extern s32 effCreatePolyTrackTaskDataEntry115;
extern f32 effCreatePolyTrackTaskDataEntry116;
extern f32 effCreatePolyTrackTaskDataEntry117;
extern f32 effCreatePolyTrackTaskDataEntry118;
extern s32 effCreatePolyTrackTaskDataEntry119;
extern s32 effCreatePolyTrackTaskDataEntry120;
extern u32 effCreatePolyTrackTaskDataEntry121;
extern u32 effCreatePolyTrackTaskDataEntry122;
extern u32 effCreatePolyTrackTaskDataEntry123;
extern u32 effCreatePolyTrackTaskDataEntry124;
extern f32 effCreatePolyTrackTaskDataEntry125;
extern s32 effCreatePolyTrackTaskDataEntry126;
extern s32 effCreatePolyTrackTaskDataEntry127;
extern f32 effCreatePolyTrackTaskDataEntry128;
extern f32 effCreatePolyTrackTaskDataEntry129;
extern f32 effCreatePolyTrackTaskDataEntry130;
extern f32 effCreatePolyTrackTaskDataEntry131;
extern f32 effCreatePolyTrackTaskDataEntry132;
extern f32 effCreatePolyTrackTaskDataEntry133;
extern f32 effCreatePolyTrackTaskDataEntry134;
extern f32 effCreatePolyTrackTaskDataEntry135;
extern f32 effCreatePolyTrackTaskDataEntry136;
extern s32 effCreatePolyTrackTaskDataEntry137;
extern s32 effCreatePolyTrackTaskDataEntry138;
extern u32 effCreatePolyTrackTaskDataEntry139;
extern u32 effCreatePolyTrackTaskDataEntry140;
extern u32 effCreatePolyTrackTaskDataEntry141;
extern u32 effCreatePolyTrackTaskDataEntry142;
extern f32 effCreatePolyTrackTaskDataEntry143;
extern s32 effCreatePolyTrackTaskDataEntry144;
extern s32 effCreatePolyTrackTaskDataEntry145;
extern f32 effCreatePolyTrackTaskDataEntry146;
extern f32 effCreatePolyTrackTaskDataEntry147;
extern f32 effCreatePolyTrackTaskDataEntry148;
extern f32 effCreatePolyTrackTaskDataEntry149;
extern char effCreatePolyTrackTaskDataEntry14[];
extern f32 effCreatePolyTrackTaskDataEntry150;
extern s32 effCreatePolyTrackTaskDataEntry151;
extern s32 effCreatePolyTrackTaskDataEntry152;
extern u32 effCreatePolyTrackTaskDataEntry153;
extern u32 effCreatePolyTrackTaskDataEntry154;
extern u32 effCreatePolyTrackTaskDataEntry155;
extern u32 effCreatePolyTrackTaskDataEntry156;
extern f32 effCreatePolyTrackTaskDataEntry157;
extern s32 effCreatePolyTrackTaskDataEntry158;
extern s32 effCreatePolyTrackTaskDataEntry159;
extern char effCreatePolyTrackTaskDataEntry15[];
extern f32 effCreatePolyTrackTaskDataEntry160;
extern f32 effCreatePolyTrackTaskDataEntry161;
extern f32 effCreatePolyTrackTaskDataEntry162;
extern f32 effCreatePolyTrackTaskDataEntry163;
extern f32 effCreatePolyTrackTaskDataEntry164;
extern char effCreatePolyTrackTaskDataEntry16[];
extern char effCreatePolyTrackTaskDataEntry17[];
extern char effCreatePolyTrackTaskDataEntry18[];
extern char effCreatePolyTrackTaskDataEntry19[];
extern char effCreatePolyTrackTaskDataEntry20[];
extern s32 effCreatePolyTrackTaskDataEntry21;
extern u8 effCreatePolyTrackTaskDataEntry22;
extern f32 effCreatePolyTrackTaskDataEntry23;
extern f32 effCreatePolyTrackTaskDataEntry24;
extern f32 effCreatePolyTrackTaskDataEntry25;
extern s32 effCreatePolyTrackTaskDataEntry26;
extern u8 effCreatePolyTrackTaskDataEntry27;
extern f32 effCreatePolyTrackTaskDataEntry28;
extern f32 effCreatePolyTrackTaskDataEntry29;
extern f32 effCreatePolyTrackTaskDataEntry30;
extern f32 effCreatePolyTrackTaskDataEntry31;
extern s32 effCreatePolyTrackTaskDataEntry32;
extern u8 effCreatePolyTrackTaskDataEntry33;
extern f32 effCreatePolyTrackTaskDataEntry34;
extern f32 effCreatePolyTrackTaskDataEntry35;
extern f32 effCreatePolyTrackTaskDataEntry36;
extern s32 effCreatePolyTrackTaskDataEntry37;
extern u8 effCreatePolyTrackTaskDataEntry38;
extern f32 effCreatePolyTrackTaskDataEntry39;
extern f32 effCreatePolyTrackTaskDataEntry40;
extern f32 effCreatePolyTrackTaskDataEntry41;
extern f32 effCreatePolyTrackTaskDataEntry42;
extern s32 effCreatePolyTrackTaskDataEntry43;
extern u8 effCreatePolyTrackTaskDataEntry44;
extern f32 effCreatePolyTrackTaskDataEntry45;
extern f32 effCreatePolyTrackTaskDataEntry46;
extern f32 effCreatePolyTrackTaskDataEntry47;
extern f32 effCreatePolyTrackTaskDataEntry48;
extern char effCreatePolyTrackTaskDataEntry6[];
extern char effCreatePolyTrackTaskDataEntry7[];
extern char effCreatePolyTrackTaskDataEntry87[];
extern char effCreatePolyTrackTaskDataEntry88[];
extern char effCreatePolyTrackTaskDataEntry89[];
extern s32 effCreatePolyTrackTaskDataEntry90;
extern s32 effCreatePolyTrackTaskDataEntry91;
extern u32 effCreatePolyTrackTaskDataEntry92;
extern u32 effCreatePolyTrackTaskDataEntry93;
extern u32 effCreatePolyTrackTaskDataEntry94;
extern u32 effCreatePolyTrackTaskDataEntry95;
extern f32 effCreatePolyTrackTaskDataEntry96;
extern s32 effCreatePolyTrackTaskDataEntry97;
extern s32 effCreatePolyTrackTaskDataEntry98;
extern f32 effCreatePolyTrackTaskDataEntry99;
extern u32 effCreatePolyTwinkleTaskDataEntry100;
extern s32 effCreatePolyTwinkleTaskDataEntry101;
extern s32 effCreatePolyTwinkleTaskDataEntry102;
extern f32 effCreatePolyTwinkleTaskDataEntry103;
extern f32 effCreatePolyTwinkleTaskDataEntry104;
extern f32 effCreatePolyTwinkleTaskDataEntry105;
extern f32 effCreatePolyTwinkleTaskDataEntry106;
extern f32 effCreatePolyTwinkleTaskDataEntry107;
extern f32 effCreatePolyTwinkleTaskDataEntry108;
extern f32 effCreatePolyTwinkleTaskDataEntry109;
extern char effCreatePolyTwinkleTaskDataEntry10[];
extern f32 effCreatePolyTwinkleTaskDataEntry110;
extern u8 effCreatePolyTwinkleTaskDataEntry111;
extern u32 effCreatePolyTwinkleTaskDataEntry112;
extern u32 effCreatePolyTwinkleTaskDataEntry113;
extern f32 effCreatePolyTwinkleTaskDataEntry114;
extern f32 effCreatePolyTwinkleTaskDataEntry115;
extern f32 effCreatePolyTwinkleTaskDataEntry116;
extern f32 effCreatePolyTwinkleTaskDataEntry117;
extern u32 effCreatePolyTwinkleTaskDataEntry118;
extern s32 effCreatePolyTwinkleTaskDataEntry119;
extern char effCreatePolyTwinkleTaskDataEntry11[];
extern s32 effCreatePolyTwinkleTaskDataEntry120;
extern f32 effCreatePolyTwinkleTaskDataEntry121;
extern f32 effCreatePolyTwinkleTaskDataEntry122;
extern f32 effCreatePolyTwinkleTaskDataEntry123;
extern u8 effCreatePolyTwinkleTaskDataEntry124;
extern u32 effCreatePolyTwinkleTaskDataEntry125;
extern u32 effCreatePolyTwinkleTaskDataEntry126;
extern f32 effCreatePolyTwinkleTaskDataEntry127;
extern f32 effCreatePolyTwinkleTaskDataEntry128;
extern f32 effCreatePolyTwinkleTaskDataEntry129;
extern f32 effCreatePolyTwinkleTaskDataEntry130;
extern u32 effCreatePolyTwinkleTaskDataEntry131;
extern s32 effCreatePolyTwinkleTaskDataEntry132;
extern s32 effCreatePolyTwinkleTaskDataEntry133;
extern f32 effCreatePolyTwinkleTaskDataEntry134;
extern f32 effCreatePolyTwinkleTaskDataEntry135;
extern f32 effCreatePolyTwinkleTaskDataEntry136;
extern f32 effCreatePolyTwinkleTaskDataEntry137;
extern f32 effCreatePolyTwinkleTaskDataEntry138;
extern f32 effCreatePolyTwinkleTaskDataEntry139;
extern f32 effCreatePolyTwinkleTaskDataEntry140;
extern f32 effCreatePolyTwinkleTaskDataEntry141;
extern f32 effCreatePolyTwinkleTaskDataEntry142;
extern u8 effCreatePolyTwinkleTaskDataEntry143;
extern u32 effCreatePolyTwinkleTaskDataEntry144;
extern u32 effCreatePolyTwinkleTaskDataEntry145;
extern f32 effCreatePolyTwinkleTaskDataEntry146;
extern f32 effCreatePolyTwinkleTaskDataEntry147;
extern f32 effCreatePolyTwinkleTaskDataEntry148;
extern f32 effCreatePolyTwinkleTaskDataEntry149;
extern u32 effCreatePolyTwinkleTaskDataEntry150;
extern s32 effCreatePolyTwinkleTaskDataEntry151;
extern s32 effCreatePolyTwinkleTaskDataEntry152;
extern f32 effCreatePolyTwinkleTaskDataEntry153;
extern f32 effCreatePolyTwinkleTaskDataEntry154;
extern f32 effCreatePolyTwinkleTaskDataEntry155;
extern f32 effCreatePolyTwinkleTaskDataEntry156;
extern f32 effCreatePolyTwinkleTaskDataEntry157;
extern u8 effCreatePolyTwinkleTaskDataEntry158;
extern u32 effCreatePolyTwinkleTaskDataEntry159;
extern u32 effCreatePolyTwinkleTaskDataEntry160;
extern f32 effCreatePolyTwinkleTaskDataEntry161;
extern f32 effCreatePolyTwinkleTaskDataEntry162;
extern f32 effCreatePolyTwinkleTaskDataEntry163;
extern f32 effCreatePolyTwinkleTaskDataEntry164;
extern u32 effCreatePolyTwinkleTaskDataEntry165;
extern s32 effCreatePolyTwinkleTaskDataEntry166;
extern s32 effCreatePolyTwinkleTaskDataEntry167;
extern f32 effCreatePolyTwinkleTaskDataEntry168;
extern f32 effCreatePolyTwinkleTaskDataEntry169;
extern f32 effCreatePolyTwinkleTaskDataEntry170;
extern f32 effCreatePolyTwinkleTaskDataEntry171;
extern f32 effCreatePolyTwinkleTaskDataEntry172;
extern s32 effCreatePolyTwinkleTaskDataEntry17;
extern u8 effCreatePolyTwinkleTaskDataEntry18;
extern f32 effCreatePolyTwinkleTaskDataEntry19;
extern f32 effCreatePolyTwinkleTaskDataEntry20;
extern f32 effCreatePolyTwinkleTaskDataEntry21;
extern s32 effCreatePolyTwinkleTaskDataEntry22;
extern u8 effCreatePolyTwinkleTaskDataEntry23;
extern f32 effCreatePolyTwinkleTaskDataEntry24;
extern f32 effCreatePolyTwinkleTaskDataEntry25;
extern f32 effCreatePolyTwinkleTaskDataEntry26;
extern f32 effCreatePolyTwinkleTaskDataEntry27;
extern s32 effCreatePolyTwinkleTaskDataEntry28;
extern u8 effCreatePolyTwinkleTaskDataEntry29;
extern f32 effCreatePolyTwinkleTaskDataEntry30;
extern f32 effCreatePolyTwinkleTaskDataEntry31;
extern f32 effCreatePolyTwinkleTaskDataEntry32;
extern s32 effCreatePolyTwinkleTaskDataEntry33;
extern u8 effCreatePolyTwinkleTaskDataEntry34;
extern f32 effCreatePolyTwinkleTaskDataEntry35;
extern f32 effCreatePolyTwinkleTaskDataEntry36;
extern f32 effCreatePolyTwinkleTaskDataEntry37;
extern f32 effCreatePolyTwinkleTaskDataEntry38;
extern s32 effCreatePolyTwinkleTaskDataEntry39;
extern u8 effCreatePolyTwinkleTaskDataEntry40;
extern f32 effCreatePolyTwinkleTaskDataEntry41;
extern f32 effCreatePolyTwinkleTaskDataEntry42;
extern f32 effCreatePolyTwinkleTaskDataEntry43;
extern f32 effCreatePolyTwinkleTaskDataEntry44;
extern char effCreatePolyTwinkleTaskDataEntry7[];
extern char effCreatePolyTwinkleTaskDataEntry83[];
extern char effCreatePolyTwinkleTaskDataEntry84[];
extern char effCreatePolyTwinkleTaskDataEntry85[];
extern char effCreatePolyTwinkleTaskDataEntry86[];
extern char effCreatePolyTwinkleTaskDataEntry87[];
extern char effCreatePolyTwinkleTaskDataEntry88[];
extern char effCreatePolyTwinkleTaskDataEntry89[];
extern char effCreatePolyTwinkleTaskDataEntry8[];
extern char effCreatePolyTwinkleTaskDataEntry90[];
extern char effCreatePolyTwinkleTaskDataEntry91[];
extern char effCreatePolyTwinkleTaskDataEntry92[];
extern u8 effCreatePolyTwinkleTaskDataEntry93;
extern u32 effCreatePolyTwinkleTaskDataEntry94;
extern u32 effCreatePolyTwinkleTaskDataEntry95;
extern f32 effCreatePolyTwinkleTaskDataEntry96;
extern f32 effCreatePolyTwinkleTaskDataEntry97;
extern f32 effCreatePolyTwinkleTaskDataEntry98;
extern f32 effCreatePolyTwinkleTaskDataEntry99;
extern char effCreatePolyTwinkleTaskDataEntry9[];
extern char effCreatePolyWindTaskDataEntry107[];
extern char effCreatePolyWindTaskDataEntry108[];
extern char effCreatePolyWindTaskDataEntry109[];
extern char effCreatePolyWindTaskDataEntry10[];
extern char effCreatePolyWindTaskDataEntry110[];
extern char effCreatePolyWindTaskDataEntry111[];
extern char effCreatePolyWindTaskDataEntry112[];
extern char effCreatePolyWindTaskDataEntry113[];
extern char effCreatePolyWindTaskDataEntry114[];
extern char effCreatePolyWindTaskDataEntry115[];
extern char effCreatePolyWindTaskDataEntry116[];
extern char effCreatePolyWindTaskDataEntry117[];
extern char effCreatePolyWindTaskDataEntry118[];
extern char effCreatePolyWindTaskDataEntry119[];
extern char effCreatePolyWindTaskDataEntry11[];
extern char effCreatePolyWindTaskDataEntry120[];
extern char effCreatePolyWindTaskDataEntry121[];
extern char effCreatePolyWindTaskDataEntry122[];
extern u8 effCreatePolyWindTaskDataEntry123;
extern f32 effCreatePolyWindTaskDataEntry124;
extern f32 effCreatePolyWindTaskDataEntry125;
extern f32 effCreatePolyWindTaskDataEntry126;
extern f32 effCreatePolyWindTaskDataEntry127;
extern f32 effCreatePolyWindTaskDataEntry128;
extern f32 effCreatePolyWindTaskDataEntry129;
extern u8 effCreatePolyWindTaskDataEntry130;
extern s32 effCreatePolyWindTaskDataEntry131;
extern s32 effCreatePolyWindTaskDataEntry132;
extern f32 effCreatePolyWindTaskDataEntry133;
extern f32 effCreatePolyWindTaskDataEntry134;
extern f32 effCreatePolyWindTaskDataEntry135;
extern f32 effCreatePolyWindTaskDataEntry136;
extern f32 effCreatePolyWindTaskDataEntry137;
extern f32 effCreatePolyWindTaskDataEntry138;
extern f32 effCreatePolyWindTaskDataEntry139;
extern f32 effCreatePolyWindTaskDataEntry140;
extern u8 effCreatePolyWindTaskDataEntry141;
extern f32 effCreatePolyWindTaskDataEntry142;
extern f32 effCreatePolyWindTaskDataEntry143;
extern f32 effCreatePolyWindTaskDataEntry144;
extern f32 effCreatePolyWindTaskDataEntry145;
extern f32 effCreatePolyWindTaskDataEntry146;
extern f32 effCreatePolyWindTaskDataEntry147;
extern u8 effCreatePolyWindTaskDataEntry148;
extern s32 effCreatePolyWindTaskDataEntry149;
extern s32 effCreatePolyWindTaskDataEntry150;
extern f32 effCreatePolyWindTaskDataEntry151;
extern f32 effCreatePolyWindTaskDataEntry152;
extern f32 effCreatePolyWindTaskDataEntry153;
extern f32 effCreatePolyWindTaskDataEntry154;
extern f32 effCreatePolyWindTaskDataEntry155;
extern f32 effCreatePolyWindTaskDataEntry156;
extern f32 effCreatePolyWindTaskDataEntry157;
extern f32 effCreatePolyWindTaskDataEntry158;
extern f32 effCreatePolyWindTaskDataEntry159;
extern u8 effCreatePolyWindTaskDataEntry160;
extern s32 effCreatePolyWindTaskDataEntry161;
extern f32 effCreatePolyWindTaskDataEntry162;
extern f32 effCreatePolyWindTaskDataEntry163;
extern f32 effCreatePolyWindTaskDataEntry164;
extern u8 effCreatePolyWindTaskDataEntry165;
extern f32 effCreatePolyWindTaskDataEntry166;
extern f32 effCreatePolyWindTaskDataEntry167;
extern f32 effCreatePolyWindTaskDataEntry168;
extern f32 effCreatePolyWindTaskDataEntry169;
extern f32 effCreatePolyWindTaskDataEntry170;
extern f32 effCreatePolyWindTaskDataEntry171;
extern u8 effCreatePolyWindTaskDataEntry172;
extern s32 effCreatePolyWindTaskDataEntry173;
extern s32 effCreatePolyWindTaskDataEntry174;
extern f32 effCreatePolyWindTaskDataEntry175;
extern f32 effCreatePolyWindTaskDataEntry176;
extern f32 effCreatePolyWindTaskDataEntry177;
extern f32 effCreatePolyWindTaskDataEntry178;
extern f32 effCreatePolyWindTaskDataEntry179;
extern u8 effCreatePolyWindTaskDataEntry180;
extern f32 effCreatePolyWindTaskDataEntry181;
extern f32 effCreatePolyWindTaskDataEntry182;
extern f32 effCreatePolyWindTaskDataEntry183;
extern f32 effCreatePolyWindTaskDataEntry184;
extern f32 effCreatePolyWindTaskDataEntry185;
extern f32 effCreatePolyWindTaskDataEntry186;
extern u8 effCreatePolyWindTaskDataEntry187;
extern s32 effCreatePolyWindTaskDataEntry188;
extern s32 effCreatePolyWindTaskDataEntry189;
extern f32 effCreatePolyWindTaskDataEntry190;
extern f32 effCreatePolyWindTaskDataEntry191;
extern f32 effCreatePolyWindTaskDataEntry192;
extern f32 effCreatePolyWindTaskDataEntry193;
extern f32 effCreatePolyWindTaskDataEntry194;
extern f32 effCreatePolyWindTaskDataEntry195;
extern f32 effCreatePolyWindTaskDataEntry196;
extern f32 effCreatePolyWindTaskDataEntry197;
extern f32 effCreatePolyWindTaskDataEntry198;
extern f32 effCreatePolyWindTaskDataEntry199;
extern f32 effCreatePolyWindTaskDataEntry200;
extern u8 effCreatePolyWindTaskDataEntry201;
extern s32 effCreatePolyWindTaskDataEntry202;
extern s32 effCreatePolyWindTaskDataEntry203;
extern f32 effCreatePolyWindTaskDataEntry204;
extern f32 effCreatePolyWindTaskDataEntry205;
extern f32 effCreatePolyWindTaskDataEntry206;
extern f32 effCreatePolyWindTaskDataEntry207;
extern f32 effCreatePolyWindTaskDataEntry208;
extern s32 effCreatePolyWindTaskDataEntry209;
extern char effCreatePolyWindTaskDataEntry20[];
extern s32 effCreatePolyWindTaskDataEntry21;
extern u8 effCreatePolyWindTaskDataEntry22;
extern u8 effCreatePolyWindTaskDataEntry23;
extern f32 effCreatePolyWindTaskDataEntry24;
extern f32 effCreatePolyWindTaskDataEntry25;
extern f32 effCreatePolyWindTaskDataEntry26;
extern s32 effCreatePolyWindTaskDataEntry27;
extern u8 effCreatePolyWindTaskDataEntry28;
extern u8 effCreatePolyWindTaskDataEntry29;
extern f32 effCreatePolyWindTaskDataEntry30;
extern f32 effCreatePolyWindTaskDataEntry31;
extern f32 effCreatePolyWindTaskDataEntry32;
extern f32 effCreatePolyWindTaskDataEntry33;
extern u8 effCreatePolyWindTaskDataEntry34;
extern s32 effCreatePolyWindTaskDataEntry35;
extern u8 effCreatePolyWindTaskDataEntry36;
extern u8 effCreatePolyWindTaskDataEntry37;
extern f32 effCreatePolyWindTaskDataEntry38;
extern f32 effCreatePolyWindTaskDataEntry39;
extern f32 effCreatePolyWindTaskDataEntry40;
extern s32 effCreatePolyWindTaskDataEntry41;
extern u8 effCreatePolyWindTaskDataEntry42;
extern u8 effCreatePolyWindTaskDataEntry43;
extern f32 effCreatePolyWindTaskDataEntry44;
extern f32 effCreatePolyWindTaskDataEntry45;
extern f32 effCreatePolyWindTaskDataEntry46;
extern f32 effCreatePolyWindTaskDataEntry47;
extern s32 effCreatePolyWindTaskDataEntry48;
extern u8 effCreatePolyWindTaskDataEntry49;
extern u8 effCreatePolyWindTaskDataEntry50;
extern f32 effCreatePolyWindTaskDataEntry51;
extern f32 effCreatePolyWindTaskDataEntry52;
extern f32 effCreatePolyWindTaskDataEntry53;
extern f32 effCreatePolyWindTaskDataEntry54;
extern u8 effCreatePolyWindTaskDataEntry55;
extern s32 effCreatePolyWindTaskDataEntry56;
extern u8 effCreatePolyWindTaskDataEntry57;
extern u8 effCreatePolyWindTaskDataEntry58;
extern f32 effCreatePolyWindTaskDataEntry59;
extern f32 effCreatePolyWindTaskDataEntry60;
extern f32 effCreatePolyWindTaskDataEntry61;
extern f32 effCreatePolyWindTaskDataEntry62;
extern char effCreatePolyWindTaskDataEntry7[];
extern char effCreatePolyWindTaskDataEntry8[];
extern char effCreatePolyWindTaskDataEntry9[];
extern char effCreateTwoDimensionalTaskData2[];
extern u8 effCreateTwoDimensionalTaskDataEntry4[];
extern s32 effCreateTwoDimensionalTaskDataEntry5;
extern s32 effCreateTwoDimensionalTaskDataEntry6;
extern s32 effCreateTwoDimensionalTaskDataEntry7;
extern char effData20Entry2[];
extern u32 effData38;
extern char effData48[];
extern char effData49[];
extern char effData50[];
extern char effData51[];
extern s32 effData9Entry20;
extern char effData9Entry21[];
extern char effData9Entry22[];
extern char effData9Entry23[];
extern char effData9Entry24[];
extern char effData9Entry25[];
extern char effData9Entry26[];
extern char effData9Entry27[];
extern s32 effData9Entry28;
extern s32 effData9Entry29;
extern f32 effData9Entry30;
extern s32 effData9Entry31;
extern u8 effData9Entry32;
extern u8 effData9Entry33;
extern u8 effData9Entry34;
extern s32 effData9Entry35;
extern char effInitializeResourceQueueData[];
extern char effLabelBlur[];
void effLeaf401(void);
extern char effLoadMaterialFileDataEntry[];
extern char effPollAndQueueCopiedFileResourceDataEntry2[];
extern f32 effPollAndQueueCopiedFileResourceDataEntry;
s32 effPollMMaterialEplFile(void);
s32 effPollMMaterialPbFile(void);
extern char effStrAmplitude[];
extern char effStrBillboard[];
extern char effStrCrackPolygon[];
extern char effStrEQuake2[];
extern char effStrEdgeOut[];
extern char effStrEmtRadi[];
extern char effStrEmtSpre[];
extern char effStrFlashPolygon[];
extern char effStrOF2Cam[];
extern char effStrSaveEp[];
extern char effStrScaleUv[];
extern char effStrToolEffectSampleBDist0Ep[];
extern char effStrToolEffectSampleBFull0Ep[];
extern char effStrToolEffectSampleBMulti0[];
extern char effStrToolEffectSampleBParal0[];
extern char effStrToolEffectSampleBRiple0[];
extern char effStrToolEffectSampleCNomal0[];
extern char effStrToolEffectSampleDBug0Ep[];
extern char effStrToolEffectSampleDBug1Ep[];
extern char effStrToolEffectSampleDPoint0[];
extern char effStrToolEffectSampleEFlare0[];
extern char effStrToolEffectSampleEQuake0[];
extern char effStrToolEffectSampleEQuake20[];
extern char effStrToolEffectSampleERain0Ep[];
extern char effStrToolEffectSampleFFull0Ep[];
extern char effStrToolEffectSampleFMulti0[];
extern char effStrToolEffectSampleGHspak0[];
extern char effStrToolEffectSampleGSmoke0[];
extern char effStrToolEffectSampleGSpark0[];
extern char effStrToolEffectSampleGSphre0[];
extern char effStrToolEffectSampleGSpirl0[];
extern char effStrToolEffectSampleKHspak0[];
extern char effStrToolEffectSampleKSmoke0[];
extern char effStrToolEffectSampleKSpark0[];
extern char effStrToolEffectSampleKSphre0[];
extern char effStrToolEffectSampleKSpirl0[];
extern char effStrToolEffectSampleMAspak0[];
extern char effStrToolEffectSampleMHspak0[];
extern char effStrToolEffectSampleMSmoke0[];
extern char effStrToolEffectSampleMSpark0[];
extern char effStrToolEffectSampleMSphre0[];
extern char effStrToolEffectSampleMSpirl0[];
extern char effStrToolEffectSampleRCrcle0[];
extern char effStrToolEffectSampleRCrcle1[];
extern char effStrToolEffectSampleRNomal0[];
extern char effStrToolEffectSampleRNomal1[];
extern char effStrToolEffectSampleRTrack0[];
extern char effStrToolEffectSampleSCylnd0[];
extern char effStrToolEffectSampleSRadT0Ep[];
extern char effStrToolEffectSampleSRadi1Ep[];
extern char effStrToolEffectSampleSRing0Ep[];
extern char effStrToolEffectSampleSRngT0Ep[];
extern char effStrToolEffectSampleSSctT0Ep[];
extern char effStrToolEffectSampleSSpark0[];
extern char effStrToolEffectSampleSSpark1[];
extern char effStrToolEffectSampleSSpkT0Ep[];
extern char effStrToolEffectSampleSSpkT1Ep[];
extern char effStrToolEffectSampleUCylnd0[];
extern char effStrToolEffectSampleUSmoke0[];
extern char effStrToolEffectSampleUSphre0[];
extern char effStrToolEffectSampleUSpirl0[];
extern char effStrToolEffectSampleWHspak0[];
extern char effStrToolEffectSampleWSmoke0[];
extern char effStrToolEffectSampleWSpark0[];
extern char effStrToolEffectSampleWSphre0[];
extern char effStrToolEffectSampleWSpirl0[];
extern char effStrToolEffectSampleXNomal0[];
extern char effStrVcEndIn[];
extern char effStrVcEndOut[];
extern char effStrVcStartIn[];
extern char effStrVcStartOut[];
extern char effUpdateCameraSnapshotGuidesDataEntry2[];
extern char effUpdatePartsListEditorData6Entry2[];
extern u8 effWork6[];
extern char fileLoadEffectSlotADataEntry2[];
extern f32 fileLoadEffectSlotADataEntry;
extern char fileLoadEffectSlotBDataEntry2[];
extern f32 fileLoadEffectSlotBDataEntry;
extern s32 sbtlRefSbtl51Data7Entry3;
extern f32 sbtlRefSbtl51Data7Entry4;
extern f32 sbtlRefSbtl51Data7Entry5;
extern f32 sbtlRefSbtl51Data7Entry6;
extern f32 sbtlRefSbtl51Data7Entry7;
extern f32 sbtlRefSbtl51Data7Entry8;
extern f32 sbtlRefSbtl51Data7Entry9;
extern char sndInitDataEntry2[];
extern char sndInitDataEntry3[];
extern f32 sndInitDataEntry4;
extern f32 sndInitDataEntry5;
extern f32 sndInitDataEntry6;
extern u8 sysBATTLEONLYDataEntry100;
extern u8 sysBATTLEONLYDataEntry101;
extern u8 sysBATTLEONLYDataEntry102;
extern u32 sysBATTLEONLYDataEntry103;
extern s32 sysBATTLEONLYDataEntry104;
extern s32 sysBATTLEONLYDataEntry105;
extern s32 sysBATTLEONLYDataEntry106;
extern s32 sysBATTLEONLYDataEntry107;
extern s32 sysBATTLEONLYDataEntry108;
extern u8 sysBATTLEONLYDataEntry109;
extern u8 sysBATTLEONLYDataEntry110;
extern s32 sysBATTLEONLYDataEntry111;
extern s32 sysBATTLEONLYDataEntry112;
extern s32 sysBATTLEONLYDataEntry113;
extern s32 sysBATTLEONLYDataEntry114;
extern f32 sysBATTLEONLYDataEntry115;
extern f32 sysBATTLEONLYDataEntry116;
extern s32 sysBATTLEONLYDataEntry117;
extern u16 sysBATTLEONLYDataEntry118;
extern s32 sysBATTLEONLYDataEntry119;
extern s32 sysBATTLEONLYDataEntry120;
extern s32 sysBATTLEONLYDataEntry121;
extern s32 sysBATTLEONLYDataEntry122;
extern s32 sysBATTLEONLYDataEntry123;
extern u32 sysBATTLEONLYDataEntry124;
extern u32 sysBATTLEONLYDataEntry125;
extern s32 sysBATTLEONLYDataEntry126;
extern u8 sysBATTLEONLYDataEntry127;
extern u8 sysBATTLEONLYDataEntry128;
extern s32 sysBATTLEONLYDataEntry129;
extern s32 sysBATTLEONLYDataEntry130;
extern s32 sysBATTLEONLYDataEntry131;
extern s32 sysBATTLEONLYDataEntry132;
extern u32 sysBATTLEONLYDataEntry133;
extern s32 sysBATTLEONLYDataEntry134;
extern char sysBATTLEONLYDataEntry22[];
extern char sysBATTLEONLYDataEntry23[];
extern char sysBATTLEONLYDataEntry24[];
extern char sysBATTLEONLYDataEntry25[];
extern char sysBATTLEONLYDataEntry26[];
extern char sysBATTLEONLYDataEntry27[];
extern char sysBATTLEONLYDataEntry28[];
extern char sysBATTLEONLYDataEntry29[];
extern char sysBATTLEONLYDataEntry30[];
extern u32 sysBATTLEONLYDataEntry31;
extern s32 sysBATTLEONLYDataEntry32;
extern s32 sysBATTLEONLYDataEntry33;
extern char sysBATTLEONLYDataEntry49[];
extern char sysBATTLEONLYDataEntry50[];
extern char sysBATTLEONLYDataEntry51[];
extern char sysBATTLEONLYDataEntry52[];
extern char sysBATTLEONLYDataEntry53[];
extern char sysBATTLEONLYDataEntry54[];
extern char sysBATTLEONLYDataEntry55[];
extern char sysBATTLEONLYDataEntry56[];
extern char sysBATTLEONLYDataEntry57[];
extern char sysBATTLEONLYDataEntry58[];
extern char sysBATTLEONLYDataEntry59[];
extern char sysBATTLEONLYDataEntry60[];
extern char sysBATTLEONLYDataEntry61[];
extern char sysBATTLEONLYDataEntry62[];
extern char sysBATTLEONLYDataEntry63[];
extern char sysBATTLEONLYDataEntry64[];
extern char sysBATTLEONLYDataEntry65[];
extern char sysBATTLEONLYDataEntry66[];
extern char sysBATTLEONLYDataEntry67[];
extern char sysBATTLEONLYDataEntry68[];
extern char sysBATTLEONLYDataEntry69[];
extern char sysBATTLEONLYDataEntry70[];
extern char sysBATTLEONLYDataEntry71[];
extern char sysBATTLEONLYDataEntry72[];
extern char sysBATTLEONLYDataEntry73[];
extern char sysBATTLEONLYDataEntry74[];
extern char sysBATTLEONLYDataEntry75[];
extern char sysBATTLEONLYDataEntry76[];
extern u16 sysBATTLEONLYDataEntry77;
extern u16 sysBATTLEONLYDataEntry78;
extern s32 sysBATTLEONLYDataEntry79;
extern u8 sysBATTLEONLYDataEntry80;
extern s32 sysBATTLEONLYDataEntry81;
extern s32 sysBATTLEONLYDataEntry82;
extern s32 sysBATTLEONLYDataEntry83;
extern s32 sysBATTLEONLYDataEntry84;
extern s32 sysBATTLEONLYDataEntry85;
extern s32 sysBATTLEONLYDataEntry86;
extern u32 sysBATTLEONLYDataEntry87;
extern u32 sysBATTLEONLYDataEntry88;
extern s32 sysBATTLEONLYDataEntry89;
extern u8 sysBATTLEONLYDataEntry90;
extern u8 sysBATTLEONLYDataEntry91;
extern u8 sysBATTLEONLYDataEntry92;
extern s32 sysBATTLEONLYDataEntry93;
extern s32 sysBATTLEONLYDataEntry94;
extern s32 sysBATTLEONLYDataEntry95;
extern u32 sysBATTLEONLYDataEntry96;
extern s32 sysBATTLEONLYDataEntry97;
extern u32 sysBATTLEONLYDataEntry98;
extern u32 sysBATTLEONLYDataEntry99;
extern char sysPARTICLEDataEntry10[];
extern char sysPARTICLEDataEntry110[];
extern char sysPARTICLEDataEntry111[];
extern char sysPARTICLEDataEntry112[];
extern char sysPARTICLEDataEntry113[];
extern s32 sysPARTICLEDataEntry114;
extern s32 sysPARTICLEDataEntry115;
extern f32 sysPARTICLEDataEntry116;
extern s32 sysPARTICLEDataEntry117;
extern s32 sysPARTICLEDataEntry118;
extern f32 sysPARTICLEDataEntry119;
extern char sysPARTICLEDataEntry11[];
extern f32 sysPARTICLEDataEntry120;
extern f32 sysPARTICLEDataEntry121;
extern f32 sysPARTICLEDataEntry122;
extern f32 sysPARTICLEDataEntry123;
extern f32 sysPARTICLEDataEntry124;
extern f32 sysPARTICLEDataEntry125;
extern u8 sysPARTICLEDataEntry126[];
extern s32 sysPARTICLEDataEntry127;
extern s32 sysPARTICLEDataEntry128;
extern f32 sysPARTICLEDataEntry129;
extern char sysPARTICLEDataEntry12[];
extern s32 sysPARTICLEDataEntry130;
extern s32 sysPARTICLEDataEntry131;
extern f32 sysPARTICLEDataEntry132;
extern f32 sysPARTICLEDataEntry133;
extern s32 sysPARTICLEDataEntry134;
extern s32 sysPARTICLEDataEntry135;
extern f32 sysPARTICLEDataEntry136;
extern s32 sysPARTICLEDataEntry137;
extern s32 sysPARTICLEDataEntry138;
extern f32 sysPARTICLEDataEntry139;
extern char sysPARTICLEDataEntry13[];
extern f32 sysPARTICLEDataEntry140;
extern f32 sysPARTICLEDataEntry141;
extern f32 sysPARTICLEDataEntry142;
extern f32 sysPARTICLEDataEntry143;
extern f32 sysPARTICLEDataEntry144;
extern f32 sysPARTICLEDataEntry145;
extern f32 sysPARTICLEDataEntry146;
extern s32 sysPARTICLEDataEntry147;
extern s32 sysPARTICLEDataEntry148;
extern f32 sysPARTICLEDataEntry149;
extern char sysPARTICLEDataEntry14[];
extern s32 sysPARTICLEDataEntry150;
extern s32 sysPARTICLEDataEntry151;
extern f32 sysPARTICLEDataEntry152;
extern f32 sysPARTICLEDataEntry153;
extern f32 sysPARTICLEDataEntry154;
extern f32 sysPARTICLEDataEntry155;
extern s32 sysPARTICLEDataEntry156;
extern s32 sysPARTICLEDataEntry157;
extern f32 sysPARTICLEDataEntry158;
extern s32 sysPARTICLEDataEntry159;
extern char sysPARTICLEDataEntry15[];
extern s32 sysPARTICLEDataEntry160;
extern f32 sysPARTICLEDataEntry161;
extern f32 sysPARTICLEDataEntry162;
extern f32 sysPARTICLEDataEntry163;
extern f32 sysPARTICLEDataEntry164;
extern s32 sysPARTICLEDataEntry165;
extern s32 sysPARTICLEDataEntry166;
extern f32 sysPARTICLEDataEntry167;
extern s32 sysPARTICLEDataEntry168;
extern s32 sysPARTICLEDataEntry169;
extern u8 sysPARTICLEDataEntry16[];
extern f32 sysPARTICLEDataEntry170;
extern f32 sysPARTICLEDataEntry171;
extern f32 sysPARTICLEDataEntry172;
extern f32 sysPARTICLEDataEntry173;
extern s32 sysPARTICLEDataEntry174;
extern u8 sysPARTICLEDataEntry17[];
extern u8 sysPARTICLEDataEntry18[];
extern s32 sysPARTICLEDataEntry27;
extern u8 sysPARTICLEDataEntry28;
extern u8 sysPARTICLEDataEntry29;
extern f32 sysPARTICLEDataEntry30;
extern f32 sysPARTICLEDataEntry31;
extern f32 sysPARTICLEDataEntry32;
extern s32 sysPARTICLEDataEntry33;
extern u8 sysPARTICLEDataEntry34;
extern u8 sysPARTICLEDataEntry35;
extern f32 sysPARTICLEDataEntry36;
extern f32 sysPARTICLEDataEntry37;
extern f32 sysPARTICLEDataEntry38;
extern f32 sysPARTICLEDataEntry39;
extern u8 sysPARTICLEDataEntry40[];
extern s32 sysPARTICLEDataEntry41;
extern u8 sysPARTICLEDataEntry42;
extern u8 sysPARTICLEDataEntry43;
extern f32 sysPARTICLEDataEntry44;
extern f32 sysPARTICLEDataEntry45;
extern f32 sysPARTICLEDataEntry46;
extern s32 sysPARTICLEDataEntry47;
extern u8 sysPARTICLEDataEntry48;
extern u8 sysPARTICLEDataEntry49;
extern f32 sysPARTICLEDataEntry50;
extern f32 sysPARTICLEDataEntry51;
extern f32 sysPARTICLEDataEntry52;
extern f32 sysPARTICLEDataEntry53;
extern u8 sysPARTICLEDataEntry54[];
extern s32 sysPARTICLEDataEntry55;
extern u8 sysPARTICLEDataEntry56;
extern u8 sysPARTICLEDataEntry57;
extern f32 sysPARTICLEDataEntry58;
extern f32 sysPARTICLEDataEntry59;
extern f32 sysPARTICLEDataEntry60;
extern f32 sysPARTICLEDataEntry61;
extern s32 sysPARTICLEDataEntry62;
extern u8 sysPARTICLEDataEntry63;
extern u8 sysPARTICLEDataEntry64;
extern f32 sysPARTICLEDataEntry65;
extern f32 sysPARTICLEDataEntry66;
extern f32 sysPARTICLEDataEntry67;
extern f32 sysPARTICLEDataEntry68;
extern char sysPOLYTEXDataEntry10[];
extern char sysPOLYTEXDataEntry11[];
extern u8 sysPOLYTEXDataEntry12[];
extern s32 sysPOLYTEXDataEntry13;
extern f32 sysPOLYTEXDataEntry14;
extern u8 sysPOLYTEXDataEntry15;
extern u8 sysPOLYTEXDataEntry16[];
extern s32 sysPOLYTEXDataEntry17;
extern s32 sysPOLYTEXDataEntry18;
extern s32 sysPOLYTEXDataEntry19;
extern f32 sysPOLYTEXDataEntry20;
extern u8 sysPOLYTEXDataEntry21;
extern char sysPOLYTEXDataEntry3[];
extern char sysPOLYTEXDataEntry8[];
extern char sysPOLYTEXDataEntry9[];
extern f32 sysPOLYWINDDataEntry100;
extern f32 sysPOLYWINDDataEntry101;
extern f32 sysPOLYWINDDataEntry102;
extern f32 sysPOLYWINDDataEntry103;
extern f32 sysPOLYWINDDataEntry104;
extern f32 sysPOLYWINDDataEntry105;
extern f32 sysPOLYWINDDataEntry106;
extern f32 sysPOLYWINDDataEntry107;
extern u8 sysPOLYWINDDataEntry108;
extern f32 sysPOLYWINDDataEntry109;
extern f32 sysPOLYWINDDataEntry110;
extern f32 sysPOLYWINDDataEntry111;
extern f32 sysPOLYWINDDataEntry112;
extern f32 sysPOLYWINDDataEntry113;
extern f32 sysPOLYWINDDataEntry114;
extern f32 sysPOLYWINDDataEntry115;
extern f32 sysPOLYWINDDataEntry116;
extern f32 sysPOLYWINDDataEntry117;
extern f32 sysPOLYWINDDataEntry118;
extern f32 sysPOLYWINDDataEntry119;
extern f32 sysPOLYWINDDataEntry120;
extern f32 sysPOLYWINDDataEntry121;
extern f32 sysPOLYWINDDataEntry122;
extern u8 sysPOLYWINDDataEntry123[];
extern s32 sysPOLYWINDDataEntry124;
extern f32 sysPOLYWINDDataEntry125;
extern f32 sysPOLYWINDDataEntry126;
extern f32 sysPOLYWINDDataEntry127;
extern f32 sysPOLYWINDDataEntry128;
extern f32 sysPOLYWINDDataEntry129;
extern f32 sysPOLYWINDDataEntry130;
extern f32 sysPOLYWINDDataEntry131;
extern f32 sysPOLYWINDDataEntry132;
extern f32 sysPOLYWINDDataEntry133;
extern f32 sysPOLYWINDDataEntry134;
extern f32 sysPOLYWINDDataEntry135;
extern f32 sysPOLYWINDDataEntry136;
extern f32 sysPOLYWINDDataEntry137;
extern f32 sysPOLYWINDDataEntry138;
extern f32 sysPOLYWINDDataEntry139;
extern s32 sysPOLYWINDDataEntry13;
extern u8 sysPOLYWINDDataEntry140[];
extern s32 sysPOLYWINDDataEntry141;
extern f32 sysPOLYWINDDataEntry142;
extern f32 sysPOLYWINDDataEntry143;
extern f32 sysPOLYWINDDataEntry144;
extern f32 sysPOLYWINDDataEntry145;
extern f32 sysPOLYWINDDataEntry146;
extern f32 sysPOLYWINDDataEntry147;
extern f32 sysPOLYWINDDataEntry148;
extern f32 sysPOLYWINDDataEntry149;
extern s32 sysPOLYWINDDataEntry14;
extern f32 sysPOLYWINDDataEntry150;
extern u8 sysPOLYWINDDataEntry151;
extern f32 sysPOLYWINDDataEntry152;
extern f32 sysPOLYWINDDataEntry153;
extern f32 sysPOLYWINDDataEntry154;
extern f32 sysPOLYWINDDataEntry155;
extern u8 sysPOLYWINDDataEntry156[];
extern s32 sysPOLYWINDDataEntry157;
extern f32 sysPOLYWINDDataEntry158;
extern f32 sysPOLYWINDDataEntry159;
extern s32 sysPOLYWINDDataEntry15;
extern f32 sysPOLYWINDDataEntry160;
extern f32 sysPOLYWINDDataEntry161;
extern f32 sysPOLYWINDDataEntry162;
extern f32 sysPOLYWINDDataEntry163;
extern f32 sysPOLYWINDDataEntry164;
extern f32 sysPOLYWINDDataEntry165;
extern f32 sysPOLYWINDDataEntry166;
extern u8 sysPOLYWINDDataEntry167;
extern f32 sysPOLYWINDDataEntry168;
extern f32 sysPOLYWINDDataEntry169;
extern s32 sysPOLYWINDDataEntry16;
extern f32 sysPOLYWINDDataEntry170;
extern f32 sysPOLYWINDDataEntry171;
extern f32 sysPOLYWINDDataEntry172;
extern f32 sysPOLYWINDDataEntry173;
extern f32 sysPOLYWINDDataEntry174;
extern f32 sysPOLYWINDDataEntry175;
extern f32 sysPOLYWINDDataEntry176;
extern f32 sysPOLYWINDDataEntry177;
extern f32 sysPOLYWINDDataEntry178;
extern u8 sysPOLYWINDDataEntry17;
extern u8 sysPOLYWINDDataEntry18;
extern s32 sysPOLYWINDDataEntry19;
extern s32 sysPOLYWINDDataEntry20;
extern s32 sysPOLYWINDDataEntry21;
extern s32 sysPOLYWINDDataEntry22;
extern u8 sysPOLYWINDDataEntry23;
extern u8 sysPOLYWINDDataEntry24;
extern f32 sysPOLYWINDDataEntry25;
extern f32 sysPOLYWINDDataEntry26;
extern f32 sysPOLYWINDDataEntry27;
extern s32 sysPOLYWINDDataEntry28;
extern s32 sysPOLYWINDDataEntry29;
extern s32 sysPOLYWINDDataEntry30;
extern s32 sysPOLYWINDDataEntry31;
extern u8 sysPOLYWINDDataEntry32;
extern u8 sysPOLYWINDDataEntry33;
extern f32 sysPOLYWINDDataEntry34;
extern f32 sysPOLYWINDDataEntry35;
extern f32 sysPOLYWINDDataEntry36;
extern s32 sysPOLYWINDDataEntry37;
extern s32 sysPOLYWINDDataEntry38;
extern s32 sysPOLYWINDDataEntry39;
extern s32 sysPOLYWINDDataEntry40;
extern u8 sysPOLYWINDDataEntry41;
extern u8 sysPOLYWINDDataEntry42;
extern f32 sysPOLYWINDDataEntry43;
extern f32 sysPOLYWINDDataEntry44;
extern f32 sysPOLYWINDDataEntry45;
extern char sysPOLYWINDDataEntry6[];
extern char sysPOLYWINDDataEntry7[];
extern char sysPOLYWINDDataEntry87[];
extern char sysPOLYWINDDataEntry88[];
extern char sysPOLYWINDDataEntry89[];
extern u8 sysPOLYWINDDataEntry8[];
extern char sysPOLYWINDDataEntry90[];
extern char sysPOLYWINDDataEntry91[];
extern char sysPOLYWINDDataEntry92[];
extern char sysPOLYWINDDataEntry93[];
extern char sysPOLYWINDDataEntry94[];
extern char sysPOLYWINDDataEntry95[];
extern char sysPOLYWINDDataEntry96[];
extern char sysPOLYWINDDataEntry97[];
extern s32 sysPOLYWINDDataEntry98;
extern f32 sysPOLYWINDDataEntry99;
extern char sysStrAccurate[];
extern char sysStrCycleRot[];
extern char sysStrDirection[];
extern char sysStrEmtCurve[];
extern char sysStrEmtHeight[];
extern char sysStrEmtRadius[];
extern char sysStrLength[];
extern char sysStrLifeCol[];
extern char sysStrLifeScl[];
extern char sysStrMaterial[];
extern char sysStrOLightS[];
extern char sysStrOMcolrS[];
extern char sysStrResource[];
extern char sysStrTBoard2[];
extern char sysStrToolEffectSampleNCyldr0[];
extern char sysStrToolEffectSampleNCylnd0[];
extern char sysStrToolEffectSampleNCylnd1[];
extern char sysStrToolEffectSampleNSphre0[];
extern char sysStrToolEffectSampleNSpirl0[];
extern char sysStrToolEffectSampleNSpirl1[];
extern char sysStrToolEffectSampleNSpirl2[];
extern char sysStrToolEffectSampleOEffL0Ep[];
extern char sysStrToolEffectSampleOFldF0Ep[];
extern char sysStrToolEffectSampleOFldL0Ep[];
extern char sysStrToolEffectSampleOFovy0Ep[];
extern char sysStrToolEffectSampleOLight0[];
extern char sysStrToolEffectSampleOLight2[];
extern char sysStrToolEffectSampleOMcolr0[];
extern char sysStrToolEffectSampleOMcolr2[];
extern char sysStrToolEffectSampleOMot0Ep[];
extern char sysStrToolEffectSampleOSe0Ep[];
extern char sysStrToolEffectSampleOUnitd0[];
extern char sysStrToolEffectSampleOVibe0Ep[];
extern char sysStrToolEffectSamplePAspak0[];
extern char sysStrToolEffectSamplePAspak1[];
extern char sysStrToolEffectSamplePHspak0[];
extern char sysStrToolEffectSamplePHspak1[];
extern char sysStrToolEffectSamplePSmoke0[];
extern char sysStrToolEffectSamplePSmoke1[];
extern char sysStrToolEffectSamplePSmoke2[];
extern char sysStrToolEffectSamplePSpark0[];
extern char sysStrToolEffectSamplePSpark1[];
extern char sysStrToolEffectSamplePSpark2[];
extern char sysStrToolEffectSamplePSphre0[];
extern char sysStrToolEffectSamplePSphre1[];
extern char sysStrToolEffectSamplePSpirl0[];
extern char sysStrToolEffectSamplePSpirl1[];
extern char sysStrToolEffectSampleTBoard0[];
extern char sysStrToolEffectSampleTBoard1[];

extern EffFileJobRequest sysPARTICLEDataEntry;
extern effDebugMenuItem_t sysPARTICLEDataEntry89[];
extern effDebugMenu_t sysPARTICLEDataEntry69;
extern effDebugMenuItem_t sysPARTICLEDataEntry90[];
extern effDebugMenu_t sysPARTICLEDataEntry70;
extern effDebugMenuItem_t sysPARTICLEDataEntry91[];
extern effDebugMenu_t sysPARTICLEDataEntry71;
extern effDebugMenuItem_t sysPARTICLEDataEntry92[];
extern effDebugMenu_t sysPARTICLEDataEntry72;
extern effDebugMenuItem_t sysPARTICLEDataEntry19[];
extern effDebugMenu_t sysPARTICLEDataEntry2;
extern EffFileJobRequest sysPARTICLEDataEntry93;
extern effDebugMenuItem_t sysPARTICLEDataEntry94[];
extern effDebugMenu_t sysPARTICLEDataEntry73;
extern effDebugMenuItem_t sysPARTICLEDataEntry95[];
extern effDebugMenu_t sysPARTICLEDataEntry74;
extern effDebugMenuItem_t sysPARTICLEDataEntry96[];
extern effDebugMenu_t sysPARTICLEDataEntry75;
extern effDebugMenuItem_t sysPARTICLEDataEntry20[];
extern effDebugMenu_t sysPARTICLEDataEntry3;
extern EffFileJobRequest sysPARTICLEDataEntry21;
extern effDebugMenuItem_t sysPARTICLEDataEntry97[];
extern effDebugMenu_t sysPARTICLEDataEntry76;
extern effDebugMenuItem_t sysPARTICLEDataEntry98[];
extern effDebugMenu_t sysPARTICLEDataEntry77;
extern effDebugMenuItem_t sysPARTICLEDataEntry99[];
extern effDebugMenu_t sysPARTICLEDataEntry78;
extern effDebugMenuItem_t sysPARTICLEDataEntry100[];
extern effDebugMenu_t sysPARTICLEDataEntry79;
extern effDebugMenuItem_t sysPARTICLEDataEntry22[];
extern effDebugMenu_t sysPARTICLEDataEntry4;
extern EffFileJobRequest sysPARTICLEDataEntry5;
extern effDebugMenuItem_t sysPARTICLEDataEntry101[];
extern effDebugMenu_t sysPARTICLEDataEntry80;
extern effDebugMenuItem_t sysPARTICLEDataEntry102[];
extern effDebugMenu_t sysPARTICLEDataEntry81;
extern effDebugMenuItem_t sysPARTICLEDataEntry103[];
extern effDebugMenu_t sysPARTICLEDataEntry82;
extern effDebugMenuItem_t sysPARTICLEDataEntry23[];
extern effDebugMenu_t sysPARTICLEDataEntry6;
extern EffFileJobRequest sysPARTICLEDataEntry24;
extern effDebugMenuItem_t sysPARTICLEDataEntry104[];
extern effDebugMenu_t sysPARTICLEDataEntry83;
extern effDebugMenuItem_t sysPARTICLEDataEntry105[];
extern effDebugMenu_t sysPARTICLEDataEntry84;
extern effDebugMenuItem_t sysPARTICLEDataEntry106[];
extern effDebugMenu_t sysPARTICLEDataEntry85;
extern effDebugMenuItem_t sysPARTICLEDataEntry25[];
extern effDebugMenu_t sysPARTICLEDataEntry7;
extern EffFileJobRequest sysPARTICLEDataEntry8;
extern effDebugMenuItem_t sysPARTICLEDataEntry107[];
extern effDebugMenu_t sysPARTICLEDataEntry86;
extern effDebugMenuItem_t sysPARTICLEDataEntry108[];
extern effDebugMenu_t sysPARTICLEDataEntry87;
extern effDebugMenuItem_t sysPARTICLEDataEntry109[];
extern effDebugMenu_t sysPARTICLEDataEntry88;
extern effDebugMenuItem_t sysPARTICLEDataEntry26[];
extern effDebugMenu_t sysPARTICLEDataEntry9;
extern EffFileJobRequest effCreatePolyTrackControlTaskDataEntry13;
extern effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry63[];
extern effDebugMenu_t effCreatePolyTrackControlTaskDataEntry51;
extern effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry64[];
extern effDebugMenu_t effCreatePolyTrackControlTaskDataEntry52;
extern effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry65[];
extern effDebugMenu_t effCreatePolyTrackControlTaskDataEntry53;
extern effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry14[];
extern effDebugMenu_t effCreatePolyTrackControlTaskDataEntry;
extern EffFileJobRequest effCreatePolyTrackControlTaskDataEntry2;
extern effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry66[];
extern effDebugMenu_t effCreatePolyTrackControlTaskDataEntry54;
extern effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry67[];
extern effDebugMenu_t effCreatePolyTrackControlTaskDataEntry55;
extern effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry15[];
extern effDebugMenu_t effCreatePolyTrackControlTaskDataEntry3;
extern EffFileJobRequest effCreatePolyTrackControlTaskDataEntry68;
extern effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry69[];
extern effDebugMenu_t effCreatePolyTrackControlTaskDataEntry56;
extern effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry70[];
extern effDebugMenu_t effCreatePolyTrackControlTaskDataEntry57;
extern effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry71[];
extern effDebugMenu_t effCreatePolyTrackControlTaskDataEntry58;
extern effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry16[];
extern effDebugMenu_t effCreatePolyTrackControlTaskDataEntry4;
extern EffFileJobRequest effCreatePolyTrackControlTaskDataEntry17;
extern effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry72[];
extern effDebugMenu_t effCreatePolyTrackControlTaskDataEntry59;
extern effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry73[];
extern effDebugMenu_t effCreatePolyTrackControlTaskDataEntry60;
extern effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry18[];
extern effDebugMenu_t effCreatePolyTrackControlTaskDataEntry5;
extern EffFileJobRequest effCreatePolyTrackControlTaskDataEntry6;
extern effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry74[];
extern effDebugMenu_t effCreatePolyTrackControlTaskDataEntry61;
extern effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry75[];
extern effDebugMenu_t effCreatePolyTrackControlTaskDataEntry62;
extern effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry19[];
extern effDebugMenu_t effCreatePolyTrackControlTaskDataEntry7;
extern EffFileJobRequest effCreatePolyTrackTaskDataEntry66;
extern effDebugMenuItem_t effCreatePolyTrackTaskDataEntry67[];
extern effDebugMenu_t effCreatePolyTrackTaskDataEntry49;
extern effDebugMenuItem_t effCreatePolyTrackTaskDataEntry68[];
extern effDebugMenu_t effCreatePolyTrackTaskDataEntry50;
extern effDebugMenuItem_t effCreatePolyTrackTaskDataEntry69[];
extern effDebugMenu_t effCreatePolyTrackTaskDataEntry51;
extern effDebugMenuItem_t effCreatePolyTrackTaskDataEntry70[];
extern effDebugMenu_t effCreatePolyTrackTaskDataEntry52;
extern effDebugMenuItem_t effCreatePolyTrackTaskDataEntry8[];
extern effDebugMenu_t effCreatePolyTrackTaskDataEntry;
extern EffFileJobRequest effCreatePolyTrackTaskDataEntry71;
extern effDebugMenuItem_t effCreatePolyTrackTaskDataEntry72[];
extern effDebugMenu_t effCreatePolyTrackTaskDataEntry53;
extern effDebugMenuItem_t effCreatePolyTrackTaskDataEntry73[];
extern effDebugMenu_t effCreatePolyTrackTaskDataEntry54;
extern effDebugMenuItem_t effCreatePolyTrackTaskDataEntry74[];
extern effDebugMenu_t effCreatePolyTrackTaskDataEntry55;
extern effDebugMenuItem_t effCreatePolyTrackTaskDataEntry9[];
extern effDebugMenu_t effCreatePolyTrackTaskDataEntry2;
extern EffFileJobRequest effCreatePolyTrackTaskDataEntry75;
extern effDebugMenuItem_t effCreatePolyTrackTaskDataEntry76[];
extern effDebugMenu_t effCreatePolyTrackTaskDataEntry56;
extern effDebugMenuItem_t effCreatePolyTrackTaskDataEntry77[];
extern effDebugMenu_t effCreatePolyTrackTaskDataEntry57;
extern effDebugMenuItem_t effCreatePolyTrackTaskDataEntry78[];
extern effDebugMenu_t effCreatePolyTrackTaskDataEntry58;
extern effDebugMenuItem_t effCreatePolyTrackTaskDataEntry79[];
extern effDebugMenu_t effCreatePolyTrackTaskDataEntry59;
extern effDebugMenuItem_t effCreatePolyTrackTaskDataEntry10[];
extern effDebugMenu_t effCreatePolyTrackTaskDataEntry3;
extern EffFileJobRequest effCreatePolyTrackTaskDataEntry80;
extern effDebugMenuItem_t effCreatePolyTrackTaskDataEntry81[];
extern effDebugMenu_t effCreatePolyTrackTaskDataEntry60;
extern effDebugMenuItem_t effCreatePolyTrackTaskDataEntry82[];
extern effDebugMenu_t effCreatePolyTrackTaskDataEntry61;
extern effDebugMenuItem_t effCreatePolyTrackTaskDataEntry83[];
extern effDebugMenu_t effCreatePolyTrackTaskDataEntry62;
extern effDebugMenuItem_t effCreatePolyTrackTaskDataEntry11[];
extern effDebugMenu_t effCreatePolyTrackTaskDataEntry4;
extern EffFileJobRequest effCreatePolyTrackTaskDataEntry12;
extern effDebugMenuItem_t effCreatePolyTrackTaskDataEntry84[];
extern effDebugMenu_t effCreatePolyTrackTaskDataEntry63;
extern effDebugMenuItem_t effCreatePolyTrackTaskDataEntry85[];
extern effDebugMenu_t effCreatePolyTrackTaskDataEntry64;
extern effDebugMenuItem_t effCreatePolyTrackTaskDataEntry86[];
extern effDebugMenu_t effCreatePolyTrackTaskDataEntry65;
extern effDebugMenuItem_t effCreatePolyTrackTaskDataEntry13[];
extern effDebugMenu_t effCreatePolyTrackTaskDataEntry5;
extern EffFileJobRequest fileLoadEffectSlotAData;
extern effDebugMenuItem_t effData9Entry13[];
extern effDebugMenu_t effData9Entry7;
extern EffFileJobRequest effPollAndQueueCopiedFileResourceData;
extern effDebugMenuItem_t effData9Entry14[];
extern effDebugMenu_t effData9Entry8;
extern EffFileJobRequest effCreateBlurTaskDataEntry10;
extern effDebugMenuItem_t effCreateBlurTaskDataEntry11[];
extern effDebugMenu_t effCreateBlurTaskDataEntry;
extern EffFileJobRequest effCreateBlurTaskDataEntry45;
extern effDebugMenuItem_t effCreateBlurTaskDataEntry46[];
extern effDebugMenu_t effCreateBlurTaskDataEntry42;
extern effDebugMenuItem_t effCreateBlurTaskDataEntry12[];
extern effDebugMenu_t effCreateBlurTaskDataEntry2;
extern EffFileJobRequest effCreateBlurTaskDataEntry3;
extern effDebugMenuItem_t effCreateBlurTaskDataEntry47[];
extern effDebugMenu_t effCreateBlurTaskDataEntry43;
extern effDebugMenuItem_t effCreateBlurTaskDataEntry13[];
extern effDebugMenu_t effCreateBlurTaskDataEntry4;
extern EffFileJobRequest effCreateBlurTaskDataEntry14;
extern effDebugMenuItem_t effCreateBlurTaskDataEntry48[];
extern effDebugMenu_t effCreateBlurTaskDataEntry44;
extern effDebugMenuItem_t effCreateBlurTaskDataEntry15[];
extern effDebugMenu_t effCreateBlurTaskDataEntry5;
extern EffFileJobRequest effCreateBlurTaskDataEntry16;
extern effDebugMenuItem_t effCreateBlurTaskDataEntry17[];
extern effDebugMenu_t effCreateBlurTaskDataEntry6;
extern EffFileJobRequest effCreateFilterTaskDataEntry5;
extern effDebugMenuItem_t effCreateFilterTaskDataEntry6[];
extern effDebugMenu_t effCreateFilterTaskDataEntry;
extern EffFileJobRequest effCreateFilterTaskDataEntry7;
extern effDebugMenuItem_t effCreateFilterTaskDataEntry15[];
extern effDebugMenu_t effCreateFilterTaskDataEntry14;
extern effDebugMenuItem_t effCreateFilterTaskDataEntry8[];
extern effDebugMenu_t effCreateFilterTaskDataEntry2;
extern EffFileJobRequest effData9Entry15;
extern effDebugMenuItem_t effData9Entry39[];
extern effDebugMenu_t effData9Entry36;
extern effDebugMenuItem_t effData9Entry16[];
extern effDebugMenu_t effData9Entry9;
extern EffFileJobRequest sysPOLYTEXDataEntry4;
extern effDebugMenuItem_t sysPOLYTEXDataEntry24[];
extern effDebugMenu_t sysPOLYTEXDataEntry22;
extern effDebugMenuItem_t sysPOLYTEXDataEntry5[];
extern effDebugMenu_t sysPOLYTEXDataEntry;
extern EffFileJobRequest sysPOLYTEXDataEntry6;
extern effDebugMenuItem_t sysPOLYTEXDataEntry25[];
extern effDebugMenu_t sysPOLYTEXDataEntry23;
extern effDebugMenuItem_t sysPOLYTEXDataEntry7[];
extern effDebugMenu_t sysPOLYTEXDataEntry2;
extern EffFileJobRequest effCreateEnvironmentTaskDataEntry10;
extern effDebugMenuItem_t effCreateEnvironmentTaskDataEntry11[];
extern effDebugMenu_t effCreateEnvironmentTaskDataEntry;
extern EffFileJobRequest effCreateEnvironmentTaskDataEntry2;
extern effDebugMenuItem_t effCreateEnvironmentTaskDataEntry12[];
extern effDebugMenu_t effCreateEnvironmentTaskDataEntry3;
extern EffFileJobRequest effCreatePolyFlashTaskDataEntry12;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry124[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry103;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry13[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry;
extern EffFileJobRequest effCreatePolyFlashTaskDataEntry125;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry126[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry104;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry127[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry105;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry14[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry2;
extern EffFileJobRequest effCreatePolyFlashTaskDataEntry128;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry129[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry106;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry130[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry107;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry131[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry108;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry15[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry3;
extern EffFileJobRequest effCreatePolyFlashTaskDataEntry132;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry133[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry109;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry134[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry110;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry16[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry4;
extern EffFileJobRequest effCreatePolyFlashTaskDataEntry135;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry136[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry111;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry137[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry112;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry138[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry113;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry17[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry5;
extern EffFileJobRequest effCreatePolyFlashTaskDataEntry18;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry139[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry114;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry140[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry115;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry141[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry116;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry142[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry117;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry19[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry6;
extern EffFileJobRequest effCreatePolyFlashTaskDataEntry143;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry144[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry118;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry145[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry119;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry146[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry120;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry20[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry7;
extern EffFileJobRequest effCreatePolyFlashTaskDataEntry147;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry148[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry121;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry149[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry122;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry150[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry123;
extern effDebugMenuItem_t effCreatePolyFlashTaskDataEntry21[];
extern effDebugMenu_t effCreatePolyFlashTaskDataEntry8;
extern EffFileJobRequest effCreateEnvironmentTaskDataEntry13;
extern effDebugMenuItem_t effCreateEnvironmentTaskDataEntry14[];
extern effDebugMenu_t effCreateEnvironmentTaskDataEntry4;
extern EffFileJobRequest effCreateEnvironmentTaskDataEntry15;
extern effDebugMenuItem_t effCreateEnvironmentTaskDataEntry16[];
extern effDebugMenu_t effCreateEnvironmentTaskDataEntry5;
extern EffFileJobRequest effCreatePolyRingTaskDataEntry28;
extern effDebugMenuItem_t effCreatePolyRingTaskDataEntry29[];
extern effDebugMenu_t effCreatePolyRingTaskDataEntry25;
extern effDebugMenuItem_t effCreatePolyRingTaskDataEntry5[];
extern effDebugMenu_t effCreatePolyRingTaskDataEntry;
extern EffFileJobRequest effCreatePolyRingTaskDataEntry6;
extern effDebugMenuItem_t effCreatePolyRingTaskDataEntry30[];
extern effDebugMenu_t effCreatePolyRingTaskDataEntry26;
extern effDebugMenuItem_t effCreatePolyRingTaskDataEntry7[];
extern effDebugMenu_t effCreatePolyRingTaskDataEntry2;
extern EffFileJobRequest effCreatePolyRingTaskDataEntry31;
extern effDebugMenuItem_t effCreatePolyRingTaskDataEntry32[];
extern effDebugMenu_t effCreatePolyRingTaskDataEntry27;
extern effDebugMenuItem_t effCreatePolyRingTaskDataEntry8[];
extern effDebugMenu_t effCreatePolyRingTaskDataEntry3;
extern EffFileJobRequest effCreatePolyThunderTaskDataEntry;
extern effDebugMenuItem_t effCreatePolyThunderTaskDataEntry23[];
extern effDebugMenu_t effCreatePolyThunderTaskDataEntry20;
extern effDebugMenuItem_t effCreatePolyThunderTaskDataEntry6[];
extern effDebugMenu_t effCreatePolyThunderTaskDataEntry2;
extern EffFileJobRequest effCreatePolyThunderTaskDataEntry24;
extern effDebugMenuItem_t effCreatePolyThunderTaskDataEntry25[];
extern effDebugMenu_t effCreatePolyThunderTaskDataEntry21;
extern effDebugMenuItem_t effCreatePolyThunderTaskDataEntry26[];
extern effDebugMenu_t effCreatePolyThunderTaskDataEntry22;
extern effDebugMenuItem_t effCreatePolyThunderTaskDataEntry7[];
extern effDebugMenu_t effCreatePolyThunderTaskDataEntry3;
extern EffFileJobRequest effCreatePolyTwinkleTaskDataEntry62;
extern effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry63[];
extern effDebugMenu_t effCreatePolyTwinkleTaskDataEntry45;
extern effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry64[];
extern effDebugMenu_t effCreatePolyTwinkleTaskDataEntry46;
extern effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry65[];
extern effDebugMenu_t effCreatePolyTwinkleTaskDataEntry47;
extern effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry66[];
extern effDebugMenu_t effCreatePolyTwinkleTaskDataEntry48;
extern effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry12[];
extern effDebugMenu_t effCreatePolyTwinkleTaskDataEntry;
extern EffFileJobRequest effCreatePolyTwinkleTaskDataEntry67;
extern effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry68[];
extern effDebugMenu_t effCreatePolyTwinkleTaskDataEntry49;
extern effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry69[];
extern effDebugMenu_t effCreatePolyTwinkleTaskDataEntry50;
extern effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry70[];
extern effDebugMenu_t effCreatePolyTwinkleTaskDataEntry51;
extern effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry13[];
extern effDebugMenu_t effCreatePolyTwinkleTaskDataEntry2;
extern EffFileJobRequest effCreatePolyTwinkleTaskDataEntry71;
extern effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry72[];
extern effDebugMenu_t effCreatePolyTwinkleTaskDataEntry52;
extern effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry73[];
extern effDebugMenu_t effCreatePolyTwinkleTaskDataEntry53;
extern effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry74[];
extern effDebugMenu_t effCreatePolyTwinkleTaskDataEntry54;
extern effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry75[];
extern effDebugMenu_t effCreatePolyTwinkleTaskDataEntry55;
extern effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry14[];
extern effDebugMenu_t effCreatePolyTwinkleTaskDataEntry3;
extern EffFileJobRequest effCreatePolyTwinkleTaskDataEntry76;
extern effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry77[];
extern effDebugMenu_t effCreatePolyTwinkleTaskDataEntry56;
extern effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry78[];
extern effDebugMenu_t effCreatePolyTwinkleTaskDataEntry57;
extern effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry79[];
extern effDebugMenu_t effCreatePolyTwinkleTaskDataEntry58;
extern effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry15[];
extern effDebugMenu_t effCreatePolyTwinkleTaskDataEntry4;
extern EffFileJobRequest effCreatePolyTwinkleTaskDataEntry5;
extern effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry80[];
extern effDebugMenu_t effCreatePolyTwinkleTaskDataEntry59;
extern effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry81[];
extern effDebugMenu_t effCreatePolyTwinkleTaskDataEntry60;
extern effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry82[];
extern effDebugMenu_t effCreatePolyTwinkleTaskDataEntry61;
extern effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry16[];
extern effDebugMenu_t effCreatePolyTwinkleTaskDataEntry6;
extern EffFileJobRequest sysPOLYWINDDataEntry;
extern effDebugMenuItem_t sysPOLYWINDDataEntry65[];
extern effDebugMenu_t sysPOLYWINDDataEntry46;
extern effDebugMenuItem_t sysPOLYWINDDataEntry66[];
extern effDebugMenu_t sysPOLYWINDDataEntry47;
extern effDebugMenuItem_t sysPOLYWINDDataEntry67[];
extern effDebugMenu_t sysPOLYWINDDataEntry48;
extern effDebugMenuItem_t sysPOLYWINDDataEntry68[];
extern effDebugMenu_t sysPOLYWINDDataEntry49;
extern effDebugMenuItem_t sysPOLYWINDDataEntry69[];
extern effDebugMenu_t sysPOLYWINDDataEntry50;
extern effDebugMenuItem_t sysPOLYWINDDataEntry70[];
extern effDebugMenu_t sysPOLYWINDDataEntry51;
extern effDebugMenuItem_t sysPOLYWINDDataEntry9[];
extern effDebugMenu_t sysPOLYWINDDataEntry2;
extern EffFileJobRequest sysPOLYWINDDataEntry71;
extern effDebugMenuItem_t sysPOLYWINDDataEntry72[];
extern effDebugMenu_t sysPOLYWINDDataEntry52;
extern effDebugMenuItem_t sysPOLYWINDDataEntry73[];
extern effDebugMenu_t sysPOLYWINDDataEntry53;
extern effDebugMenuItem_t sysPOLYWINDDataEntry74[];
extern effDebugMenu_t sysPOLYWINDDataEntry54;
extern effDebugMenuItem_t sysPOLYWINDDataEntry75[];
extern effDebugMenu_t sysPOLYWINDDataEntry55;
extern effDebugMenuItem_t sysPOLYWINDDataEntry76[];
extern effDebugMenu_t sysPOLYWINDDataEntry56;
extern effDebugMenuItem_t sysPOLYWINDDataEntry10[];
extern effDebugMenu_t sysPOLYWINDDataEntry3;
extern EffFileJobRequest sysPOLYWINDDataEntry77;
extern effDebugMenuItem_t sysPOLYWINDDataEntry78[];
extern effDebugMenu_t sysPOLYWINDDataEntry57;
extern effDebugMenuItem_t sysPOLYWINDDataEntry79[];
extern effDebugMenu_t sysPOLYWINDDataEntry58;
extern effDebugMenuItem_t sysPOLYWINDDataEntry80[];
extern effDebugMenu_t sysPOLYWINDDataEntry59;
extern effDebugMenuItem_t sysPOLYWINDDataEntry11[];
extern effDebugMenu_t sysPOLYWINDDataEntry4;
extern EffFileJobRequest sysPOLYWINDDataEntry81;
extern effDebugMenuItem_t sysPOLYWINDDataEntry82[];
extern effDebugMenu_t sysPOLYWINDDataEntry60;
extern effDebugMenuItem_t sysPOLYWINDDataEntry83[];
extern effDebugMenu_t sysPOLYWINDDataEntry61;
extern effDebugMenuItem_t sysPOLYWINDDataEntry84[];
extern effDebugMenu_t sysPOLYWINDDataEntry62;
extern effDebugMenuItem_t sysPOLYWINDDataEntry85[];
extern effDebugMenu_t sysPOLYWINDDataEntry63;
extern effDebugMenuItem_t sysPOLYWINDDataEntry86[];
extern effDebugMenu_t sysPOLYWINDDataEntry64;
extern effDebugMenuItem_t sysPOLYWINDDataEntry12[];
extern effDebugMenu_t sysPOLYWINDDataEntry5;
extern EffFileJobRequest effCreatePolyScalyTaskDataEntry44;
extern effDebugMenuItem_t effCreatePolyScalyTaskDataEntry45[];
extern effDebugMenu_t effCreatePolyScalyTaskDataEntry34;
extern effDebugMenuItem_t effCreatePolyScalyTaskDataEntry46[];
extern effDebugMenu_t effCreatePolyScalyTaskDataEntry35;
extern effDebugMenuItem_t effCreatePolyScalyTaskDataEntry47[];
extern effDebugMenu_t effCreatePolyScalyTaskDataEntry36;
extern effDebugMenuItem_t effCreatePolyScalyTaskDataEntry7[];
extern effDebugMenu_t effCreatePolyScalyTaskDataEntry;
extern EffFileJobRequest effCreatePolyScalyTaskDataEntry48;
extern effDebugMenuItem_t effCreatePolyScalyTaskDataEntry49[];
extern effDebugMenu_t effCreatePolyScalyTaskDataEntry37;
extern effDebugMenuItem_t effCreatePolyScalyTaskDataEntry50[];
extern effDebugMenu_t effCreatePolyScalyTaskDataEntry38;
extern effDebugMenuItem_t effCreatePolyScalyTaskDataEntry51[];
extern effDebugMenu_t effCreatePolyScalyTaskDataEntry39;
extern effDebugMenuItem_t effCreatePolyScalyTaskDataEntry8[];
extern effDebugMenu_t effCreatePolyScalyTaskDataEntry2;
extern EffFileJobRequest effCreatePolyScalyTaskDataEntry52;
extern effDebugMenuItem_t effCreatePolyScalyTaskDataEntry53[];
extern effDebugMenu_t effCreatePolyScalyTaskDataEntry40;
extern effDebugMenuItem_t effCreatePolyScalyTaskDataEntry54[];
extern effDebugMenu_t effCreatePolyScalyTaskDataEntry41;
extern effDebugMenuItem_t effCreatePolyScalyTaskDataEntry55[];
extern effDebugMenu_t effCreatePolyScalyTaskDataEntry42;
extern effDebugMenuItem_t effCreatePolyScalyTaskDataEntry9[];
extern effDebugMenu_t effCreatePolyScalyTaskDataEntry3;
extern EffFileJobRequest effCreatePolyScalyTaskDataEntry56;
extern effDebugMenuItem_t effCreatePolyScalyTaskDataEntry57[];
extern effDebugMenu_t effCreatePolyScalyTaskDataEntry43;
extern effDebugMenuItem_t effCreatePolyScalyTaskDataEntry10[];
extern effDebugMenu_t effCreatePolyScalyTaskDataEntry4;
extern EffFileJobRequest fileLoadEffectSlotBData;
extern effDebugMenuItem_t effData9Entry17[];
extern effDebugMenu_t effData9Entry10;
extern EffFileJobRequest effCreatePolyCrackTaskDataEntry10;
extern effDebugMenuItem_t effCreatePolyCrackTaskDataEntry11[];
extern effDebugMenu_t effCreatePolyCrackTaskDataEntry7;
extern effDebugMenuItem_t effCreatePolyCrackTaskDataEntry12[];
extern effDebugMenu_t effCreatePolyCrackTaskDataEntry8;
extern effDebugMenuItem_t datData29[];
extern effDebugMenu_t effCreatePolyCrackTaskDataEntry31;
extern effDebugMenuItem_t effCreatePolyCrackTaskDataEntry13[];
extern effDebugMenu_t effCreatePolyCrackTaskDataEntry9;
extern effDebugMenuItem_t effCreatePolyCrackTaskDataEntry2[];
extern effDebugMenu_t effCreatePolyCrackTaskDataEntry;
extern EffFileJobRequest effCreatePolyWindTaskDataEntry83;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry84[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry63;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry85[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry64;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry86[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry65;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry87[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry66;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry12[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry;
extern EffFileJobRequest effCreatePolyWindTaskDataEntry88;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry89[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry67;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry90[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry68;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry91[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry69;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry13[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry2;
extern EffFileJobRequest effCreatePolyWindTaskDataEntry14;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry92[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry70;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry93[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry71;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry94[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry72;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry95[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry73;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry15[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry3;
extern EffFileJobRequest effCreatePolyWindTaskDataEntry96;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry97[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry74;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry98[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry75;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry99[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry76;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry16[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry4;
extern EffFileJobRequest effCreatePolyWindTaskDataEntry100;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry101[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry77;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry102[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry78;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry103[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry79;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry17[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry5;
extern EffFileJobRequest effCreatePolyWindTaskDataEntry18;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry104[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry80;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry105[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry81;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry106[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry82;
extern effDebugMenuItem_t effCreatePolyWindTaskDataEntry19[];
extern effDebugMenu_t effCreatePolyWindTaskDataEntry6;
extern EffFileJobRequest sysBATTLEONLYDataEntry;
extern effDebugMenuItem_t sysBATTLEONLYDataEntry34[];
extern effDebugMenu_t sysBATTLEONLYDataEntry2;
extern EffFileJobRequest sysBATTLEONLYDataEntry3;
extern effDebugMenuItem_t sysBATTLEONLYDataEntry35[];
extern effDebugMenu_t sysBATTLEONLYDataEntry4;
extern EffFileJobRequest sysBATTLEONLYDataEntry5;
extern effDebugMenuItem_t sysBATTLEONLYDataEntry36[];
extern effDebugMenu_t sysBATTLEONLYDataEntry6;
extern EffFileJobRequest effLoadFileSlotF2Data;
extern effDebugMenuItem_t effData9Entry18[];
extern effDebugMenu_t effData9Entry11;
extern EffFileJobRequest sysBATTLEONLYDataEntry37;
extern effDebugMenuItem_t sysBATTLEONLYDataEntry38[];
extern effDebugMenu_t sysBATTLEONLYDataEntry7;
extern EffFileJobRequest sysBATTLEONLYDataEntry8;
extern effDebugMenuItem_t sysBATTLEONLYDataEntry39[];
extern effDebugMenu_t sysBATTLEONLYDataEntry9;
extern EffFileJobRequest sysBATTLEONLYDataEntry10;
extern effDebugMenuItem_t sysBATTLEONLYDataEntry40[];
extern effDebugMenu_t sysBATTLEONLYDataEntry11;
extern EffFileJobRequest sysBATTLEONLYDataEntry12;
extern effDebugMenuItem_t sysBATTLEONLYDataEntry41[];
extern effDebugMenu_t sysBATTLEONLYDataEntry13;
extern EffFileJobRequest sysBATTLEONLYDataEntry42;
extern effDebugMenuItem_t sysBATTLEONLYDataEntry43[];
extern effDebugMenu_t sysBATTLEONLYDataEntry14;
extern EffFileJobRequest sysBATTLEONLYDataEntry15;
extern effDebugMenuItem_t sysBATTLEONLYDataEntry44[];
extern effDebugMenu_t sysBATTLEONLYDataEntry16;
extern EffFileJobRequest effLoadMaterialFileData;
extern effDebugMenuItem_t effData9Entry19[];
extern effDebugMenu_t effData9Entry12;
extern EffFileJobRequest effCreateTwoDimensionalTaskDataEntry2;
extern effDebugMenuItem_t effCreateTwoDimensionalTaskDataEntry9[];
extern effDebugMenu_t effCreateTwoDimensionalTaskDataEntry8;
extern effDebugMenuItem_t effCreateTwoDimensionalTaskDataEntry3[];
extern effDebugMenu_t effCreateTwoDimensionalTaskDataEntry;
extern EffFileJobRequest sysBATTLEONLYDataEntry45;
extern effDebugMenuItem_t sysBATTLEONLYDataEntry46[];
extern effDebugMenu_t sysBATTLEONLYDataEntry17;
extern EffFileJobRequest sysBATTLEONLYDataEntry18;
extern effDebugMenuItem_t sysBATTLEONLYDataEntry47[];
extern effDebugMenu_t sysBATTLEONLYDataEntry19;
extern EffFileJobRequest sysBATTLEONLYDataEntry20;
extern effDebugMenuItem_t sysBATTLEONLYDataEntry48[];
extern effDebugMenu_t sysBATTLEONLYDataEntry21;
extern EffCameraCreateRow sysPARTICLEData[];
extern EffCameraCreateRow effCreatePolyTrackControlTaskData[];
extern EffCameraCreateRow effCreatePolyTrackTaskData[];
extern EffCameraCreateRow effData9Entry[];
extern EffCameraCreateRow effCreateBlurTaskData[];
extern EffCameraCreateRow effCreateFilterTaskData[];
extern EffCameraCreateRow effData9Entry2[];
extern EffCameraCreateRow sysPOLYTEXData[];
extern EffCameraCreateRow effCreateEnvironmentTaskData[];
extern EffCameraCreateRow effCreatePolyFlashTaskData[];
extern EffCameraCreateRow effCreatePolyRingTaskData[];
extern EffCameraCreateRow effCreatePolyThunderTaskData[];
extern EffCameraCreateRow effCreatePolyTwinkleTaskData[];
extern EffCameraCreateRow sysPOLYWINDData[];
extern EffCameraCreateRow effCreatePolyScalyTaskData[];
extern EffCameraCreateRow effData9Entry3[];
extern EffCameraCreateRow effCreatePolyCrackTaskData[];
extern EffCameraCreateRow sysBATTLEONLYData[];
extern EffCameraCreateRow effData9Entry4[];
extern EffCameraCreateRow effData9Entry5[];
extern EffCameraCreateRow effData9Entry6[];
extern EffCameraCreateRow effCreateTwoDimensionalTaskData[];
extern EffCameraCreateRow effCreatePolyWindTaskData[];

EffFileJobRequest sysPARTICLEDataEntry = {
    sysPARTICLEDataEntry10, 1, { 0 }, 1, { 0 }, sysPARTICLEDataEntry16, 0xF0,
};

effDebugMenuItem_t sysPARTICLEDataEntry89[2] = {
    { sysStrLength, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry117 }, { .minValue = 0 }, 0xA, 1.0f },
    { sysStrAccurate, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry118 }, { .minValue = 1 }, 8, 1.0f },
};

effDebugMenu_t sysPARTICLEDataEntry69 = {
    NULL, 0xB8, 8, 0, 0, sysPARTICLEDataEntry89, 2, &sysPARTICLEDataEntry,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPARTICLEDataEntry90[4] = {
    { sysStrResource, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollResourceQueueWithTlp }, { .release = effReleaseResourceBankDescriptors } },
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollTextureResourceFile }, { .release = effBattleResourceDescriptorRelease } },
    { sysPARTICLEDataEntry111, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollEpResourceFile }, { .release = effBattleResourceDescriptorRelease } },
    { sysPARTICLEDataEntry110, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollEplResourceFile }, { .release = effBattleResourceDescriptorRelease } },
};

effDebugMenu_t sysPARTICLEDataEntry70 = {
    NULL, 0xB8, 8, 0, 0, sysPARTICLEDataEntry90, 4, &sysPARTICLEDataEntry,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPARTICLEDataEntry91[5] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry114 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry115 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry116 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackControlTaskDataEntry76, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry119 }, { .minValue = 0 }, 0, 1.0f },
};

effDebugMenu_t sysPARTICLEDataEntry71 = {
    NULL, 0xB8, 8, 0, 0, sysPARTICLEDataEntry91, 5, &sysPARTICLEDataEntry,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPARTICLEDataEntry92[6] = {
    { effCreatePolyWindTaskDataEntry110, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry120 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry109, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry121 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyWindTaskDataEntry108, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry122 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry107, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry123 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry124 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry125 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t sysPARTICLEDataEntry72 = {
    NULL, 0xB8, 8, 0, 0, sysPARTICLEDataEntry92, 6, &sysPARTICLEDataEntry,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPARTICLEDataEntry19[17] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPARTICLEDataEntry71 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry27 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPARTICLEDataEntry28 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPARTICLEDataEntry29 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effMapObjectWithTemporaryMappingTable }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditPScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditPRotationTrack }, { .release = btlResetEffectWork } },
    { effLabelBlur, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPARTICLEDataEntry69 }, { .minValue = 0 }, 0, -123456.0f },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPARTICLEDataEntry70 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry30 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry31 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry32 }, { .minValue = 0 }, 0, 0.01f },
    { effStrAmplitude, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPARTICLEDataEntry72 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t sysPARTICLEDataEntry2 = {
    sysPARTICLEDataEntry10, 0x8, 8, 0, 0, sysPARTICLEDataEntry19, 17, &sysPARTICLEDataEntry,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest sysPARTICLEDataEntry93 = {
    sysPARTICLEDataEntry11, 1, { 0 }, 2, { 0 }, sysPARTICLEDataEntry126, 0xE0,
};

effDebugMenuItem_t sysPARTICLEDataEntry94[2] = {
    { sysStrLength, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry130 }, { .minValue = 0 }, 0xA, 1.0f },
    { sysStrAccurate, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry131 }, { .minValue = 1 }, 8, 1.0f },
};

effDebugMenu_t sysPARTICLEDataEntry73 = {
    NULL, 0xB8, 8, 0, 0, sysPARTICLEDataEntry94, 2, &sysPARTICLEDataEntry93,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPARTICLEDataEntry95[4] = {
    { sysStrResource, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollResourceQueueWithTlp }, { .release = effReleaseResourceBankDescriptors } },
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollTextureResourceFile }, { .release = effBattleResourceDescriptorRelease } },
    { sysPARTICLEDataEntry111, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollEpResourceFile }, { .release = effBattleResourceDescriptorRelease } },
    { sysPARTICLEDataEntry110, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollEplResourceFile }, { .release = effBattleResourceDescriptorRelease } },
};

effDebugMenu_t sysPARTICLEDataEntry74 = {
    NULL, 0xB8, 8, 0, 0, sysPARTICLEDataEntry95, 4, &sysPARTICLEDataEntry93,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPARTICLEDataEntry96[6] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry127 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry128 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry129 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackControlTaskDataEntry76, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry132 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry83, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry133 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t sysPARTICLEDataEntry75 = {
    NULL, 0xB8, 8, 0, 0, sysPARTICLEDataEntry96, 6, &sysPARTICLEDataEntry93,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPARTICLEDataEntry20[17] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPARTICLEDataEntry75 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry33 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPARTICLEDataEntry34 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPARTICLEDataEntry35 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effMapObjectWithTemporaryMappingTable }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditPScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditPRotationTrack }, { .release = btlResetEffectWork } },
    { effLabelBlur, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPARTICLEDataEntry73 }, { .minValue = 0 }, 0, -123456.0f },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPARTICLEDataEntry74 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry36 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry37 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry38 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry39 }, { .minValue = 0 }, 0, 0.01f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t sysPARTICLEDataEntry3 = {
    sysPARTICLEDataEntry11, 0x8, 8, 0, 0, sysPARTICLEDataEntry20, 17, &sysPARTICLEDataEntry93,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest sysPARTICLEDataEntry21 = {
    sysPARTICLEDataEntry12, 1, { 0 }, 3, { 0 }, sysPARTICLEDataEntry40, 0xF4,
};

effDebugMenuItem_t sysPARTICLEDataEntry97[2] = {
    { sysStrLength, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry137 }, { .minValue = 0 }, 0xA, 1.0f },
    { sysStrAccurate, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry138 }, { .minValue = 1 }, 8, 1.0f },
};

effDebugMenu_t sysPARTICLEDataEntry76 = {
    NULL, 0xB8, 8, 0, 0, sysPARTICLEDataEntry97, 2, &sysPARTICLEDataEntry21,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPARTICLEDataEntry98[4] = {
    { sysStrResource, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollResourceQueueWithTlp }, { .release = effReleaseResourceBankDescriptors } },
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollTextureResourceFile }, { .release = effBattleResourceDescriptorRelease } },
    { sysPARTICLEDataEntry111, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollEpResourceFile }, { .release = effBattleResourceDescriptorRelease } },
    { sysPARTICLEDataEntry110, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollEplResourceFile }, { .release = effBattleResourceDescriptorRelease } },
};

effDebugMenu_t sysPARTICLEDataEntry77 = {
    NULL, 0xB8, 8, 0, 0, sysPARTICLEDataEntry98, 4, &sysPARTICLEDataEntry21,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPARTICLEDataEntry99[9] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry134 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry135 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry136 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackControlTaskDataEntry79, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry139 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry30, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry140 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry29, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry141 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyThunderTaskDataEntry28, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry142 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry27, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry143 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t sysPARTICLEDataEntry78 = {
    NULL, 0xB8, 8, 0, 0, sysPARTICLEDataEntry99, 9, &sysPARTICLEDataEntry21,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPARTICLEDataEntry100[3] = {
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry144 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry59, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry145 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry146 }, { .minValue = 0 }, 0, 0.001f },
};

effDebugMenu_t sysPARTICLEDataEntry79 = {
    NULL, 0xB8, 8, 0, 0, sysPARTICLEDataEntry100, 3, &sysPARTICLEDataEntry21,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPARTICLEDataEntry22[17] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPARTICLEDataEntry78 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry41 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPARTICLEDataEntry42 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPARTICLEDataEntry43 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effMapObjectWithTemporaryMappingTable }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditPScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditPRotationTrack }, { .release = btlResetEffectWork } },
    { effLabelBlur, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPARTICLEDataEntry76 }, { .minValue = 0 }, 0, -123456.0f },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPARTICLEDataEntry77 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry44 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry45 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry46 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPARTICLEDataEntry79 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t sysPARTICLEDataEntry4 = {
    sysPARTICLEDataEntry12, 0x8, 8, 0, 0, sysPARTICLEDataEntry22, 17, &sysPARTICLEDataEntry21,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest sysPARTICLEDataEntry5 = {
    sysPARTICLEDataEntry13, 1, { 0 }, 4, { 0 }, sysPARTICLEDataEntry17, 0xE8,
};

effDebugMenuItem_t sysPARTICLEDataEntry101[2] = {
    { sysStrLength, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry150 }, { .minValue = 0 }, 0xA, 1.0f },
    { sysStrAccurate, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry151 }, { .minValue = 1 }, 8, 1.0f },
};

effDebugMenu_t sysPARTICLEDataEntry80 = {
    NULL, 0xB8, 8, 0, 0, sysPARTICLEDataEntry101, 2, &sysPARTICLEDataEntry5,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPARTICLEDataEntry102[4] = {
    { sysStrResource, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollResourceQueueWithTlp }, { .release = effReleaseResourceBankDescriptors } },
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollTextureResourceFile }, { .release = effBattleResourceDescriptorRelease } },
    { sysPARTICLEDataEntry111, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollEpResourceFile }, { .release = effBattleResourceDescriptorRelease } },
    { sysPARTICLEDataEntry110, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollEplResourceFile }, { .release = effBattleResourceDescriptorRelease } },
};

effDebugMenu_t sysPARTICLEDataEntry81 = {
    NULL, 0xB8, 8, 0, 0, sysPARTICLEDataEntry102, 4, &sysPARTICLEDataEntry5,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPARTICLEDataEntry103[8] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry147 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry148 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry149 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyThunderTaskDataEntry30, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry152 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry29, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry153 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyThunderTaskDataEntry28, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry154 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry27, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry155 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t sysPARTICLEDataEntry82 = {
    NULL, 0xB8, 8, 0, 0, sysPARTICLEDataEntry103, 8, &sysPARTICLEDataEntry5,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPARTICLEDataEntry23[17] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPARTICLEDataEntry82 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry47 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPARTICLEDataEntry48 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPARTICLEDataEntry49 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effMapObjectWithTemporaryMappingTable }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditPScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditPRotationTrack }, { .release = btlResetEffectWork } },
    { effLabelBlur, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPARTICLEDataEntry80 }, { .minValue = 0 }, 0, -123456.0f },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPARTICLEDataEntry81 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry50 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry59, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry51 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry52 }, { .minValue = 0 }, 0, 0.001f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry53 }, { .minValue = 0 }, 0, 0.01f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t sysPARTICLEDataEntry6 = {
    sysPARTICLEDataEntry13, 0x8, 8, 0, 0, sysPARTICLEDataEntry23, 17, &sysPARTICLEDataEntry5,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest sysPARTICLEDataEntry24 = {
    sysPARTICLEDataEntry14, 1, { 0 }, 5, { 0 }, sysPARTICLEDataEntry54, 0xE8,
};

effDebugMenuItem_t sysPARTICLEDataEntry104[2] = {
    { sysStrLength, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry159 }, { .minValue = 0 }, 0xA, 1.0f },
    { sysStrAccurate, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry160 }, { .minValue = 1 }, 8, 1.0f },
};

effDebugMenu_t sysPARTICLEDataEntry83 = {
    NULL, 0xB8, 8, 0, 0, sysPARTICLEDataEntry104, 2, &sysPARTICLEDataEntry24,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPARTICLEDataEntry105[4] = {
    { sysStrResource, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollResourceQueueWithTlp }, { .release = effReleaseResourceBankDescriptors } },
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollTextureResourceFile }, { .release = effBattleResourceDescriptorRelease } },
    { sysPARTICLEDataEntry111, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollEpResourceFile }, { .release = effBattleResourceDescriptorRelease } },
    { sysPARTICLEDataEntry110, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollEplResourceFile }, { .release = effBattleResourceDescriptorRelease } },
};

effDebugMenu_t sysPARTICLEDataEntry84 = {
    NULL, 0xB8, 8, 0, 0, sysPARTICLEDataEntry105, 4, &sysPARTICLEDataEntry24,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPARTICLEDataEntry106[8] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry156 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry157 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry158 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackControlTaskDataEntry76, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry161 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry83, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry162 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyWindTaskDataEntry112, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry163 }, { .minValue = 0 }, 1, 0.1f },
    { effCreatePolyWindTaskDataEntry111, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry164 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t sysPARTICLEDataEntry85 = {
    NULL, 0xB8, 8, 0, 0, sysPARTICLEDataEntry106, 8, &sysPARTICLEDataEntry24,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPARTICLEDataEntry25[17] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPARTICLEDataEntry85 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry55 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPARTICLEDataEntry56 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPARTICLEDataEntry57 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effMapObjectWithTemporaryMappingTable }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditPScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditPRotationTrack }, { .release = btlResetEffectWork } },
    { effLabelBlur, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPARTICLEDataEntry83 }, { .minValue = 0 }, 0, -123456.0f },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPARTICLEDataEntry84 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry58 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry59 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry60 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry61 }, { .minValue = 0 }, 0, 0.01f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t sysPARTICLEDataEntry7 = {
    sysPARTICLEDataEntry14, 0x8, 8, 0, 0, sysPARTICLEDataEntry25, 17, &sysPARTICLEDataEntry24,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest sysPARTICLEDataEntry8 = {
    sysPARTICLEDataEntry15, 1, { 0 }, 6, { 0 }, sysPARTICLEDataEntry18, 0xEC,
};

effDebugMenuItem_t sysPARTICLEDataEntry107[2] = {
    { sysStrLength, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry168 }, { .minValue = 0 }, 0xA, 1.0f },
    { sysStrAccurate, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry169 }, { .minValue = 1 }, 8, 1.0f },
};

effDebugMenu_t sysPARTICLEDataEntry86 = {
    NULL, 0xB8, 8, 0, 0, sysPARTICLEDataEntry107, 2, &sysPARTICLEDataEntry8,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPARTICLEDataEntry108[4] = {
    { sysStrResource, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollResourceQueueWithTlp }, { .release = effReleaseResourceBankDescriptors } },
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollTextureResourceFile }, { .release = effBattleResourceDescriptorRelease } },
    { sysPARTICLEDataEntry111, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollEpResourceFile }, { .release = effBattleResourceDescriptorRelease } },
    { sysPARTICLEDataEntry110, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollEplResourceFile }, { .release = effBattleResourceDescriptorRelease } },
};

effDebugMenu_t sysPARTICLEDataEntry87 = {
    NULL, 0xB8, 8, 0, 0, sysPARTICLEDataEntry108, 4, &sysPARTICLEDataEntry8,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPARTICLEDataEntry109[9] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry165 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry166 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry167 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackControlTaskDataEntry76, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry170 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry83, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry171 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyWindTaskDataEntry112, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry172 }, { .minValue = 0 }, 1, 0.1f },
    { effCreatePolyWindTaskDataEntry111, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry173 }, { .minValue = 0 }, 1, 1.0f },
    { sysPARTICLEDataEntry113, 0x05, { 0 }, { .s32Value = &sysPARTICLEDataEntry174 }, { .minValue = 0 }, 0x168, 1.0f },
};

effDebugMenu_t sysPARTICLEDataEntry88 = {
    NULL, 0xB8, 8, 0, 0, sysPARTICLEDataEntry109, 9, &sysPARTICLEDataEntry8,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPARTICLEDataEntry26[17] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPARTICLEDataEntry88 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPARTICLEDataEntry62 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPARTICLEDataEntry63 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPARTICLEDataEntry64 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effMapObjectWithTemporaryMappingTable }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditPScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditPRotationTrack }, { .release = btlResetEffectWork } },
    { effLabelBlur, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPARTICLEDataEntry86 }, { .minValue = 0 }, 0, -123456.0f },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPARTICLEDataEntry87 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry65 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry66 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry67 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPARTICLEDataEntry68 }, { .minValue = 0 }, 0, 0.01f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t sysPARTICLEDataEntry9 = {
    sysPARTICLEDataEntry15, 0x8, 8, 0, 0, sysPARTICLEDataEntry26, 17, &sysPARTICLEDataEntry8,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyTrackControlTaskDataEntry13 = {
    effCreatePolyTrackControlTaskDataEntry20, 2, { 0 }, 1, { 0 }, (u8 *)&effCreatePolyTrackControlTaskDataEntry21, 0x10C,
};

effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry63[26] = {
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry21 }, { .minValue = 1 }, 0x64, 1.0f },
    { effCreatePolyScalyTaskDataEntry60, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry80 }, { .minValue = 1 }, 0xA, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcStartIn, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry81 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry81 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry81 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry81 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcStartOut, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry82 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry82 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry82 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry82 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcEndIn, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry83 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry83 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry83 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry83 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcEndOut, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry84 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry84 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry84 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry84 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyTrackControlTaskDataEntry51 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackControlTaskDataEntry63, 26, &effCreatePolyTrackControlTaskDataEntry13,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry64[5] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry85 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry86 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry87 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackControlTaskDataEntry76, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry88 }, { .minValue = 0 }, 0, 1.0f },
};

effDebugMenu_t effCreatePolyTrackControlTaskDataEntry52 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackControlTaskDataEntry64, 5, &effCreatePolyTrackControlTaskDataEntry13,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry65[6] = {
    { effCreatePolyWindTaskDataEntry110, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry89 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry109, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry90 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyWindTaskDataEntry108, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry91 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry107, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry92 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry93 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry94 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyTrackControlTaskDataEntry53 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackControlTaskDataEntry65, 6, &effCreatePolyTrackControlTaskDataEntry13,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry14[14] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTrackControlTaskDataEntry52 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry22 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyTrackControlTaskDataEntry23 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditGkColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditGkScaleTrack }, { .release = btlResetEffectWork } },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyTrackControlTaskDataEntry51 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry24 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry25 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry26 }, { .minValue = 0 }, 0, 0.01f },
    { effStrAmplitude, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTrackControlTaskDataEntry53 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyTrackControlTaskDataEntry = {
    effCreatePolyTrackControlTaskDataEntry20, 0x8, 8, 0, 0, effCreatePolyTrackControlTaskDataEntry14, 14, &effCreatePolyTrackControlTaskDataEntry13,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyTrackControlTaskDataEntry2 = {
    effCreatePolyTrackControlTaskDataEntry8, 2, { 0 }, 2, { 0 }, (u8 *)&effCreatePolyTrackControlTaskDataEntry12, 0xFC,
};

effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry66[26] = {
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry12 }, { .minValue = 1 }, 0x64, 1.0f },
    { effCreatePolyScalyTaskDataEntry60, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry95 }, { .minValue = 1 }, 0xA, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcStartIn, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry96 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry96 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry96 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry96 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcStartOut, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry97 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry97 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry97 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry97 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcEndIn, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry98 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry98 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry98 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry98 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcEndOut, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry99 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry99 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry99 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry99 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyTrackControlTaskDataEntry54 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackControlTaskDataEntry66, 26, &effCreatePolyTrackControlTaskDataEntry2,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry67[6] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry100 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry101 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry102 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackControlTaskDataEntry76, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry103 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry83, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry104 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyTrackControlTaskDataEntry55 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackControlTaskDataEntry67, 6, &effCreatePolyTrackControlTaskDataEntry2,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry15[14] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTrackControlTaskDataEntry55 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry27 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyTrackControlTaskDataEntry28 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditGkColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditGkScaleTrack }, { .release = btlResetEffectWork } },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyTrackControlTaskDataEntry54 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry29 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry30 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry31 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry32 }, { .minValue = 0 }, 0, 0.01f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyTrackControlTaskDataEntry3 = {
    effCreatePolyTrackControlTaskDataEntry8, 0x8, 8, 0, 0, effCreatePolyTrackControlTaskDataEntry15, 14, &effCreatePolyTrackControlTaskDataEntry2,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyTrackControlTaskDataEntry68 = {
    effCreatePolyTrackControlTaskDataEntry9, 2, { 0 }, 3, { 0 }, (u8 *)&effCreatePolyTrackControlTaskDataEntry105, 0x110,
};

effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry69[26] = {
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry105 }, { .minValue = 1 }, 0x64, 1.0f },
    { effCreatePolyScalyTaskDataEntry60, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry106 }, { .minValue = 1 }, 0xA, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcStartIn, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry107 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry107 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry107 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry107 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcStartOut, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry108 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry108 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry108 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry108 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcEndIn, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry109 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry109 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry109 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry109 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcEndOut, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry110 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry110 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry110 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry110 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyTrackControlTaskDataEntry56 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackControlTaskDataEntry69, 26, &effCreatePolyTrackControlTaskDataEntry68,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry70[9] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry111 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry112 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry113 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackControlTaskDataEntry79, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry114 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry30, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry115 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry29, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry116 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyThunderTaskDataEntry28, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry117 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry27, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry118 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyTrackControlTaskDataEntry57 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackControlTaskDataEntry70, 9, &effCreatePolyTrackControlTaskDataEntry68,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry71[3] = {
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry119 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry59, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry120 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry121 }, { .minValue = 0 }, 0, 0.001f },
};

effDebugMenu_t effCreatePolyTrackControlTaskDataEntry58 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackControlTaskDataEntry71, 3, &effCreatePolyTrackControlTaskDataEntry68,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry16[14] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTrackControlTaskDataEntry57 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry33 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyTrackControlTaskDataEntry34 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditGkColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditGkScaleTrack }, { .release = btlResetEffectWork } },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyTrackControlTaskDataEntry56 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry35 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry36 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry37 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTrackControlTaskDataEntry58 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyTrackControlTaskDataEntry4 = {
    effCreatePolyTrackControlTaskDataEntry9, 0x8, 8, 0, 0, effCreatePolyTrackControlTaskDataEntry16, 14, &effCreatePolyTrackControlTaskDataEntry68,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyTrackControlTaskDataEntry17 = {
    effCreatePolyTrackControlTaskDataEntry10, 2, { 0 }, 4, { 0 }, (u8 *)&effCreatePolyTrackControlTaskDataEntry38, 0x104,
};

effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry72[26] = {
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry38 }, { .minValue = 1 }, 0x64, 1.0f },
    { effCreatePolyScalyTaskDataEntry60, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry122 }, { .minValue = 1 }, 0xA, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcStartIn, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry123 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry123 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry123 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry123 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcStartOut, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry124 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry124 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry124 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry124 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcEndIn, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry125 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry125 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry125 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry125 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcEndOut, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry126 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry126 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry126 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry126 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyTrackControlTaskDataEntry59 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackControlTaskDataEntry72, 26, &effCreatePolyTrackControlTaskDataEntry17,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry73[8] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry127 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry128 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry129 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyThunderTaskDataEntry30, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry130 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry29, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry131 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyThunderTaskDataEntry28, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry132 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry27, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry133 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyTrackControlTaskDataEntry60 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackControlTaskDataEntry73, 8, &effCreatePolyTrackControlTaskDataEntry17,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry18[14] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTrackControlTaskDataEntry60 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry39 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyTrackControlTaskDataEntry40 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditGkColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditGkScaleTrack }, { .release = btlResetEffectWork } },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyTrackControlTaskDataEntry59 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry41 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry59, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry42 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry43 }, { .minValue = 0 }, 0, 0.001f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry44 }, { .minValue = 0 }, 0, 0.01f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyTrackControlTaskDataEntry5 = {
    effCreatePolyTrackControlTaskDataEntry10, 0x8, 8, 0, 0, effCreatePolyTrackControlTaskDataEntry18, 14, &effCreatePolyTrackControlTaskDataEntry17,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyTrackControlTaskDataEntry6 = {
    effCreatePolyTrackControlTaskDataEntry11, 2, { 0 }, 5, { 0 }, (u8 *)&effCreatePolyTrackControlTaskDataEntry134, 0x104,
};

effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry74[26] = {
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry134 }, { .minValue = 1 }, 0x64, 1.0f },
    { effCreatePolyScalyTaskDataEntry60, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry135 }, { .minValue = 1 }, 0xA, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcStartIn, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry136 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry136 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry136 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry136 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcStartOut, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry137 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry137 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry137 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry137 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcEndIn, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry138 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry138 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry138 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry138 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcEndOut, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry139 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry139 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry139 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackControlTaskDataEntry139 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyTrackControlTaskDataEntry61 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackControlTaskDataEntry74, 26, &effCreatePolyTrackControlTaskDataEntry6,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry75[8] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry140 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry141 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry142 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackControlTaskDataEntry76, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry143 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry83, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry144 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyWindTaskDataEntry112, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry145 }, { .minValue = 0 }, 1, 0.1f },
    { effCreatePolyWindTaskDataEntry111, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry146 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyTrackControlTaskDataEntry62 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackControlTaskDataEntry75, 8, &effCreatePolyTrackControlTaskDataEntry6,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackControlTaskDataEntry19[14] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTrackControlTaskDataEntry62 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackControlTaskDataEntry45 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyTrackControlTaskDataEntry46 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditGkColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditGkScaleTrack }, { .release = btlResetEffectWork } },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyTrackControlTaskDataEntry61 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry47 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry48 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry49 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackControlTaskDataEntry50 }, { .minValue = 0 }, 0, 0.01f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyTrackControlTaskDataEntry7 = {
    effCreatePolyTrackControlTaskDataEntry11, 0x8, 8, 0, 0, effCreatePolyTrackControlTaskDataEntry19, 14, &effCreatePolyTrackControlTaskDataEntry6,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyTrackTaskDataEntry66 = {
    effCreatePolyTrackTaskDataEntry6, 15, { 0 }, 1, { 0 }, (u8 *)&effCreatePolyTrackTaskDataEntry90, 0x10C,
};

effDebugMenuItem_t effCreatePolyTrackTaskDataEntry67[26] = {
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry90 }, { .minValue = 1 }, 0x64, 1.0f },
    { effCreatePolyScalyTaskDataEntry60, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry91 }, { .minValue = 1 }, 0xA, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcStartIn, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry92 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry92 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry92 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry92 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcStartOut, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry93 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry93 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry93 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry93 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcEndIn, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry94 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry94 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry94 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry94 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcEndOut, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry95 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry95 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry95 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry95 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyTrackTaskDataEntry49 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackTaskDataEntry67, 26, &effCreatePolyTrackTaskDataEntry66,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackTaskDataEntry68[5] = {
    { sysStrResource, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollAlternateResourceQueueWithTlp }, { .release = effReleaseResourceBankDescriptors } },
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollAlternateTextureResourceFile }, { .release = effBattleResourceDescriptorRelease } },
    { sysPARTICLEDataEntry111, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollAlternateEpResourceFile }, { .release = effBattleResourceDescriptorRelease } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry96 }, { .minValue = 0 }, 0, 1.0f },
};

effDebugMenu_t effCreatePolyTrackTaskDataEntry50 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackTaskDataEntry68, 5, &effCreatePolyTrackTaskDataEntry66,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackTaskDataEntry69[5] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry97 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry98 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry99 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackControlTaskDataEntry76, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry100 }, { .minValue = 0 }, 0, 1.0f },
};

effDebugMenu_t effCreatePolyTrackTaskDataEntry51 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackTaskDataEntry69, 5, &effCreatePolyTrackTaskDataEntry66,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackTaskDataEntry70[6] = {
    { effCreatePolyWindTaskDataEntry110, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry101 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry109, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry102 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyWindTaskDataEntry108, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry103 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry107, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry104 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry105 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry106 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyTrackTaskDataEntry52 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackTaskDataEntry70, 6, &effCreatePolyTrackTaskDataEntry66,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackTaskDataEntry8[16] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTrackTaskDataEntry51 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry21 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyTrackTaskDataEntry22 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditGkColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditGkScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditKRotationTrack }, { .release = btlResetEffectWork } },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyTrackTaskDataEntry49 }, { .minValue = 0 }, 0, 0.0f },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTrackTaskDataEntry50 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry23 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry24 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry25 }, { .minValue = 0 }, 0, 0.01f },
    { effStrAmplitude, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTrackTaskDataEntry52 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyTrackTaskDataEntry = {
    effCreatePolyTrackTaskDataEntry6, 0x8, 8, 0, 0, effCreatePolyTrackTaskDataEntry8, 16, &effCreatePolyTrackTaskDataEntry66,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyTrackTaskDataEntry71 = {
    effCreatePolyTrackTaskDataEntry87, 15, { 0 }, 2, { 0 }, (u8 *)&effCreatePolyTrackTaskDataEntry107, 0xFC,
};

effDebugMenuItem_t effCreatePolyTrackTaskDataEntry72[26] = {
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry107 }, { .minValue = 1 }, 0x64, 1.0f },
    { effCreatePolyScalyTaskDataEntry60, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry108 }, { .minValue = 1 }, 0xA, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcStartIn, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry109 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry109 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry109 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry109 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcStartOut, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry110 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry110 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry110 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry110 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcEndIn, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry111 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry111 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry111 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry111 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcEndOut, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry112 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry112 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry112 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry112 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyTrackTaskDataEntry53 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackTaskDataEntry72, 26, &effCreatePolyTrackTaskDataEntry71,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackTaskDataEntry73[5] = {
    { sysStrResource, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollAlternateResourceQueueWithTlp }, { .release = effReleaseResourceBankDescriptors } },
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollAlternateTextureResourceFile }, { .release = effBattleResourceDescriptorRelease } },
    { sysPARTICLEDataEntry111, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollAlternateEpResourceFile }, { .release = effBattleResourceDescriptorRelease } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry113 }, { .minValue = 0 }, 0, 1.0f },
};

effDebugMenu_t effCreatePolyTrackTaskDataEntry54 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackTaskDataEntry73, 5, &effCreatePolyTrackTaskDataEntry71,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackTaskDataEntry74[6] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry114 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry115 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry116 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackControlTaskDataEntry76, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry117 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry83, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry118 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyTrackTaskDataEntry55 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackTaskDataEntry74, 6, &effCreatePolyTrackTaskDataEntry71,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackTaskDataEntry9[16] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTrackTaskDataEntry55 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry26 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyTrackTaskDataEntry27 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditGkColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditGkScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditKRotationTrack }, { .release = btlResetEffectWork } },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyTrackTaskDataEntry53 }, { .minValue = 0 }, 0, 0.0f },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTrackTaskDataEntry54 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry28 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry29 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry30 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry31 }, { .minValue = 0 }, 0, 0.01f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyTrackTaskDataEntry2 = {
    effCreatePolyTrackTaskDataEntry87, 0x8, 8, 0, 0, effCreatePolyTrackTaskDataEntry9, 16, &effCreatePolyTrackTaskDataEntry71,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyTrackTaskDataEntry75 = {
    effCreatePolyTrackTaskDataEntry88, 15, { 0 }, 3, { 0 }, (u8 *)&effCreatePolyTrackTaskDataEntry119, 0x110,
};

effDebugMenuItem_t effCreatePolyTrackTaskDataEntry76[26] = {
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry119 }, { .minValue = 1 }, 0x64, 1.0f },
    { effCreatePolyScalyTaskDataEntry60, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry120 }, { .minValue = 1 }, 0xA, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcStartIn, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry121 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry121 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry121 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry121 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcStartOut, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry122 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry122 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry122 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry122 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcEndIn, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry123 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry123 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry123 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry123 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcEndOut, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry124 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry124 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry124 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry124 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyTrackTaskDataEntry56 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackTaskDataEntry76, 26, &effCreatePolyTrackTaskDataEntry75,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackTaskDataEntry77[5] = {
    { sysStrResource, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollAlternateResourceQueueWithTlp }, { .release = effReleaseResourceBankDescriptors } },
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollAlternateTextureResourceFile }, { .release = effBattleResourceDescriptorRelease } },
    { sysPARTICLEDataEntry111, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollAlternateEpResourceFile }, { .release = effBattleResourceDescriptorRelease } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry125 }, { .minValue = 0 }, 0, 1.0f },
};

effDebugMenu_t effCreatePolyTrackTaskDataEntry57 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackTaskDataEntry77, 5, &effCreatePolyTrackTaskDataEntry75,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackTaskDataEntry78[9] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry126 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry127 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry128 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackControlTaskDataEntry79, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry129 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry30, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry130 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry29, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry131 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyThunderTaskDataEntry28, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry132 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry27, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry133 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyTrackTaskDataEntry58 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackTaskDataEntry78, 9, &effCreatePolyTrackTaskDataEntry75,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackTaskDataEntry79[3] = {
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry134 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry59, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry135 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry136 }, { .minValue = 0 }, 0, 0.001f },
};

effDebugMenu_t effCreatePolyTrackTaskDataEntry59 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackTaskDataEntry79, 3, &effCreatePolyTrackTaskDataEntry75,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackTaskDataEntry10[16] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTrackTaskDataEntry58 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry32 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyTrackTaskDataEntry33 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditGkColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditGkScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditKRotationTrack }, { .release = btlResetEffectWork } },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyTrackTaskDataEntry56 }, { .minValue = 0 }, 0, 0.0f },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTrackTaskDataEntry57 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry34 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry35 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry36 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTrackTaskDataEntry59 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyTrackTaskDataEntry3 = {
    effCreatePolyTrackTaskDataEntry88, 0x8, 8, 0, 0, effCreatePolyTrackTaskDataEntry10, 16, &effCreatePolyTrackTaskDataEntry75,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyTrackTaskDataEntry80 = {
    effCreatePolyTrackTaskDataEntry89, 15, { 0 }, 4, { 0 }, (u8 *)&effCreatePolyTrackTaskDataEntry137, 0x104,
};

effDebugMenuItem_t effCreatePolyTrackTaskDataEntry81[26] = {
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry137 }, { .minValue = 1 }, 0x64, 1.0f },
    { effCreatePolyScalyTaskDataEntry60, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry138 }, { .minValue = 1 }, 0xA, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcStartIn, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry139 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry139 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry139 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry139 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcStartOut, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry140 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry140 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry140 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry140 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcEndIn, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry141 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry141 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry141 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry141 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcEndOut, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry142 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry142 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry142 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry142 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyTrackTaskDataEntry60 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackTaskDataEntry81, 26, &effCreatePolyTrackTaskDataEntry80,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackTaskDataEntry82[5] = {
    { sysStrResource, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollAlternateResourceQueueWithTlp }, { .release = effReleaseResourceBankDescriptors } },
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollAlternateTextureResourceFile }, { .release = effBattleResourceDescriptorRelease } },
    { sysPARTICLEDataEntry111, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollAlternateEpResourceFile }, { .release = effBattleResourceDescriptorRelease } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry143 }, { .minValue = 0 }, 0, 1.0f },
};

effDebugMenu_t effCreatePolyTrackTaskDataEntry61 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackTaskDataEntry82, 5, &effCreatePolyTrackTaskDataEntry80,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackTaskDataEntry83[8] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry144 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry145 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry146 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyThunderTaskDataEntry30, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry147 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry29, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry148 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyThunderTaskDataEntry28, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry149 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry27, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry150 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyTrackTaskDataEntry62 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackTaskDataEntry83, 8, &effCreatePolyTrackTaskDataEntry80,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackTaskDataEntry11[16] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTrackTaskDataEntry62 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry37 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyTrackTaskDataEntry38 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditGkColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditGkScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditKRotationTrack }, { .release = btlResetEffectWork } },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyTrackTaskDataEntry60 }, { .minValue = 0 }, 0, 0.0f },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTrackTaskDataEntry61 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry39 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry59, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry40 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry41 }, { .minValue = 0 }, 0, 0.001f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry42 }, { .minValue = 0 }, 0, 0.01f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyTrackTaskDataEntry4 = {
    effCreatePolyTrackTaskDataEntry89, 0x8, 8, 0, 0, effCreatePolyTrackTaskDataEntry11, 16, &effCreatePolyTrackTaskDataEntry80,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyTrackTaskDataEntry12 = {
    effCreatePolyTrackTaskDataEntry7, 15, { 0 }, 5, { 0 }, (u8 *)&effCreatePolyTrackTaskDataEntry151, 0x104,
};

effDebugMenuItem_t effCreatePolyTrackTaskDataEntry84[26] = {
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry151 }, { .minValue = 1 }, 0x64, 1.0f },
    { effCreatePolyScalyTaskDataEntry60, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry152 }, { .minValue = 1 }, 0xA, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcStartIn, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry153 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry153 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry153 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry153 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcStartOut, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry154 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry154 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry154 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry154 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcEndIn, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry155 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry155 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry155 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry155 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrVcEndOut, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry156 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry156 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry156 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTrackTaskDataEntry156 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyTrackTaskDataEntry63 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackTaskDataEntry84, 26, &effCreatePolyTrackTaskDataEntry12,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackTaskDataEntry85[5] = {
    { sysStrResource, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollAlternateResourceQueueWithTlp }, { .release = effReleaseResourceBankDescriptors } },
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollAlternateTextureResourceFile }, { .release = effBattleResourceDescriptorRelease } },
    { sysPARTICLEDataEntry111, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollAlternateEpResourceFile }, { .release = effBattleResourceDescriptorRelease } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry157 }, { .minValue = 0 }, 0, 1.0f },
};

effDebugMenu_t effCreatePolyTrackTaskDataEntry64 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackTaskDataEntry85, 5, &effCreatePolyTrackTaskDataEntry12,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackTaskDataEntry86[8] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry158 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry159 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry160 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackControlTaskDataEntry76, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry161 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry83, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry162 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyWindTaskDataEntry112, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry163 }, { .minValue = 0 }, 1, 0.1f },
    { effCreatePolyWindTaskDataEntry111, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry164 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyTrackTaskDataEntry65 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTrackTaskDataEntry86, 8, &effCreatePolyTrackTaskDataEntry12,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTrackTaskDataEntry13[16] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTrackTaskDataEntry65 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTrackTaskDataEntry43 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyTrackTaskDataEntry44 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditGkColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditGkScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditKRotationTrack }, { .release = btlResetEffectWork } },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyTrackTaskDataEntry63 }, { .minValue = 0 }, 0, 0.0f },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTrackTaskDataEntry64 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry45 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry46 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry47 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTrackTaskDataEntry48 }, { .minValue = 0 }, 0, 0.01f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyTrackTaskDataEntry5 = {
    effCreatePolyTrackTaskDataEntry7, 0x8, 8, 0, 0, effCreatePolyTrackTaskDataEntry13, 16, &effCreatePolyTrackTaskDataEntry12,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest fileLoadEffectSlotAData = {
    fileLoadEffectSlotADataEntry2, 3, { 0 }, 0, { 0 }, (u8 *)&fileLoadEffectSlotADataEntry, 0x4,
};

effDebugMenuItem_t effData9Entry13[3] = {
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FLOAT_TENTH, { 0 }, { .f32Value = &fileLoadEffectSlotADataEntry }, { .minValue = 0 }, 0, 0.1f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effData9Entry7 = {
    fileLoadEffectSlotADataEntry2, 0x8, 8, 0, 0, effData9Entry13, 3, &fileLoadEffectSlotAData,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effPollAndQueueCopiedFileResourceData = {
    effPollAndQueueCopiedFileResourceDataEntry2, 25, { 0 }, 0, { 0 }, (u8 *)&effPollAndQueueCopiedFileResourceDataEntry, 0x8,
};

effDebugMenuItem_t effData9Entry14[4] = {
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FLOAT_TENTH, { 0 }, { .f32Value = &effPollAndQueueCopiedFileResourceDataEntry }, { .minValue = 0 }, 0, 0.1f },
    { effData9Entry21, 0x31, { 0 }, { .s32Value = &effData9Entry20 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effData9Entry8 = {
    effPollAndQueueCopiedFileResourceDataEntry2, 0x8, 8, 0, 0, effData9Entry14, 4, &effPollAndQueueCopiedFileResourceData,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreateBlurTaskDataEntry10 = {
    effCreateBlurTaskDataEntry18, 4, { 0 }, 1, { 0 }, effCreateBlurTaskDataEntry23, 0xE8,
};

effDebugMenuItem_t effCreateBlurTaskDataEntry11[7] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreateBlurTaskDataEntry24 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditBfColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditBScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditBRotationTrack }, { .release = btlResetEffectWork } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreateBlurTaskDataEntry = {
    effCreateBlurTaskDataEntry18, 0x8, 8, 0, 0, effCreateBlurTaskDataEntry11, 7, &effCreateBlurTaskDataEntry10,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreateBlurTaskDataEntry45 = {
    effCreateBlurTaskDataEntry49, 4, { 0 }, 2, { 0 }, effCreateBlurTaskDataEntry50, 0xEC,
};

effDebugMenuItem_t effCreateBlurTaskDataEntry46[2] = {
    { sysStrResource, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollResourceQueueWithoutTlp }, { .release = effReleaseResourceBankDescriptors } },
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollBfMaterialTmxFile }, { .release = effBattleResourceDescriptorRelease } },
};

effDebugMenu_t effCreateBlurTaskDataEntry42 = {
    NULL, 0xB8, 8, 0, 0, effCreateBlurTaskDataEntry46, 2, &effCreateBlurTaskDataEntry45,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreateBlurTaskDataEntry12[10] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreateBlurTaskDataEntry25 }, { .minValue = 0 }, 0, 1.0f },
    { effCreateBlurTaskDataEntry19, 0x1D, { 0 }, { .s32Value = &effCreateBlurTaskDataEntry26 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditBfColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditBScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditBRotationTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry10, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditBfAreaTrack }, { .release = btlResetEffectWork } },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreateBlurTaskDataEntry42 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreateBlurTaskDataEntry2 = {
    effCreateBlurTaskDataEntry49, 0x8, 8, 0, 0, effCreateBlurTaskDataEntry12, 10, &effCreateBlurTaskDataEntry45,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreateBlurTaskDataEntry3 = {
    effCreateBlurTaskDataEntry7, 4, { 0 }, 3, { 0 }, effCreateBlurTaskDataEntry9, 0xEC,
};

effDebugMenuItem_t effCreateBlurTaskDataEntry47[2] = {
    { sysStrResource, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollResourceQueueWithoutTlp }, { .release = effReleaseResourceBankDescriptors } },
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollBfMaterialTmxFile }, { .release = effBattleResourceDescriptorRelease } },
};

effDebugMenu_t effCreateBlurTaskDataEntry43 = {
    NULL, 0xB8, 8, 0, 0, effCreateBlurTaskDataEntry47, 2, &effCreateBlurTaskDataEntry3,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreateBlurTaskDataEntry13[15] = {
    { effCreateEnvironmentTaskDataEntry17, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreateBlurTaskDataEntry29 }, { .minValue = 0 }, 0x64, 1.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreateBlurTaskDataEntry27 }, { .minValue = 0 }, 0, 1.0f },
    { effCreateBlurTaskDataEntry19, 0x1D, { 0 }, { .s32Value = &effCreateBlurTaskDataEntry28 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditBfColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditBScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditBRotationTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry10, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditBfAreaTrack }, { .release = btlResetEffectWork } },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreateBlurTaskDataEntry43 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreateBlurTaskDataEntry20, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreateBlurTaskDataEntry30 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreateBlurTaskDataEntry31 }, { .minValue = 0 }, 0, 0.1f },
    { effCreatePolyTwinkleTaskDataEntry88, 0x05, { 0 }, { .s32Value = &effCreateBlurTaskDataEntry32 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreateBlurTaskDataEntry4 = {
    effCreateBlurTaskDataEntry7, 0x8, 8, 0, 0, effCreateBlurTaskDataEntry13, 15, &effCreateBlurTaskDataEntry3,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreateBlurTaskDataEntry14 = {
    effCreateBlurTaskDataEntry21, 4, { 0 }, 4, { 0 }, effCreateBlurTaskDataEntry33, 0xEC,
};

effDebugMenuItem_t effCreateBlurTaskDataEntry48[2] = {
    { sysStrResource, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollResourceQueueWithoutTlp }, { .release = effReleaseResourceBankDescriptors } },
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollBfMaterialTmxFile }, { .release = effBattleResourceDescriptorRelease } },
};

effDebugMenu_t effCreateBlurTaskDataEntry44 = {
    NULL, 0xB8, 8, 0, 0, effCreateBlurTaskDataEntry48, 2, &effCreateBlurTaskDataEntry14,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreateBlurTaskDataEntry15[15] = {
    { effCreateEnvironmentTaskDataEntry17, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreateBlurTaskDataEntry36 }, { .minValue = 0 }, 0x28, 1.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreateBlurTaskDataEntry34 }, { .minValue = 0 }, 0, 1.0f },
    { effCreateBlurTaskDataEntry19, 0x1D, { 0 }, { .s32Value = &effCreateBlurTaskDataEntry35 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditBfColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditBScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditBRotationTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry10, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditBfAreaTrack }, { .release = btlResetEffectWork } },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreateBlurTaskDataEntry44 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreateBlurTaskDataEntry20, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreateBlurTaskDataEntry38 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreateBlurTaskDataEntry37 }, { .minValue = 0 }, 0, 0.1f },
    { effCreateBlurTaskDataEntry22, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreateBlurTaskDataEntry39 }, { .minValue = 0 }, 0, 0.1f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreateBlurTaskDataEntry5 = {
    effCreateBlurTaskDataEntry21, 0x8, 8, 0, 0, effCreateBlurTaskDataEntry15, 15, &effCreateBlurTaskDataEntry14,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreateBlurTaskDataEntry16 = {
    effCreateBlurTaskDataEntry8, 4, { 0 }, 5, { 0 }, effCreateBlurTaskDataEntry40, 0xE8,
};

effDebugMenuItem_t effCreateBlurTaskDataEntry17[7] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreateBlurTaskDataEntry41 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditBfColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditBScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditBRotationTrack }, { .release = btlResetEffectWork } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreateBlurTaskDataEntry6 = {
    effCreateBlurTaskDataEntry8, 0x8, 8, 0, 0, effCreateBlurTaskDataEntry17, 7, &effCreateBlurTaskDataEntry16,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreateFilterTaskDataEntry5 = {
    effCreateFilterTaskDataEntry3, 5, { 0 }, 1, { 0 }, effCreateFilterTaskDataEntry9, 0xD8,
};

effDebugMenuItem_t effCreateFilterTaskDataEntry6[5] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreateFilterTaskDataEntry10 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditBfColorTrack }, { .release = effResetMappingFlags } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreateFilterTaskDataEntry = {
    effCreateFilterTaskDataEntry3, 0x8, 8, 0, 0, effCreateFilterTaskDataEntry6, 5, &effCreateFilterTaskDataEntry5,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreateFilterTaskDataEntry7 = {
    effCreateFilterTaskDataEntry4, 5, { 0 }, 2, { 0 }, effCreateFilterTaskDataEntry11, 0xE4,
};

effDebugMenuItem_t effCreateFilterTaskDataEntry15[2] = {
    { sysStrResource, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollResourceQueueWithoutTlp }, { .release = effReleaseResourceBankDescriptors } },
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollBfMaterialTmxFile }, { .release = effBattleResourceDescriptorRelease } },
};

effDebugMenu_t effCreateFilterTaskDataEntry14 = {
    NULL, 0xB8, 8, 0, 0, effCreateFilterTaskDataEntry15, 2, &effCreateFilterTaskDataEntry7,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreateFilterTaskDataEntry8[8] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreateFilterTaskDataEntry12 }, { .minValue = 0 }, 0, 1.0f },
    { effCreateBlurTaskDataEntry19, 0x1D, { 0 }, { .s32Value = &effCreateFilterTaskDataEntry13 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditBfColorTrack }, { .release = effResetMappingFlags } },
    { sysPOLYTEXDataEntry10, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditBfAreaTrack }, { .release = btlResetEffectWork } },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreateFilterTaskDataEntry14 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreateFilterTaskDataEntry2 = {
    effCreateFilterTaskDataEntry4, 0x8, 8, 0, 0, effCreateFilterTaskDataEntry8, 8, &effCreateFilterTaskDataEntry7,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effHelper_Params_t fileLoadEffectSlotHelpData = {
    0, 0x80808080, 0x80808080, { 0, 0, 0, 0, 0, 0 },
    0x80, 1, 0.1f, 0.9f, 0x64, 1.0f,
};

EffFileJobRequest effData9Entry15 = {
    effCreatePolyCrackTaskDataEntry14, 6, { 0 }, 0, { 0 }, effWork6, 0x3C,
};

effDebugMenuItem_t effData9Entry39[1] = {
    { sysPARTICLEDataEntry110, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollEplResourceFile }, { .release = effBattleResourceDescriptorRelease } },
};

effDebugMenu_t effData9Entry36 = {
    NULL, 0xB8, 8, 0, 0, effData9Entry39, 1, &effData9Entry15,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effData9Entry16[9] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effData9Entry29 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effEditHlpColorTrack }, { .release = effResetMappingFlags } },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effData9Entry36 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT_TENTH, { 0 }, { .f32Value = &effData9Entry30 }, { .minValue = 0 }, 0, 0.1f },
    { effData9Entry22, 0x25, { 0 }, { .s32Value = &effData9Entry28 }, { .minValue = 0 }, 7, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effData9Entry9 = {
    effCreatePolyCrackTaskDataEntry14, 0x8, 8, 0, 0, effData9Entry16, 9, &effData9Entry15,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest sysPOLYTEXDataEntry4 = {
    sysPOLYTEXDataEntry3, 7, { 0 }, 1, { 0 }, sysPOLYTEXDataEntry12, 0x98,
};

effDebugMenuItem_t sysPOLYTEXDataEntry24[2] = {
    { sysStrResource, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollResourceQueueWithTlp }, { .release = effReleaseResourceBankDescriptors } },
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollTextureResourceFile }, { .release = effBattleResourceDescriptorRelease } },
};

effDebugMenu_t sysPOLYTEXDataEntry22 = {
    NULL, 0xB8, 8, 0, 0, sysPOLYTEXDataEntry24, 2, &sysPOLYTEXDataEntry4,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPOLYTEXDataEntry5[10] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPOLYTEXDataEntry13 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry10, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYTEXDataEntry14 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditTBoardColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditTBoardScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditTBoardRotationTrack }, { .release = btlResetEffectWork } },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPOLYTEXDataEntry22 }, { .minValue = 0 }, 0, 0.0f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPOLYTEXDataEntry15 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t sysPOLYTEXDataEntry = {
    sysPOLYTEXDataEntry3, 0x8, 8, 0, 0, sysPOLYTEXDataEntry5, 10, &sysPOLYTEXDataEntry4,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest sysPOLYTEXDataEntry6 = {
    sysStrTBoard2, 7, { 0 }, 2, { 0 }, sysPOLYTEXDataEntry16, 0xA0,
};

effDebugMenuItem_t sysPOLYTEXDataEntry25[2] = {
    { sysStrResource, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollResourceQueueWithTlp }, { .release = effReleaseResourceBankDescriptors } },
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollTextureResourceFile }, { .release = effBattleResourceDescriptorRelease } },
};

effDebugMenu_t sysPOLYTEXDataEntry23 = {
    NULL, 0xB8, 8, 0, 0, sysPOLYTEXDataEntry25, 2, &sysPOLYTEXDataEntry6,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPOLYTEXDataEntry7[12] = {
    { sysStrLifeCol, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPOLYTEXDataEntry17 }, { .minValue = 0 }, 0, 1.0f },
    { sysStrLifeScl, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPOLYTEXDataEntry18 }, { .minValue = 0 }, 0, 1.0f },
    { sysStrCycleRot, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPOLYTEXDataEntry19 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry10, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYTEXDataEntry20 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditTBoardColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditTBoardScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditTBoardRotationTrack }, { .release = btlResetEffectWork } },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPOLYTEXDataEntry23 }, { .minValue = 0 }, 0, 0.0f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPOLYTEXDataEntry21 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t sysPOLYTEXDataEntry2 = {
    sysStrTBoard2, 0x8, 8, 0, 0, sysPOLYTEXDataEntry7, 12, &sysPOLYTEXDataEntry6,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreateEnvironmentTaskDataEntry10 = {
    effCreateEnvironmentTaskDataEntry6, 8, { 0 }, 0, { 0 }, effCreateEnvironmentTaskDataEntry22, 0x40,
};

effDebugMenuItem_t effCreateEnvironmentTaskDataEntry11[6] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreateEnvironmentTaskDataEntry23 }, { .minValue = 0 }, 0, 1.0f },
    { effCreateEnvironmentTaskDataEntry17, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreateEnvironmentTaskDataEntry24 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreateEnvironmentTaskDataEntry25 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditRainColorTrack }, { .release = effResetMappingFlags } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreateEnvironmentTaskDataEntry = {
    effCreateEnvironmentTaskDataEntry6, 0x8, 8, 0, 0, effCreateEnvironmentTaskDataEntry11, 6, &effCreateEnvironmentTaskDataEntry10,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreateEnvironmentTaskDataEntry2 = {
    effCreateEnvironmentTaskDataEntry7, 9, { 0 }, 0, { 0 }, effCreateEnvironmentTaskDataEntry9, 0x40,
};

effDebugMenuItem_t effCreateEnvironmentTaskDataEntry12[6] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreateEnvironmentTaskDataEntry27 }, { .minValue = 0 }, 0, 1.0f },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreateEnvironmentTaskDataEntry26 }, { .minValue = 0 }, 0, 1.0f },
    { effCreateEnvironmentTaskDataEntry18, EFF_DEBUG_ITEM_BYTE, { 0 }, { .u8Value = &effCreateEnvironmentTaskDataEntry28 }, { .minValue = 0 }, 1, 1.0f },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditFlareColorTrack }, { .release = effResetMappingFlags } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreateEnvironmentTaskDataEntry3 = {
    effCreateEnvironmentTaskDataEntry7, 0x8, 8, 0, 0, effCreateEnvironmentTaskDataEntry12, 6, &effCreateEnvironmentTaskDataEntry2,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyFlashTaskDataEntry12 = {
    effCreatePolyFlashTaskDataEntry9, 10, { 0 }, 1, { 0 }, effCreatePolyFlashTaskDataEntry26, 0x80,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry124[20] = {
    { effData50, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry171 }, { .minValue = 0 }, 0, 1.0f },
    { effData49, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry172 }, { .minValue = 0 }, 0, 1.0f },
    { effData48, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry173 }, { .minValue = 0 }, 1, 1.0f },
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry174 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyRingTaskDataEntry9, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry175 }, { .minValue = 0 }, 1, 1.0f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyFlashTaskDataEntry168 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry153, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry169 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry169 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry169 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry169 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry152, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry170 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry170 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry170 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry170 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry151, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditSPolygonAlphaTrack }, { .release = effLeaf401 } },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry103 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyFlashTaskDataEntry124, 20, &effCreatePolyFlashTaskDataEntry12,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry13[15] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry27 }, { .minValue = 0 }, 0, 1.0f },
    { effCreateEnvironmentTaskDataEntry17, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry28 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyFlashTaskDataEntry32 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditSColorTrack }, { .release = effResetMappingFlags } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry24, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry29 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry23, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry30 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry22, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyFlashTaskDataEntry31 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyFlashTaskDataEntry103 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry33 }, { .minValue = 0 }, 0, 0.001f },
    { effCreatePolyScalyTaskDataEntry59, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry34 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry35 }, { .minValue = 0 }, 0, 0.001f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry = {
    effCreatePolyFlashTaskDataEntry9, 0x8, 8, 0, 0, effCreatePolyFlashTaskDataEntry13, 15, &effCreatePolyFlashTaskDataEntry12,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyFlashTaskDataEntry125 = {
    effCreatePolyFlashTaskDataEntry154, 10, { 0 }, 2, { 0 }, effCreatePolyFlashTaskDataEntry176, 0x8C,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry126[19] = {
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry180 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry155, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry181 }, { .minValue = 0 }, 1, 1.0f },
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry182 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyRingTaskDataEntry9, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry183 }, { .minValue = 0 }, 1, 1.0f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyFlashTaskDataEntry177 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry153, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry178 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry178 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry178 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry178 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry152, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry179 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry179 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry179 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry179 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry151, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditSPolygonAlphaTrack }, { .release = effLeaf401 } },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry104 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyFlashTaskDataEntry126, 19, &effCreatePolyFlashTaskDataEntry125,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry127[2] = {
    { effCreatePolyTrackControlTaskDataEntry76, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry184 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry83, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry185 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry105 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyFlashTaskDataEntry127, 2, &effCreatePolyFlashTaskDataEntry125,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry14[17] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry36 }, { .minValue = 0 }, 0, 1.0f },
    { effCreateEnvironmentTaskDataEntry17, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry37 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyFlashTaskDataEntry41 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditSColorTrack }, { .release = effResetMappingFlags } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry24, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry38 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry23, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry39 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry22, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyFlashTaskDataEntry40 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyFlashTaskDataEntry104 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyFlashTaskDataEntry105 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry42 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry43 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry44 }, { .minValue = 0 }, 0, 0.01f },
    { effCreateEnvironmentTaskDataEntry18, 0x1C, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry45 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry2 = {
    effCreatePolyFlashTaskDataEntry154, 0x8, 8, 0, 0, effCreatePolyFlashTaskDataEntry14, 17, &effCreatePolyFlashTaskDataEntry125,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyFlashTaskDataEntry128 = {
    effCreatePolyFlashTaskDataEntry156, 10, { 0 }, 3, { 0 }, effCreatePolyFlashTaskDataEntry186, 0x9C,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry129[19] = {
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry190 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry155, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry191 }, { .minValue = 0 }, 1, 1.0f },
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry192 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyRingTaskDataEntry9, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry193 }, { .minValue = 0 }, 1, 1.0f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyFlashTaskDataEntry187 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry153, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry188 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry188 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry188 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry188 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry152, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry189 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry189 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry189 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry189 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry151, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditSPolygonAlphaTrack }, { .release = effLeaf401 } },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry106 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyFlashTaskDataEntry129, 19, &effCreatePolyFlashTaskDataEntry128,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry130[4] = {
    { effCreatePolyThunderTaskDataEntry30, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry194 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry29, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry195 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyThunderTaskDataEntry28, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry196 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry27, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry197 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry107 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyFlashTaskDataEntry130, 4, &effCreatePolyFlashTaskDataEntry128,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry131[3] = {
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry198 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry59, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry199 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry200 }, { .minValue = 0 }, 0, 0.001f },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry108 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyFlashTaskDataEntry131, 3, &effCreatePolyFlashTaskDataEntry128,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry15[17] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry46 }, { .minValue = 0 }, 0, 1.0f },
    { effCreateEnvironmentTaskDataEntry17, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry47 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyFlashTaskDataEntry51 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditSColorTrack }, { .release = effResetMappingFlags } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry24, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry48 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry23, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry49 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry22, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyFlashTaskDataEntry50 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyFlashTaskDataEntry106 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyFlashTaskDataEntry107 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry52 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry53 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry54 }, { .minValue = 0 }, 0, 0.001f },
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyFlashTaskDataEntry108 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry3 = {
    effCreatePolyFlashTaskDataEntry156, 0x8, 8, 0, 0, effCreatePolyFlashTaskDataEntry15, 17, &effCreatePolyFlashTaskDataEntry128,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyFlashTaskDataEntry132 = {
    effCreatePolyFlashTaskDataEntry10, 10, { 0 }, 4, { 0 }, effCreatePolyFlashTaskDataEntry201, 0x80,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry133[3] = {
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollSTMaterialTmxFile }, { .release = effBattleResourceDescriptorRelease } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effUpdateCameraSnapshotGuidesDataEntry2, EFF_DEBUG_ITEM_COMMAND, { 0 }, { .command = effFileJobSecondaryDataSet }, { .color = 0xFFFFFF } },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry109 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyFlashTaskDataEntry133, 3, &effCreatePolyFlashTaskDataEntry132,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry134[20] = {
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry205 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry155, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry206 }, { .minValue = 0 }, 1, 1.0f },
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry207 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyRingTaskDataEntry9, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry208 }, { .minValue = 0 }, 1, 1.0f },
    { effData51, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry209 }, { .minValue = 1 }, 0xFFFFFFF, 1.0f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyFlashTaskDataEntry202 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry153, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry203 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry203 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry203 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry203 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry152, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry204 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry204 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry204 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry204 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry151, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditSPolygonAlphaTrack }, { .release = effLeaf401 } },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry110 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyFlashTaskDataEntry134, 20, &effCreatePolyFlashTaskDataEntry132,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry16[16] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry55 }, { .minValue = 0 }, 0, 1.0f },
    { effCreateEnvironmentTaskDataEntry17, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry56 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyFlashTaskDataEntry60 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditSColorTrack }, { .release = effResetMappingFlags } },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyFlashTaskDataEntry109 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry24, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry57 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry23, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry58 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry22, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyFlashTaskDataEntry59 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyFlashTaskDataEntry110 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry61 }, { .minValue = 0 }, 0, 0.001f },
    { effCreatePolyScalyTaskDataEntry59, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry62 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry63 }, { .minValue = 0 }, 0, 0.001f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry4 = {
    effCreatePolyFlashTaskDataEntry10, 0x8, 8, 0, 0, effCreatePolyFlashTaskDataEntry16, 16, &effCreatePolyFlashTaskDataEntry132,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyFlashTaskDataEntry135 = {
    effCreatePolyFlashTaskDataEntry157, 10, { 0 }, 5, { 0 }, effCreatePolyFlashTaskDataEntry210, 0x88,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry136[3] = {
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollSTMaterialTmxFile }, { .release = effBattleResourceDescriptorRelease } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effUpdateCameraSnapshotGuidesDataEntry2, EFF_DEBUG_ITEM_COMMAND, { 0 }, { .command = effFileJobSecondaryDataSet }, { .color = 0xFFFFFF } },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry111 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyFlashTaskDataEntry136, 3, &effCreatePolyFlashTaskDataEntry135,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry137[19] = {
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry214 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry155, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry215 }, { .minValue = 0 }, 1, 1.0f },
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry216 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyRingTaskDataEntry9, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry217 }, { .minValue = 0 }, 1, 1.0f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyFlashTaskDataEntry211 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry153, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry212 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry212 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry212 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry212 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry152, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry213 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry213 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry213 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry213 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry151, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditSPolygonAlphaTrack }, { .release = effLeaf401 } },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry112 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyFlashTaskDataEntry137, 19, &effCreatePolyFlashTaskDataEntry135,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry138[2] = {
    { effCreatePolyTrackControlTaskDataEntry76, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry218 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry83, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry219 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry113 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyFlashTaskDataEntry138, 2, &effCreatePolyFlashTaskDataEntry135,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry17[18] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry64 }, { .minValue = 0 }, 0, 1.0f },
    { effCreateEnvironmentTaskDataEntry17, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry65 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyFlashTaskDataEntry69 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditSColorTrack }, { .release = effResetMappingFlags } },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyFlashTaskDataEntry111 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry24, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry66 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry23, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry67 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry22, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyFlashTaskDataEntry68 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyFlashTaskDataEntry112 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyFlashTaskDataEntry113 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry70 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry71 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry72 }, { .minValue = 0 }, 0, 0.01f },
    { effCreateEnvironmentTaskDataEntry18, 0x1C, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry73 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry5 = {
    effCreatePolyFlashTaskDataEntry157, 0x8, 8, 0, 0, effCreatePolyFlashTaskDataEntry17, 18, &effCreatePolyFlashTaskDataEntry135,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyFlashTaskDataEntry18 = {
    effCreatePolyFlashTaskDataEntry25, 10, { 0 }, 6, { 0 }, effCreatePolyFlashTaskDataEntry74, 0xA4,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry139[3] = {
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollSTMaterialTmxFile }, { .release = effBattleResourceDescriptorRelease } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effUpdateCameraSnapshotGuidesDataEntry2, EFF_DEBUG_ITEM_COMMAND, { 0 }, { .command = effFileJobSecondaryDataSet }, { .color = 0xFFFFFF } },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry114 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyFlashTaskDataEntry139, 3, &effCreatePolyFlashTaskDataEntry18,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry140[21] = {
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry223 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry155, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry224 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyFlashTaskDataEntry161, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry225 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry160, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry226 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyFlashTaskDataEntry159, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry227 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry158, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry228 }, { .minValue = 0 }, 1, 1.0f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyFlashTaskDataEntry220 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry153, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry221 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry221 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry221 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry221 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry152, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry222 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry222 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry222 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry222 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry151, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditSPolygonAlphaTrack }, { .release = effLeaf401 } },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry115 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyFlashTaskDataEntry140, 21, &effCreatePolyFlashTaskDataEntry18,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry141[4] = {
    { effCreatePolyThunderTaskDataEntry30, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry229 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry29, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry230 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyThunderTaskDataEntry28, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry231 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry27, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry232 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry116 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyFlashTaskDataEntry141, 4, &effCreatePolyFlashTaskDataEntry18,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry142[4] = {
    { effCreatePolyFlashTaskDataEntry165, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry233 }, { .minValue = -1 }, 1, 0.1f },
    { effCreatePolyFlashTaskDataEntry164, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry234 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyFlashTaskDataEntry163, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry235 }, { .minValue = -1 }, 1, 0.1f },
    { effCreatePolyFlashTaskDataEntry162, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry236 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry117 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyFlashTaskDataEntry142, 4, &effCreatePolyFlashTaskDataEntry18,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry19[18] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry75 }, { .minValue = 0 }, 0, 1.0f },
    { effCreateEnvironmentTaskDataEntry17, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry76 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyFlashTaskDataEntry80 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditSColorTrack }, { .release = effResetMappingFlags } },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyFlashTaskDataEntry114 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry24, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry77 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry23, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry78 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry22, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyFlashTaskDataEntry79 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyFlashTaskDataEntry115 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrEmtRadi, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyFlashTaskDataEntry116 }, { .minValue = 0 }, 0, 0.0f },
    { effStrEmtSpre, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyFlashTaskDataEntry117 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry81 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry59, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry82 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry83 }, { .minValue = 0 }, 0, 0.001f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry6 = {
    effCreatePolyFlashTaskDataEntry25, 0x8, 8, 0, 0, effCreatePolyFlashTaskDataEntry19, 18, &effCreatePolyFlashTaskDataEntry18,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyFlashTaskDataEntry143 = {
    effCreatePolyFlashTaskDataEntry11, 10, { 0 }, 7, { 0 }, effCreatePolyFlashTaskDataEntry237, 0xA8,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry144[22] = {
    { effData50, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry241 }, { .minValue = 0 }, 0, 1.0f },
    { effData49, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry242 }, { .minValue = 0 }, 0, 1.0f },
    { effData48, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry243 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyFlashTaskDataEntry161, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry244 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry160, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry245 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyFlashTaskDataEntry159, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry246 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry158, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry247 }, { .minValue = 0 }, 1, 1.0f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyFlashTaskDataEntry238 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry153, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry239 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry239 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry239 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry239 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry152, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry240 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry240 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry240 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry240 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry151, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditSPolygonAlphaTrack }, { .release = effLeaf401 } },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry118 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyFlashTaskDataEntry144, 22, &effCreatePolyFlashTaskDataEntry143,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry145[4] = {
    { effCreatePolyThunderTaskDataEntry30, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry248 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry29, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry249 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyThunderTaskDataEntry28, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry250 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry27, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry251 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry119 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyFlashTaskDataEntry145, 4, &effCreatePolyFlashTaskDataEntry143,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry146[4] = {
    { effCreatePolyFlashTaskDataEntry165, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry252 }, { .minValue = -1 }, 1, 0.1f },
    { effCreatePolyFlashTaskDataEntry164, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry253 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyFlashTaskDataEntry163, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry254 }, { .minValue = -1 }, 1, 0.1f },
    { effCreatePolyFlashTaskDataEntry162, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry255 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry120 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyFlashTaskDataEntry146, 4, &effCreatePolyFlashTaskDataEntry143,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry20[17] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry84 }, { .minValue = 0 }, 0, 1.0f },
    { effCreateEnvironmentTaskDataEntry17, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry85 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyFlashTaskDataEntry89 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditSColorTrack }, { .release = effResetMappingFlags } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry24, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry86 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry23, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry87 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry22, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyFlashTaskDataEntry88 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyFlashTaskDataEntry118 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrEmtRadi, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyFlashTaskDataEntry119 }, { .minValue = 0 }, 0, 0.0f },
    { effStrEmtSpre, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyFlashTaskDataEntry120 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry90 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry59, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry91 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry92 }, { .minValue = 0 }, 0, 0.001f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry7 = {
    effCreatePolyFlashTaskDataEntry11, 0x8, 8, 0, 0, effCreatePolyFlashTaskDataEntry20, 17, &effCreatePolyFlashTaskDataEntry143,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyFlashTaskDataEntry147 = {
    effCreatePolyFlashTaskDataEntry166, 10, { 0 }, 8, { 0 }, effCreatePolyFlashTaskDataEntry256, 0x98,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry148[3] = {
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollSTMaterialTmxFile }, { .release = effBattleResourceDescriptorRelease } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effUpdateCameraSnapshotGuidesDataEntry2, EFF_DEBUG_ITEM_COMMAND, { 0 }, { .command = effFileJobSecondaryDataSet }, { .color = 0xFFFFFF } },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry121 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyFlashTaskDataEntry148, 3, &effCreatePolyFlashTaskDataEntry147,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry149[19] = {
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry260 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry155, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry261 }, { .minValue = 0 }, 1, 1.0f },
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry262 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyRingTaskDataEntry9, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry263 }, { .minValue = 0 }, 1, 1.0f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyFlashTaskDataEntry257 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry153, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry258 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry258 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry258 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry258 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry152, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry259 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry259 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry259 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyFlashTaskDataEntry259 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry151, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditSPolygonAlphaTrack }, { .release = effLeaf401 } },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry122 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyFlashTaskDataEntry149, 19, &effCreatePolyFlashTaskDataEntry147,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry150[7] = {
    { effCreatePolyThunderTaskDataEntry30, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry264 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry29, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry265 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyThunderTaskDataEntry28, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry266 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry27, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry267 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyWindTaskDataEntry112, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry268 }, { .minValue = -1 }, 1, 0.1f },
    { effCreatePolyFlashTaskDataEntry167, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry269 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry123 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyFlashTaskDataEntry150, 7, &effCreatePolyFlashTaskDataEntry147,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyFlashTaskDataEntry21[18] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry93 }, { .minValue = 0 }, 0, 1.0f },
    { effCreateEnvironmentTaskDataEntry17, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry94 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyFlashTaskDataEntry98 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditSColorTrack }, { .release = effResetMappingFlags } },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyFlashTaskDataEntry121 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry24, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry95 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry23, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry96 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry22, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyFlashTaskDataEntry97 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyFlashTaskDataEntry122 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyFlashTaskDataEntry123 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry99 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry100 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyFlashTaskDataEntry101 }, { .minValue = 0 }, 0, 0.01f },
    { effCreateEnvironmentTaskDataEntry18, 0x1C, { 0 }, { .s32Value = &effCreatePolyFlashTaskDataEntry102 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyFlashTaskDataEntry8 = {
    effCreatePolyFlashTaskDataEntry166, 0x8, 8, 0, 0, effCreatePolyFlashTaskDataEntry21, 18, &effCreatePolyFlashTaskDataEntry147,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreateEnvironmentTaskDataEntry13 = {
    effCreateEnvironmentTaskDataEntry8, 11, { 0 }, 0, { 0 }, (u8 *)&effCreateEnvironmentTaskDataEntry20, 0x8,
};

effDebugMenuItem_t effCreateEnvironmentTaskDataEntry14[4] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreateEnvironmentTaskDataEntry20 }, { .minValue = 0 }, 0, 1.0f },
    { effCreateEnvironmentTaskDataEntry19, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreateEnvironmentTaskDataEntry21 }, { .minValue = 0 }, 0x190, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreateEnvironmentTaskDataEntry4 = {
    effCreateEnvironmentTaskDataEntry8, 0x8, 8, 0, 0, effCreateEnvironmentTaskDataEntry14, 4, &effCreateEnvironmentTaskDataEntry13,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreateEnvironmentTaskDataEntry15 = {
    effStrEQuake2, 21, { 0 }, 0, { 0 }, (u8 *)&effCreateEnvironmentTaskDataEntry29, 0x10,
};

effDebugMenuItem_t effCreateEnvironmentTaskDataEntry16[6] = {
    { sysBATTLEONLYDataEntry52, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreateEnvironmentTaskDataEntry29 }, { .minValue = 0 }, 0, 1.0f },
    { sysBATTLEONLYDataEntry51, EFF_DEBUG_ITEM_INT_RANGE, { 0 }, { .s32Value = &effCreateEnvironmentTaskDataEntry30 }, { .minValue = 0 }, 0x1F4, 1.0f },
    { sysBATTLEONLYDataEntry50, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreateEnvironmentTaskDataEntry31 }, { .minValue = 0 }, 0, 1.0f },
    { sysBATTLEONLYDataEntry49, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreateEnvironmentTaskDataEntry32 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreateEnvironmentTaskDataEntry5 = {
    effStrEQuake2, 0x8, 8, 0, 0, effCreateEnvironmentTaskDataEntry16, 6, &effCreateEnvironmentTaskDataEntry15,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyRingTaskDataEntry28 = {
    effCreatePolyRingTaskDataEntry33, 12, { 0 }, 1, { 0 }, effCreatePolyRingTaskDataEntry36, 0x6C,
};

effDebugMenuItem_t effCreatePolyRingTaskDataEntry29[24] = {
    { effCreatePolyScalyTaskDataEntry60, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyRingTaskDataEntry37 }, { .minValue = 3 }, 0xFF, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry76, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyRingTaskDataEntry39 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyRingTaskDataEntry38 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyRingTaskDataEntry34, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyRingTaskDataEntry43 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry40 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry40 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry40 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry40 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTwinkleTaskDataEntry89, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyRingTaskDataEntry44 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry41 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry41 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry41 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry41 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrEdgeOut, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyRingTaskDataEntry45 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry42 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry42 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry42 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry42 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyRingTaskDataEntry25 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyRingTaskDataEntry29, 24, &effCreatePolyRingTaskDataEntry28,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyRingTaskDataEntry5[9] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyRingTaskDataEntry11 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditRColorTrack }, { .release = effResetMappingFlags } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyRingTaskDataEntry25 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyRingTaskDataEntry12 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyRingTaskDataEntry13 }, { .minValue = 0 }, 0, 0.01f },
    { effCreateEnvironmentTaskDataEntry18, 0x1C, { 0 }, { .s32Value = &effCreatePolyRingTaskDataEntry14 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyRingTaskDataEntry = {
    effCreatePolyRingTaskDataEntry33, 0x8, 8, 0, 0, effCreatePolyRingTaskDataEntry5, 9, &effCreatePolyRingTaskDataEntry28,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyRingTaskDataEntry6 = {
    effCreatePolyRingTaskDataEntry10, 12, { 0 }, 2, { 0 }, effCreatePolyRingTaskDataEntry15, 0xD0,
};

effDebugMenuItem_t effCreatePolyRingTaskDataEntry30[24] = {
    { effCreatePolyScalyTaskDataEntry60, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyRingTaskDataEntry46 }, { .minValue = 3 }, 0xFF, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry76, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyRingTaskDataEntry48 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyRingTaskDataEntry47 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyRingTaskDataEntry34, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyRingTaskDataEntry52 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry49 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry49 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry49 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry49 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTwinkleTaskDataEntry89, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyRingTaskDataEntry53 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry50 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry50 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry50 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry50 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrEdgeOut, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyRingTaskDataEntry54 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry51 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry51 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry51 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry51 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyRingTaskDataEntry26 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyRingTaskDataEntry30, 24, &effCreatePolyRingTaskDataEntry6,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyRingTaskDataEntry7[11] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyRingTaskDataEntry16 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditRTrackColorTrack }, { .release = effResetMappingFlags } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyRingTaskDataEntry26 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyRingTaskDataEntry17 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyRingTaskDataEntry18 }, { .minValue = 0 }, 0, 0.01f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyRingTaskDataEntry19 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyRingTaskDataEntry9, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyRingTaskDataEntry20 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyRingTaskDataEntry2 = {
    effCreatePolyRingTaskDataEntry10, 0x8, 8, 0, 0, effCreatePolyRingTaskDataEntry7, 11, &effCreatePolyRingTaskDataEntry6,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyRingTaskDataEntry31 = {
    effCreatePolyRingTaskDataEntry4, 12, { 0 }, 3, { 0 }, effCreatePolyRingTaskDataEntry55, 0x68,
};

effDebugMenuItem_t effCreatePolyRingTaskDataEntry32[22] = {
    { effCreatePolyScalyTaskDataEntry60, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyRingTaskDataEntry56 }, { .minValue = 3 }, 0xFF, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry76, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyRingTaskDataEntry58 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyRingTaskDataEntry57 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyRingTaskDataEntry35, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry59 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry59 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry59 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry59 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyThunderTaskDataEntry31, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry60 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry60 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry60 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry60 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTwinkleTaskDataEntry90, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyRingTaskDataEntry62 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry61 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry61 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry61 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyRingTaskDataEntry61 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyRingTaskDataEntry27 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyRingTaskDataEntry32, 22, &effCreatePolyRingTaskDataEntry31,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyRingTaskDataEntry8[9] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyRingTaskDataEntry21 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditRColorTrack }, { .release = effResetMappingFlags } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyRingTaskDataEntry27 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyRingTaskDataEntry22 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyRingTaskDataEntry23 }, { .minValue = 0 }, 0, 0.01f },
    { effCreateEnvironmentTaskDataEntry18, 0x1C, { 0 }, { .s32Value = &effCreatePolyRingTaskDataEntry24 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyRingTaskDataEntry3 = {
    effCreatePolyRingTaskDataEntry4, 0x8, 8, 0, 0, effCreatePolyRingTaskDataEntry8, 9, &effCreatePolyRingTaskDataEntry31,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyThunderTaskDataEntry = {
    effCreatePolyThunderTaskDataEntry4, 13, { 0 }, 1, { 0 }, effCreatePolyThunderTaskDataEntry5, 0x88,
};

effDebugMenuItem_t effCreatePolyThunderTaskDataEntry23[26] = {
    { effCreatePolyScalyTaskDataEntry60, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyThunderTaskDataEntry35 }, { .minValue = 3 }, 0x64, 1.0f },
    { effCreatePolyThunderTaskDataEntry33, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyThunderTaskDataEntry36 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry32, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyThunderTaskDataEntry37 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyThunderTaskDataEntry38 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTwinkleTaskDataEntry90, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyThunderTaskDataEntry43 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyThunderTaskDataEntry41 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyThunderTaskDataEntry41 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyThunderTaskDataEntry41 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyThunderTaskDataEntry41 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTwinkleTaskDataEntry89, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyThunderTaskDataEntry42 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyThunderTaskDataEntry39 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyThunderTaskDataEntry39 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyThunderTaskDataEntry39 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyThunderTaskDataEntry39 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyThunderTaskDataEntry31, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyThunderTaskDataEntry40 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyThunderTaskDataEntry40 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyThunderTaskDataEntry40 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyThunderTaskDataEntry40 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyScalyTaskDataEntry61, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditDoPolygonFadeTrack }, { .release = effLeaf401 } },
};

effDebugMenu_t effCreatePolyThunderTaskDataEntry20 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyThunderTaskDataEntry23, 26, &effCreatePolyThunderTaskDataEntry,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyThunderTaskDataEntry6[9] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyThunderTaskDataEntry9 }, { .minValue = 0 }, 0, 1.0f },
    { effCreateEnvironmentTaskDataEntry17, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyThunderTaskDataEntry10 }, { .minValue = 0 }, 0x1E, 1.0f },
    { effCreatePolyThunderTaskDataEntry8, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyThunderTaskDataEntry11 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditDoColorTrack }, { .release = effResetMappingFlags } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyThunderTaskDataEntry20 }, { .minValue = 0 }, 0, 0.0f },
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyThunderTaskDataEntry12 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyThunderTaskDataEntry2 = {
    effCreatePolyThunderTaskDataEntry4, 0x8, 8, 0, 0, effCreatePolyThunderTaskDataEntry6, 9, &effCreatePolyThunderTaskDataEntry,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyThunderTaskDataEntry24 = {
    effCreatePolyThunderTaskDataEntry34, 13, { 0 }, 3, { 0 }, effCreatePolyThunderTaskDataEntry44, 0xAC,
};

effDebugMenuItem_t effCreatePolyThunderTaskDataEntry25[26] = {
    { effCreatePolyScalyTaskDataEntry60, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyThunderTaskDataEntry45 }, { .minValue = 3 }, 0x64, 1.0f },
    { effCreatePolyThunderTaskDataEntry33, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyThunderTaskDataEntry46 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry32, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyThunderTaskDataEntry47 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyThunderTaskDataEntry48 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTwinkleTaskDataEntry90, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyThunderTaskDataEntry53 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyThunderTaskDataEntry51 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyThunderTaskDataEntry51 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyThunderTaskDataEntry51 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyThunderTaskDataEntry51 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTwinkleTaskDataEntry89, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyThunderTaskDataEntry52 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyThunderTaskDataEntry49 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyThunderTaskDataEntry49 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyThunderTaskDataEntry49 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyThunderTaskDataEntry49 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyThunderTaskDataEntry31, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyThunderTaskDataEntry50 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyThunderTaskDataEntry50 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyThunderTaskDataEntry50 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyThunderTaskDataEntry50 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyScalyTaskDataEntry61, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditDoPolygonFadeTrack }, { .release = effLeaf401 } },
};

effDebugMenu_t effCreatePolyThunderTaskDataEntry21 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyThunderTaskDataEntry25, 26, &effCreatePolyThunderTaskDataEntry24,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyThunderTaskDataEntry26[6] = {
    { effCreateEnvironmentTaskDataEntry18, 0x1F, { 0 }, { .s32Value = &effCreatePolyThunderTaskDataEntry54 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyThunderTaskDataEntry30, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyThunderTaskDataEntry55 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry29, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyThunderTaskDataEntry56 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyThunderTaskDataEntry28, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyThunderTaskDataEntry57 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry27, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyThunderTaskDataEntry58 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry79, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyThunderTaskDataEntry59 }, { .minValue = 0 }, 0, 1.0f },
};

effDebugMenu_t effCreatePolyThunderTaskDataEntry22 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyThunderTaskDataEntry26, 6, &effCreatePolyThunderTaskDataEntry24,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyThunderTaskDataEntry7[13] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyThunderTaskDataEntry13 }, { .minValue = 0 }, 0, 1.0f },
    { effCreateEnvironmentTaskDataEntry17, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyThunderTaskDataEntry14 }, { .minValue = 0 }, 0x1E, 1.0f },
    { effCreatePolyThunderTaskDataEntry8, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyThunderTaskDataEntry15 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditDoColorTrack }, { .release = effResetMappingFlags } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyThunderTaskDataEntry21 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyThunderTaskDataEntry22 }, { .minValue = 0 }, 0, 0.0f },
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyThunderTaskDataEntry16 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyThunderTaskDataEntry17 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry59, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyThunderTaskDataEntry18 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyThunderTaskDataEntry19 }, { .minValue = 0 }, 0, 0.001f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyThunderTaskDataEntry3 = {
    effCreatePolyThunderTaskDataEntry34, 0x8, 8, 0, 0, effCreatePolyThunderTaskDataEntry7, 13, &effCreatePolyThunderTaskDataEntry24,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyTwinkleTaskDataEntry62 = {
    effCreatePolyTwinkleTaskDataEntry7, 14, { 0 }, 1, { 0 }, (u8 *)&effCreatePolyTwinkleTaskDataEntry93, 0x110,
};

effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry63[16] = {
    { effCreatePolyTwinkleTaskDataEntry92, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry97 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry91, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry98 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry96 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyTwinkleTaskDataEntry93 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTwinkleTaskDataEntry89, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry94 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry94 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry94 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry94 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTwinkleTaskDataEntry90, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry95 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry95 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry95 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry95 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyTwinkleTaskDataEntry45 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTwinkleTaskDataEntry63, 16, &effCreatePolyTwinkleTaskDataEntry62,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry64[6] = {
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry99 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry100 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry100 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry100 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry100 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyTwinkleTaskDataEntry46 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTwinkleTaskDataEntry64, 6, &effCreatePolyTwinkleTaskDataEntry62,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry65[5] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTwinkleTaskDataEntry101 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTwinkleTaskDataEntry102 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry103 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackControlTaskDataEntry76, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry104 }, { .minValue = 0 }, 0, 1.0f },
};

effDebugMenu_t effCreatePolyTwinkleTaskDataEntry47 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTwinkleTaskDataEntry65, 5, &effCreatePolyTwinkleTaskDataEntry62,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry66[6] = {
    { effCreatePolyWindTaskDataEntry110, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry105 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry109, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry106 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyWindTaskDataEntry108, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry107 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry107, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry108 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry109 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry110 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyTwinkleTaskDataEntry48 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTwinkleTaskDataEntry66, 6, &effCreatePolyTwinkleTaskDataEntry62,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry12[16] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTwinkleTaskDataEntry47 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTwinkleTaskDataEntry17 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyTwinkleTaskDataEntry18 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effResolveMappingWithTemporaryTable }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditWScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditWRotationTrack }, { .release = btlResetEffectWork } },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyTwinkleTaskDataEntry45 }, { .minValue = 0 }, 0, 0.0f },
    { effStrBillboard, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTwinkleTaskDataEntry46 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry19 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry20 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry21 }, { .minValue = 0 }, 0, 0.01f },
    { effStrAmplitude, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTwinkleTaskDataEntry48 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyTwinkleTaskDataEntry = {
    effCreatePolyTwinkleTaskDataEntry7, 0x8, 8, 0, 0, effCreatePolyTwinkleTaskDataEntry12, 16, &effCreatePolyTwinkleTaskDataEntry62,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyTwinkleTaskDataEntry67 = {
    effCreatePolyTwinkleTaskDataEntry8, 14, { 0 }, 2, { 0 }, (u8 *)&effCreatePolyTwinkleTaskDataEntry111, 0x100,
};

effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry68[16] = {
    { effCreatePolyTwinkleTaskDataEntry92, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry115 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry91, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry116 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry114 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyTwinkleTaskDataEntry111 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTwinkleTaskDataEntry89, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry112 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry112 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry112 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry112 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTwinkleTaskDataEntry90, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry113 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry113 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry113 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry113 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyTwinkleTaskDataEntry49 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTwinkleTaskDataEntry68, 16, &effCreatePolyTwinkleTaskDataEntry67,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry69[6] = {
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry117 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry118 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry118 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry118 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry118 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyTwinkleTaskDataEntry50 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTwinkleTaskDataEntry69, 6, &effCreatePolyTwinkleTaskDataEntry67,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry70[6] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTwinkleTaskDataEntry119 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTwinkleTaskDataEntry120 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry121 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackControlTaskDataEntry76, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry122 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry83, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry123 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyTwinkleTaskDataEntry51 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTwinkleTaskDataEntry70, 6, &effCreatePolyTwinkleTaskDataEntry67,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry13[16] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTwinkleTaskDataEntry51 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTwinkleTaskDataEntry22 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyTwinkleTaskDataEntry23 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effResolveMappingWithTemporaryTable }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditWScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditWRotationTrack }, { .release = btlResetEffectWork } },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyTwinkleTaskDataEntry49 }, { .minValue = 0 }, 0, 0.0f },
    { effStrBillboard, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTwinkleTaskDataEntry50 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry24 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry25 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry26 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry27 }, { .minValue = 0 }, 0, 0.01f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyTwinkleTaskDataEntry2 = {
    effCreatePolyTwinkleTaskDataEntry8, 0x8, 8, 0, 0, effCreatePolyTwinkleTaskDataEntry13, 16, &effCreatePolyTwinkleTaskDataEntry67,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyTwinkleTaskDataEntry71 = {
    effCreatePolyTwinkleTaskDataEntry9, 14, { 0 }, 3, { 0 }, (u8 *)&effCreatePolyTwinkleTaskDataEntry124, 0x114,
};

effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry72[16] = {
    { effCreatePolyTwinkleTaskDataEntry92, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry128 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry91, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry129 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry127 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyTwinkleTaskDataEntry124 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTwinkleTaskDataEntry89, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry125 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry125 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry125 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry125 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTwinkleTaskDataEntry90, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry126 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry126 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry126 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry126 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyTwinkleTaskDataEntry52 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTwinkleTaskDataEntry72, 16, &effCreatePolyTwinkleTaskDataEntry71,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry73[6] = {
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry130 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry131 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry131 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry131 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry131 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyTwinkleTaskDataEntry53 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTwinkleTaskDataEntry73, 6, &effCreatePolyTwinkleTaskDataEntry71,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry74[9] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTwinkleTaskDataEntry132 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTwinkleTaskDataEntry133 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry134 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackControlTaskDataEntry79, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry135 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry30, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry136 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry29, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry137 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyThunderTaskDataEntry28, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry138 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry27, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry139 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyTwinkleTaskDataEntry54 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTwinkleTaskDataEntry74, 9, &effCreatePolyTwinkleTaskDataEntry71,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry75[3] = {
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry140 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry59, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry141 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry142 }, { .minValue = 0 }, 0, 0.001f },
};

effDebugMenu_t effCreatePolyTwinkleTaskDataEntry55 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTwinkleTaskDataEntry75, 3, &effCreatePolyTwinkleTaskDataEntry71,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry14[16] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTwinkleTaskDataEntry54 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTwinkleTaskDataEntry28 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyTwinkleTaskDataEntry29 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effResolveMappingWithTemporaryTable }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditWScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditWRotationTrack }, { .release = btlResetEffectWork } },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyTwinkleTaskDataEntry52 }, { .minValue = 0 }, 0, 0.0f },
    { effStrBillboard, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTwinkleTaskDataEntry53 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry30 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry31 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry32 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTwinkleTaskDataEntry55 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyTwinkleTaskDataEntry3 = {
    effCreatePolyTwinkleTaskDataEntry9, 0x8, 8, 0, 0, effCreatePolyTwinkleTaskDataEntry14, 16, &effCreatePolyTwinkleTaskDataEntry71,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyTwinkleTaskDataEntry76 = {
    effCreatePolyTwinkleTaskDataEntry10, 14, { 0 }, 4, { 0 }, (u8 *)&effCreatePolyTwinkleTaskDataEntry143, 0x108,
};

effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry77[16] = {
    { effCreatePolyTwinkleTaskDataEntry92, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry147 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry91, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry148 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry146 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyTwinkleTaskDataEntry143 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTwinkleTaskDataEntry89, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry144 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry144 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry144 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry144 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTwinkleTaskDataEntry90, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry145 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry145 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry145 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry145 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyTwinkleTaskDataEntry56 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTwinkleTaskDataEntry77, 16, &effCreatePolyTwinkleTaskDataEntry76,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry78[6] = {
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry149 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry150 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry150 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry150 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry150 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyTwinkleTaskDataEntry57 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTwinkleTaskDataEntry78, 6, &effCreatePolyTwinkleTaskDataEntry76,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry79[8] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTwinkleTaskDataEntry151 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTwinkleTaskDataEntry152 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry153 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyThunderTaskDataEntry30, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry154 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry29, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry155 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyThunderTaskDataEntry28, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry156 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry27, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry157 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyTwinkleTaskDataEntry58 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTwinkleTaskDataEntry79, 8, &effCreatePolyTwinkleTaskDataEntry76,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry15[16] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTwinkleTaskDataEntry58 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTwinkleTaskDataEntry33 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyTwinkleTaskDataEntry34 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effResolveMappingWithTemporaryTable }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditWScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditWRotationTrack }, { .release = btlResetEffectWork } },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyTwinkleTaskDataEntry56 }, { .minValue = 0 }, 0, 0.0f },
    { effStrBillboard, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTwinkleTaskDataEntry57 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry35 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry59, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry36 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry37 }, { .minValue = 0 }, 0, 0.001f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry38 }, { .minValue = 0 }, 0, 0.01f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyTwinkleTaskDataEntry4 = {
    effCreatePolyTwinkleTaskDataEntry10, 0x8, 8, 0, 0, effCreatePolyTwinkleTaskDataEntry15, 16, &effCreatePolyTwinkleTaskDataEntry76,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyTwinkleTaskDataEntry5 = {
    effCreatePolyTwinkleTaskDataEntry11, 14, { 0 }, 5, { 0 }, (u8 *)&effCreatePolyTwinkleTaskDataEntry158, 0x108,
};

effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry80[16] = {
    { effCreatePolyTwinkleTaskDataEntry92, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry162 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry91, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry163 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry161 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyTwinkleTaskDataEntry158 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTwinkleTaskDataEntry89, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry159 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry159 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry159 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry159 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTwinkleTaskDataEntry90, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry160 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry160 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry160 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry160 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyTwinkleTaskDataEntry59 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTwinkleTaskDataEntry80, 16, &effCreatePolyTwinkleTaskDataEntry5,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry81[6] = {
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry164 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry165 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry165 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry165 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyTwinkleTaskDataEntry165 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyTwinkleTaskDataEntry60 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTwinkleTaskDataEntry81, 6, &effCreatePolyTwinkleTaskDataEntry5,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry82[8] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTwinkleTaskDataEntry166 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTwinkleTaskDataEntry167 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry168 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackControlTaskDataEntry76, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry169 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry83, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry170 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyWindTaskDataEntry112, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry171 }, { .minValue = 0 }, 1, 0.1f },
    { effCreatePolyWindTaskDataEntry111, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry172 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyTwinkleTaskDataEntry61 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyTwinkleTaskDataEntry82, 8, &effCreatePolyTwinkleTaskDataEntry5,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyTwinkleTaskDataEntry16[16] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTwinkleTaskDataEntry61 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyTwinkleTaskDataEntry39 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyTwinkleTaskDataEntry40 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effResolveMappingWithTemporaryTable }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditWScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditWRotationTrack }, { .release = btlResetEffectWork } },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyTwinkleTaskDataEntry59 }, { .minValue = 0 }, 0, 0.0f },
    { effStrBillboard, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyTwinkleTaskDataEntry60 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry41 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry42 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry43 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyTwinkleTaskDataEntry44 }, { .minValue = 0 }, 0, 0.01f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyTwinkleTaskDataEntry6 = {
    effCreatePolyTwinkleTaskDataEntry11, 0x8, 8, 0, 0, effCreatePolyTwinkleTaskDataEntry16, 16, &effCreatePolyTwinkleTaskDataEntry5,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest sysPOLYWINDDataEntry = {
    sysPOLYWINDDataEntry6, 16, { 0 }, 1, { 0 }, sysPOLYWINDDataEntry8, 0xF8,
};

effDebugMenuItem_t sysPOLYWINDDataEntry65[3] = {
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollNMaterialTmxFile }, { .release = effBattleResourceDescriptorRelease } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effUpdateCameraSnapshotGuidesDataEntry2, EFF_DEBUG_ITEM_COMMAND, { 0 }, { .command = effFileJobSecondaryDataSet }, { .color = 0xFFFFFF } },
};

effDebugMenu_t sysPOLYWINDDataEntry46 = {
    NULL, 0xB8, 8, 0, 0, sysPOLYWINDDataEntry65, 3, &sysPOLYWINDDataEntry,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPOLYWINDDataEntry66[15] = {
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry106 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry155, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry107 }, { .minValue = 0 }, 1, 1.0f },
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry104 }, { .minValue = 0 }, 0, 0.1f },
    { sysPOLYWINDDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry105 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry60, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPOLYWINDDataEntry98 }, { .minValue = 3 }, 0x64, 1.0f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPOLYWINDDataEntry108 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyScalyTaskDataEntry64, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry99 }, { .minValue = 0 }, 0, 1.0f },
    { sndInitDataEntry3, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry100 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry63, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry101 }, { .minValue = 0 }, 0, 1.0f },
    { sndInitDataEntry2, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry102 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry62, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry103 }, { .minValue = 0 }, 0, 0.01f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditNPolygonColorTrack }, { .release = effResetMappingFlags } },
    { effCreatePolyScalyTaskDataEntry61, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditNPolygonFadeTrack }, { .release = effLeaf401 } },
};

effDebugMenu_t sysPOLYWINDDataEntry47 = {
    NULL, 0xB8, 8, 0, 0, sysPOLYWINDDataEntry66, 15, &sysPOLYWINDDataEntry,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPOLYWINDDataEntry67[4] = {
    { sysPOLYWINDDataEntry92, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry113 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYWINDDataEntry91, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry114 }, { .minValue = 0 }, 1, 1.0f },
    { sysPOLYWINDDataEntry90, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry115 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYWINDDataEntry89, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry116 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t sysPOLYWINDDataEntry48 = {
    NULL, 0xB8, 8, 0, 0, sysPOLYWINDDataEntry67, 4, &sysPOLYWINDDataEntry,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPOLYWINDDataEntry68[4] = {
    { effCreatePolyThunderTaskDataEntry30, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry109 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry29, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry110 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyThunderTaskDataEntry28, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry111 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry27, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry112 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t sysPOLYWINDDataEntry49 = {
    NULL, 0xB8, 8, 0, 0, sysPOLYWINDDataEntry68, 4, &sysPOLYWINDDataEntry,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPOLYWINDDataEntry69[3] = {
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry120 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry59, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry121 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry122 }, { .minValue = 0 }, 0, 0.001f },
};

effDebugMenu_t sysPOLYWINDDataEntry50 = {
    NULL, 0xB8, 8, 0, 0, sysPOLYWINDDataEntry69, 3, &sysPOLYWINDDataEntry,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPOLYWINDDataEntry70[3] = {
    { sysPOLYWINDDataEntry95, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry119 }, { .minValue = 0 }, 0, 10.0f },
    { sysPOLYWINDDataEntry94, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry117 }, { .minValue = 0 }, 1, 1.0f },
    { sysPOLYWINDDataEntry93, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry118 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t sysPOLYWINDDataEntry51 = {
    NULL, 0xB8, 8, 0, 0, sysPOLYWINDDataEntry70, 3, &sysPOLYWINDDataEntry,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPOLYWINDDataEntry9[17] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPOLYWINDDataEntry13 }, { .minValue = 0 }, 0, 1.0f },
    { effCreateEnvironmentTaskDataEntry17, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPOLYWINDDataEntry14 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPOLYWINDDataEntry18 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditNColorTrack }, { .release = effResetMappingFlags } },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPOLYWINDDataEntry46 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry24, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPOLYWINDDataEntry15 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry23, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPOLYWINDDataEntry16 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry22, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPOLYWINDDataEntry17 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &sysPOLYWINDDataEntry47 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysStrEmtRadius, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPOLYWINDDataEntry49 }, { .minValue = 0 }, 0, 0.0f },
    { sysStrEmtHeight, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPOLYWINDDataEntry48 }, { .minValue = 0 }, 0, 0.0f },
    { sysStrEmtCurve, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPOLYWINDDataEntry51 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPOLYWINDDataEntry50 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t sysPOLYWINDDataEntry2 = {
    sysPOLYWINDDataEntry6, 0x8, 8, 0, 0, sysPOLYWINDDataEntry9, 17, &sysPOLYWINDDataEntry,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest sysPOLYWINDDataEntry71 = {
    sysPOLYWINDDataEntry7, 16, { 0 }, 2, { 0 }, sysPOLYWINDDataEntry123, 0xF4,
};

effDebugMenuItem_t sysPOLYWINDDataEntry72[3] = {
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollNMaterialTmxFile }, { .release = effBattleResourceDescriptorRelease } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effUpdateCameraSnapshotGuidesDataEntry2, EFF_DEBUG_ITEM_COMMAND, { 0 }, { .command = effFileJobSecondaryDataSet }, { .color = 0xFFFFFF } },
};

effDebugMenu_t sysPOLYWINDDataEntry52 = {
    NULL, 0xB8, 8, 0, 0, sysPOLYWINDDataEntry72, 3, &sysPOLYWINDDataEntry71,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPOLYWINDDataEntry73[15] = {
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry127 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry155, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry128 }, { .minValue = 0 }, 1, 1.0f },
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry125 }, { .minValue = 0 }, 0, 0.1f },
    { sysPOLYWINDDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry126 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry60, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPOLYWINDDataEntry124 }, { .minValue = 3 }, 0x64, 1.0f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &bssWork68 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyScalyTaskDataEntry64, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &bssWork66 }, { .minValue = 0 }, 0, 1.0f },
    { sndInitDataEntry3, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sndInitDataEntry4 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry63, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sndInitDataEntry5 }, { .minValue = 0 }, 0, 1.0f },
    { sndInitDataEntry2, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sndInitDataEntry6 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry62, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &bssWork67 }, { .minValue = 0 }, 0, 0.01f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditNPolygonColorTrack }, { .release = effResetMappingFlags } },
    { effCreatePolyScalyTaskDataEntry61, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditNPolygonFadeTrack }, { .release = effLeaf401 } },
};

effDebugMenu_t sysPOLYWINDDataEntry53 = {
    NULL, 0xB8, 8, 0, 0, sysPOLYWINDDataEntry73, 15, &sysPOLYWINDDataEntry71,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPOLYWINDDataEntry74[5] = {
    { effCreatePolyTrackControlTaskDataEntry79, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry133 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry30, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry129 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry29, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry130 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyThunderTaskDataEntry28, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry131 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry27, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry132 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t sysPOLYWINDDataEntry54 = {
    NULL, 0xB8, 8, 0, 0, sysPOLYWINDDataEntry74, 5, &sysPOLYWINDDataEntry71,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPOLYWINDDataEntry75[3] = {
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry134 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry59, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry135 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry136 }, { .minValue = 0 }, 0, 0.001f },
};

effDebugMenu_t sysPOLYWINDDataEntry55 = {
    NULL, 0xB8, 8, 0, 0, sysPOLYWINDDataEntry75, 3, &sysPOLYWINDDataEntry71,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPOLYWINDDataEntry76[3] = {
    { sysPOLYWINDDataEntry95, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry139 }, { .minValue = 0 }, 0, 10.0f },
    { sysPOLYWINDDataEntry94, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry137 }, { .minValue = 0 }, 1, 1.0f },
    { sysPOLYWINDDataEntry93, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry138 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t sysPOLYWINDDataEntry56 = {
    NULL, 0xB8, 8, 0, 0, sysPOLYWINDDataEntry76, 3, &sysPOLYWINDDataEntry71,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPOLYWINDDataEntry10[19] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPOLYWINDDataEntry19 }, { .minValue = 0 }, 0, 1.0f },
    { effCreateEnvironmentTaskDataEntry17, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPOLYWINDDataEntry20 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPOLYWINDDataEntry24 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditNColorTrack }, { .release = effResetMappingFlags } },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPOLYWINDDataEntry52 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry24, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPOLYWINDDataEntry21 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry23, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPOLYWINDDataEntry22 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry22, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPOLYWINDDataEntry23 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &sysPOLYWINDDataEntry53 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPOLYWINDDataEntry54 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry25 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry26 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry27 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPOLYWINDDataEntry55 }, { .minValue = 0 }, 0, 0.0f },
    { sysPOLYWINDDataEntry95, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPOLYWINDDataEntry56 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t sysPOLYWINDDataEntry3 = {
    sysPOLYWINDDataEntry7, 0x8, 8, 0, 0, sysPOLYWINDDataEntry10, 19, &sysPOLYWINDDataEntry71,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest sysPOLYWINDDataEntry77 = {
    sysPOLYWINDDataEntry96, 16, { 0 }, 3, { 0 }, sysPOLYWINDDataEntry140, 0xD8,
};

effDebugMenuItem_t sysPOLYWINDDataEntry78[3] = {
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollNMaterialTmxFile }, { .release = effBattleResourceDescriptorRelease } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effUpdateCameraSnapshotGuidesDataEntry2, EFF_DEBUG_ITEM_COMMAND, { 0 }, { .command = effFileJobSecondaryDataSet }, { .color = 0xFFFFFF } },
};

effDebugMenu_t sysPOLYWINDDataEntry57 = {
    NULL, 0xB8, 8, 0, 0, sysPOLYWINDDataEntry78, 3, &sysPOLYWINDDataEntry77,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPOLYWINDDataEntry79[15] = {
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry149 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry155, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry150 }, { .minValue = 0 }, 1, 1.0f },
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry147 }, { .minValue = 0 }, 0, 0.1f },
    { sysPOLYWINDDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry148 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry60, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPOLYWINDDataEntry141 }, { .minValue = 3 }, 0x64, 1.0f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPOLYWINDDataEntry151 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyScalyTaskDataEntry64, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry142 }, { .minValue = 0 }, 0, 1.0f },
    { sndInitDataEntry3, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry143 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry63, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry144 }, { .minValue = 0 }, 0, 1.0f },
    { sndInitDataEntry2, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry145 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry62, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry146 }, { .minValue = 0 }, 0, 0.01f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditNPolygonColorTrack }, { .release = effResetMappingFlags } },
    { effCreatePolyScalyTaskDataEntry61, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditNPolygonFadeTrack }, { .release = effLeaf401 } },
};

effDebugMenu_t sysPOLYWINDDataEntry58 = {
    NULL, 0xB8, 8, 0, 0, sysPOLYWINDDataEntry79, 15, &sysPOLYWINDDataEntry77,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPOLYWINDDataEntry80[4] = {
    { effCreatePolyThunderTaskDataEntry30, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry152 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry29, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry153 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyThunderTaskDataEntry28, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry154 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry27, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry155 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t sysPOLYWINDDataEntry59 = {
    NULL, 0xB8, 8, 0, 0, sysPOLYWINDDataEntry80, 4, &sysPOLYWINDDataEntry77,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPOLYWINDDataEntry11[17] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPOLYWINDDataEntry28 }, { .minValue = 0 }, 0, 1.0f },
    { effCreateEnvironmentTaskDataEntry17, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPOLYWINDDataEntry29 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPOLYWINDDataEntry33 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditNColorTrack }, { .release = effResetMappingFlags } },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPOLYWINDDataEntry57 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry24, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPOLYWINDDataEntry30 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry23, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPOLYWINDDataEntry31 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry22, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPOLYWINDDataEntry32 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &sysPOLYWINDDataEntry58 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPOLYWINDDataEntry59 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry34 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry59, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry35 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry36 }, { .minValue = 0 }, 0, 0.001f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t sysPOLYWINDDataEntry4 = {
    sysPOLYWINDDataEntry96, 0x8, 8, 0, 0, sysPOLYWINDDataEntry11, 17, &sysPOLYWINDDataEntry77,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest sysPOLYWINDDataEntry81 = {
    sysPOLYWINDDataEntry97, 16, { 0 }, 4, { 0 }, sysPOLYWINDDataEntry156, 0xF4,
};

effDebugMenuItem_t sysPOLYWINDDataEntry82[3] = {
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollNMaterialTmxFile }, { .release = effBattleResourceDescriptorRelease } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effUpdateCameraSnapshotGuidesDataEntry2, EFF_DEBUG_ITEM_COMMAND, { 0 }, { .command = effFileJobSecondaryDataSet }, { .color = 0xFFFFFF } },
};

effDebugMenu_t sysPOLYWINDDataEntry60 = {
    NULL, 0xB8, 8, 0, 0, sysPOLYWINDDataEntry82, 3, &sysPOLYWINDDataEntry81,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPOLYWINDDataEntry83[15] = {
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry165 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry155, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry166 }, { .minValue = 0 }, 1, 1.0f },
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry163 }, { .minValue = 0 }, 0, 0.1f },
    { sysPOLYWINDDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry164 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry60, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPOLYWINDDataEntry157 }, { .minValue = 3 }, 0x64, 1.0f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPOLYWINDDataEntry167 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyScalyTaskDataEntry64, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry158 }, { .minValue = 0 }, 0, 1.0f },
    { sndInitDataEntry3, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry159 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry63, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry160 }, { .minValue = 0 }, 0, 1.0f },
    { sndInitDataEntry2, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry161 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry62, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry162 }, { .minValue = 0 }, 0, 0.01f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditNPolygonColorTrack }, { .release = effResetMappingFlags } },
    { effCreatePolyScalyTaskDataEntry61, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditNPolygonFadeTrack }, { .release = effLeaf401 } },
};

effDebugMenu_t sysPOLYWINDDataEntry61 = {
    NULL, 0xB8, 8, 0, 0, sysPOLYWINDDataEntry83, 15, &sysPOLYWINDDataEntry81,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPOLYWINDDataEntry84[5] = {
    { effCreatePolyTrackControlTaskDataEntry79, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry172 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry30, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry168 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry29, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry169 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyThunderTaskDataEntry28, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry170 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry27, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry171 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t sysPOLYWINDDataEntry62 = {
    NULL, 0xB8, 8, 0, 0, sysPOLYWINDDataEntry84, 5, &sysPOLYWINDDataEntry81,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPOLYWINDDataEntry85[3] = {
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry173 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry59, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry174 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry175 }, { .minValue = 0 }, 0, 0.001f },
};

effDebugMenu_t sysPOLYWINDDataEntry63 = {
    NULL, 0xB8, 8, 0, 0, sysPOLYWINDDataEntry85, 3, &sysPOLYWINDDataEntry81,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPOLYWINDDataEntry86[3] = {
    { sysPOLYWINDDataEntry95, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry178 }, { .minValue = 0 }, 0, 10.0f },
    { sysPOLYWINDDataEntry94, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry176 }, { .minValue = 0 }, 1, 1.0f },
    { sysPOLYWINDDataEntry93, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry177 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t sysPOLYWINDDataEntry64 = {
    NULL, 0xB8, 8, 0, 0, sysPOLYWINDDataEntry86, 3, &sysPOLYWINDDataEntry81,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t sysPOLYWINDDataEntry12[19] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPOLYWINDDataEntry37 }, { .minValue = 0 }, 0, 1.0f },
    { effCreateEnvironmentTaskDataEntry17, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPOLYWINDDataEntry38 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPOLYWINDDataEntry42 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditNColorTrack }, { .release = effResetMappingFlags } },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPOLYWINDDataEntry60 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry24, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPOLYWINDDataEntry39 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry23, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysPOLYWINDDataEntry40 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry22, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysPOLYWINDDataEntry41 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &sysPOLYWINDDataEntry61 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPOLYWINDDataEntry62 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry43 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry44 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysPOLYWINDDataEntry45 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPOLYWINDDataEntry63 }, { .minValue = 0 }, 0, 0.0f },
    { sysPOLYWINDDataEntry95, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &sysPOLYWINDDataEntry64 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t sysPOLYWINDDataEntry5 = {
    sysPOLYWINDDataEntry97, 0x8, 8, 0, 0, sysPOLYWINDDataEntry12, 19, &sysPOLYWINDDataEntry81,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyScalyTaskDataEntry44 = {
    effCreatePolyScalyTaskDataEntry5, 17, { 0 }, 1, { 0 }, effCreatePolyScalyTaskDataEntry73, 0x10C,
};

effDebugMenuItem_t effCreatePolyScalyTaskDataEntry45[14] = {
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry81 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry155, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry82 }, { .minValue = 0 }, 1, 1.0f },
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry79 }, { .minValue = 0 }, 0, 0.01f },
    { sysPOLYWINDDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry80 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry60, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyScalyTaskDataEntry74 }, { .minValue = 3 }, 0x64, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyScalyTaskDataEntry64, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry75 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyScalyTaskDataEntry66, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry77 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry63, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry76 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyScalyTaskDataEntry65, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry78 }, { .minValue = 0 }, 0, 0.01f },
    { effStrScaleUv, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditUCylinderPolygonUvScaleTrack }, { .release = btlResetEffectWork } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditUCylinderPolygonColorTrack }, { .release = effResetMappingFlags } },
    { effCreatePolyScalyTaskDataEntry61, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditUCylinderPolygonFadeTrack }, { .release = effLeaf401 } },
};

effDebugMenu_t effCreatePolyScalyTaskDataEntry34 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyScalyTaskDataEntry45, 14, &effCreatePolyScalyTaskDataEntry44,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyScalyTaskDataEntry46[5] = {
    { effCreatePolyTrackControlTaskDataEntry79, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry87 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry30, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry83 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry29, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry84 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyThunderTaskDataEntry28, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry85 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry27, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry86 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyScalyTaskDataEntry35 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyScalyTaskDataEntry46, 5, &effCreatePolyScalyTaskDataEntry44,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyScalyTaskDataEntry47[3] = {
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry88 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry59, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry89 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry90 }, { .minValue = 0 }, 0, 0.001f },
};

effDebugMenu_t effCreatePolyScalyTaskDataEntry36 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyScalyTaskDataEntry47, 3, &effCreatePolyScalyTaskDataEntry44,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyScalyTaskDataEntry7[17] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyScalyTaskDataEntry12 }, { .minValue = 0 }, 0, 1.0f },
    { effCreateEnvironmentTaskDataEntry17, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyScalyTaskDataEntry13 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyScalyTaskDataEntry17 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditUCylinderColorTrack }, { .release = effResetMappingFlags } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry24, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyScalyTaskDataEntry14 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry23, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyScalyTaskDataEntry15 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyFlashTaskDataEntry22, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyScalyTaskDataEntry16 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyScalyTaskDataEntry34 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyScalyTaskDataEntry35 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry18 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry19 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry20 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyScalyTaskDataEntry36 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyScalyTaskDataEntry = {
    effCreatePolyScalyTaskDataEntry5, 0x8, 8, 0, 0, effCreatePolyScalyTaskDataEntry7, 17, &effCreatePolyScalyTaskDataEntry44,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyScalyTaskDataEntry48 = {
    effCreatePolyScalyTaskDataEntry67, 17, { 0 }, 2, { 0 }, effCreatePolyScalyTaskDataEntry91, 0x12C,
};

effDebugMenuItem_t effCreatePolyScalyTaskDataEntry49[5] = {
    { effCreatePolyScalyTaskDataEntry64, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry92 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyScalyTaskDataEntry66, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry94 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry63, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry93 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyScalyTaskDataEntry65, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry95 }, { .minValue = 0 }, 0, 0.01f },
    { effStrScaleUv, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditUSmokeSpiralPolygonUvScaleTrack }, { .release = btlResetEffectWork } },
};

effDebugMenu_t effCreatePolyScalyTaskDataEntry37 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyScalyTaskDataEntry49, 5, &effCreatePolyScalyTaskDataEntry48,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyScalyTaskDataEntry50[5] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyScalyTaskDataEntry96 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyScalyTaskDataEntry97 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry98 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackControlTaskDataEntry76, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry99 }, { .minValue = 0 }, 0, 1.0f },
};

effDebugMenu_t effCreatePolyScalyTaskDataEntry38 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyScalyTaskDataEntry50, 5, &effCreatePolyScalyTaskDataEntry48,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyScalyTaskDataEntry51[6] = {
    { effCreatePolyWindTaskDataEntry110, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry100 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry109, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry101 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyWindTaskDataEntry108, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry102 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry107, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry103 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry104 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry105 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyScalyTaskDataEntry39 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyScalyTaskDataEntry51, 6, &effCreatePolyScalyTaskDataEntry48,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyScalyTaskDataEntry8[16] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyScalyTaskDataEntry38 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyScalyTaskDataEntry21 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyScalyTaskDataEntry22 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyScalyTaskDataEntry23 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditUSmokeSpiralColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditUSmokeSpiralScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditUSmokeSpiralRotationTrack }, { .release = btlResetEffectWork } },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyScalyTaskDataEntry37 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry24 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry25 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry26 }, { .minValue = 0 }, 0, 0.01f },
    { effStrAmplitude, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyScalyTaskDataEntry39 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyScalyTaskDataEntry2 = {
    effCreatePolyScalyTaskDataEntry67, 0x8, 8, 0, 0, effCreatePolyScalyTaskDataEntry8, 16, &effCreatePolyScalyTaskDataEntry48,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyScalyTaskDataEntry52 = {
    effCreatePolyScalyTaskDataEntry6, 17, { 0 }, 3, { 0 }, effCreatePolyScalyTaskDataEntry106, 0x130,
};

effDebugMenuItem_t effCreatePolyScalyTaskDataEntry53[5] = {
    { effCreatePolyScalyTaskDataEntry64, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry107 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyScalyTaskDataEntry66, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry109 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry63, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry108 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyScalyTaskDataEntry65, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry110 }, { .minValue = 0 }, 0, 0.01f },
    { effStrScaleUv, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditUSmokeSpiralPolygonUvScaleTrack }, { .release = btlResetEffectWork } },
};

effDebugMenu_t effCreatePolyScalyTaskDataEntry40 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyScalyTaskDataEntry53, 5, &effCreatePolyScalyTaskDataEntry52,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyScalyTaskDataEntry54[9] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyScalyTaskDataEntry111 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyScalyTaskDataEntry112 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry113 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackControlTaskDataEntry79, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry114 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry30, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry115 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry29, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry116 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyThunderTaskDataEntry28, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry117 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry27, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry118 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyScalyTaskDataEntry41 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyScalyTaskDataEntry54, 9, &effCreatePolyScalyTaskDataEntry52,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyScalyTaskDataEntry55[3] = {
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry119 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry59, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry120 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry121 }, { .minValue = 0 }, 0, 0.001f },
};

effDebugMenu_t effCreatePolyScalyTaskDataEntry42 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyScalyTaskDataEntry55, 3, &effCreatePolyScalyTaskDataEntry52,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyScalyTaskDataEntry9[16] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyScalyTaskDataEntry41 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyScalyTaskDataEntry27 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyScalyTaskDataEntry28 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyScalyTaskDataEntry29 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditUSmokeSpiralColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditUSmokeSpiralScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditUSmokeSpiralRotationTrack }, { .release = btlResetEffectWork } },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyScalyTaskDataEntry40 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry30 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry31 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry32 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyScalyTaskDataEntry42 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyScalyTaskDataEntry3 = {
    effCreatePolyScalyTaskDataEntry6, 0x8, 8, 0, 0, effCreatePolyScalyTaskDataEntry9, 16, &effCreatePolyScalyTaskDataEntry52,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyScalyTaskDataEntry56 = {
    effCreatePolyScalyTaskDataEntry68, 17, { 0 }, 4, { 0 }, effCreatePolyScalyTaskDataEntry122, 0x98,
};

effDebugMenuItem_t effCreatePolyScalyTaskDataEntry57[11] = {
    { effCreatePolyScalyTaskDataEntry60, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyScalyTaskDataEntry123 }, { .minValue = 4 }, 0x50, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry76, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry129 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry10, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry130 }, { .minValue = 0 }, 0xB4, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyScalyTaskDataEntry72, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry124 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyScalyTaskDataEntry71, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry125 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyScalyTaskDataEntry70, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry126 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyScalyTaskDataEntry69, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry127 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyScalyTaskDataEntry62, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyScalyTaskDataEntry128 }, { .minValue = 0 }, 0, 0.01f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyScalyTaskDataEntry61, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditUSpherePolygonFadeTrack }, { .release = effLeaf401 } },
};

effDebugMenu_t effCreatePolyScalyTaskDataEntry43 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyScalyTaskDataEntry57, 11, &effCreatePolyScalyTaskDataEntry56,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyScalyTaskDataEntry10[7] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyScalyTaskDataEntry33 }, { .minValue = 0 }, 0, 1.0f },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditUSphereColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditUSphereScaleTrack }, { .release = btlResetEffectWork } },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyScalyTaskDataEntry43 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyScalyTaskDataEntry4 = {
    effCreatePolyScalyTaskDataEntry68, 0x8, 8, 0, 0, effCreatePolyScalyTaskDataEntry10, 7, &effCreatePolyScalyTaskDataEntry56,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest fileLoadEffectSlotBData = {
    fileLoadEffectSlotBDataEntry2, 18, { 0 }, 0, { 0 }, (u8 *)&fileLoadEffectSlotBDataEntry, 0x4,
};

effDebugMenuItem_t effData9Entry17[3] = {
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FLOAT_TENTH, { 0 }, { .f32Value = &fileLoadEffectSlotBDataEntry }, { .minValue = 0 }, 0, 0.1f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effData9Entry10 = {
    fileLoadEffectSlotBDataEntry2, 0x8, 8, 0, 0, effData9Entry17, 3, &fileLoadEffectSlotBData,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyCrackTaskDataEntry10 = {
    effCreatePolyCrackTaskDataEntry15, 19, { 0 }, 1, { 0 }, effCreatePolyCrackTaskDataEntry18, 0xC0,
};

effDebugMenuItem_t effCreatePolyCrackTaskDataEntry11[1] = {
    { effCreatePolyCrackTaskDataEntry14, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollHlpPbResourceFile }, { .release = effBattleResourceDescriptorRelease } },
};

effDebugMenu_t effCreatePolyCrackTaskDataEntry7 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyCrackTaskDataEntry11, 1, &effCreatePolyCrackTaskDataEntry10,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyCrackTaskDataEntry12[16] = {
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditCrackPolygonColorTrack }, { .release = effResetMappingFlags } },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyCrackTaskDataEntry19 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyCrackTaskDataEntry16, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyCrackTaskDataEntry20 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry88, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyCrackTaskDataEntry23 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry153, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyCrackTaskDataEntry21 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyCrackTaskDataEntry21 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyCrackTaskDataEntry21 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyCrackTaskDataEntry21 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry152, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &effCreatePolyCrackTaskDataEntry22 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &effCreatePolyCrackTaskDataEntry22 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &effCreatePolyCrackTaskDataEntry22 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &effCreatePolyCrackTaskDataEntry22 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyCrackTaskDataEntry8 = {
    effStrCrackPolygon, 0xB8, 8, 0, 0, effCreatePolyCrackTaskDataEntry12, 16, &effCreatePolyCrackTaskDataEntry10,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t datData29[18] = {
    { effData50, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &bssWork74 }, { .minValue = 0 }, 0, 1.0f },
    { effData49, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &bssWork75 }, { .minValue = 0 }, 0, 1.0f },
    { effData48, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &bssWork76 }, { .minValue = 0 }, 1, 1.0f },
    { sysPOLYWINDDataEntry87, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &bssWork77 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyRingTaskDataEntry9, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &bssWork78 }, { .minValue = 0 }, 1, 1.0f },
    { effData51, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &bssWork79 }, { .minValue = 1 }, 0xFFFFFFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry153, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &bssWork72 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &bssWork72 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &bssWork72 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &bssWork72 }, { .minValue = 0 }, 0xFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyFlashTaskDataEntry152, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &bssWork73 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &bssWork73 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &bssWork73 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry84, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &bssWork73 }, { .minValue = 0 }, 0xFF, 1.0f },
};

effDebugMenu_t effCreatePolyCrackTaskDataEntry31 = {
    NULL, 0xB8, 8, 0, 0, datData29, 18, &effCreatePolyCrackTaskDataEntry10,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyCrackTaskDataEntry13[11] = {
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditFlashPolygonColorTrack }, { .release = effResetMappingFlags } },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyCrackTaskDataEntry24 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyCrackTaskDataEntry16, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyCrackTaskDataEntry25 }, { .minValue = 0 }, 1, 1.0f },
    { effCreateEnvironmentTaskDataEntry18, EFF_DEBUG_ITEM_BYTE, { 0 }, { .u8Value = &effCreatePolyCrackTaskDataEntry26 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyThunderTaskDataEntry8, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyCrackTaskDataEntry27 }, { .minValue = 0 }, 0, 1.0f },
    { effCreateBlurTaskDataEntry20, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyCrackTaskDataEntry28 }, { .minValue = 1 }, 0xFFFFFFF, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyCrackTaskDataEntry29 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyCrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyCrackTaskDataEntry30 }, { .minValue = 0 }, 0, 0.001f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry20, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyCrackTaskDataEntry31 }, { .minValue = 0 }, 0, 0.0f },
};

effDebugMenu_t effCreatePolyCrackTaskDataEntry9 = {
    effStrFlashPolygon, 0xB8, 8, 0, 0, effCreatePolyCrackTaskDataEntry13, 11, &effCreatePolyCrackTaskDataEntry10,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyCrackTaskDataEntry2[8] = {
    { effCreatePolyCrackTaskDataEntry4, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyCrackTaskDataEntry8 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyCrackTaskDataEntry3, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyCrackTaskDataEntry9 }, { .minValue = 0 }, 0, 0.0f },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyCrackTaskDataEntry7 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FLOAT_TENTH, { 0 }, { .f32Value = &effCreatePolyCrackTaskDataEntry5 }, { .minValue = 0 }, 0, 0.1f },
    { sysPOLYTEXDataEntry11, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyCrackTaskDataEntry6 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyCrackTaskDataEntry = {
    effCreatePolyCrackTaskDataEntry15, 0x8, 8, 0, 0, effCreatePolyCrackTaskDataEntry2, 8, &effCreatePolyCrackTaskDataEntry10,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyWindTaskDataEntry83 = {
    effCreatePolyWindTaskDataEntry7, 24, { 0 }, 1, { 0 }, (u8 *)&effCreatePolyWindTaskDataEntry123, 0x140,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry84[11] = {
    { effCreatePolyWindTaskDataEntry120, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyWindTaskDataEntry123 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry119, EFF_DEBUG_ITEM_BYTE_TOGGLE, { 0 }, { .u8Value = &effCreatePolyWindTaskDataEntry130 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyWindTaskDataEntry118, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyWindTaskDataEntry117, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry124 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry116, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry125 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry115, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry126 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry127 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyWindTaskDataEntry114, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry128 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyWindTaskDataEntry113, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry129 }, { .minValue = 0 }, 0, 0.001f },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry63 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyWindTaskDataEntry84, 11, &effCreatePolyWindTaskDataEntry83,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry85[2] = {
    { effCreatePolyWindTaskDataEntry121, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollMMaterialPbFile }, { .release = effBattleResourceDescriptorRelease } },
    { sysPARTICLEDataEntry110, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollMMaterialEplFile }, { .release = effBattleResourceDescriptorRelease } },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry64 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyWindTaskDataEntry85, 2, &effCreatePolyWindTaskDataEntry83,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry86[5] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyWindTaskDataEntry131 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyWindTaskDataEntry132 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry133 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackControlTaskDataEntry76, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry134 }, { .minValue = 0 }, 0, 1.0f },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry65 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyWindTaskDataEntry86, 5, &effCreatePolyWindTaskDataEntry83,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry87[6] = {
    { effCreatePolyWindTaskDataEntry110, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry135 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry109, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry136 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyWindTaskDataEntry108, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry137 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry107, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry138 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry139 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry140 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry66 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyWindTaskDataEntry87, 6, &effCreatePolyWindTaskDataEntry83,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry12[16] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyWindTaskDataEntry65 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyWindTaskDataEntry21 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyWindTaskDataEntry22 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyWindTaskDataEntry23 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObjectNoResult = effEditMColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditMScaleTrack }, { .release = btlResetEffectWork } },
    { effCreatePolyWindTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyWindTaskDataEntry63 }, { .minValue = 0 }, 0, 0.0f },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyWindTaskDataEntry64 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry24 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry25 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry26 }, { .minValue = 0 }, 0, 0.01f },
    { effStrAmplitude, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyWindTaskDataEntry66 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry = {
    effCreatePolyWindTaskDataEntry7, 0x8, 8, 0, 0, effCreatePolyWindTaskDataEntry12, 16, &effCreatePolyWindTaskDataEntry83,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyWindTaskDataEntry88 = {
    effCreatePolyWindTaskDataEntry122, 24, { 0 }, 2, { 0 }, (u8 *)&effCreatePolyWindTaskDataEntry141, 0x130,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry89[11] = {
    { effCreatePolyWindTaskDataEntry120, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyWindTaskDataEntry141 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry119, EFF_DEBUG_ITEM_BYTE_TOGGLE, { 0 }, { .u8Value = &effCreatePolyWindTaskDataEntry148 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyWindTaskDataEntry118, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyWindTaskDataEntry117, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry142 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry116, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry143 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry115, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry144 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry145 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyWindTaskDataEntry114, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry146 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyWindTaskDataEntry113, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry147 }, { .minValue = 0 }, 0, 0.001f },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry67 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyWindTaskDataEntry89, 11, &effCreatePolyWindTaskDataEntry88,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry90[2] = {
    { effCreatePolyWindTaskDataEntry121, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollMMaterialPbFile }, { .release = effBattleResourceDescriptorRelease } },
    { sysPARTICLEDataEntry110, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollMMaterialEplFile }, { .release = effBattleResourceDescriptorRelease } },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry68 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyWindTaskDataEntry90, 2, &effCreatePolyWindTaskDataEntry88,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry91[6] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyWindTaskDataEntry149 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyWindTaskDataEntry150 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry151 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackControlTaskDataEntry76, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry152 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry83, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry153 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry69 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyWindTaskDataEntry91, 6, &effCreatePolyWindTaskDataEntry88,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry13[16] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyWindTaskDataEntry69 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyWindTaskDataEntry27 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyWindTaskDataEntry28 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyWindTaskDataEntry29 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObjectNoResult = effEditMColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditMScaleTrack }, { .release = btlResetEffectWork } },
    { effCreatePolyWindTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyWindTaskDataEntry67 }, { .minValue = 0 }, 0, 0.0f },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyWindTaskDataEntry68 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry30 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry31 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry32 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry33 }, { .minValue = 0 }, 0, 0.01f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry2 = {
    effCreatePolyWindTaskDataEntry122, 0x8, 8, 0, 0, effCreatePolyWindTaskDataEntry13, 16, &effCreatePolyWindTaskDataEntry88,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyWindTaskDataEntry14 = {
    effCreatePolyWindTaskDataEntry8, 24, { 0 }, 3, { 0 }, (u8 *)&effCreatePolyWindTaskDataEntry34, 0x144,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry92[11] = {
    { effCreatePolyWindTaskDataEntry120, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyWindTaskDataEntry34 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry119, EFF_DEBUG_ITEM_BYTE_TOGGLE, { 0 }, { .u8Value = &effCreatePolyWindTaskDataEntry160 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyWindTaskDataEntry118, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyWindTaskDataEntry117, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry154 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry116, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry155 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry115, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry156 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry157 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyWindTaskDataEntry114, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry158 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyWindTaskDataEntry113, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry159 }, { .minValue = 0 }, 0, 0.001f },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry70 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyWindTaskDataEntry92, 11, &effCreatePolyWindTaskDataEntry14,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry93[2] = {
    { effCreatePolyWindTaskDataEntry121, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollMMaterialPbFile }, { .release = effBattleResourceDescriptorRelease } },
    { sysPARTICLEDataEntry110, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollMMaterialEplFile }, { .release = effBattleResourceDescriptorRelease } },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry71 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyWindTaskDataEntry93, 2, &effCreatePolyWindTaskDataEntry14,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry94[9] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyWindTaskDataEntry161 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sbtlRefSbtl51Data7Entry3 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sbtlRefSbtl51Data7Entry4 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackControlTaskDataEntry79, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sbtlRefSbtl51Data7Entry5 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry30, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sbtlRefSbtl51Data7Entry6 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry29, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sbtlRefSbtl51Data7Entry7 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyThunderTaskDataEntry28, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sbtlRefSbtl51Data7Entry8 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry27, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sbtlRefSbtl51Data7Entry9 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry72 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyWindTaskDataEntry94, 9, &effCreatePolyWindTaskDataEntry14,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry95[3] = {
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry162 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry59, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry163 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry164 }, { .minValue = 0 }, 0, 0.001f },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry73 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyWindTaskDataEntry95, 3, &effCreatePolyWindTaskDataEntry14,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry15[16] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyWindTaskDataEntry72 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyWindTaskDataEntry35 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyWindTaskDataEntry36 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyWindTaskDataEntry37 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObjectNoResult = effEditMColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditMScaleTrack }, { .release = btlResetEffectWork } },
    { effCreatePolyWindTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyWindTaskDataEntry70 }, { .minValue = 0 }, 0, 0.0f },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyWindTaskDataEntry71 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry38 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry39 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry40 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyWindTaskDataEntry73 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry3 = {
    effCreatePolyWindTaskDataEntry8, 0x8, 8, 0, 0, effCreatePolyWindTaskDataEntry15, 16, &effCreatePolyWindTaskDataEntry14,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyWindTaskDataEntry96 = {
    effCreatePolyWindTaskDataEntry9, 24, { 0 }, 4, { 0 }, (u8 *)&effCreatePolyWindTaskDataEntry165, 0x138,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry97[11] = {
    { effCreatePolyWindTaskDataEntry120, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyWindTaskDataEntry165 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry119, EFF_DEBUG_ITEM_BYTE_TOGGLE, { 0 }, { .u8Value = &effCreatePolyWindTaskDataEntry172 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyWindTaskDataEntry118, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyWindTaskDataEntry117, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry166 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry116, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry167 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry115, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry168 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry169 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyWindTaskDataEntry114, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry170 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyWindTaskDataEntry113, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry171 }, { .minValue = 0 }, 0, 0.001f },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry74 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyWindTaskDataEntry97, 11, &effCreatePolyWindTaskDataEntry96,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry98[2] = {
    { effCreatePolyWindTaskDataEntry121, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollMMaterialPbFile }, { .release = effBattleResourceDescriptorRelease } },
    { sysPARTICLEDataEntry110, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollMMaterialEplFile }, { .release = effBattleResourceDescriptorRelease } },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry75 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyWindTaskDataEntry98, 2, &effCreatePolyWindTaskDataEntry96,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry99[8] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyWindTaskDataEntry173 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyWindTaskDataEntry174 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry175 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyThunderTaskDataEntry30, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry176 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry29, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry177 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyThunderTaskDataEntry28, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry178 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyThunderTaskDataEntry27, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry179 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry76 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyWindTaskDataEntry99, 8, &effCreatePolyWindTaskDataEntry96,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry16[16] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyWindTaskDataEntry76 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyWindTaskDataEntry41 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyWindTaskDataEntry42 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyWindTaskDataEntry43 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObjectNoResult = effEditMColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditMScaleTrack }, { .release = btlResetEffectWork } },
    { effCreatePolyWindTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyWindTaskDataEntry74 }, { .minValue = 0 }, 0, 0.0f },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyWindTaskDataEntry75 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyScalyTaskDataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry44 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyScalyTaskDataEntry59, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry45 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry46 }, { .minValue = 0 }, 0, 0.001f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry47 }, { .minValue = 0 }, 0, 0.01f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry4 = {
    effCreatePolyWindTaskDataEntry9, 0x8, 8, 0, 0, effCreatePolyWindTaskDataEntry16, 16, &effCreatePolyWindTaskDataEntry96,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyWindTaskDataEntry100 = {
    effCreatePolyWindTaskDataEntry10, 24, { 0 }, 5, { 0 }, (u8 *)&effCreatePolyWindTaskDataEntry180, 0x138,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry101[11] = {
    { effCreatePolyWindTaskDataEntry120, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyWindTaskDataEntry180 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry119, EFF_DEBUG_ITEM_BYTE_TOGGLE, { 0 }, { .u8Value = &effCreatePolyWindTaskDataEntry187 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyWindTaskDataEntry118, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyWindTaskDataEntry117, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry181 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry116, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry182 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry115, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry183 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry184 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyWindTaskDataEntry114, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry185 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyWindTaskDataEntry113, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry186 }, { .minValue = 0 }, 0, 0.001f },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry77 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyWindTaskDataEntry101, 11, &effCreatePolyWindTaskDataEntry100,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry102[2] = {
    { effCreatePolyWindTaskDataEntry121, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollMMaterialPbFile }, { .release = effBattleResourceDescriptorRelease } },
    { sysPARTICLEDataEntry110, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollMMaterialEplFile }, { .release = effBattleResourceDescriptorRelease } },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry78 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyWindTaskDataEntry102, 2, &effCreatePolyWindTaskDataEntry100,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry103[8] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyWindTaskDataEntry188 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyWindTaskDataEntry189 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry190 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackControlTaskDataEntry76, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry191 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry83, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry192 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyWindTaskDataEntry112, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry193 }, { .minValue = 0 }, 1, 0.1f },
    { effCreatePolyWindTaskDataEntry111, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry194 }, { .minValue = 0 }, 1, 1.0f },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry79 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyWindTaskDataEntry103, 8, &effCreatePolyWindTaskDataEntry100,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry17[16] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyWindTaskDataEntry79 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyWindTaskDataEntry48 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyWindTaskDataEntry49 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyWindTaskDataEntry50 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObjectNoResult = effEditMColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditMScaleTrack }, { .release = btlResetEffectWork } },
    { effCreatePolyWindTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyWindTaskDataEntry77 }, { .minValue = 0 }, 0, 0.0f },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyWindTaskDataEntry78 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry51 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry52 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry53 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry54 }, { .minValue = 0 }, 0, 0.01f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry5 = {
    effCreatePolyWindTaskDataEntry10, 0x8, 8, 0, 0, effCreatePolyWindTaskDataEntry17, 16, &effCreatePolyWindTaskDataEntry100,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreatePolyWindTaskDataEntry18 = {
    effCreatePolyWindTaskDataEntry11, 24, { 0 }, 6, { 0 }, (u8 *)&effCreatePolyWindTaskDataEntry55, 0x13C,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry104[11] = {
    { effCreatePolyWindTaskDataEntry120, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyWindTaskDataEntry55 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry119, EFF_DEBUG_ITEM_BYTE_TOGGLE, { 0 }, { .u8Value = &effCreatePolyWindTaskDataEntry201 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyWindTaskDataEntry118, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyWindTaskDataEntry117, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry195 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry116, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry196 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry115, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry197 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry198 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyWindTaskDataEntry114, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry199 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyWindTaskDataEntry113, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry200 }, { .minValue = 0 }, 0, 0.001f },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry80 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyWindTaskDataEntry104, 11, &effCreatePolyWindTaskDataEntry18,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry105[2] = {
    { effCreatePolyWindTaskDataEntry121, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollMMaterialPbFile }, { .release = effBattleResourceDescriptorRelease } },
    { sysPARTICLEDataEntry110, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollMMaterialEplFile }, { .release = effBattleResourceDescriptorRelease } },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry81 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyWindTaskDataEntry105, 2, &effCreatePolyWindTaskDataEntry18,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry106[9] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyWindTaskDataEntry202 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry78, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyWindTaskDataEntry203 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry77, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry204 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackControlTaskDataEntry76, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry205 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry83, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry206 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyWindTaskDataEntry112, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry207 }, { .minValue = 0 }, 1, 0.1f },
    { effCreatePolyWindTaskDataEntry111, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry208 }, { .minValue = 0 }, 1, 1.0f },
    { sysPARTICLEDataEntry113, 0x05, { 0 }, { .s32Value = &effCreatePolyWindTaskDataEntry209 }, { .minValue = 0 }, 0x168, 1.0f },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry82 = {
    NULL, 0xB8, 8, 0, 0, effCreatePolyWindTaskDataEntry106, 9, &effCreatePolyWindTaskDataEntry18,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreatePolyWindTaskDataEntry19[16] = {
    { effCreatePolyTrackTaskDataEntry19, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyWindTaskDataEntry82 }, { .minValue = 0 }, 0, 0.0f },
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreatePolyWindTaskDataEntry56 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry18, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyWindTaskDataEntry57 }, { .minValue = 0 }, 0, 1.0f },
    { effData20Entry2, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &effCreatePolyWindTaskDataEntry58 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObjectNoResult = effEditMColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditMScaleTrack }, { .release = btlResetEffectWork } },
    { effCreatePolyWindTaskDataEntry20, EFF_DEBUG_ITEM_CHILD_PAGE, { 0 }, { .child = &effCreatePolyWindTaskDataEntry80 }, { .minValue = 0 }, 0, 0.0f },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreatePolyWindTaskDataEntry81 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effCreatePolyTrackTaskDataEntry16, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry59 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyTrackTaskDataEntry15, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry60 }, { .minValue = 0 }, 1, 1.0f },
    { effCreatePolyScalyTaskDataEntry58, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry61 }, { .minValue = 0 }, 0, 0.01f },
    { effCreatePolyTrackTaskDataEntry17, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &effCreatePolyWindTaskDataEntry62 }, { .minValue = 0 }, 0, 0.01f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreatePolyWindTaskDataEntry6 = {
    effCreatePolyWindTaskDataEntry11, 0x8, 8, 0, 0, effCreatePolyWindTaskDataEntry19, 16, &effCreatePolyWindTaskDataEntry18,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest sysBATTLEONLYDataEntry = {
    sysBATTLEONLYDataEntry22, 20, { 0 }, 1, { 0 }, (u8 *)&sysBATTLEONLYDataEntry82, 0x28,
};

effDebugMenuItem_t sysBATTLEONLYDataEntry34[26] = {
    { sysBATTLEONLYDataEntry52, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry82 }, { .minValue = 0 }, 0, 1.0f },
    { sysBATTLEONLYDataEntry58, 0x20, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry89 }, { .minValue = 1 }, 7, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_BLANK },
    { sysBATTLEONLYDataEntry50, 0x05, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry83 }, { .minValue = -1 }, 0xFFFFFF, 1.0f },
    { sysBATTLEONLYDataEntry49, 0x05, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry84 }, { .minValue = -1 }, 0xFFFFFF, 1.0f },
    { effInitializeResourceQueueData, EFF_DEBUG_ITEM_BLANK },
    { sysBATTLEONLYDataEntry57, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry87 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry87 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry87 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effInitializeResourceQueueData, EFF_DEBUG_ITEM_BLANK },
    { sysBATTLEONLYDataEntry56, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry88 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry88 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry88 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effInitializeResourceQueueData, EFF_DEBUG_ITEM_BLANK },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysStrDirection, EFF_DEBUG_ITEM_BLANK },
    { sysBATTLEONLYDataEntry50, 0x05, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry85 }, { .minValue = -1 }, 0xFFFFFF, 1.0f },
    { sysBATTLEONLYDataEntry49, 0x05, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry86 }, { .minValue = -1 }, 0xFFFFFF, 1.0f },
    { sysBATTLEONLYDataEntry55, 0x22, { 0 }, { .u8Value = &sysBATTLEONLYDataEntry90 }, { .minValue = 0 }, 7, 1.0f },
    { sysBATTLEONLYDataEntry54, 0x23, { 0 }, { .u8Value = &sysBATTLEONLYDataEntry91 }, { .minValue = 0 }, 5, 1.0f },
    { sysBATTLEONLYDataEntry53, 0x24, { 0 }, { .u8Value = &sysBATTLEONLYDataEntry92 }, { .minValue = 0 }, 0xA, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t sysBATTLEONLYDataEntry2 = {
    sysBATTLEONLYDataEntry22, 0x8, 8, 0, 0, sysBATTLEONLYDataEntry34, 26, &sysBATTLEONLYDataEntry,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest sysBATTLEONLYDataEntry3 = {
    sysBATTLEONLYDataEntry23, 20, { 0 }, 2, { 0 }, (u8 *)&sysBATTLEONLYDataEntry93, 0x14,
};

effDebugMenuItem_t sysBATTLEONLYDataEntry35[12] = {
    { sysBATTLEONLYDataEntry52, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry93 }, { .minValue = 0 }, 0, 1.0f },
    { sysBATTLEONLYDataEntry58, 0x20, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry97 }, { .minValue = 0 }, 7, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysBATTLEONLYDataEntry50, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry94 }, { .minValue = 0 }, 0, 1.0f },
    { sysBATTLEONLYDataEntry49, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry95 }, { .minValue = 0 }, 0, 1.0f },
    { effInitializeResourceQueueData, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry96 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry96 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry96 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effInitializeResourceQueueData, EFF_DEBUG_ITEM_BLANK },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t sysBATTLEONLYDataEntry4 = {
    sysBATTLEONLYDataEntry23, 0x8, 8, 0, 0, sysBATTLEONLYDataEntry35, 12, &sysBATTLEONLYDataEntry3,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest sysBATTLEONLYDataEntry5 = {
    sysBATTLEONLYDataEntry24, 20, { 0 }, 3, { 0 }, (u8 *)&sysBATTLEONLYDataEntry98, 0x10,
};

effDebugMenuItem_t sysBATTLEONLYDataEntry36[17] = {
    { sysBATTLEONLYDataEntry57, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry98 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry98 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry98 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effInitializeResourceQueueData, EFF_DEBUG_ITEM_BLANK },
    { sysBATTLEONLYDataEntry56, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry99 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry99 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry99 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effInitializeResourceQueueData, EFF_DEBUG_ITEM_BLANK },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysStrDirection, EFF_DEBUG_ITEM_BLANK },
    { sysBATTLEONLYDataEntry55, 0x22, { 0 }, { .u8Value = &sysBATTLEONLYDataEntry100 }, { .minValue = 1 }, 7, 1.0f },
    { sysBATTLEONLYDataEntry54, 0x23, { 0 }, { .u8Value = &sysBATTLEONLYDataEntry101 }, { .minValue = 0 }, 5, 1.0f },
    { sysBATTLEONLYDataEntry53, 0x24, { 0 }, { .u8Value = &sysBATTLEONLYDataEntry102 }, { .minValue = 0 }, 0xA, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t sysBATTLEONLYDataEntry6 = {
    sysBATTLEONLYDataEntry24, 0x8, 8, 1, 0, sysBATTLEONLYDataEntry36, 17, &sysBATTLEONLYDataEntry5,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effLoadFileSlotF2Data = {
    effStrOF2Cam, 20, { 0 }, 4, { 0 }, (u8 *)&effData9Entry31, 0xC,
};

effDebugMenuItem_t effData9Entry18[8] = {
    { effData9Entry25, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effData9Entry31 }, { .minValue = 0 }, 0, 1.0f },
    { effData9Entry24, 0x21, { 0 }, { .s32Value = &effData9Entry35 }, { .minValue = 1 }, 8, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effData9Entry22, 0x22, { 0 }, { .u8Value = &effData9Entry32 }, { .minValue = 1 }, 7, 1.0f },
    { effCreatePolyTrackControlTaskDataEntry79, 0x23, { 0 }, { .u8Value = &effData9Entry33 }, { .minValue = 0 }, 5, 1.0f },
    { effData9Entry23, 0x24, { 0 }, { .u8Value = &effData9Entry34 }, { .minValue = 0 }, 0xA, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effData9Entry11 = {
    effStrOF2Cam, 0x8, 8, 0, 0, effData9Entry18, 8, &effLoadFileSlotF2Data,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest sysBATTLEONLYDataEntry37 = {
    sysBATTLEONLYDataEntry25, 20, { 0 }, 5, { 0 }, (u8 *)&sysBATTLEONLYDataEntry77, 0x8,
};

effDebugMenuItem_t sysBATTLEONLYDataEntry38[6] = {
    { effCreateEnvironmentTaskDataEntry17, 0x29, { 0 }, { .u16Value = &sysBATTLEONLYDataEntry77 }, { .minValue = 0 }, 0x1C, 1.0f },
    { sysBATTLEONLYDataEntry61, 0x2A, { 0 }, { .u16Value = &sysBATTLEONLYDataEntry78 }, { .minValue = 0 }, 2, 1.0f },
    { sysBATTLEONLYDataEntry60, 0x20, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry79 }, { .minValue = 1 }, 7, 1.0f },
    { sysBATTLEONLYDataEntry59, EFF_DEBUG_ITEM_BYTE_TOGGLE, { 0 }, { .u8Value = &sysBATTLEONLYDataEntry80 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t sysBATTLEONLYDataEntry7 = {
    sysBATTLEONLYDataEntry25, 0x8, 8, 0, 0, sysBATTLEONLYDataEntry38, 6, &sysBATTLEONLYDataEntry37,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest sysBATTLEONLYDataEntry8 = {
    sysBATTLEONLYDataEntry26, 20, { 0 }, 6, { 0 }, (u8 *)&sysBATTLEONLYDataEntry81, 0x4,
};

effDebugMenuItem_t sysBATTLEONLYDataEntry39[3] = {
    { sysBATTLEONLYDataEntry62, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry81 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t sysBATTLEONLYDataEntry9 = {
    sysBATTLEONLYDataEntry26, 0x8, 8, 0, 0, sysBATTLEONLYDataEntry39, 3, &sysBATTLEONLYDataEntry8,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest sysBATTLEONLYDataEntry10 = {
    sysBATTLEONLYDataEntry27, 20, { 0 }, 7, { 0 }, (u8 *)&sysBATTLEONLYDataEntry31, 0xC,
};

effDebugMenuItem_t sysBATTLEONLYDataEntry40[14] = {
    { sysBATTLEONLYDataEntry63, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry104 }, { .minValue = 0 }, 0, 1.0f },
    { effInitializeResourceQueueData, EFF_DEBUG_ITEM_BLANK },
    { sysBATTLEONLYDataEntry57, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry31 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry31 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry31 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effInitializeResourceQueueData, EFF_DEBUG_ITEM_BLANK },
    { sysBATTLEONLYDataEntry56, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry103 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry103 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry103 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effInitializeResourceQueueData, EFF_DEBUG_ITEM_BLANK },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t sysBATTLEONLYDataEntry11 = {
    sysBATTLEONLYDataEntry27, 0x8, 8, 0, 0, sysBATTLEONLYDataEntry40, 14, &sysBATTLEONLYDataEntry10,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest sysBATTLEONLYDataEntry12 = {
    sysBATTLEONLYDataEntry28, 20, { 0 }, 8, { 0 }, (u8 *)&sysBATTLEONLYDataEntry105, 0x1C,
};

effDebugMenuItem_t sysBATTLEONLYDataEntry41[11] = {
    { sysBATTLEONLYDataEntry65, EFF_DEBUG_ITEM_BLANK },
    { sysBATTLEONLYDataEntry52, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry105 }, { .minValue = 0 }, 0, 1.0f },
    { sysBATTLEONLYDataEntry51, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysBATTLEONLYDataEntry109 }, { .minValue = 0 }, 1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysBATTLEONLYDataEntry64, EFF_DEBUG_ITEM_BLANK },
    { sysBATTLEONLYDataEntry52, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry106 }, { .minValue = 0 }, 0, 1.0f },
    { sysBATTLEONLYDataEntry50, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry107 }, { .minValue = 0 }, 0, 1.0f },
    { sysBATTLEONLYDataEntry49, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry108 }, { .minValue = 0 }, 0, 1.0f },
    { sysBATTLEONLYDataEntry51, EFF_DEBUG_ITEM_BYTE, { 0 }, { .u8Value = &sysBATTLEONLYDataEntry110 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t sysBATTLEONLYDataEntry13 = {
    sysBATTLEONLYDataEntry28, 0x8, 8, 0, 0, sysBATTLEONLYDataEntry41, 11, &sysBATTLEONLYDataEntry12,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest sysBATTLEONLYDataEntry42 = {
    sysBATTLEONLYDataEntry29, 20, { 0 }, 9, { 0 }, (u8 *)&sysBATTLEONLYDataEntry111, 0x10,
};

effDebugMenuItem_t sysBATTLEONLYDataEntry43[6] = {
    { sysBATTLEONLYDataEntry52, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry111 }, { .minValue = 0 }, 0, 1.0f },
    { sysBATTLEONLYDataEntry50, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry113 }, { .minValue = 0 }, 0, 1.0f },
    { sysBATTLEONLYDataEntry49, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry114 }, { .minValue = 0 }, 0, 1.0f },
    { sysBATTLEONLYDataEntry66, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry112 }, { .minValue = 0 }, 0x80, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t sysBATTLEONLYDataEntry14 = {
    sysBATTLEONLYDataEntry29, 0x8, 8, 0, 0, sysBATTLEONLYDataEntry43, 6, &sysBATTLEONLYDataEntry42,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest sysBATTLEONLYDataEntry15 = {
    sysBATTLEONLYDataEntry30, 20, { 0 }, 10, { 0 }, (u8 *)&sysBATTLEONLYDataEntry32, 0xC,
};

effDebugMenuItem_t sysBATTLEONLYDataEntry44[5] = {
    { sysBATTLEONLYDataEntry68, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry32 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry110, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysBATTLEONLYDataEntry115 }, { .minValue = 1 }, 0x96, 1.0f },
    { sysBATTLEONLYDataEntry67, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &sysBATTLEONLYDataEntry116 }, { .minValue = 1 }, 0x96, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t sysBATTLEONLYDataEntry16 = {
    sysBATTLEONLYDataEntry30, 0x8, 8, 0, 0, sysBATTLEONLYDataEntry44, 5, &sysBATTLEONLYDataEntry15,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effMuzzleFlash_Params_t effData9Entry37 = { 1.0f, 1.0f };

EffFileJobRequest effLoadMaterialFileData = {
    effLoadMaterialFileDataEntry, 22, { 0 }, 0, { 0 }, (u8 *)&effData9Entry37, 0x38,
};

effDebugMenuItem_t effData9Entry19[4] = {
    { effData9Entry27, EFF_DEBUG_ITEM_FLOAT_TENTH, { 0 }, { .f32Value = &effData9Entry37.scale_h }, { .minValue = 0 }, 0, 0.1f },
    { effData9Entry26, EFF_DEBUG_ITEM_FLOAT_TENTH, { 0 }, { .f32Value = &effData9Entry37.scale_w }, { .minValue = 0 }, 0, 0.1f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effData9Entry12 = {
    effLoadMaterialFileDataEntry, 0x8, 8, 0, 0, effData9Entry19, 4, &effLoadMaterialFileData,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest effCreateTwoDimensionalTaskDataEntry2 = {
    effCreateTwoDimensionalTaskData2, 23, { 0 }, 1, { 0 }, effCreateTwoDimensionalTaskDataEntry4, 0x98,
};

effDebugMenuItem_t effCreateTwoDimensionalTaskDataEntry9[2] = {
    { sysStrResource, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollResourceQueueWithTlp }, { .release = effReleaseResourceBankDescriptors } },
    { sysPARTICLEDataEntry112, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .poll = effPollTextureResourceFile }, { .release = effBattleResourceDescriptorRelease } },
};

effDebugMenu_t effCreateTwoDimensionalTaskDataEntry8 = {
    NULL, 0xB8, 8, 0, 0, effCreateTwoDimensionalTaskDataEntry9, 2, &effCreateTwoDimensionalTaskDataEntry2,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

effDebugMenuItem_t effCreateTwoDimensionalTaskDataEntry3[10] = {
    { effCreatePolyTrackTaskDataEntry14, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreateTwoDimensionalTaskDataEntry5 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry117, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreateTwoDimensionalTaskDataEntry6 }, { .minValue = 0 }, 0, 1.0f },
    { effCreatePolyWindTaskDataEntry116, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &effCreateTwoDimensionalTaskDataEntry7 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editRequest = effEditTwoDimensionalColorTrack }, { .release = effResetMappingFlags } },
    { effUpdatePartsListEditorData6Entry2, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditTwoDimensionalScaleTrack }, { .release = btlResetEffectWork } },
    { sysPOLYTEXDataEntry8, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .editObject = effEditTwoDimensionalRotationTrack }, { .release = btlResetEffectWork } },
    { sysStrMaterial, EFF_DEBUG_ITEM_CHILD, { 0 }, { .child = &effCreateTwoDimensionalTaskDataEntry8 }, { .minValue = 0 }, 0, 0.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t effCreateTwoDimensionalTaskDataEntry = {
    effCreateTwoDimensionalTaskData2, 0x8, 8, 0, 0, effCreateTwoDimensionalTaskDataEntry3, 10, &effCreateTwoDimensionalTaskDataEntry2,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest sysBATTLEONLYDataEntry45 = {
    sysBATTLEONLYDataEntry69, 20, { 0 }, 11, { 0 }, (u8 *)&sysBATTLEONLYDataEntry117, 0xC,
};

effDebugMenuItem_t sysBATTLEONLYDataEntry46[6] = {
    { sysBATTLEONLYDataEntry60, 0x2E, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry117 }, { .minValue = 0 }, 8, 1.0f },
    { sysBATTLEONLYDataEntry61, 0x2A, { 0 }, { .u16Value = &sysBATTLEONLYDataEntry118 }, { .minValue = 0 }, 2, 1.0f },
    { sysBATTLEONLYDataEntry71, 0x2F, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry119 }, { .minValue = 0 }, 1, 1.0f },
    { sysBATTLEONLYDataEntry70, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry120 }, { .minValue = 0 }, 0x80, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t sysBATTLEONLYDataEntry17 = {
    sysBATTLEONLYDataEntry69, 0x8, 8, 0, 0, sysBATTLEONLYDataEntry46, 6, &sysBATTLEONLYDataEntry45,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest sysBATTLEONLYDataEntry18 = {
    sysStrOLightS, 20, { 0 }, 12, { 0 }, (u8 *)&sysBATTLEONLYDataEntry121, 0x20,
};

effDebugMenuItem_t sysBATTLEONLYDataEntry47[25] = {
    { sysBATTLEONLYDataEntry52, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry121 }, { .minValue = 0 }, 0, 1.0f },
    { sysBATTLEONLYDataEntry58, 0x2E, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry126 }, { .minValue = 0 }, 8, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysPOLYTEXDataEntry9, EFF_DEBUG_ITEM_BLANK },
    { sysBATTLEONLYDataEntry50, 0x05, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry122 }, { .minValue = -1 }, 0xFFFFFF, 1.0f },
    { sysBATTLEONLYDataEntry49, 0x05, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry123 }, { .minValue = -1 }, 0xFFFFFF, 1.0f },
    { effInitializeResourceQueueData, EFF_DEBUG_ITEM_BLANK },
    { sysBATTLEONLYDataEntry57, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry124 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry124 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry124 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effInitializeResourceQueueData, EFF_DEBUG_ITEM_BLANK },
    { sysBATTLEONLYDataEntry56, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry125 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry125 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry125 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effInitializeResourceQueueData, EFF_DEBUG_ITEM_BLANK },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysBATTLEONLYDataEntry76, EFF_DEBUG_ITEM_BLANK },
    { sysBATTLEONLYDataEntry75, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysBATTLEONLYDataEntry127 }, { .minValue = 0 }, 1, 1.0f },
    { sysBATTLEONLYDataEntry74, EFF_DEBUG_ITEM_BYTE_FLAG, { 0 }, { .u8Value = &sysBATTLEONLYDataEntry128 }, { .minValue = 0 }, 1, 1.0f },
    { sysBATTLEONLYDataEntry73, 0x30, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry129 }, { .minValue = 0 }, 0x3E8, 1.0f },
    { sysBATTLEONLYDataEntry72, 0x30, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry130 }, { .minValue = 0 }, 0x3E8, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t sysBATTLEONLYDataEntry19 = {
    sysStrOLightS, 0x8, 8, 0, 0, sysBATTLEONLYDataEntry47, 25, &sysBATTLEONLYDataEntry18,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffFileJobRequest sysBATTLEONLYDataEntry20 = {
    sysStrOMcolrS, 20, { 0 }, 13, { 0 }, (u8 *)&sysBATTLEONLYDataEntry33, 0x14,
};

effDebugMenuItem_t sysBATTLEONLYDataEntry48[12] = {
    { sysBATTLEONLYDataEntry52, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry33 }, { .minValue = 0 }, 0, 1.0f },
    { sysBATTLEONLYDataEntry58, 0x2E, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry134 }, { .minValue = 0 }, 8, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { sysBATTLEONLYDataEntry50, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry131 }, { .minValue = 0 }, 0, 1.0f },
    { sysBATTLEONLYDataEntry49, EFF_DEBUG_ITEM_INT, { 0 }, { .s32Value = &sysBATTLEONLYDataEntry132 }, { .minValue = 0 }, 0, 1.0f },
    { effInitializeResourceQueueData, EFF_DEBUG_ITEM_BLANK },
    { effCreatePolyTwinkleTaskDataEntry87, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry133 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry86, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry133 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effCreatePolyTwinkleTaskDataEntry85, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &sysBATTLEONLYDataEntry133 }, { .minValue = 0 }, 0xFF, 1.0f },
    { effInitializeResourceQueueData, EFF_DEBUG_ITEM_BLANK },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { effStrSaveEp, EFF_DEBUG_ITEM_FUNCTION, { 0 }, { .command = effPollPrimaryFile }, { .release = effReleaseQueuedResourceName } },
};

effDebugMenu_t sysBATTLEONLYDataEntry21 = {
    sysStrOMcolrS, 0x8, 8, 0, 0, sysBATTLEONLYDataEntry48, 12, &sysBATTLEONLYDataEntry20,
    effCreateBattleCameraJob, effInvokeFileJobWithBattleCamera, effFileJobQueueRelease, &effData38,
    effCreateBattleCameraJob, NULL,
};

EffCameraCreateRow sysPARTICLEData[15] __attribute__((aligned(16))) = {
    { { &sysPARTICLEDataEntry, &sysPARTICLEDataEntry2, sysStrToolEffectSamplePSmoke0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &sysPARTICLEDataEntry, &sysPARTICLEDataEntry2, sysStrToolEffectSamplePSmoke1, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &sysPARTICLEDataEntry, &sysPARTICLEDataEntry2, sysStrToolEffectSamplePSmoke2, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &sysPARTICLEDataEntry93, &sysPARTICLEDataEntry3, sysStrToolEffectSamplePSpark0, { 0.0f, -150.0f, 0.0f } }, 0 },
    { { &sysPARTICLEDataEntry93, &sysPARTICLEDataEntry3, sysStrToolEffectSamplePSpark1, { 0.0f, -150.0f, 0.0f } }, 0 },
    { { &sysPARTICLEDataEntry93, &sysPARTICLEDataEntry3, sysStrToolEffectSamplePSpark2, { 0.0f, -150.0f, 0.0f } }, 0 },
    { { &sysPARTICLEDataEntry21, &sysPARTICLEDataEntry4, sysStrToolEffectSamplePSpirl0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &sysPARTICLEDataEntry21, &sysPARTICLEDataEntry4, sysStrToolEffectSamplePSpirl1, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &sysPARTICLEDataEntry5, &sysPARTICLEDataEntry6, sysStrToolEffectSamplePSphre0, { 0.0f, -150.0f, 0.0f } }, 0 },
    { { &sysPARTICLEDataEntry5, &sysPARTICLEDataEntry6, sysStrToolEffectSamplePSphre1, { 0.0f, -150.0f, 0.0f } }, 0 },
    { { &sysPARTICLEDataEntry24, &sysPARTICLEDataEntry7, sysStrToolEffectSamplePHspak0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &sysPARTICLEDataEntry24, &sysPARTICLEDataEntry7, sysStrToolEffectSamplePHspak1, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &sysPARTICLEDataEntry8, &sysPARTICLEDataEntry9, sysStrToolEffectSamplePAspak0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &sysPARTICLEDataEntry8, &sysPARTICLEDataEntry9, sysStrToolEffectSamplePAspak1, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { NULL } },
};

EffCameraCreateRow effCreatePolyTrackControlTaskData[6] __attribute__((aligned(16))) = {
    { { &effCreatePolyTrackControlTaskDataEntry13, &effCreatePolyTrackControlTaskDataEntry, effStrToolEffectSampleGSmoke0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &effCreatePolyTrackControlTaskDataEntry2, &effCreatePolyTrackControlTaskDataEntry3, effStrToolEffectSampleGSpark0, { 0.0f, -150.0f, 0.0f } }, 0 },
    { { &effCreatePolyTrackControlTaskDataEntry68, &effCreatePolyTrackControlTaskDataEntry4, effStrToolEffectSampleGSpirl0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &effCreatePolyTrackControlTaskDataEntry17, &effCreatePolyTrackControlTaskDataEntry5, effStrToolEffectSampleGSphre0, { 0.0f, -150.0f, 0.0f } }, 0 },
    { { &effCreatePolyTrackControlTaskDataEntry6, &effCreatePolyTrackControlTaskDataEntry7, effStrToolEffectSampleGHspak0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { NULL } },
};

EffCameraCreateRow effCreatePolyTrackTaskData[6] __attribute__((aligned(16))) = {
    { { &effCreatePolyTrackTaskDataEntry66, &effCreatePolyTrackTaskDataEntry, effStrToolEffectSampleKSmoke0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &effCreatePolyTrackTaskDataEntry71, &effCreatePolyTrackTaskDataEntry2, effStrToolEffectSampleKSpark0, { 0.0f, -150.0f, 0.0f } }, 0 },
    { { &effCreatePolyTrackTaskDataEntry75, &effCreatePolyTrackTaskDataEntry3, effStrToolEffectSampleKSpirl0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &effCreatePolyTrackTaskDataEntry80, &effCreatePolyTrackTaskDataEntry4, effStrToolEffectSampleKSphre0, { 0.0f, -150.0f, 0.0f } }, 0 },
    { { &effCreatePolyTrackTaskDataEntry12, &effCreatePolyTrackTaskDataEntry5, effStrToolEffectSampleKHspak0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { NULL } },
};

EffCameraCreateRow effData9Entry[2] __attribute__((aligned(16))) = {
    { { &fileLoadEffectSlotAData, &effData9Entry7, NULL, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { NULL } },
};

EffCameraCreateRow effCreateBlurTaskData[6] __attribute__((aligned(16))) = {
    { { &effCreateBlurTaskDataEntry10, &effCreateBlurTaskDataEntry, effStrToolEffectSampleBFull0Ep, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &effCreateBlurTaskDataEntry45, &effCreateBlurTaskDataEntry2, effStrToolEffectSampleBMulti0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &effCreateBlurTaskDataEntry3, &effCreateBlurTaskDataEntry4, effStrToolEffectSampleBDist0Ep, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &effCreateBlurTaskDataEntry14, &effCreateBlurTaskDataEntry5, effStrToolEffectSampleBRiple0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &effCreateBlurTaskDataEntry16, &effCreateBlurTaskDataEntry6, effStrToolEffectSampleBParal0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { NULL } },
};

EffCameraCreateRow effCreateFilterTaskData[3] __attribute__((aligned(16))) = {
    { { &effCreateFilterTaskDataEntry5, &effCreateFilterTaskDataEntry, effStrToolEffectSampleFFull0Ep, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &effCreateFilterTaskDataEntry7, &effCreateFilterTaskDataEntry2, effStrToolEffectSampleFMulti0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { NULL } },
};

EffCameraCreateRow effData9Entry2[2] __attribute__((aligned(16))) = {
    { { &effData9Entry15, &effData9Entry9, NULL, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { NULL } },
};

EffCameraCreateRow sysPOLYTEXData[3] __attribute__((aligned(16))) = {
    { { &sysPOLYTEXDataEntry4, &sysPOLYTEXDataEntry, sysStrToolEffectSampleTBoard0, { 0.0f, -10.0f, 0.0f } }, 0 },
    { { &sysPOLYTEXDataEntry6, &sysPOLYTEXDataEntry2, sysStrToolEffectSampleTBoard1, { 0.0f, -10.0f, 0.0f } }, 0 },
    { { NULL } },
};

EffCameraCreateRow effCreateEnvironmentTaskData[5] __attribute__((aligned(16))) = {
    { { &effCreateEnvironmentTaskDataEntry10, &effCreateEnvironmentTaskDataEntry, effStrToolEffectSampleERain0Ep, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &effCreateEnvironmentTaskDataEntry2, &effCreateEnvironmentTaskDataEntry3, effStrToolEffectSampleEFlare0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &effCreateEnvironmentTaskDataEntry13, &effCreateEnvironmentTaskDataEntry4, effStrToolEffectSampleEQuake0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &effCreateEnvironmentTaskDataEntry15, &effCreateEnvironmentTaskDataEntry5, effStrToolEffectSampleEQuake20, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { NULL } },
};

EffCameraCreateRow effCreatePolyFlashTaskData[11] __attribute__((aligned(16))) = {
    { { &effCreatePolyFlashTaskDataEntry132, &effCreatePolyFlashTaskDataEntry4, effStrToolEffectSampleSRadT0Ep, { 0.0f, -200.0f, 0.0f } }, 0 },
    { { &effCreatePolyFlashTaskDataEntry135, &effCreatePolyFlashTaskDataEntry5, effStrToolEffectSampleSSpkT0Ep, { 0.0f, -200.0f, 0.0f } }, 0 },
    { { &effCreatePolyFlashTaskDataEntry135, &effCreatePolyFlashTaskDataEntry5, effStrToolEffectSampleSSpkT1Ep, { 0.0f, -200.0f, 0.0f } }, 0 },
    { { &effCreatePolyFlashTaskDataEntry147, &effCreatePolyFlashTaskDataEntry8, effStrToolEffectSampleSSctT0Ep, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &effCreatePolyFlashTaskDataEntry18, &effCreatePolyFlashTaskDataEntry6, effStrToolEffectSampleSRngT0Ep, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &effCreatePolyFlashTaskDataEntry12, &effCreatePolyFlashTaskDataEntry, effStrToolEffectSampleSRadi1Ep, { 0.0f, -200.0f, 0.0f } }, 0 },
    { { &effCreatePolyFlashTaskDataEntry125, &effCreatePolyFlashTaskDataEntry2, effStrToolEffectSampleSSpark0, { 0.0f, -200.0f, 0.0f } }, 0 },
    { { &effCreatePolyFlashTaskDataEntry125, &effCreatePolyFlashTaskDataEntry2, effStrToolEffectSampleSSpark1, { 0.0f, -200.0f, 0.0f } }, 0 },
    { { &effCreatePolyFlashTaskDataEntry143, &effCreatePolyFlashTaskDataEntry7, effStrToolEffectSampleSRing0Ep, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &effCreatePolyFlashTaskDataEntry128, &effCreatePolyFlashTaskDataEntry3, effStrToolEffectSampleSCylnd0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { NULL } },
};

EffCameraCreateRow effCreatePolyRingTaskData[6] __attribute__((aligned(16))) = {
    { { &effCreatePolyRingTaskDataEntry28, &effCreatePolyRingTaskDataEntry, effStrToolEffectSampleRNomal0, { 0.0f, -200.0f, 0.0f } }, 0 },
    { { &effCreatePolyRingTaskDataEntry28, &effCreatePolyRingTaskDataEntry, effStrToolEffectSampleRNomal1, { 0.0f, -200.0f, 0.0f } }, 0 },
    { { &effCreatePolyRingTaskDataEntry6, &effCreatePolyRingTaskDataEntry2, effStrToolEffectSampleRTrack0, { 0.0f, -200.0f, 0.0f } }, 0 },
    { { &effCreatePolyRingTaskDataEntry31, &effCreatePolyRingTaskDataEntry3, effStrToolEffectSampleRCrcle0, { 0.0f, -200.0f, 0.0f } }, 0 },
    { { &effCreatePolyRingTaskDataEntry31, &effCreatePolyRingTaskDataEntry3, effStrToolEffectSampleRCrcle1, { 0.0f, -200.0f, 0.0f } }, 0 },
    { { NULL } },
};

EffCameraCreateRow effCreatePolyThunderTaskData[4] __attribute__((aligned(16))) = {
    { { &effCreatePolyThunderTaskDataEntry, &effCreatePolyThunderTaskDataEntry2, effStrToolEffectSampleDPoint0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &effCreatePolyThunderTaskDataEntry24, &effCreatePolyThunderTaskDataEntry3, effStrToolEffectSampleDBug0Ep, { 0.0f, -200.0f, 0.0f } }, 0 },
    { { &effCreatePolyThunderTaskDataEntry24, &effCreatePolyThunderTaskDataEntry3, effStrToolEffectSampleDBug1Ep, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { NULL } },
};

EffCameraCreateRow effCreatePolyTwinkleTaskData[6] __attribute__((aligned(16))) = {
    { { &effCreatePolyTwinkleTaskDataEntry62, &effCreatePolyTwinkleTaskDataEntry, effStrToolEffectSampleWSmoke0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &effCreatePolyTwinkleTaskDataEntry67, &effCreatePolyTwinkleTaskDataEntry2, effStrToolEffectSampleWSpark0, { 0.0f, -150.0f, 0.0f } }, 0 },
    { { &effCreatePolyTwinkleTaskDataEntry71, &effCreatePolyTwinkleTaskDataEntry3, effStrToolEffectSampleWSpirl0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &effCreatePolyTwinkleTaskDataEntry76, &effCreatePolyTwinkleTaskDataEntry4, effStrToolEffectSampleWSphre0, { 0.0f, -150.0f, 0.0f } }, 0 },
    { { &effCreatePolyTwinkleTaskDataEntry5, &effCreatePolyTwinkleTaskDataEntry6, effStrToolEffectSampleWHspak0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { NULL } },
};

EffCameraCreateRow sysPOLYWINDData[8] __attribute__((aligned(16))) = {
    { { &sysPOLYWINDDataEntry, &sysPOLYWINDDataEntry2, sysStrToolEffectSampleNSpirl0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &sysPOLYWINDDataEntry, &sysPOLYWINDDataEntry2, sysStrToolEffectSampleNSpirl1, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &sysPOLYWINDDataEntry, &sysPOLYWINDDataEntry2, sysStrToolEffectSampleNSpirl2, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &sysPOLYWINDDataEntry77, &sysPOLYWINDDataEntry4, sysStrToolEffectSampleNSphre0, { 0.0f, -200.0f, 0.0f } }, 0 },
    { { &sysPOLYWINDDataEntry71, &sysPOLYWINDDataEntry3, sysStrToolEffectSampleNCylnd0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &sysPOLYWINDDataEntry71, &sysPOLYWINDDataEntry3, sysStrToolEffectSampleNCylnd1, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &sysPOLYWINDDataEntry81, &sysPOLYWINDDataEntry5, sysStrToolEffectSampleNCyldr0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { NULL } },
};

EffCameraCreateRow effCreatePolyScalyTaskData[5] __attribute__((aligned(16))) = {
    { { &effCreatePolyScalyTaskDataEntry44, &effCreatePolyScalyTaskDataEntry, effStrToolEffectSampleUCylnd0, { 0.0f, -100.0f, 0.0f } }, 0 },
    { { &effCreatePolyScalyTaskDataEntry56, &effCreatePolyScalyTaskDataEntry4, effStrToolEffectSampleUSphre0, { 0.0f, -160.0f, 0.0f } }, 0 },
    { { &effCreatePolyScalyTaskDataEntry48, &effCreatePolyScalyTaskDataEntry2, effStrToolEffectSampleUSmoke0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &effCreatePolyScalyTaskDataEntry52, &effCreatePolyScalyTaskDataEntry3, effStrToolEffectSampleUSpirl0, { 0.0f, -100.0f, 0.0f } }, 0 },
    { { NULL } },
};

EffCameraCreateRow effData9Entry3[2] __attribute__((aligned(16))) = {
    { { &fileLoadEffectSlotBData, &effData9Entry10, NULL, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { NULL } },
};

EffCameraCreateRow effCreatePolyCrackTaskData[2] __attribute__((aligned(16))) = {
    { { &effCreatePolyCrackTaskDataEntry10, &effCreatePolyCrackTaskDataEntry, effStrToolEffectSampleCNomal0, { 0.0f, -1.0f, 0.0f } }, 0 },
    { { NULL } },
};

EffCameraCreateRow sysBATTLEONLYData[13] __attribute__((aligned(16))) = {
    { { &sysBATTLEONLYDataEntry, &sysBATTLEONLYDataEntry2, sysStrToolEffectSampleOLight0, { 0.0f, -1.0f, 0.0f } }, 0 },
    { { &sysBATTLEONLYDataEntry3, &sysBATTLEONLYDataEntry4, sysStrToolEffectSampleOMcolr0, { 0.0f, -1.0f, 0.0f } }, 0 },
    { { &sysBATTLEONLYDataEntry5, &sysBATTLEONLYDataEntry6, sysStrToolEffectSampleOEffL0Ep, { 0.0f, -1.0f, 0.0f } }, 0 },
    { { &sysBATTLEONLYDataEntry37, &sysBATTLEONLYDataEntry7, sysStrToolEffectSampleOMot0Ep, { 0.0f, -1.0f, 0.0f } }, 0 },
    { { &sysBATTLEONLYDataEntry8, &sysBATTLEONLYDataEntry9, sysStrToolEffectSampleOSe0Ep, { 0.0f, -1.0f, 0.0f } }, 0 },
    { { &sysBATTLEONLYDataEntry10, &sysBATTLEONLYDataEntry11, sysStrToolEffectSampleOFldL0Ep, { 0.0f, -1.0f, 0.0f } }, 0 },
    { { &sysBATTLEONLYDataEntry42, &sysBATTLEONLYDataEntry14, sysStrToolEffectSampleOFldF0Ep, { 0.0f, -1.0f, 0.0f } }, 0 },
    { { &sysBATTLEONLYDataEntry12, &sysBATTLEONLYDataEntry13, sysStrToolEffectSampleOVibe0Ep, { 0.0f, -1.0f, 0.0f } }, 0 },
    { { &sysBATTLEONLYDataEntry15, &sysBATTLEONLYDataEntry16, sysStrToolEffectSampleOFovy0Ep, { 0.0f, -1.0f, 0.0f } }, 0 },
    { { &sysBATTLEONLYDataEntry45, &sysBATTLEONLYDataEntry17, sysStrToolEffectSampleOUnitd0, { 0.0f, -1.0f, 0.0f } }, 0 },
    { { &sysBATTLEONLYDataEntry18, &sysBATTLEONLYDataEntry19, sysStrToolEffectSampleOLight2, { 0.0f, -1.0f, 0.0f } }, 0 },
    { { &sysBATTLEONLYDataEntry20, &sysBATTLEONLYDataEntry21, sysStrToolEffectSampleOMcolr2, { 0.0f, -1.0f, 0.0f } }, 0 },
    { { NULL } },
};

EffCameraCreateRow effData9Entry4[2] __attribute__((aligned(16))) = {
    { { &effPollAndQueueCopiedFileResourceData, &effData9Entry8, NULL, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { NULL } },
};

EffCameraCreateRow effData9Entry5[2] __attribute__((aligned(16))) = {
    { { &effLoadFileSlotF2Data, &effData9Entry11, NULL, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { NULL } },
};

EffCameraCreateRow effData9Entry6[2] __attribute__((aligned(16))) = {
    { { &effLoadMaterialFileData, &effData9Entry12, NULL, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { NULL } },
};

EffCameraCreateRow effCreateTwoDimensionalTaskData[2] __attribute__((aligned(16))) = {
    { { &effCreateTwoDimensionalTaskDataEntry2, &effCreateTwoDimensionalTaskDataEntry, effStrToolEffectSampleXNomal0, { 0.0f, -10.0f, 0.0f } }, 0 },
    { { NULL } },
};

EffCameraCreateRow effCreatePolyWindTaskData[7] __attribute__((aligned(16))) = {
    { { &effCreatePolyWindTaskDataEntry83, &effCreatePolyWindTaskDataEntry, effStrToolEffectSampleMSmoke0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &effCreatePolyWindTaskDataEntry88, &effCreatePolyWindTaskDataEntry2, effStrToolEffectSampleMSpark0, { 0.0f, -150.0f, 0.0f } }, 0 },
    { { &effCreatePolyWindTaskDataEntry14, &effCreatePolyWindTaskDataEntry3, effStrToolEffectSampleMSpirl0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &effCreatePolyWindTaskDataEntry96, &effCreatePolyWindTaskDataEntry4, effStrToolEffectSampleMSphre0, { 0.0f, -150.0f, 0.0f } }, 0 },
    { { &effCreatePolyWindTaskDataEntry100, &effCreatePolyWindTaskDataEntry5, effStrToolEffectSampleMHspak0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { &effCreatePolyWindTaskDataEntry18, &effCreatePolyWindTaskDataEntry6, effStrToolEffectSampleMAspak0, { 0.0f, 0.0f, 0.0f } }, 0 },
    { { NULL } },
};

EffCameraCreateRow *effData9[24] = {
    NULL, sysPARTICLEData, effCreatePolyTrackControlTaskData, effCreatePolyTrackTaskData,
    effData9Entry, effCreateBlurTaskData, effCreateFilterTaskData, effData9Entry2,
    sysPOLYTEXData, effCreateEnvironmentTaskData, effCreatePolyFlashTaskData, effCreatePolyRingTaskData,
    effCreatePolyThunderTaskData, effCreatePolyTwinkleTaskData, sysPOLYWINDData, effCreatePolyScalyTaskData,
    effData9Entry3, effCreatePolyCrackTaskData, sysBATTLEONLYData, effData9Entry5,
    effData9Entry6, effCreateTwoDimensionalTaskData, effCreatePolyWindTaskData, effData9Entry4,
};

#endif
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

