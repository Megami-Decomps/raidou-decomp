#ifndef CODE_R1_0021_CE60_H
#define CODE_R1_0021_CE60_H

#include "common.h"
#include "sdf_resource.h"
#include "ps2asm.h"
#include "model.h"
#include "code_r1_002250F8.h"

/* Result values shared by the resource browser and name-entry selectors. */
enum BtlResourceSelectionStatus {
    BTL_RESOURCE_SELECTION_PENDING = 0,
    BTL_RESOURCE_SELECTION_ACCEPTED = 1,
    BTL_RESOURCE_SELECTION_CANCELED = 2,
};

/* Selects the operation and callback rows used by a file job. */
typedef u16 FileJobType;

/* File-job buffer offsets are relative to the job unless allocation is retained. */
typedef struct FileJobBufferSlot {
    u32 offset;
    u32 size;
    void *allocation;
    u16 selector;
    u16 unkE;
} FileJobBufferSlot;

/* Runtime payload buffers own a general-heap descriptor; queue image slots
 * keep their separate opaque allocation word in FileJobBufferSlot. */
typedef struct FileJobPayloadBufferSlot {
    u32 offset;
    u32 size;
    struct sdfMemHandle_t *allocation;
    u16 selector;
    u16 unkE;
} FileJobPayloadBufferSlot;

/* Separately allocated payload and its serialized 0x2C header. Queue entries
 * refer to this object through their address-valued id field. */
typedef struct FileJobPayload {
    u32 unk0;
    FileJobType type;
    u16 unk6;
    void *data;
    u16 option;             /* selects primary payload interpretation */
    u16 unkE;
    FileJobPayloadBufferSlot primary; /* selector at 0x1C describes the secondary payload */
    struct {
        u32 offset;
        u32 size;
        struct sdfMemHandle_t *allocation;
    } secondary;            /* 0x20; no selector follows this descriptor */
} FileJobPayload;

typedef char FileJobPayload_size_must_be_0x2C[
    (sizeof(FileJobPayload) == 0x2C) ? 1 : -1];

#define FILE_JOB_FLAG_SECTOR_FOLLOWER 0x2

/* Effect/file queue entries share this C0-byte record in both games. */
typedef struct FileJob {
    u32 unk0;
    FileJobType type;
    u16 unk6;
    void *data;
    u16 option;
    u16 unkE;
    FileJobBufferSlot slots[2];
    u8 unk30[0x10];
    f32 offset[4];    /* 0x40 */
    f32 quat[4];      /* 0x50 */
    f32 scale;        /* 0x60 */
    u32 color;        /* 0x64 */
    u32 xformFlags;   /* 0x68 */
    u8 unk6C[0x14];
    s32 startFrame;   /* 0x80: earliest queue frame eligible for updates */
    u32 updateFlags;  /* 0x84 */
    u8 unk88[8];
    u32 id;
    u32 sector;
    u32 flags;
    char name[0x10];  /* 0x9C: queue-entry resource name */
    struct FileJob *next;
    struct FileJob *prev;
    u8 padB4[0xC];
} FileJob;

typedef char FileJobBufferSlot_size_must_be_0x10[
    (sizeof(FileJobBufferSlot) == 0x10) ? 1 : -1];

typedef char FileJobPayloadBufferSlot_size_must_be_0x10[
    (sizeof(FileJobPayloadBufferSlot) == 0x10) ? 1 : -1];

typedef char FileJob_size_must_be_0xC0[
    (sizeof(FileJob) == 0xC0) ? 1 : -1];

/* Placement of an effect parts list: queues, camera snapshots, and jobs from 0x40 on. */
typedef struct effPartsListPose_t {
    f32 posBase[4];
    f32 origin[4];
    u8 unk20[0x20];
    f32 pos[4];       /* 0x40 */
    f32 rot[4];       /* 0x50 */
    f32 scl;          /* 0x60 */
    u32 abgr;         /* 0x64: modulation colour */
    u32 flag;         /* 0x68: 0x18 billboard position, 0x60 billboard rotation */
    u32 unk6C;
} effPartsListPose_t;

typedef char effPartsListPose_size_must_be_0x70[
    (sizeof(effPartsListPose_t) == 0x70) ? 1 : -1];

/* Runtime queue owner; serialized images retain the same 0x90-byte header. */
typedef struct FileQueue {
    effPartsListPose_t pose;
    u8 pad70[4];
    f32 scaleMultiplier; /* 0x74: multiplies queue scale during updates */
    u8 pad78[8];
    s32 count;        /* 0x80 */
    u32 updateFrame;  /* 0x84 */
    union {
        FileJob *last;   /* runtime linked-list tail */
        u32 entryOffset; /* relative first-entry offset in serialized images */
    };               /* 0x88 */
    FileJob *first;   /* 0x8C: runtime traversal start */
} FileQueue;

typedef char FileQueue_size_must_be_0x90[
    (sizeof(FileQueue) == 0x90) ? 1 : -1];

void camFollowOffsetVec(effPartsListPose_t *obj, void *dst);
void PoseRotBillboard(effPartsListPose_t *obj, void *dst);
void fileQueueLinkJobToSectorLeader(FileQueue *queue, FileJob *job, FileJob *leader);

typedef struct FileSlot {
    f32 pos[4];       /* 0x00 */
    s32 state;        /* 0x10: signed animation frame or lifecycle sentinel */
    u32 color;        /* 0x14 */
    f32 scale;        /* 0x18 */
    f32 angle;        /* 0x1C */
} FileSlot;

typedef struct FileSlotTable {
    u16 type;
    u8 pad02[2];
    u32 instances;         /* 0x04: target's primary-slot count */
    u32 count;             /* 0x08: primary + trailing group cells */
    u32 flags;             /* 0x0C */
    u32 references;        /* 0x10: acquisition/frame counter */
    f32 spawnRemainder;    /* 0x14 */
    FileSlot *slots; /* 0x18 */
    u8 *instanceWork;             /* 0x1C: per-type instance work */
    u8 *data0;             /* 0x20 */
    u8 *data1;             /* 0x24 */
    struct sdfMemHandle_t *allocation; /* 0x28: owns the retained table storage */
} FileSlotTable;

/* The existing root-pointer conventions are 4-byte aligned, not VU types. */
typedef char FileSlotSizeCheck[(sizeof(FileSlot) == 0x20) ? 1 : -1];

typedef char FileSlotTableSizeCheck[(sizeof(FileSlotTable) == 0x2C) ? 1 : -1];

/* The slots start after the table, rounded up to 16 bytes for the VU0 quad loads. */
#define FILE_SLOT_TABLE_HEADER_BYTES 0x30

/* A file job type's operations (0x28), fileJobTypeOperations[type]. Each
 * receives the job's data; a NULL slot is skipped where the caller checks. */
typedef struct FileTypeCallbacks {
    void *(*create)(void *request);                     /* 0x00 */
    void (*invoke)(void *data);                         /* 0x04 */
    void (*destroy)(void *data);                        /* 0x08 */
    void *(*createChild)(void *data, u16 type);         /* 0x0C */
    void (*notifyPair)(void *left, void *right);        /* 0x10 */
    void (*reset)(void *data);                          /* 0x14 */
    void (*setPosition)(void *data, const f32 position[4]); /* 0x18 */
    void (*setRotation)(void *data, const f32 rotation[4]); /* 0x1C */
    void (*setScale)(void *data, f32 scale);            /* 0x20 */
    void (*setColor)(void *data, u32 color);            /* 0x24 */
} FileTypeCallbacks;

typedef char FileTypeCallbacksSizeAssert[sizeof(FileTypeCallbacks) == 0x28 ? 1 : -1];

extern FileTypeCallbacks fileJobTypeOperations[];

FileJobPayload *fileCreateJob(FileJobType type);

/* Protocol IDs use distinct byte-sized storage codes in sdfFileCmdHandle_t. */
typedef u8 SdfDevOperationCode;

typedef s8 SdfDevStateCode;

/* Complete 0x40-byte owner allocated by sdfFileCmdNewHandle. */
typedef struct sdfFileCmdHandle_t {
    struct sdfFileCmdHandle_t *next; /* 0x00 */
    struct sdfFileCmdHandle_t *previous; /* 0x04 */
    struct sdfFileCmdHandle_t *workerNext; /* 0x08 */
    struct sdfFileCmdHandle_t *workerPrev; /* 0x0C */
    char *resolvedPath; /* 0x10 */
    u8 workerIndex; /* 0x14 */
    SdfDevOperationCode operation; /* 0x15 */
    SdfDevStateCode state; /* 0x16 */
    u8 pad17; /* 0x17 */
    s32 seekOffset; /* 0x18 */
    s32 requestExtra; /* 0x1C */
    void *requestData; /* 0x20 */
    s32 options; /* 0x24 */
    s32 fileHandle; /* 0x28: SCE descriptor, or -1 when no file is open */
    s32 result; /* 0x2C */
    s32 transferred; /* 0x30 */
    u8 pad34[4]; /* 0x34 */
    void (*callback)(struct sdfFileCmdHandle_t *, s32, s32, s32, s32); /* 0x38 */
    void *callbackContext; /* 0x3C: caller-owned callback context */
} sdfFileCmdHandle_t;

typedef char DevState_size_must_be_0x40[(sizeof(sdfFileCmdHandle_t) == 0x40) ? 1 : -1];

typedef char DevState_operation_size_must_be_1[
    (sizeof(((sdfFileCmdHandle_t *)0)->operation) == 1) ? 1 : -1];

typedef char DevState_state_size_must_be_1[
    (sizeof(((sdfFileCmdHandle_t *)0)->state) == 1) ? 1 : -1];

/* The flag-list factory appends this owner after its vertex, color and mark
 * arrays. Its parameter record is copied whole into the owner. */
typedef struct SdfColorTrack {
    u8 segmentMode; /* enum SdfColorTrackMode, stored as a byte */
    u8 pad01[3];
    u32 initialColor;
    u32 finalColor;
    /* The editor selects either intermediate key with an eight-byte stride. */
    struct {
        u32 color;
        f32 fraction;
    } keys[2];
    u8 pad1C[8];
} SdfColorTrack;

typedef struct SdfAlphaTrack {
    u32 alpha;
    s32 surfaceIndex;
    f32 fadeInFraction;
    f32 fadeOutFraction;
} SdfAlphaTrack;

typedef char SdfColorTrack_key_stride_must_be_8[(sizeof(((SdfColorTrack *)0)->keys[0]) == 8) ? 1 : -1];

typedef char SdfColorTrack_size_must_be_0x24[(sizeof(SdfColorTrack) == 0x24) ? 1 : -1];

typedef char SdfAlphaTrack_size_must_be_0x10[(sizeof(SdfAlphaTrack) == 0x10) ? 1 : -1];

void fileJobDestroy(FileJobPayload *job);

FileQueue *fileQueueCreate(void);

FileJob *fileQueueGetAt(FileQueue *queue, s32 index);

void fileJobCopyHeader(FileJob *destination, const FileJob *source);

void fileQueueRemoveAndDestroyJob(FileQueue *queue, FileJob *job);

/* Piecewise-linear scalar keys. Frames and duration are signed. */
typedef struct EffScalarCurve {
    u8 mode; /* enum EffScalarCurveMode, stored as a byte */
    u8 reserved01[3];
    f32 initialValue;
    f32 finalValue;
    u8 reserved0C[4];
    u8 headingMode; /* Used by heading tracks: 2 selects projected direction. */
    u8 reserved11[3];
    struct {
        f32 value;
        f32 fraction;
    } keys[2];
} EffScalarCurve; /* 0x24 */

/* Scalar tracks in effect configurations reserve eight trailing bytes. */
typedef struct EffScalarTrack {
    EffScalarCurve curve;
    u8 reserved24[8];
} EffScalarTrack; /* 0x2C */

#define FILE_SLOT_FRAME(slot) ((slot)->state)

FileSlotTable *fileAllocateGridRecordSlots(u16 type, u32 count, const void *configuration);

void fileClearRecordReferences(FileSlotTable *record);

/* The track's +0x0C word is a real emitter random multiplier; the ordinary
 * curve sampler treats it as reserved. Both are members of the serialized
 * parameter format, not a second runtime-record view. */
typedef union FileKeyScalarTrack {
    EffScalarTrack track;
    struct {
        u8 mode;
        u8 reserved01[3];
        f32 initialValue;
        f32 finalValue;
        f32 randomness;
        u8 headingMode;
        u8 reserved11[3];
        f32 firstValue;
        f32 firstFraction;
        f32 secondValue;
        f32 secondFraction;
        u8 reserved24[8];
    } emitter;
} FileKeyScalarTrack;

/* FileSlotTable.type selects the serialized emitter tail. Its
 * descriptors copy E0..124 bytes; FileKeyBlock is their existing primary
 * parameter owner. Its +54 surface index is also consumed as signed low 16
 * bits by billboard openers. Grid-group dimensions occupy C0/C4. */
typedef struct FileKeyBlock {
    f32 pos[4];             /* 0x00 */
    f32 orientation[4];    /* 0x10: emitter quaternion */
    s32 emissionDuration;   /* 0x20 */
    u32 spawnRate;          /* 0x24: emitter samplers convert this count unsigned */
    f32 spawnVariance;      /* 0x28 */
    SdfColorTrack colorTrack; /* 0x2C */
    SdfAlphaTrack alphaTrack; /* 0x50: surfaceIndex is read word-wide or narrowed to s16 */
    FileKeyScalarTrack scale;   /* 0x60 */
    FileKeyScalarTrack heading; /* 0x8C */
    s32 length;            /* 0xB8 */
    u8 padBC;              /* 0xBC: allocator's relative-position flag */
    u8 prewarm;            /* 0xBD */
    u8 padBE[2];
    s32 columns;            /* 0xC0: trailing-group columns */
    s32 rows;               /* 0xC4: trailing-group rows */
    union {                /* 0xC8: record-type-specific emitter parameters */
        struct {
            f32 radius;
            f32 radiusRandomness;
            f32 speed;
            f32 speedRandomness;
            f32 acceleration;
            f32 gravity;
        } radial;
        struct {
            f32 radius;
            f32 radiusRandomness;
            f32 spread;
            f32 spreadRandomness;
            f32 speed;
            f32 speedRandomness;
            f32 acceleration;
            f32 gravity;
        } directed;
        struct {
            f32 radius;
            f32 radiusRandomness;
            f32 spread;
            f32 spreadRandomness;
            f32 speed;
            f32 speedRandomness;
            f32 acceleration;
            f32 gravity;
            s32 azimuthDegrees; /* 0xE8: signed angular extent */
        } sector;
        struct {
            f32 initialRadius;
            f32 axialSpeed;
            f32 axialSpeedRandomness;
            f32 axialDeceleration;
            f32 initialAmplitude;
            f32 initialAmplitudeRandomness;
            f32 finalAmplitude;
            f32 finalAmplitudeRandomness;
            f32 phaseStep;
            f32 phaseStepRandomness;
        } wave;
        struct {
            f32 initialAxialExtent;
            f32 initialRadius;
            f32 initialRadiusRandomness;
            f32 finalRadius;
            f32 finalRadiusRandomness;
            f32 angularSpeed;
            f32 angularSpeedRandomness;
            f32 axialSpeed;
            f32 axialSpeedRandomness;
            f32 angularAcceleration;
            f32 gravity;
        } circular;
        struct {
            f32 initialRadius;
            f32 initialRadiusRandomness;
            f32 finalRadius;
            f32 finalRadiusRandomness;
            f32 angularSpeed;
            f32 angularSpeedRandomness;
            f32 angularAcceleration;
            f32 gravity;
        } orientedRing;
        struct {
            s16 firstPercent;       /* C8 */
            u8 padCA[2];
            f32 firstOffset;
            f32 firstRandomness;
            s16 secondPercent;      /* D4 */
            u8 padD6[2];
            f32 secondOffset;
            f32 secondRandomness;
            s16 spreadDegrees;      /* E0 */
            u8 padE2[2];
            f32 angularSpeed;
            f32 angularRandomness;
            f32 angularAcceleration;
            f32 travelSpeed;
            f32 travelRandomness;
            f32 travelAcceleration;
            f32 start[3];           /* FC */
            f32 end[3];             /* 108 */
            u8 reserved114[8];
            f32 endpointRadius;     /* 11C */
            f32 endpointDrop;
        } curve;
    } emitter;
} FileKeyBlock;

typedef char FileKeyScalarTrackSizeCheck[(sizeof(FileKeyScalarTrack) == 0x2C) ? 1 : -1];

/* Surface parameters copied into the node: 0x1C bytes at +0x10. */
typedef struct EffSurfaceParams {
    u32 opaque00[6];
    f32 scale;
} EffSurfaceParams;

/*
 * SDK word carriers used by effColorAnimation and the bit-scaled
 * billboard blends. Each conversion owns its native 16-byte workspace.
 * Other unpack/pack operand contracts keep their existing macros.
 * See docs/idioms.md, "SDK RGBA word carriers".
 */
static inline void eeUnpackRGBAWord(u32 color, u32 unitBits) {
    u32 rgba[4];
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK_READONLY(rgba, unitBits);
}

static inline u32 eePackRGBAWord(f32 unit) {
    u32 rgba[4];
    EE_MMI_RGBA_PACK_UNIT(rgba[0], unit);
    return rgba[0];
}

struct FileQueue *Create_effPartsList(struct FileQueue *source);

struct FileQueue *effPartsCreateFinish(const struct FileQueue *source);

void effPartsDelete(struct FileQueue *queue);

typedef struct SdfFlagListParams {
    SdfColorTrack color;
    SdfAlphaTrack alpha;
    s32 maxFrames;
    u32 count;
    f32 speed;
} SdfFlagListParams;

typedef struct SdfFlagListMark {
    s32 timer;
    u8 alpha;
    u8 pad05[3];
} SdfFlagListMark;

typedef struct SdfFlagListWork {
    s32 frame;
    u32 unk04;
    SdfFlagListMark *marks;
    f32 (*vertices)[4];
    u32 *colors;
    SdfFlagListParams params;
    sdfMemHandle_t *allocation;
} SdfFlagListWork;

typedef char SdfFlagListParams_size_must_be_0x40[(sizeof(SdfFlagListParams) == 0x40) ? 1 : -1];

typedef char SdfFlagListMark_size_must_be_0x08[(sizeof(SdfFlagListMark) == 0x08) ? 1 : -1];

typedef char SdfFlagListWork_size_must_be_0x58[(sizeof(SdfFlagListWork) == 0x58) ? 1 : -1];

SdfFlagListWork *sdfCreateFlagListWork(const SdfFlagListParams *);

void sdfReleaseFlagListResource(SdfFlagListWork *);

SdfFlagListWork *sdfInitializeFlagListFromResource(void *);

void sdfUpdateFlagListEntries(SdfFlagListWork *);

void sdfDrawFlagListVertices(SdfFlagListWork *);

FileJobPayload *Copy_effParts(FileJobPayload *request);

f32 effSampleScalarCurve(const EffScalarCurve *curve, s32 frame, s32 duration);

u32 effColorAnimation(const struct SdfColorTrack *colorTrack,
                              const struct SdfAlphaTrack *alphaTrack,
                              s32 frame, s32 duration);

typedef struct EffFileJobRequest {
    char *name;                       /* 0x00 */
    u16 fileKind;                     /* 0x04 */
    u8 pad06[2];
    u16 transferMode;                /* 0x08 */
    u8 pad0A[2];
    u8 *outputBuffer;                 /* 0x0C */
    u32 size;                         /* 0x10 */
    struct sdfMemHandle_t *allocation;   /* 0x14 */
    u16 resourceMode;                 /* 0x18 */
    u8 pad1A[2];
    const char *secondaryCommandPath; /* 0x1C */
} EffFileJobRequest;

typedef char EffFileJobRequestSizeCheck[(sizeof(EffFileJobRequest) == 0x20) ? 1 : -1];

/* Class operations select the copied parameter format and own this 0x40-byte
 * header. Each resource kind owns the pointer returned by its factory. */
typedef struct EffClassWork {
    union {
        u8 transform[0x20];
        struct {
            f32 position[4];
            f32 orientation[4];
        } vectors;
    };
    f32 scale;
    u32 color;
    u32 frame;
    s32 kind;
    void *resource;
    void *payload;
    u8 pad38[8];
} EffClassWork;

/* Count-bearing serialized formats share the first 0x3C bytes. Native
 * point callbacks compare the time word signed; emitter callbacks compare it
 * unsigned. These are two real interpretations of the same serialized word. */
typedef struct EffBillTimedHeader {
    SdfColorTrack colorTrack;
    SdfAlphaTrack alphaTrack;
    union {
        u32 progress;
        s32 duration;
    } time;
    u32 count;
} EffBillTimedHeader;

/* Track-set allocations end with this 0x30-byte header. Kind selects the
 * geometry and optional column strides; tail follows the geometry buffer. */
typedef struct EffTrackSet {
    u32 type;
    u32 color;
    s32 rows;
    u16 kind;
    u8 pad0E[2];
    s32 count;
    u8 flag;
    u8 pad15[3];
    struct RefObj *shared;
    u8 *buffer;
    u8 *columns;
    u8 *tail;
    sdfMat_t *handle;
    struct sdfMemHandle_t *allocation;
} EffTrackSet;

typedef char EffTrackSet_size_must_be_0x30[(sizeof(EffTrackSet) == 0x30) ? 1 : -1];

typedef struct EffBillPointConfig {
    EffBillTimedHeader timed;
    /* Bolt/range updaters interpret the
     * layer word as an unsigned division count for floating-point math. */
    s32 layers;
    s32 respawnDelay;
    /* Initial lateral-wave amplitude of alternating five-point ribbons. */
    f32 alternatingAmplitude; /* 0x44 */
    u8 pad48[4];
    u32 periods;
    f32 amplitude;
    f32 speed;
    f32 speedJitter;
    u8 drawFlag;
    u8 pad5D[0xB];
    f32 rangeFadeInEnd;
    f32 rangeFadeOutStart;
    u32 colorA;
    u32 colorB;
    u32 colorC;
    f32 halfWidth;
    f32 edgeWidth;
    f32 totalLength; /* 0x84: divided by layer count to size each mesh segment. */
} EffBillPointConfig;

typedef char EffClassWorkSizeCheck[(sizeof(EffClassWork) == 0x40) ? 1 : -1];

typedef char EffBillTimedHeaderSizeCheck[(sizeof(EffBillTimedHeader) == 0x3C) ? 1 : -1];

typedef char EffBillPointConfigSizeCheck[(sizeof(EffBillPointConfig) == 0x88) ? 1 : -1];

/* Point-set node: `rows` 16-byte entries in `buffer`, then `tail`. */
typedef struct EffPointSet {
    u32 type;       // 0x00
    u32 color;      // 0x04
    s32 rows;       // 0x08
    u8 flag;        // 0x0C
    u8 pad_0D[3];
    u8 *buffer;     // 0x10
    u8 *tail;       // 0x14
    struct sdfMat_t *handle; // 0x18
    struct sdfMemHandle_t *allocation; // 0x1C
} EffPointSet;

typedef struct EffScaleRangeEntry {
    struct EffPointSet *set;
    f32 position[3];
    u8 pad10[4];
    s32 negativeSeed;
    u32 color;
    f32 yaw;
    f32 pitch;
    f32 angularRate;
    f32 phase;
    f32 height;
} EffScaleRangeEntry;

typedef char EffScaleRangeEntrySizeCheck[(sizeof(EffScaleRangeEntry) == 0x30) ? 1 : -1];

typedef char EffPointSetSizeCheck[sizeof(EffPointSet) == 0x20 ? 1 : -1];

/* Span table layout of the model-effect implementations. */
typedef struct EffSpanEntry {
    f32 first;
    f32 second;
    u32 referenceAge;
} EffSpanEntry;

typedef struct EffSpanRecord {
    EffSpanEntry *entries;
    struct EffPointSet *pointSet;
    struct EffTrackSet *references;
    u32 pointCount;
} EffSpanRecord;

typedef struct EffSpanTable {
    EffSpanRecord *records;
    u32 count;
    u16 total;
    u8 pad0A[2];
    struct sdfMemHandle_t *allocation;
} EffSpanTable;

/* Inline source prefix consumed by the paired particle-span owners. Bytes in
 * each explicit pad remain opaque; the operation table supplies copy size, so
 * this declaration does not assert the full copied payload extent. */
typedef struct EffSpanConfig {
    SdfColorTrack pointColorTrack;       /* 0x00 */
    SdfAlphaTrack pointAlphaTrack;       /* 0x24 */
    u32 progress;                        /* 0x34 */
    u8 drawPoints;                       /* 0x38 */
    u8 pad39[3];
    u32 middleColor;                     /* 0x3C */
    u32 edgeColor;                       /* 0x40 */
    u8 pad44[4];
    f32 drawScale;                       /* 0x48 */
    f32 pointHalfWidth;                  /* 0x4C: ribbon half-width at the span points */
    SdfColorTrack referenceColorTrack;   /* 0x50 */
    SdfAlphaTrack referenceAlphaTrack;   /* 0x74 */
    s32 referenceLifetime;               /* 0x84 */
    u8 drawReferences;                   /* 0x88 */
    u8 allowMultipleReferenceStarts;     /* 0x89 */
    u8 pad8A[2];
    s32 geometryStartUpdateCount;        /* 0x8C */
    u32 referenceColorA;                 /* 0x90 */
    u32 referenceColorB;                 /* 0x94 */
    f32 positionScaleA;                  /* 0x98 */
    f32 positionScaleB;                  /* 0x9C */
    f32 firstRand;                       /* 0xA0 */
    f32 secondBase;                      /* 0xA4 */
    f32 rangeRand;                       /* 0xA8 */
    s32 referenceRampDuration;           /* 0xAC */
    u32 perSpan;                         /* 0xB0 */
    f32 rotationAngularVelocity;          /* 0xB4 */
    f32 rotationAngularAcceleration;      /* 0xB8 */
    u8 pointSetFlag;                     /* 0xBC */
} EffSpanConfig;

/* An effect's model resource: its placement, color, model context and span configuration. */
typedef struct EffModelResource {
    f32 position[4];
    f32 orientation[4]; /* Quaternion. */
    f32 scale;
    u32 color;
    s32 updateCount;
    s32 kind;
    mdlObj_t *model;
    u32 attributes;
    struct EffSpanTable *childResource;
    struct EffSpanConfig *source;
} EffModelResource;

typedef char EffSpanEntry_size_check[
    (sizeof(EffSpanEntry) == 0x0C) ? 1 : -1];

typedef char EffSpanRecord_size_check[
    (sizeof(EffSpanRecord) == 0x10) ? 1 : -1];

typedef char EffSpanTable_size_check[
    (sizeof(EffSpanTable) == 0x10) ? 1 : -1];

#define VU0_SET_VF10_COMPONENT(axis, f) \
    VU0_SCALAR_OP_CLOBBER(f, "vaddx." #axis " vf10, vf0, vf2x")

SdfFlagListWork *effCreateSelectionFlagListFromWork(const SdfFlagListParams *);

FileJobPayload *fileJobCreateFromCommandState(const char *entry);

void fileJobInvokeTypeCallback(FileJobPayload *job);

void fileWriteToPfs(FileJobPayload *job, const char *filePath);

void fileJobSetPrimaryData(FileJobPayload *job, const void *src, s32 size, u16 option);

void fileJobSetSecondaryData(FileJobPayload *job, const void *src, s32 size, u16 selector);

void fileJobCopyCommandIntoSecondaryData(FileJobPayload *job, const char *commandPath, u16 selector);

FileQueue *fileQueueCreateFromCommandState(const char *entry);

void fileQueueSaveVersionedImage(FileQueue *queue, const char *filePath);

void fileQueueSaveImage(FileQueue *queue, const char *filePath);

FileJob *fileAppendJobFromCommandPath(FileQueue *queue, const char *commandPath);

FileJob *fileJobDuplicateAfter(FileQueue *queue, FileJob *source);

void fileQueueDetachSectorFollower(FileQueue *queue, FileJob *job);

typedef struct EffCameraCreateRequest {
    struct EffFileJobRequest *resource;
    struct effDebugMenu_t *object;
    u32 existingJob;
    f32 position[3];
} EffCameraCreateRequest;

typedef char EffCameraCreateRequest_size_must_be_0x18[
    (sizeof(EffCameraCreateRequest) == 0x18) ? 1 : -1];

struct BaObj;
struct effDebugMenu_t;

/* The item at +8, selected by effDebugMenuItem_t.type (EFF_DEBUG_ITEM_*). The function members are
 * one engine slot; they differ only in the prototypes the editors were given. */
typedef union effDebugMenuValue {
    u8 *u8Value;                          /* BYTE, BYTE_TOGGLE, BYTE_CHOICE */
    u16 *u16Value;                        /* SHORT_CHOICE */
    s32 *s32Value;                        /* INT, INT_RANGE, COUNT, CHOICE */
    u32 *rgbaValue;                       /* RED, GREEN, BLUE, ALPHA: one byte lane each */
    f32 *f32Value;                        /* FLOAT, FLOAT_TENTH */
    struct effDebugMenu_t *child;         /* CHILD, CHILD_PAGE */
    s32 (*poll)(void);                    /* FUNCTION: file choosers */
    u32 (*command)(void);                 /* FUNCTION, COMMAND */
    s32 (*editObject)(struct BaObj *);    /* FUNCTION: scale and rotation track editors */
    void (*editObjectNoResult)(struct BaObj *);
    s32 (*editRequest)(s32);              /* FUNCTION: colour and fade track editors */
    void (*action)(void);
} effDebugMenuValue;

/* The item at +0xC: the lower bound, or the second function or colour of a command row. */
typedef union effDebugMenuValue2 {
    s32 minValue;
    void (*release)(void);                /* FUNCTION */
    u32 color;                            /* COMMAND */
} effDebugMenuValue2;

/* Item types of the effect editor; the engine that interprets them is not in retail. */
#define EFF_DEBUG_ITEM_BYTE 0x02
#define EFF_DEBUG_ITEM_INT_RANGE 0x04
#define EFF_DEBUG_ITEM_INT 0x06
#define EFF_DEBUG_ITEM_FLOAT 0x08
#define EFF_DEBUG_ITEM_BYTE_FLAG 0x0C
#define EFF_DEBUG_ITEM_BYTE_TOGGLE 0x0E
#define EFF_DEBUG_ITEM_RED 0x0F
#define EFF_DEBUG_ITEM_GREEN 0x10
#define EFF_DEBUG_ITEM_BLUE 0x11
#define EFF_DEBUG_ITEM_ALPHA 0x12
#define EFF_DEBUG_ITEM_SEPARATOR 0x14
#define EFF_DEBUG_ITEM_BLANK 0x15
#define EFF_DEBUG_ITEM_CHILD 0x16
#define EFF_DEBUG_ITEM_CHILD_PAGE 0x17
#define EFF_DEBUG_ITEM_COMMAND 0x19
#define EFF_DEBUG_ITEM_FUNCTION 0x1A
#define EFF_DEBUG_ITEM_FLOAT_TENTH 0x1E

/* An effect editor item (0x18 bytes). SMT3 names; the step is f32 here. */
typedef struct effDebugMenuItem_t {
    const char *label;
    u8 type;
    u8 pad5[3];
    effDebugMenuValue value;
    effDebugMenuValue2 value2;
    s32 maxValue;
    f32 step;
} effDebugMenuItem_t;

/* A page of the effect editor and the battle camera presets (0x38 bytes). SMT3's record with
 * flags added, no copy fields, and the parent moved to the end. */
typedef struct effDebugMenu_t {
    const char *title;                    /* 0x00 */
    s32 x;                                /* 0x04: copied to the preset selector at 002B74AC. */
    s32 y;                                /* 0x08 */
    s32 selected;                         /* 0x0C */
    u32 flags;                            /* 0x10 */
    effDebugMenuItem_t *items;            /* 0x14 */
    s32 itemCount;                        /* 0x18 */
    struct EffFileJobRequest *source;     /* 0x1C */
    u32 (*create)(u8 *request);           /* 0x20 */
    void (*update)(u32 job);              /* 0x24 */
    void (*destroy)(u32 job);             /* 0x28 */
    u32 *param;                           /* 0x2C */
    u32 (*recreate)(u8 *request);         /* 0x30 */
    struct effDebugMenu_t *parent;        /* 0x34 */
} effDebugMenu_t;

typedef char effDebugMenuItemSizeCheck[(sizeof(effDebugMenuItem_t) == 0x18) ? 1 : -1];

typedef char effDebugMenuSizeCheck[(sizeof(effDebugMenu_t) == 0x38) ? 1 : -1];

/* Helper effect parameters (0x3C bytes): HD's effHelper_Params_t with the colour and alpha
 * parameters inline instead of behind pointers. */
typedef struct effHelper_Params_t {
    u32 rgbType;      /* 0x00 */
    u32 rgbStart;     /* 0x04: RGBA bytes */
    u32 rgbEnd;       /* 0x08 */
    u32 rgbPoint[6];  /* 0x0C */
    u32 transparency; /* 0x24 */
    s32 blend;        /* 0x28: TNEFFECT_BLEND */
    f32 fadeInRate;   /* 0x2C */
    f32 fadeOutRate;  /* 0x30 */
    s32 life;         /* 0x34 */
    f32 speed;        /* 0x38 */
} effHelper_Params_t;

typedef char effHelperParamsSizeCheck[(sizeof(effHelper_Params_t) == 0x3C) ? 1 : -1];

/* Muzzle flash parameters as the effect editor allocates them (0x38 bytes). */
typedef struct effMuzzleFlash_Params_t {
    f32 scale_h;      /* 0x00 */
    f32 scale_w;      /* 0x04 */
    u8 unk_08[0x30];
} effMuzzleFlash_Params_t;

typedef char effMuzzleFlashParamsSizeCheck[(sizeof(effMuzzleFlash_Params_t) == 0x38) ? 1 : -1];

struct Standard;
struct ptcHandle_t;

struct ptcHandle_t *PartsCreate_effParticle(struct Standard *pParts);

#endif
