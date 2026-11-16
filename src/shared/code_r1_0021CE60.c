#include "common.h"
#include "kernel.h"
#include "scr.h"
#include "event.h"
#include "field.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "billManager.h"
#include "sdfQuat.h"
#include "sdfSound.h"
#include "sdfFile.h"
#include "effPCPMisc_00224D10.h"
#include "code_r1_0021CE60.h"

extern void fclShopProcessStart(void *pPlace);

#include "sdk_libc.h"

void CallShopTest(s32 flg, void *data);

#include "fileManager.h"
#include "newdata.h"
#include "sdk_libkernl.h"
#include "include_asm.h"
#include "sdf_resource.h"
#include "billboard.h"
#include "sdf_types.h"
#include "draw.h"
#include "libsdf.h"
#include "sdf_movie_types.h"
#include "effect.h"
#include "ps2asm.h"
#include "basic.h"
#include "evt_world.h"
#include "fpu.h"
#include "code_r1_001CC1D0.h"
#include "slMain.h"
#include "code_r1_0021C768.h"
#include "sdfMath.h"
#include "sdk_libm.h"
#include "dds3Randomize.h"

/* Convert three Euler angle components to the VU0 quaternion output. */

/* Multiply the quaternions in vf10 and vf11, writing the product to vf10. */

/* Convert the quaternion in vf10 into the primary matrix bank vf28-vf31. */
f32 acosf(f32 x);
f32 atan2f(f32 y, f32 x);

/* The selected kind interprets the opaque payload word. */
struct billHandle_t *dds3BillboardGeneral(s32 index);

/* Apply volume and pan to a sequence track through the command queue. */

/* Stable state APIs. Callback factories keep caller-specific callback types. */
sdfFileCmdHandle_t *sdfFileOpen(const char *path);

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */

#define SDK_SKIP_memset

enum SdfColorTrackMode {
    SDF_COLOR_TRACK_MODE_ENDPOINTS = 0,
    SDF_COLOR_TRACK_MODE_ONE_INTERMEDIATE_KEY = 1,
    SDF_COLOR_TRACK_MODE_TWO_INTERMEDIATE_KEYS = 2
};

fileJobPayload_t *fileDuplicateJob(fileJobPayload_t *request);

void fileJobWriteSerializedPayload(s32 fd, fileJobPayload_t *job);

void effPartsPositionSet(struct plistHandle_t *queue, const f32 position[4]);

void effPartsRotateSet(struct plistHandle_t *queue, const f32 rotation[4]);

void effPartsScaleSet(struct plistHandle_t *queue, f32 scale);

void effPartsDraw(struct plistHandle_t *queue);

void fileQueueNotifyAllJobsComplete(struct plistHandle_t *queue);

#define FILE_JOB_FLAG_SHARED_PAYLOAD 0x1

#define FILE_JOB_UPDATE_FLAG_APPLY_QUEUE_SCALE 0x1

void ColorSet(plistHandle_t *queue, u32 color);

enum EffScalarCurveMode {
    EFF_SCALAR_CURVE_MODE_ENDPOINTS = 0,
    EFF_SCALAR_CURVE_MODE_ONE_INTERMEDIATE_KEY = 1,
    EFF_SCALAR_CURVE_MODE_TWO_INTERMEDIATE_KEYS = 2
};

#define FILE_SLOT_FRAME_NONE (-1)

/* A surface node that also owns one cloned file queue per active record. */
typedef struct effQueuedSurfaceNode_t {
    u32 handleCount;
    u32 color;
    f32 opacity;
    u32 kind;
    effSurfaceParams_t params; /* 0x10: copied source header */
    u32 index;
    struct sdfMemHandle_t *handleBuffer; // 0x30
    billHandle_t *resource;
    fileJobPayload_t **jobs;          // 0x38
    struct sdfMemHandle_t *jobAllocation;      // 0x3C
    struct plistHandle_t **queues;      // 0x40: cloned queues, one per active record
    struct sdfMemHandle_t *queueAllocation;    // 0x44: backing allocation descriptor
    struct ctexlHandle_t *resourceHolder; // 0x48
    bhvrHandle_t *record; // 0x4C: retained grid record table
    u16 active;         // 0x50: initialized to 1
} effQueuedSurfaceNode_t;

typedef struct effScaleEntry_t {
    f32 unk0;
    f32 value;
} effScaleEntry_t;

/* Access view through +0xF4; intervening storage and any allocation tail stay opaque. */
typedef struct effScaleSet_t {
    u8 pad0[0x64];
    f32 unk64;
    f32 unk68;
    u8 pad6C[4];
    effScaleEntry_t entries[3];
    u8 pad88[0x40];
    f32 unkC8;
    f32 unkCC;
    f32 unkD0;
    f32 unkD4;
    f32 unkD8;
    f32 unkDC;
    f32 unkE0;
    f32 unkE4;
    u8 padE8[8];
    f32 unkF0;
} effScaleSet_t;

typedef struct effScaleOwner_t {
    u8 pad0[0x20];
    effScaleSet_t *dst;
    effScaleSet_t *src;
} effScaleOwner_t;

typedef char effScaleEntry_size_must_be_0x08[(sizeof(effScaleEntry_t) == 0x08) ? 1 : -1];

typedef s32 (*FileMenuContinuationCallback)(void);

struct sprHandle_t;

enum { FILE_CONFIG_SET, FILE_CONFIG_FRAME, FILE_CONFIG_X, FILE_CONFIG_Y };

typedef struct mnuWork_t {
    fileSavePreviewRecord_t header;
    s8 applyLoadResult;
    s8 hardInitSelected;
    u16 flowConditionMask;
    u16 startBranchFlag; /* 0x34: selects one of two initial menu flows */
    u16 flowRowCursor;
    u16 unk38;
    u16 unk3A;
    struct sdfMemHandle_t *unk3C;
} mnuWork_t;

typedef struct MdlFlagPair {
    s32 flag;
    s32 unk4;
} MdlFlagPair;

/* Eight-byte story-flag rows select a unit and a six-byte bonus record. */
typedef struct fileStatBonus_t {
    s32 flag;
    u16 unitId;
    u8 bonusIndex;
    u8 pad07;
} fileStatBonus_t;

struct mnuListNode_t;

struct sdfMemHandle_t;

extern u32 D_00437D18;
extern u32 D_00437D1C;
extern u32 D_00437D20;
extern struct ctexlHandle_t *effCacheTexCreate(u32);
extern s8 dds3PadButtonState[];
extern void sdfVuMatrixToQuaternion(const f32 matrix[4][4]);

/* Effect parameter-set dispatch tables. Every effect kind owns one 0x28-byte
 * entry per table; the handler lives at +0x0. Slots are declared as separate
 * arrays (D_00353710/14/18/1C/20/24/28/2C/30/34 and D_00353880/84/88/90/94/
 * 98/9C/A0/A4). The family2 create table (D_00353880) additionally carries a
 * fallback selector at +0x0C: nonzero calls the entry handler directly,
 * zero falls back through D_003536A0.
 */
/* 8-byte parameter work (family1): kind id plus one data pointer. */
typedef struct effDispatchExtra_t {
    void (*func)(void *, f32 vector[4]); /* 0x00 vector handler, may be NULL */
    u8 pad04[0x24];
} effDispatchExtra_t;

typedef struct effDispatchScale_t {
    void (*func)(void *, f32); /* 0x00 handler, may be NULL */
    u8 pad04[0x24];
} effDispatchScale_t;

typedef struct effDispatchColor_t {
    void (*func)(void *, u32); /* 0x00 handler, may be NULL */
    u8 pad04[0x24];
} effDispatchColor_t;

typedef struct fileLoadMirror_t {
    u32 current;
    u32 previous;
} fileLoadMirror_t;

extern void effCreate(effQueuedSurfaceNode_t *node, u32 entryId, void *resource);
extern void BillboardSet(effQueuedSurfaceNode_t *node, u32 resourceId);
extern void BillboardGeneralSet(effQueuedSurfaceNode_t *node, u32 resourceId);
extern void fileLoadObjectOpenNamedDevice(effQueuedSurfaceNode_t *node, u32 resourceId);
extern void fileReplaceEffectSurfaceJobs(effQueuedSurfaceNode_t *node, fileJobPayload_t *job);
extern void PartsListSet(effQueuedSurfaceNode_t *node, plistHandle_t *queue);
extern void fileReplaceReferenceHolder(effQueuedSurfaceNode_t *node, u32 resource);
extern void fileJobFreePrimaryBuffer(fileJobPayload_t *job);
extern void fileJobFreeSecondaryBuffer(fileJobPayload_t *job);
extern void fileQueueAppend(plistHandle_t *queue, fileJob_t *job);
extern fileJob_t *fileJobCreate(void);
extern fileJob_t *fileQueueFindById(plistHandle_t *queue, u32 id);
extern s32 fileFindQueuedJobIndex(plistHandle_t *queue, fileJob_t *target);
extern u8 sdfHddValid;
extern char fileWriteToPfsData[];
extern char fileWriteToPfsData2[];
extern char fileWriteToPfsData3[];

#define FILE_MAIN_BLOB_SIZE 0x1E840

u8 fileIsLoadedAndConditionTrue(s32 condition);
s32 slChkWindowOpenDone(void);

struct sdfMemHandle_t;

extern FileSelectionCallback D_00439024, D_00439028;

/* Yes/no cursor input while the card request is idle. */
static inline void filePollPromptChoice(void) {
    if (D_00437D20 == 0 && slChkWindowOpenDone() != 0) {
        if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x27] < 0) != 0) {
            if (D_00437D18 != 1) {
                p2sdPlaySE(0, 0x7F, 0x3F);
            }
            D_00437D18 = 1;
        } else if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x26] < 0) != 0) {
            if (D_00437D18 != 0) {
                p2sdPlaySE(0, 0x7F, 0x3F);
            }
            D_00437D18 = 0;
        } else if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x21] < 0) != 0) {
            if (D_00437D18 == 0 && (D_00437D1C == 1 || D_00437D1C == 3)) {
                p2sdPlaySE(0x11, 0x7F, 0x3F);
            } else {
                p2sdPlaySE(8, 0x7F, 0x3F);
            }
            if (D_00437D18 != 0) {
                D_00437D20 = 2;
            } else {
                D_00437D20 = 1;
            }
        } else if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x23] < 0) != 0) {
            p2sdPlaySE(0xA, 0x7F, 0x3F);
            D_00437D20 = 2;
        }
    }
}

/* Lower the selected low/alpha byte, retaining the native signed, unmasked bound. */

/* Init record at fileCursorPulseState. */
typedef struct fileInit374E0_t {
    s8 mode[3];      /* 0x00 */
    u8 pad3;         /* 0x03 */
    s32 pos[3][2];   /* 0x04 */
    s32 counter[3];  /* 0x1C */
    u8 pad28[0xC];   /* 0x28 */
    s32 alpha[3];    /* 0x34 */
} fileInit374E0_t;

typedef struct fileFlowEntry_t {
    u16 state;
    u16 mask;
} fileFlowEntry_t;

typedef struct fileMultiFlagBonus_t {
    s32 flags[4];
    u8 bonusIndex;
    u8 pad11[3];
} fileMultiFlagBonus_t;

extern u8 aViewEye[];

/* vu0 routine: point at t between p[1] and p[2] of a Catmull-Rom (Hermite, 0.5 tangents) spline, left in vf10 */
void vuCatmullRomPoint(f32 (*p)[4], f32 t);
extern u8 aViewTarget[];

/* The quaternion result is returned in vf10 for the VU transform routines. */
void sdfVuMatrixToQuaternion(const f32 matrix[4][4]);

#define FILE_JOB_LOCATION_AS_JOB_RELATIVE_OFFSET(payload) ((payload).offset)

#define FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(payload) ((payload).offset)

#define FILE_JOB_LOCATION_CLEAR(payload) ((payload).offset = 0)

void fileJobFreePrimaryBuffer(fileJobPayload_t *job);
void fileJobFreeSecondaryBuffer(fileJobPayload_t *job);
void fileJobNotifyPair(fileJobPayload_t *left, fileJobPayload_t *right);
u32 fileJobSerializedSize(fileJobPayload_t *job);
void fileJobResetAndInitTransform(fileJob_t *job);
void fileQueueAppend(plistHandle_t *queue, fileJob_t *job);
void fileQueueInsertAfter(plistHandle_t *queue, fileJob_t *after, fileJob_t *job);
void fileQueueRemove(plistHandle_t *queue, fileJob_t *job);
fileJob_t *fileJobCreate(void);
void fileDestroyJob(fileJob_t *job);
fileJob_t *fileAppendJob(plistHandle_t *queue, u32 id);
fileJob_t *fileDuplicateAndAppendJob(plistHandle_t *queue, fileJobPayload_t *source);
extern u8 fileAppendJobFromCommandPathData[];
extern fileJob_t *fileQueueFindBySector(plistHandle_t *queue, u32 sector);
extern fileJob_t *fileQueueFindFlaggedById(plistHandle_t *queue, u32 id);

/* Makes the first job chained to owner's sector the leader and rechains the rest to it. */
static inline void fileQueueRechainSectorFollowers(plistHandle_t *queue, fileJob_t *owner, fileJob_t *leader) {
    fileJob_t *next;

    leader->sector = 0;
    leader->flags &= ~FILE_JOB_FLAG_SECTOR_FOLLOWER;
    fileQueueLinkJobToSectorLeader(queue, leader, leader);
    next = fileQueueFindBySector(queue, owner->id);
    while (next != NULL) {
        fileQueueLinkJobToSectorLeader(queue, next, leader);
        next = fileQueueFindBySector(queue, owner->id);
    }
}

extern char fileQueueImagePaddingBytes[];
extern s32 fileQueueCountLinkedJobs(plistHandle_t *queue);

/* Sixteen-byte preamble of a saved file queue. */
typedef struct fileQueueImageHeader_t {
    s32 version;
    s32 unk4;
    s32 unk8;
    f32 unkC;
} fileQueueImageHeader_t;

extern char fileQueueVersionedImagePaddingBytes[];
fileJob_t *fileQueueFindById(plistHandle_t *queue, u32 id);
fileJob_t *fileQueueFindFlaggedById(plistHandle_t *queue, u32 id);
fileJob_t *fileQueueFindBySector(plistHandle_t *queue, u32 sector);
s32 fileFindQueuedJobIndex(plistHandle_t *queue, fileJob_t *target);
s32 fileQueueCountLinkedJobs(plistHandle_t *queue);
effQueuedSurfaceNode_t *fileCreateSurfaceLoaderState(s32 capacity);
effQueuedSurfaceNode_t *fileCreateGridLoaderRecord(fileKeyBlock_t *hdr);
void Delete_effParticle(effQueuedSurfaceNode_t *node);
void ResourceCopy(effQueuedSurfaceNode_t *dst, effQueuedSurfaceNode_t *src);
effQueuedSurfaceNode_t *Copy_effParticle(effQueuedSurfaceNode_t *owner);
void effCreate(effQueuedSurfaceNode_t *node, u32 entryId, void *resource);
void fileLoadObjectOpenNamedDevice(effQueuedSurfaceNode_t *node, u32 resourceId);
void BillboardSet(effQueuedSurfaceNode_t *node, u32 resourceId);
void BillboardGeneralSet(effQueuedSurfaceNode_t *node, u32 resourceId);
void fileReplaceEffectSurfaceJobs(effQueuedSurfaceNode_t *node, fileJobPayload_t *job);
void PartsListSet(effQueuedSurfaceNode_t *node, plistHandle_t *queue);
void fileReplaceReferenceHolder(effQueuedSurfaceNode_t *obj, u32 resource);
void fileClearLoadObjectReferences(effQueuedSurfaceNode_t *obj);

void fileSendLoadObjectRecordVector(effQueuedSurfaceNode_t *obj, const u128 *value);
void fileCopyLoadObjectRecordVector(effQueuedSurfaceNode_t *obj, const u128 *value);
void fileSetRecordWordFour(s32 record, u32 value);
void fileSetLoadObjectScale(effQueuedSurfaceNode_t *obj, f32 scale);
void fileResetSlotStates(bhvrHandle_t *table);

void fileInvalidateSlotGroup(bhvrHandle_t *table, effPosture_t *slot);
void fileCopyAndInvalidateSlotGroup(bhvrHandle_t *table, effPosture_t *source);
void effUpdateGridRecordSlotHistory(bhvrHandle_t *record, effPosture_t *slot);
void fileSampleKeyTracks(effPosture_t *out, const fileKeyBlock_t *block, s32 frame, const f32 *target);
extern void effUpdateGridRecordSlotHistory(bhvrHandle_t *, effPosture_t *);

typedef struct fileWaveMotion_t {
    f32 axis[4];             /* 0x00: xyz only */
    f32 swayDirection[4];    /* 0x10: xyz only */
    f32 phase;
    f32 previousSine;
    f32 amplitude;
    f32 amplitudeStep;
    f32 phaseStep;
    f32 speed;
    f32 scaleMultiplier;
    f32 angle;
    f32 angleMultiplier;
} fileWaveMotion_t;            /* 0x44, ordinary word alignment */

void effScaleWaveEmitterParameters(effScaleOwner_t *owner, f32 scale);

typedef struct fileSlotMotion_t {
    f32 direction[4];     /* 0x00; this callback initializes xyz only */
    f32 speed;            /* 0x10 */
    f32 scaleMultiplier;  /* 0x14 */
    f32 angle;            /* 0x18 */
    f32 angleMultiplier;  /* 0x1C */
} fileSlotMotion_t;

typedef struct fileCircularMotion_t {
    f32 origin[4];          /* 0x00: xyz only */
    f32 orientation[4];     /* 0x10: saved emitter quaternion */
    f32 height;             /* 0x20 */
    f32 radius;
    f32 radiusStep;
    f32 initialPhase;
    f32 angularSpeed;
    f32 axialSpeed;
    f32 scaleMultiplier;
    f32 angle;
    f32 angleMultiplier;
} fileCircularMotion_t;       /* 0x44, ordinary word alignment */

typedef struct fileOrientedRingMotion_t {
    f32 origin[4];          /* 0x00: xyz only */
    f32 orientationAngle;   /* 0x10 */
    f32 tiltAngle;
    f32 radius;
    f32 radiusStep;
    f32 angularSpeed;
    f32 initialPhase;
    f32 height;
    f32 scaleMultiplier;
    f32 angle;
    f32 angleMultiplier;
} fileOrientedRingMotion_t;   /* 0x38, ordinary word alignment */

typedef struct fileCurveMotion_t {
    f32 firstControl[3];
    f32 secondControl[3];
    f32 rotationReference[3]; /* native live-path input; writer unresolved */
    f32 endpointOffset[3];
    f32 angularSpeed;
    f32 travelSpeed;
    f32 pathLength;
    f32 progress;
    f32 scaleMultiplier;
    f32 angle;
    f32 angleMultiplier;
} fileCurveMotion_t; /* 0x4C, naturally four-byte aligned */

void effPCPHelperBEDGetModel(bhvrHandle_t *record);

void fileJobFreeSecondaryBuffer(fileJobPayload_t *job) {
    struct sdfMemHandle_t *allocation = job->secondary.allocation;
    if (allocation != NULL) {
        sdfMemFreeDirect(allocation);
        FILE_JOB_LOCATION_CLEAR(job->secondary);
        job->secondary.size = 0;
        job->secondary.allocation = NULL;
    }
}

fileJobPayload_t *Copy_effParts(fileJobPayload_t *request) {
    fileJobPayload_t *job = fileCreateJob(request->type);
    fileTypeCallbacks_t *cb = &fileJobTypeOperations[job->type];

    job->option = request->option;
    job->primary.selector = request->primary.selector;
    job->data = cb->createChild(request->data, job->type);
    return job;
}

void fileJobNotifyPair(fileJobPayload_t *left, fileJobPayload_t *right) {
    void (*cb)(void *, void *) = fileJobTypeOperations[right->type].notifyPair;

    if (cb != NULL) {
        cb(left->data, right->data);
    }
}

void ScaleSet_effParts(fileJobPayload_t *job) {
    u16 id = job->type;

    if (fileJobTypeOperations[id].reset != NULL) {
        fileJobTypeOperations[id].reset(job->data);
    }
}

void fileJobInvokeTypeCallback(fileJobPayload_t *job) {
    u16 idx = job->type;
    void *data = job->data;

    fileJobTypeOperations[idx].invoke(data);
}

void PositionSet(fileJobPayload_t *job, const f32 position[4]) {
    u16 id = job->type;

    if (fileJobTypeOperations[id].setPosition != NULL) {
        fileJobTypeOperations[id].setPosition(job->data, position);
    }
}

void fileJobInvokeRotationCallback(fileJobPayload_t *job, const f32 rotation[4]) {
    u16 id = job->type;

    if (fileJobTypeOperations[id].setRotation != NULL) {
        fileJobTypeOperations[id].setRotation(job->data, rotation);
    }
}

void fileJobInvokeScaleCallback(fileJobPayload_t *job, f32 scale) {
    u16 id = job->type;

    if (fileJobTypeOperations[id].setScale != NULL) {
        fileJobTypeOperations[id].setScale(job->data, scale);
    }
}

void fileDispatchJobTypeCallback(fileJobPayload_t *job, u32 color) {
    u16 id = job->type;

    if (fileJobTypeOperations[id].setColor != NULL) {
        fileJobTypeOperations[id].setColor(job->data, color);
    }
}

void fileJobSetPrimaryData(fileJobPayload_t *job, const void *src, s32 size, u16 option) {
    fileJobFreePrimaryBuffer(job);
    if (src != NULL && size > 0) {
        job->primary.allocation = sdfMemAllocDirect(size);
        FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(job->primary) = sdfMemLock(job->primary.allocation);
        job->primary.size = size;
        job->option = option;
        memcpy((void *)FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(job->primary), src, size);
    }
}

void fileJobCopyCommandIntoPrimaryData(fileJobPayload_t *job, const char *commandPath, u16 option) {
    sdfFileCmdHandle_t *command;
    s32 size;
    struct sdfMemHandle_t *allocation;
    void *buffer;

    command = sdfFileOpen(commandPath);
    if (command != 0) {
        size = sdfFileLength(command);
        allocation = sdfMemAllocDirect(size);
        buffer = (void *)sdfMemLock(allocation);
        sdfFileRead(command, buffer, size);
        sdfFileClose(command);
        fileJobSetPrimaryData(job, buffer, size, option);
        sdfMemFreeDirect(allocation);
        return;
    }
}

void fileJobSetSecondaryData(fileJobPayload_t *job, const void *src, s32 size, u16 selector) {
    fileJobFreeSecondaryBuffer(job);
    if (src != NULL && size > 0) {
        job->secondary.allocation = sdfMemAllocDirect(size);
        FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(job->secondary) = sdfMemLock(job->secondary.allocation);
        job->secondary.size = size;
        job->primary.selector = selector;
        memcpy((void *)FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(job->secondary), src, size);
    }
}

void fileJobCopyCommandIntoSecondaryData(fileJobPayload_t *job, const char *commandPath, u16 selector) {
    sdfFileCmdHandle_t *command;
    s32 size;
    struct sdfMemHandle_t *allocation;
    void *buffer;

    command = sdfFileOpen(commandPath);
    if (command != 0) {
        size = sdfFileLength(command);
        allocation = sdfMemAllocDirect(size);
        buffer = (void *)sdfMemLock(allocation);
        sdfFileRead(command, buffer, size);
        sdfFileClose(command);
        fileJobSetSecondaryData(job, buffer, size, selector);
        sdfMemFreeDirect(allocation);
        return;
    }
}

INCLUDE_ASM(const s32, "code_r1_0021CE60", fileJobWriteSerializedPayload);

void fileWriteToPfs(fileJobPayload_t *job, const char *filePath) {
    char path[0xD0];
    s32 fd;

    if (sdfHddValid != 0) {
        sprintf(path, fileWriteToPfsData, filePath);
        fd = sceOpen(path, 0x602, 0x1B6);
    } else {
        sprintf(path, fileWriteToPfsData2, sdfFileGetHostRoot(), filePath);
        fd = sceOpen(path, 0x602);
    }
    fileJobWriteSerializedPayload(fd, job);
    sceClose(fd);
    sceSync(fileWriteToPfsData3, 0);
}

INCLUDE_ASM(const s32, "code_r1_0021CE60", fileDuplicateJob);

/* NULL when the command file does not open: retail's v0 is sdfFileOpen's
 * null result there. */
fileJobPayload_t *fileJobCreateFromCommandState(const char *entry) {
    sdfFileCmdHandle_t *command;
    s32 size;
    struct sdfMemHandle_t *allocation;
    void *buffer;
    fileJobPayload_t *job;

    command = sdfFileOpen(entry);
    if (command != 0) {
        size = sdfFileLength(command);
        allocation = sdfMemAllocDirect(size);
        buffer = (void *)sdfMemLock(allocation);
        sdfFileRead(command, buffer, size);
        sdfFileClose(command);
        job = fileDuplicateJob((fileJobPayload_t *)buffer);
        sdfMemFreeDirect(allocation);
        return job;
    }
    return NULL;
}

u32 fileJobSerializedSize(fileJobPayload_t *job) {
    u32 size = 0x2C;

    if (fileResolvePrimaryBuffer(job) != NULL) {
        size = job->primary.size + 0x2C;
    }
    if (fileResolveSecondaryBuffer(job) != NULL) {
        u32 aligned = size >> 4;
        if ((size & 0xF) != 0) {
            aligned = (aligned + 1) << 4;
        } else {
            aligned = aligned << 4;
        }
        size = aligned + job->secondary.size;
    }
    return size;
}

INCLUDE_ASM(const s32, "code_r1_0021CE60", effLeaf346);

INCLUDE_ASM(const s32, "code_r1_0021CE60", effLeaf347);

void fileQueueAppend(plistHandle_t *queue, fileJob_t *job) {
    job->next = NULL;
    if (queue->last != NULL) {
        queue->last->next = job;
        job->prev = queue->last;
    } else {
        queue->first = job;
        job->prev = NULL;
    }
    queue->last = job;
    queue->count++;
}

void fileQueueInsertAfter(plistHandle_t *queue, fileJob_t *after, fileJob_t *job) {
    if (after->next != NULL) {
        after->next->prev = job;
        job->next = after->next;
    } else {
        job->next = NULL;
        queue->last = job;
    }
    after->next = job;
    job->prev = after;
    queue->count++;
}

void fileQueueRemove(plistHandle_t *queue, fileJob_t *job) {
    if (job->prev != NULL) {
        job->prev->next = job->next;
    } else {
        queue->first = job->next;
    }
    if (job->next != NULL) {
        job->next->prev = job->prev;
    } else {
        queue->last = job->prev;
    }
    queue->count--;
}

INCLUDE_ASM(const s32, "code_r1_0021CE60", effCallChipAllocZeroDirect);

INCLUDE_ASM(const s32, "code_r1_0021CE60", effCallChipAllocZeroDirect2);

void fileDestroyJob(fileJob_t *job) {
    sdfChipFreeDirect(job);
}

plistHandle_t *Create_effPartsList(plistHandle_t *source) {
    plistHandle_t *queue = fileQueueCreate();
    fileJob_t *entry;
    fileJob_t *job;
    f32 vec[4] __attribute__((aligned(16)));

    SDF_VU0_COPYVECTOR(queue->pose.posBase, source->pose.posBase);
    SDF_VU0_COPYVECTOR(queue->pose.origin, source->pose.posBase);
    queue->scaleMultiplier = source->scaleMultiplier;
    queue->pose.flag = source->pose.flag;
    if (source->first != NULL) {
        for (entry = source->first; entry != NULL; entry = entry->next) {
            job = fileJobCreate();

            if ((entry->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) == 0) {
                job->id = (u32)Create_effParts((fileJobPayload_t *)entry->id);
            } else {
                fileJob_t *parent = fileQueueFindById(source, entry->id);
                s32 index = fileFindQueuedJobIndex(source, parent);

                parent = fileQueueGetAt(queue, index);
                job->id = (u32)Copy_effParts((fileJobPayload_t *)parent->id);
            }
            fileJobCopyHeader(job, entry);
            strcpy(job->name, entry->name);
            fileQueueAppend(queue, job);
        }
    } else {
        fileJob_t *records;

        entry = (fileJob_t *)((u8 *)source + (u32)source->last);
        records = entry;
        if (source->count > 0) {
            s32 count = source->count;

            do {
                job = fileJobCreate();

                if ((entry->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) == 0) {
                    fileJobPayload_t *request = (fileJobPayload_t *)((u8 *)source + entry->id);

                    if (entry->flags & FILE_JOB_FLAG_SECTOR_FOLLOWER) {
                        fileJobPayload_t *secondary = (fileJobPayload_t *)((u8 *)source + records[entry->sector].id);

                        request->secondary.size = secondary->secondary.size;
                        FILE_JOB_LOCATION_AS_JOB_RELATIVE_OFFSET(request->secondary) =
                            (u32)fileResolveSecondaryBuffer(secondary) - (u32)request;
                        job->id = (u32)Create_effParts(request);
                        FILE_JOB_LOCATION_AS_JOB_RELATIVE_OFFSET(request->secondary) = request->secondary.size = 0;
                    } else {
                        job->id = (u32)Create_effParts(request);
                    }
                } else {
                    fileJob_t *parent = fileQueueGetAt(queue, entry->id);

                    job->id = (u32)Copy_effParts((fileJobPayload_t *)parent->id);
                }
                fileJobCopyHeader(job, entry);
                strcpy(job->name, entry->name);
                fileQueueAppend(queue, job);
                entry++;
            } while (--count != 0);
        }
    }
    VU0_STORE_VF(vf0, vec);
    effPartsPositionSet(queue, vec);
    effPartsRotateSet(queue, vec);
    effPartsScaleSet(queue, 1.0f);
    ColorSet(queue, 0x80808080);
    return queue;
}

void CallShopTest(s32 flg, void *data)
{
    fclShopProcessStart(data);
}

INCLUDE_ASM(const s32, "code_r1_0021CE60", effPartsDraw);

/* A queued job's +0x90 word is a child pointer when flag 1 is clear; it is
 * an ID for other job kinds. Destroy children before their parent jobs. */
void effPartsDelete(plistHandle_t *queue) {
    fileJob_t *job = queue->first;
    while (job != NULL) {
        fileJob_t *next = job->next;
        if ((job->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) == 0) {
            fileJobDestroy((fileJobPayload_t *)job->id);
        }
        fileDestroyJob(job);
        job = next;
    }
    sdfChipFreeDirect(queue);
}

plistHandle_t *effPartsCreateFinish(const plistHandle_t *source) {
    plistHandle_t *queue = fileQueueCreate();
    const fileJob_t *src;
    f32 vec[4] __attribute__((aligned(16)));

    SDF_VU0_COPYVECTOR(queue, source);
    SDF_VU0_COPYVECTOR(queue->pose.origin, source->pose.origin);
    queue->scaleMultiplier = source->scaleMultiplier;
    queue->pose.flag = source->pose.flag;
    for (src = source->first; src != NULL; src = src->next) {
        fileJob_t *job = fileJobCreate();
        job->id = (u32)Copy_effParts((fileJobPayload_t *)src->id);
        fileJobCopyHeader(job, src);
        fileQueueAppend(queue, job);
    }
    VU0_STORE_VF(vf0, vec);
    effPartsPositionSet(queue, vec);
    effPartsRotateSet(queue, vec);
    effPartsScaleSet(queue, 1.0f);
    ColorSet(queue, 0x80808080);
    return queue;
}

INCLUDE_ASM(const s32, "code_r1_0021CE60", fileQueueNotifyAllJobsComplete);

void effPartsPositionSet(plistHandle_t *queue, const f32 vec[4])
{
    f32 rot[16];
    f32 base[4];
    f32 pos[4];
    fileJob_t *job;
    f32 scale;

    SDF_VU0_LOADVECTOR(vec);
    SDF_VU0_STOREVECTOR(queue->pose.pos);
    SDF_VU0_LOADVECTOR_B(queue->pose.posBase);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(base);
    SDF_VU0_LOADVECTOR(queue->pose.rot);
    ToMatrix();
    VU0_STORE_MATRIX(rot);
    scale = queue->pose.scl * queue->scaleMultiplier;
    for (job = queue->first; job != NULL; job = job->next) {
        SDF_VU0_LOADVECTOR(base);
        if (job->xformFlags & 4) {
            VU0_SCALAR_OP(-5.0f, "vaddx.y vf10, vf0, vf2x");
        }
        SDF_VU0_LOADVECTOR_B(job->offset);
        if (job->xformFlags & 0x80) {
            VU0_SCALE_VF(vf11, scale);
        }
        VU0_LOAD_MATRIX(rot);
        VU0_ROTATE_VEC(vf11, vf11);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR_NOMEM(pos);
        PositionSet((fileJobPayload_t *)job->id, pos);
    }
}

void effPartsRotateSet(plistHandle_t *queue, const f32 rot[4])
{
    f32 quat[4];
    f32 pos[4];
    fileJob_t *job;

    SDF_VU0_LOADVECTOR(rot);
    SDF_VU0_STOREVECTOR(queue->pose.rot);
    SDF_VU0_STOREVECTOR(quat);
    ToMatrix();
    SDF_VU0_LOADVECTOR(queue->pose.origin);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR(queue->pose.posBase);
    for (job = queue->first; job != NULL; job = job->next) {
        SDF_VU0_LOADVECTOR(job->quat);
        SDF_VU0_LOADVECTOR_B(quat);
        Mult();
        SDF_VU0_STOREVECTOR_NOMEM(pos);
        fileJobInvokeRotationCallback((fileJobPayload_t *)job->id, pos);
    }
    effPartsPositionSet(queue, queue->pose.pos);
}

void effPartsScaleSet(plistHandle_t *queue, f32 scale)
{
    s128 pos;
    fileJob_t *job;
    f32 total;
    f32 jobScale;

    queue->pose.scl = scale;
    total = scale * queue->scaleMultiplier;
    for (job = queue->first; job != NULL; job = job->next) {
        jobScale = job->scale;
        if (job->updateFlags & FILE_JOB_UPDATE_FLAG_APPLY_QUEUE_SCALE) {
            jobScale = jobScale * total;
        }
        fileJobInvokeScaleCallback((fileJobPayload_t *)job->id, jobScale);
        if (job->xformFlags & 0x80) {
            SDF_VU0_LOADVECTOR(queue->pose.pos);
            SDF_VU0_LOADVECTOR_B(queue->pose.posBase);
            SDF_VU0_ADDVECTOR();
            if (job->xformFlags & 4) {
                VU0_SCALAR_OP(-5.0f, "vaddx.y vf10, vf0, vf2x");
            }
            SDF_VU0_LOADVECTOR_B(job->offset);
            VU0_SCALAR_OP(total, "vmulx.xyzw vf11, vf11, vf2x");
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR_NOMEM(&pos);
            PositionSet((fileJobPayload_t *)job->id, (f32 *)&pos);
        }
    }
}

/* Store the queue's modulation colour, then send every job its own colour
 * modulated by it ((job / 128) * (queue / 128) * 128 per channel). The tint
 * store is the unclobbered SDK spelling: as a bare volatile asm it flushes the
 * CSE table, so the loop's 1/128 is rematerialized rather than copied from the
 * first unpack's FPR. Reading the payload before the packed store keeps the
 * store from feeding the argument load. */
void ColorSet(plistHandle_t *queue, u32 color) {
    f32 tint[4] __attribute__((aligned(16)));
    u32 queueColor[4] __attribute__((aligned(16)));
    u32 jobColor[4] __attribute__((aligned(16)));
    u32 packed[4] __attribute__((aligned(16)));
    fileJob_t *job;

    queue->pose.abgr = color;
    queueColor[0] = color;
    EE_MMI_RGBA_UNPACK(queueColor, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(tint);
    for (job = queue->first; job != NULL; job = job->next) {
        u32 result;
        fileJobPayload_t *payload;

        jobColor[0] = job->color;
        EE_MMI_RGBA_UNPACK(jobColor, 1.0f / 128.0f);
        SDF_VU0_LOADVECTOR_B(tint);
        SDF_VU0_MULVECTOR();
        EE_MMI_RGBA_PACK_UNIT(result, 0x43000000);
        payload = (fileJobPayload_t *)job->id;
        packed[0] = result;
        fileDispatchJobTypeCallback(payload, result);
    }
}

void effPartsRotateMatrixSet(plistHandle_t *queue, f32 matrix[4][4]) {
    f32 vec[4] __attribute__((aligned(16)));
    sdfVuMatrixToQuaternion(matrix);
    SDF_VU0_STOREVECTOR(vec);
    effPartsRotateSet(queue, vec);
}

fileJob_t *fileAppendJob(plistHandle_t *queue, u32 id) {
    fileJob_t *job = fileJobCreate();

    job->id = id;
    fileQueueAppend(queue, job);
    return job;
}

fileJob_t *fileDuplicateAndAppendJob(plistHandle_t *queue, fileJobPayload_t *source) {
    fileJobPayload_t *job = fileDuplicateJob(source);
    return fileAppendJob(queue, (u32)job);
}

fileJob_t *fileAppendJobFromCommandPath(plistHandle_t *queue, const char *commandPath) {
    printf(fileAppendJobFromCommandPathData);
    return fileAppendJob(queue, (u32)fileJobCreateFromCommandState(commandPath));
}

fileJob_t *fileJobDuplicateAfter(plistHandle_t *queue, fileJob_t *src) {
    fileJob_t *job = fileJobCreate();

    memcpy(job, src, 0xC0);
    fileJobResetAndInitTransform(job);
    job->flags |= FILE_JOB_FLAG_SHARED_PAYLOAD;
    job->id = src->id;
    fileQueueInsertAfter(queue, src, job);
    return job;
}

void fileQueueLinkJobToSectorLeader(plistHandle_t *queue, fileJob_t *job, fileJob_t *ref) {
    fileJob_t *node;
    fileJob_t *found;
    fileJobPayload_t *source;
    u32 refIndex;
    u32 flags;

    if (!(job->flags & FILE_JOB_FLAG_SHARED_PAYLOAD)) {
        for (node = queue->first; node != NULL; node = node->next) {
            if ((node->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) && node->id == job->id) {
                fileQueueLinkJobToSectorLeader(queue, node, ref);
            }
        }
    }
    if (job == ref) {
        return;
    }
    flags = job->flags;
    if (!(flags & FILE_JOB_FLAG_SHARED_PAYLOAD)) {
        fileJobFreeSecondaryBuffer((fileJobPayload_t *)job->id);
        source = (fileJobPayload_t *)ref->id;
        fileJobSetSecondaryData((fileJobPayload_t *)job->id,
                                (void *)FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(source->secondary),
                                source->secondary.size,
                                source->primary.selector);
        flags = job->flags;
    }
    job->flags = flags | FILE_JOB_FLAG_SECTOR_FOLLOWER;
    job->sector = ref->id;
    refIndex = fileFindQueuedJobIndex(queue, ref);
    if (fileFindQueuedJobIndex(queue, job) < refIndex) {
        found = fileQueueFindBySector(queue, ref->id);
        if (found != NULL) {
            fileQueueRechainSectorFollowers(queue, ref, found);
            fileQueueLinkJobToSectorLeader(queue, ref, found);
        }
    }
}

void fileQueueDetachSectorFollower(plistHandle_t *queue, fileJob_t *job) {
    u32 flags = job->flags;
    fileJob_t *first;

    job->flags = flags & ~FILE_JOB_FLAG_SECTOR_FOLLOWER;
    if (!(flags & FILE_JOB_FLAG_SHARED_PAYLOAD)) {
        first = fileQueueFindBySector(queue, job->id);
        if (first != NULL) {
            fileQueueRechainSectorFollowers(queue, job, first);
        }
    }
    job->sector = 0;
}

INCLUDE_ASM(const s32, "code_r1_0021CE60", fileQueueRemoveAndDestroyJob);

INCLUDE_ASM(const s32, "code_r1_0021CE60", fileJobCopyHeader);

/* Export a queue: the queue header, one relocated record per job, then each
 * owning job's 16-byte-aligned payload. Child jobs store their parent's index. */
void fileQueueSaveImage(plistHandle_t *queue, const char *filePath) {
    char path[0xD0];
    fileJobPayload_t payload;
    plistHandle_t header;
    fileJob_t record;
    s32 fd;
    s32 count;
    s32 offset;
    s32 size;
    fileJob_t *job;

    if (sdfHddValid != 0) {
        sprintf(path, fileWriteToPfsData, filePath);
        fd = sceOpen(path, 0x602, 0x1B6);
    } else {
        sprintf(path, fileWriteToPfsData2, sdfFileGetHostRoot(), filePath);
        fd = sceOpen(path, 0x602);
    }
    header = *queue;
    header.updateFrame = 0;
    size = sizeof(plistHandle_t);
    header.entryOffset = size;
    header.first = NULL;
    count = fileQueueCountLinkedJobs(queue);
    sceWrite(fd, &header, sizeof(plistHandle_t));
    offset = count * sizeof(fileJob_t) + sizeof(plistHandle_t);

    for (job = queue->first; job != NULL; job = job->next) {
        record = *job;
        if ((job->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) == 0) {
            if ((job->flags & FILE_JOB_FLAG_SECTOR_FOLLOWER) == 0) {
                size = fileJobSerializedSize((fileJobPayload_t *)job->id);
            } else {
                payload = *(fileJobPayload_t *)job->id;
                FILE_JOB_LOCATION_CLEAR(payload.secondary);
                payload.secondary.size = 0;
                payload.secondary.allocation = NULL;
                size = fileJobSerializedSize(&payload);
            }
            record.id = offset;
        } else {
            size = 0;
            record.id = fileFindQueuedJobIndex(queue, fileQueueFindById(queue, job->id));
        }
        if (job->flags & FILE_JOB_FLAG_SECTOR_FOLLOWER) {
            record.sector = fileFindQueuedJobIndex(queue, fileQueueFindById(queue, job->sector));
        }
        record.next = NULL;
        record.prev = NULL;
        sceWrite(fd, &record, sizeof(fileJob_t));
        offset += (size & 0xF) != 0 ? ((size >> 4) + 1) << 4 : (size >> 4) << 4;
    }

    for (job = queue->first; job != NULL; job = job->next) {
        s32 blocks;
        s32 aligned;
        s32 padding;

        if (job->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) {
            continue;
        }
        if ((job->flags & FILE_JOB_FLAG_SECTOR_FOLLOWER) == 0) {
            size = fileJobSerializedSize((fileJobPayload_t *)job->id);
            fileJobWriteSerializedPayload(fd, (fileJobPayload_t *)job->id);
        } else {
            payload = *(fileJobPayload_t *)job->id;
            FILE_JOB_LOCATION_CLEAR(payload.secondary);
            payload.secondary.size = 0;
            payload.secondary.allocation = NULL;
            size = fileJobSerializedSize(&payload);
            fileJobWriteSerializedPayload(fd, &payload);
        }
        blocks = size >> 4;
        if ((size & 0xF) != 0) {
            aligned = (blocks + 1) << 4;
        } else {
            aligned = blocks << 4;
        }
        padding = aligned - size;
        if (padding > 0) {
            sceWrite(fd, fileQueueImagePaddingBytes, padding);
        }
    }
    sceClose(fd);
    sceSync(fileWriteToPfsData3, 0);
}

/* Save a queue image: the queue header, one relocated record per job, then each
 * owning job's 16-byte-aligned payload. Child jobs store their parent's index. */
void fileQueueSaveVersionedImage(plistHandle_t *queue, const char *filePath) {
    char path[0xD0];
    fileJobPayload_t payload;
    plistHandle_t header;
    fileJob_t record;
    fileQueueImageHeader_t image;
    s32 fd;
    s32 count;
    s32 offset;
    s32 size;
    fileJob_t *job;

    if (sdfHddValid != 0) {
        sprintf(path, fileWriteToPfsData, filePath);
        fd = sceOpen(path, 0x602, 0x1B6);
    } else {
        sprintf(path, fileWriteToPfsData2, sdfFileGetHostRoot(), filePath);
        fd = sceOpen(path, 0x602);
    }
    image.version = 5;
    image.unk4 = 0;
    image.unkC = 1.03f;
    sceWrite(fd, &image, sizeof(image));

    header = *queue;
    header.updateFrame = 0;
    size = sizeof(plistHandle_t);
    header.entryOffset = size;
    header.first = NULL;
    count = fileQueueCountLinkedJobs(queue);
    sceWrite(fd, &header, sizeof(plistHandle_t));
    offset = count * sizeof(fileJob_t) + sizeof(plistHandle_t);

    for (job = queue->first; job != NULL; job = job->next) {
        record = *job;
        if ((job->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) == 0) {
            if ((job->flags & FILE_JOB_FLAG_SECTOR_FOLLOWER) == 0) {
                size = fileJobSerializedSize((fileJobPayload_t *)job->id);
            } else {
                payload = *(fileJobPayload_t *)job->id;
                FILE_JOB_LOCATION_CLEAR(payload.secondary);
                payload.secondary.size = 0;
                payload.secondary.allocation = NULL;
                size = fileJobSerializedSize(&payload);
            }
            record.id = offset;
        } else {
            size = 0;
            record.id = fileFindQueuedJobIndex(queue, fileQueueFindById(queue, job->id));
        }
        if (job->flags & FILE_JOB_FLAG_SECTOR_FOLLOWER) {
            record.sector = fileFindQueuedJobIndex(queue, fileQueueFindById(queue, job->sector));
        }
        record.next = NULL;
        record.prev = NULL;
        sceWrite(fd, &record, sizeof(fileJob_t));
        offset += (size & 0xF) != 0 ? ((size >> 4) + 1) << 4 : (size >> 4) << 4;
    }

    for (job = queue->first; job != NULL; job = job->next) {
        s32 blocks;
        s32 aligned;
        s32 padding;

        if (job->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) {
            continue;
        }
        if ((job->flags & FILE_JOB_FLAG_SECTOR_FOLLOWER) == 0) {
            size = fileJobSerializedSize((fileJobPayload_t *)job->id);
            fileJobWriteSerializedPayload(fd, (fileJobPayload_t *)job->id);
        } else {
            payload = *(fileJobPayload_t *)job->id;
            FILE_JOB_LOCATION_CLEAR(payload.secondary);
            payload.secondary.size = 0;
            payload.secondary.allocation = NULL;
            size = fileJobSerializedSize(&payload);
            fileJobWriteSerializedPayload(fd, &payload);
        }
        blocks = size >> 4;
        if ((size & 0xF) != 0) {
            aligned = (blocks + 1) << 4;
        } else {
            aligned = blocks << 4;
        }
        padding = aligned - size;
        if (padding > 0) {
            sceWrite(fd, fileQueueVersionedImagePaddingBytes, padding);
        }
    }
    sceClose(fd);
    sceSync(fileWriteToPfsData3, 0);
}

plistHandle_t *fileQueueCreateFromCommandState(const char *entry) {
    sdfFileCmdHandle_t *command = sdfFileOpen(entry);
    s32 size;
    struct sdfMemHandle_t *allocation;
    plistHandle_t *image;
    plistHandle_t *queue;
    fileJob_t *copy;
    fileJob_t *src;
    u32 i;

    if (command == NULL) {
        return NULL;
    }
    size = sdfFileLength(command);
    allocation = sdfMemAllocDirect(size);
    image = (plistHandle_t *)sdfMemLock(allocation);
    sdfFileRead(command, image, size);
    sdfFileClose(command);
    queue = fileQueueCreate();
    SDF_VU0_COPYVECTOR(queue->pose.posBase, image->pose.posBase);
    SDF_VU0_COPYVECTOR(queue->pose.origin, image->pose.posBase);
    queue->scaleMultiplier = image->scaleMultiplier;
    queue->pose.flag = image->pose.flag;
    for (i = 0, src = (fileJob_t *)((u8 *)image + image->entryOffset);
         i < image->count; i++, src++) {
        if ((src->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) == 0) {
            copy = fileDuplicateAndAppendJob(queue, (fileJobPayload_t *)((u8 *)image + src->id));
        } else {
            copy = fileJobDuplicateAfter(queue, fileQueueGetAt(queue, src->id));
        }
        if (src->flags & FILE_JOB_FLAG_SECTOR_FOLLOWER) {
            fileQueueLinkJobToSectorLeader(queue, copy, fileQueueGetAt(queue, src->sector));
        }
        fileJobCopyHeader(copy, src);
        strcpy(copy->name, src->name);
        copy->flags = src->flags;
    }
    sdfMemFreeDirect(allocation);
    return queue;
}

fileJob_t *fileQueueFindById(plistHandle_t *queue, u32 id) {
    fileJob_t *job = queue->first;
    while (job != NULL) {
        if (job->id == id) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

fileJob_t *fileQueueFindFlaggedById(plistHandle_t *queue, u32 id) {
    fileJob_t *job = queue->first;
    while (job != NULL) {
        if ((job->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) != 0 && job->id == id) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

fileJob_t *fileQueueFindBySector(plistHandle_t *queue, u32 sector) {
    fileJob_t *job = queue->first;
    while (job != NULL) {
        if ((job->flags & (FILE_JOB_FLAG_SHARED_PAYLOAD | FILE_JOB_FLAG_SECTOR_FOLLOWER)) ==
            FILE_JOB_FLAG_SECTOR_FOLLOWER && job->sector == sector) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

fileJob_t *fileQueueGetAt(plistHandle_t *queue, s32 index) {
    fileJob_t *job = queue->first;
    while (job != NULL) {
        if (index-- == 0) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

s32 fileFindQueuedJobIndex(plistHandle_t *queue, fileJob_t *target) {
    fileJob_t *job = queue->first;
    s32 index = 0;
    while (job != NULL) {
        if (job == target) {
            return index;
        }
        job = job->next;
        index++;
    }
    return 0;
}

s32 fileQueueCountLinkedJobs(plistHandle_t *queue) {
    fileJob_t *job;
    s32 count;

    count = 0;
    for (job = queue->first; job != 0; job = job->next) {
        count = count + 1;
    }
    return count;
}

/* vu0 routine: dst = |pos| * unit(aViewEye - aViewTarget); flag 0x10 flattens y, then puts pos.y back */
void camFollowOffsetVec(effPartsListPose_t *obj, void *dst)
{
    f32 len[4];
    f32 length;
    u32 flags = obj->flag;

    if ((flags & 0x18) == 0) {
        SDF_VU0_COPYVECTOR(dst, obj->pos);
        return;
    }
    SDF_VU0_LOADVECTOR(obj->pos);
    if (obj->flag & 0x10) {
        VU0_SCALAR_OP(0.0f, "vaddx.y vf10, vf0, vf2x");
    }
    SDF_VU0_GETLENGTH(length);
    VEC3_SPLAT(len, length);
    SDF_VU0_LOADVECTOR(aViewEye);
    SDF_VU0_LOADVECTOR_B(aViewTarget);
    SDF_VU0_SUBVECTOR();
    if (obj->flag & 0x10) {
        VU0_SCALAR_OP(0.0f, "vaddx.y vf10, vf0, vf2x");
    }
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(len);
    SDF_VU0_MULVECTOR();
    if (obj->flag & 0x10) {
        VU0_SCALAR_OP(obj->pos[1], "vaddx.y vf10, vf0, vf2x");
    }
    SDF_VU0_STOREVECTOR(dst);
}

/* dst = rotation facing unit(aViewEye - aViewTarget) (pitch and yaw); flag 0x40 composes it onto obj->rot, no flag 0x60 copies obj->rot */
void PoseRotBillboard(effPartsListPose_t *obj, void *dst)
{
    f32 v[4];
    f32 pitch;
    u32 flags = obj->flag;

    if ((flags & 0x60) == 0) {
        SDF_VU0_COPYVECTOR(dst, obj->rot);
        return;
    }
    SDF_VU0_LOADVECTOR(aViewEye);
    SDF_VU0_LOADVECTOR_B(aViewTarget);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(v);
    pitch = 0.0f;
    if ((flags & 0x40) == 0) {
        pitch = -acosf(-v[1]);
    }
    FromEuler(pitch, atan2f(v[0], v[2]), 0.0f);
    if (obj->flag & 0x40) {
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(obj->rot);
        Mult();
    }
    SDF_VU0_STOREVECTOR(dst);
}

effQueuedSurfaceNode_t *fileCreateSurfaceLoaderState(s32 capacity) {
    effQueuedSurfaceNode_t *rec = (effQueuedSurfaceNode_t *)sdfChipAllocZeroDirect(sizeof(effQueuedSurfaceNode_t));
    u32 color = 0x80808080;

    rec->handleCount = capacity;
    rec->active = 1;
    rec->color = color;
    rec->opacity = 1.0f;
    rec->resource = NULL;
    rec->jobs = NULL;
    rec->jobAllocation = 0;
    rec->queues = NULL;
    rec->queueAllocation = 0;
    rec->record = 0;
    return rec;
}

effQueuedSurfaceNode_t *fileCreateGridLoaderRecord(fileKeyBlock_t *hdr) {
    u32 rows = hdr->emissionDuration;
    u32 count = (rows != 0 ? rows : hdr->spawnRate) * (rows != 0 ? hdr->spawnRate : hdr->length);

    return fileCreateSurfaceLoaderState(count <= 0x12C ? count : 0x12C);
}

INCLUDE_ASM(const s32, "code_r1_0021CE60", PartsCreate_effParticle);

void Delete_effParticle(effQueuedSurfaceNode_t *node) {
    u32 count;
    u32 i;

    if (node->resource != NULL) {
        dds3BillboardDelete(node->resource);
    }
    if (node->jobAllocation != 0) {
        count = node->record->count;
        for (i = 0; i < count; i++) {
            fileJobDestroy(node->jobs[i]);
        }
        sdfMemFreeDirect(node->jobAllocation);
    }
    if (node->queueAllocation != 0) {
        count = node->record->count;
        for (i = 0; i < count; i++) {
            effPartsDelete(node->queues[i]);
        }
        sdfMemFreeDirect(node->queueAllocation);
    }
    if (node->resourceHolder != NULL) {
        effCacheTexDelete(node->resourceHolder);
    }
    if (node->record != 0) {
        effPCPHelperBEDGetModel(node->record);
    }
    sdfChipFreeDirect(node);
}

effQueuedSurfaceNode_t *Copy_effParticle(effQueuedSurfaceNode_t *owner) {
    fileKeyBlock_t *source = (fileKeyBlock_t *)owner->record->data1;
    effQueuedSurfaceNode_t *result = fileCreateGridLoaderRecord(source);

    effCreate(result, owner->record->type, source);
    ResourceCopy(result, owner);
    return result;
}

INCLUDE_ASM(const s32, "code_r1_0021CE60", ResourceCopy);

void effCreate(effQueuedSurfaceNode_t *node, u32 entryId, void *resource) {
    if (node->record != 0) {
        effPCPHelperBEDGetModel(node->record);
    }
    node->record = fileAllocateGridRecordSlots((u16)entryId, node->handleCount, resource);
}

void fileLoadObjectOpenNamedDevice(effQueuedSurfaceNode_t *node, u32 resourceId) {
    struct billHandle_t *resource = node->resource;
    if (resource != 0) {
        dds3BillboardDelete(resource);
    }
    resource = dds3BillboardGeneral(resourceId);
    node->resource = resource;
    if (node->record != 0) {
        dds3BillboardSetMode(resource, (s16)((fileKeyBlock_t *)node->record->data0)->alphaTrack.surfaceIndex);
    }
}

void BillboardSet(effQueuedSurfaceNode_t *node, u32 resourceId) {
    struct billHandle_t *resource = node->resource;
    if (resource != 0) {
        dds3BillboardDelete(resource);
    }
    resource = dds3BillboardCreate(0, resourceId);
    node->resource = resource;
    if (node->record != 0) {
        dds3BillboardSetMode(resource, (s16)((fileKeyBlock_t *)node->record->data0)->alphaTrack.surfaceIndex);
    }
}

void BillboardGeneralSet(effQueuedSurfaceNode_t *node, u32 resourceId) {
    struct billHandle_t *resource = node->resource;
    if (resource != 0) {
        dds3BillboardDelete(resource);
    }
    resource = dds3BillboardCreate(1, resourceId);
    node->resource = resource;
    dds3BillboardSetAnimColorAdvMode(resource);
    if (node->record != 0) {
        dds3BillboardSetMode(node->resource, (s16)((fileKeyBlock_t *)node->record->data0)->alphaTrack.surfaceIndex);
    }
}

void fileReplaceEffectSurfaceJobs(effQueuedSurfaceNode_t *node, fileJobPayload_t *job) {
    u32 count = node->record->count;
    u32 i;
    s32 size;

    if (node->jobAllocation != 0) {
        for (i = 0; i < count; i++) {
            fileJobDestroy(node->jobs[i]);
        }
        sdfMemFreeDirect(node->jobAllocation);
        node->jobs = 0;
        node->jobAllocation = 0;
    }
    size = count * 4;
    if (size != 0) {
        node->jobAllocation = sdfMemAllocDirect(size);
        node->jobs = (fileJobPayload_t **)sdfMemLock(node->jobAllocation);
        node->jobs[0] = Create_effParts(job);
        for (i = 1; i < count; i++) {
            node->jobs[i] = Copy_effParts(node->jobs[0]);
        }
    }
}

void PartsListSet(effQueuedSurfaceNode_t *node, plistHandle_t *queue) {
    u32 count = node->record->count;
    u32 i;
    s32 size;

    if (node->queueAllocation != 0) {
        for (i = 0; i < count; i++) {
            effPartsDelete(node->queues[i]);
        }
        sdfMemFreeDirect(node->queueAllocation);
        node->queues = 0;
        node->queueAllocation = 0;
    }
    size = count * 4;
    if (size != 0) {
        node->queueAllocation = sdfMemAllocDirect(size);
        node->queues = sdfMemLock(node->queueAllocation);
        node->queues[0] = Create_effPartsList(queue);
        for (i = 1; i < count; i++) {
            node->queues[i] = effPartsCreateFinish(node->queues[0]);
        }
    }
}

void fileReplaceReferenceHolder(effQueuedSurfaceNode_t *obj, u32 resource) {
    if (obj->resourceHolder != NULL) {
        effCacheTexDelete(obj->resourceHolder);
    }
    obj->resourceHolder = effCacheTexCreate(resource);
}

void fileClearLoadObjectReferences(effQueuedSurfaceNode_t *obj) {
    if (obj->record != 0) {
        fileClearRecordReferences(obj->record);
        return;
    }
}

INCLUDE_ASM(const s32, "code_r1_0021CE60", effLeaf348);

INCLUDE_ASM(const s32, "code_r1_0021CE60", fileDrawLoadObjectSlots);

INCLUDE_ASM(const s32, "code_r1_0021CE60", effCallSRAnimation);

void fileSendLoadObjectRecordVector(effQueuedSurfaceNode_t *obj, const u128 *value) {
    mnuRecordSetVector(obj->record, value);
}

void fileCopyLoadObjectRecordVector(effQueuedSurfaceNode_t *obj, const u128 *value) {
    fileSetRecordSecondVector(obj->record, value);
}

void fileSetRecordWordFour(s32 record, u32 value) {
    *(u32 *)(record + 4) = value;
}

void fileSetLoadObjectScale(effQueuedSurfaceNode_t *obj, f32 scale) {
    obj->opacity = scale;
    dds3DispatchIndexedCallback(obj->record, scale);
}

void fileResetSlotStates(bhvrHandle_t *table) {
    u32 count;
    effPosture_t *slot;
    u32 index;

    count = table->count;
    index = 0;
    slot = table->slots;
    if (count != 0) {
        do {
            index = index + 1;
            FILE_SLOT_FRAME(slot) = FILE_SLOT_FRAME_NONE;
            slot++;
        } while (index < count);
    }
}

/* vu0 routine: preserve the packed RGB snapshot while applying the alpha track. */
u32 effColorAnimation(const effBehavior_RGBParams_t *colorTrack,
                              const effBehavior_AlphaParams_t *alphaTrack,
                              s32 frame, s32 duration) {
    u32 color0;
    u32 color1;
    u32 alpha;
    u32 packedColor;
    u32 unitBits;
    f32 durationFloat;
    f32 factor;
    f32 inverseFactor;
    f32 alphaFactor;
    f32 alphaValue;
    s32 firstFrame;
    s32 secondFrame;
    s32 fadeInFrame;
    s32 fadeOutFrame;

    if (duration == 0) {
        return (colorTrack->initialColor & 0x00FFFFFF) |
               (alphaTrack->alpha << 24);
    }

    durationFloat = (f32)duration;
    switch (colorTrack->segmentMode) {
    case SDF_COLOR_TRACK_MODE_ENDPOINTS:
        factor = (f32)frame / durationFloat;
        color1 = colorTrack->initialColor;
        color0 = colorTrack->finalColor;
        break;
    case SDF_COLOR_TRACK_MODE_ONE_INTERMEDIATE_KEY:
        firstFrame = (s32)(colorTrack->keys[0].fraction * durationFloat);
        if (frame < firstFrame) {
            factor = (f32)frame / firstFrame;
            color1 = colorTrack->initialColor;
            color0 = colorTrack->keys[0].color;
        } else {
            f32 span = duration - firstFrame;
            factor = (f32)(frame - firstFrame) / span;
            color1 = colorTrack->keys[0].color;
            color0 = colorTrack->finalColor;
        }
        break;
    case SDF_COLOR_TRACK_MODE_TWO_INTERMEDIATE_KEYS:
        firstFrame = (s32)(colorTrack->keys[0].fraction * durationFloat);
        if (frame < firstFrame) {
            factor = (f32)frame / firstFrame;
            color1 = colorTrack->initialColor;
            color0 = colorTrack->keys[0].color;
        } else {
            secondFrame = (s32)(colorTrack->keys[1].fraction * durationFloat);
            if (frame < secondFrame) {
                f32 span = secondFrame - firstFrame;
                factor = (f32)(frame - firstFrame) / span;
                color1 = colorTrack->keys[0].color;
                color0 = colorTrack->keys[1].color;
            } else {
                f32 span = duration - secondFrame;
                factor = (f32)(frame - secondFrame) / span;
                color1 = colorTrack->keys[1].color;
                color0 = colorTrack->finalColor;
            }
        }
        break;
    default:
        color1 = colorTrack->initialColor;
        color0 = colorTrack->finalColor;
        factor = 0.0f;
        break;
    }

    unitBits = 0x3C000000;
    eeUnpackRGBAWord(color0, unitBits);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(color1, unitBits);
    inverseFactor = 1.0f - factor;
    VU0_SCALE_VF(vf10, inverseFactor);
    VU0_SCALE_VF(vf11, factor);
    SDF_VU0_ADDVECTOR();
    packedColor = eePackRGBAWord(128.0f);

    fadeInFrame = (s32)(alphaTrack->fadeInFraction * durationFloat);
    fadeOutFrame = (s32)(alphaTrack->fadeOutFraction * durationFloat);
    if (frame < fadeInFrame) {
        alphaFactor = (f32)frame / (f32)fadeInFrame;
    } else if (frame > fadeOutFrame) {
        alphaFactor = (f32)(duration - frame) / (f32)(duration - fadeOutFrame);
    } else {
        alphaFactor = 1.0f;
    }

    alphaValue = (f32)alphaTrack->alpha * alphaFactor;
    alpha = (u32)alphaValue;
    return (packedColor & 0x00FFFFFF) | (alpha << 24);
}

f32 effSampleScalarCurve(const effScalarCurve_t *curve, s32 frame, s32 duration) {
    f32 from, to, factor, durationFloat;
    s32 firstFrame, secondFrame;
    if (duration == 0) {
        return curve->initialValue;
    }
    durationFloat = duration;
    switch (curve->mode) {
    case EFF_SCALAR_CURVE_MODE_ENDPOINTS:
        factor = (f32)frame / durationFloat;
        from = curve->initialValue;
        to = curve->finalValue;
        break;
    case EFF_SCALAR_CURVE_MODE_ONE_INTERMEDIATE_KEY:
        firstFrame = (s32)(curve->keys[0].fraction * durationFloat);
        if (frame < firstFrame) {
            factor = (f32)frame / firstFrame;
            from = curve->initialValue;
            to = curve->keys[0].value;
        } else {
            f32 span = duration - firstFrame;
            factor = (f32)(frame - firstFrame) / span;
            from = curve->keys[0].value;
            to = curve->finalValue;
        }
        break;
    case EFF_SCALAR_CURVE_MODE_TWO_INTERMEDIATE_KEYS:
        firstFrame = (s32)(curve->keys[0].fraction * durationFloat);
        if (frame < firstFrame) {
            factor = (f32)frame / firstFrame;
            from = curve->initialValue;
            to = curve->keys[0].value;
        } else {
            secondFrame = (s32)(curve->keys[1].fraction * durationFloat);
            if (frame < secondFrame) {
                f32 span = secondFrame - firstFrame;
                factor = (f32)(frame - firstFrame) / span;
                from = curve->keys[0].value;
                to = curve->keys[1].value;
            } else {
                f32 span = duration - secondFrame;
                factor = (f32)(frame - secondFrame) / span;
                from = curve->keys[1].value;
                to = curve->finalValue;
            }
        }
        break;
    default:
        from = curve->initialValue;
        to = curve->finalValue;
        factor = 0.0f;
        break;
    }
    return from + (to - from) * factor;
}

INCLUDE_ASM(const s32, "code_r1_0021CE60", fileSampleKeyTracks);

INCLUDE_ASM(const s32, "code_r1_0021CE60", effCallColorAnimation);

void fileInvalidateSlotGroup(bhvrHandle_t *table, effPosture_t *slot) {
    fileKeyBlock_t *grid = (fileKeyBlock_t *)table->data0;
    s32 rows = grid->rows;
    s32 columns = grid->columns;
    s32 n = columns * rows;
    u32 index;
    effPosture_t *groupSlot;
    s32 i;

    if (n != 0) {
        index = ((u32)slot - (u32)table->slots) >> 5;
        groupSlot = table->slots + (table->instances + index * n);
        for (i = 0; i < n; i++) {
            FILE_SLOT_FRAME(groupSlot) = FILE_SLOT_FRAME_NONE;
            groupSlot++;
        }
    }
}

void fileCopyAndInvalidateSlotGroup(bhvrHandle_t *table, effPosture_t *source) {
    fileKeyBlock_t *grid = (fileKeyBlock_t *)table->data0;
    s32 rows = grid->rows;
    s32 columns = grid->columns;
    s32 n = columns * rows;
    u32 index;
    effPosture_t *slot;

    if (n != 0) {
        index = ((u32)source - (u32)table->slots) >> 5;
        slot = table->slots + (table->instances + index * n);
        *slot = *source;
        FILE_SLOT_FRAME(slot) = FILE_SLOT_FRAME_NONE;
    }
}

/* vu0 routine: shifts one slot's history and interpolates its trailing samples. */
void effUpdateGridRecordSlotHistory(bhvrHandle_t *record, effPosture_t *slot) {
    f32 points[4][4];
    f32 color[4];
    f32 colorStep[4];
    u32 curveStartColor[4];
    u32 curveEndColor[4];
    u32 curveResultColor[4];
    u32 linearStartColor[4];
    u32 linearEndColor[4];
    u32 linearResultColor[4];
    fileKeyBlock_t *dims = (fileKeyBlock_t *)record->data0;
    u32 rows = (u32)dims->rows;
    u32 columns = (u32)dims->columns;
    u32 count = columns * rows;
    u32 remaining;
    u32 i;
    u32 historyIndex;
    s32 frame;
    effPosture_t *group;
    effPosture_t *shiftTarget;
    effPosture_t *cursor;
    f32 fraction;
    f32 step;
    f32 alpha;
    f32 scale;
    f32 angle;
    f32 scaleDelta;
    f32 angleDelta;

    if (count != 0) {
        u8 *base = (u8 *)record->slots;
        frame = FILE_SLOT_FRAME(slot);
        group = (effPosture_t *)(base +
            ((record->instances + ((u32)((u8 *)slot - base) >> 5) * count) << 5));
        if (frame == 0) {
            *group = *slot;
            cursor = group;
            for (historyIndex = 0; historyIndex < count; historyIndex++) {
                FILE_SLOT_FRAME(cursor) = FILE_SLOT_FRAME_NONE;
                cursor++;
            }
        } else {
            FILE_SLOT_FRAME(group) = (s32)((u32)frame - 1u);
            step = 1.0f / (f32)count;
            fraction = 0.0f;
            alpha = (f32)(slot->color >> 24);
            remaining = count - rows;
            shiftTarget = group + count - 1;
            cursor = shiftTarget - rows;
            for (historyIndex = 0; historyIndex < remaining; historyIndex++) {
                u32 fadedAlpha;
                *shiftTarget = *cursor;
                fadedAlpha = (u32)(alpha * fraction);
                shiftTarget->color = (cursor->color & 0xFFFFFF) |
                    (fadedAlpha << 24);
                fraction += step;
                cursor--;
                shiftTarget--;
            }
            *group = *slot;
            FILE_SLOT_FRAME(group) = FILE_SLOT_FRAME_NONE;
            if (rows != 1 && columns >= 2) {
                if (frame >= 2 && columns >= 3) {
                    if (FILE_SLOT_FRAME(&group[rows * 2]) >= 0) {
                        effPosture_t *previous;
                        SDF_VU0_COPYVECTOR(points[0], group[rows * 2].pos);
                        previous = group + rows;
                        SDF_VU0_COPYVECTOR(points[1], previous->pos);
                        SDF_VU0_COPYVECTOR(points[2], group->pos);
                        SDF_VU0_COPYVECTOR(points[3], group->pos);
                        fraction = 0.0f;
                        cursor = previous - 1;
                        step = 1.0f / (f32)(rows + 1);
                        scale = previous->scale;
                        scaleDelta = group->scale - scale;
                        angle = previous->angle;
                        angleDelta = group->angle - angle;
                        curveStartColor[0] = group->color;
                        EE_MMI_RGBA_UNPACK(curveStartColor, 0.0078125f);
                        SDF_VU0_COPYVECTOR_A_TO_B();
                        curveEndColor[0] = previous->color;
                        EE_MMI_RGBA_UNPACK(curveEndColor, 0.0078125f);
                        SDF_VU0_STOREVECTOR_NOMEM(color);
                        SDF_VU0_SUBVECTOR_B_A();
                        SDF_VU0_SETSCALE1VECTOR();
                        VU0_SCALE_VF(vf10, step);
                        SDF_VU0_MULVECTOR();
                        SDF_VU0_STOREVECTOR_NOMEM(colorStep);
                        for (i = 0; i < rows - 1; i++) {
                            fraction += step;
                            vuCatmullRomPoint(points, fraction);
                            SDF_VU0_STOREVECTOR_NOMEM(cursor->pos);
                            cursor->scale = scale + scaleDelta * fraction;
                            cursor->angle = angle + angleDelta * fraction;
                            SDF_VU0_LOADVECTOR(color);
                            SDF_VU0_LOADVECTOR_B(colorStep);
                            VU0_ADD_EXTENDED(vf10, vf10, vf11);
                            SDF_VU0_STOREVECTOR_NOMEM(color);
                            EE_MMI_RGBA_PACK_UNIT(curveResultColor[0], 128.0f);
                            cursor->color = curveResultColor[0];
                            FILE_SLOT_FRAME(cursor) = frame;
                            cursor--;
                        }
                    }
                } else if (frame > 0) {
                    if (FILE_SLOT_FRAME(&group[rows]) >= 0) {
                        effPosture_t *previous = group + rows;
                        shiftTarget = previous;
                        fraction = 0.0f;
                        cursor = previous - 1;
                        step = 1.0f / (f32)(rows + 1);
                        scale = previous->scale;
                        scaleDelta = group->scale - scale;
                        angle = previous->angle;
                        angleDelta = group->angle - angle;
                        linearStartColor[0] = group->color;
                        EE_MMI_RGBA_UNPACK(linearStartColor, 0.0078125f);
                        SDF_VU0_COPYVECTOR_A_TO_B();
                        linearEndColor[0] = previous->color;
                        EE_MMI_RGBA_UNPACK(linearEndColor, 0.0078125f);
                        SDF_VU0_STOREVECTOR_NOMEM(color);
                        SDF_VU0_SUBVECTOR_B_A();
                        SDF_VU0_SETSCALE1VECTOR();
                        VU0_SCALE_VF(vf10, step);
                        SDF_VU0_MULVECTOR();
                        SDF_VU0_STOREVECTOR_NOMEM(colorStep);
                        {
                            u32 *__restrict packedOutput = linearResultColor;

                            for (i = 0; i < rows - 1; i++) {
                                fraction += step;
                                SDF_VU0_LOADVECTOR(shiftTarget->pos);
                                SDF_VU0_LOADVECTOR_B(group->pos);
                                VU0_LERP_VF10(fraction);
                                SDF_VU0_STOREVECTOR_NOMEM(cursor->pos);
                                cursor->scale = scale + scaleDelta * fraction;
                                cursor->angle = angle + angleDelta * fraction;
                                SDF_VU0_LOADVECTOR(color);
                                SDF_VU0_LOADVECTOR_B(colorStep);
                                VU0_ADD_EXTENDED(vf10, vf10, vf11);
                                SDF_VU0_STOREVECTOR_NOMEM(color);
                                {
                                    u8 *__restrict slotBytes;

                                    EE_MMI_RGBA_PACK_UNIT(packedOutput[0], 128.0f);
                                    slotBytes = (u8 *)cursor;
                                    *(u32 *)(slotBytes + (u32)&((effPosture_t *)0)->color) = packedOutput[0];
                                    *(s32 *)(slotBytes + (u32)&FILE_SLOT_FRAME((effPosture_t *)0)) = frame;
                                }
                                cursor--;
                            }
                        }
                    }
                }
            }
        }
    }
}

/* NON_MATCHING: 542 of 546 words match. Arg register order of the fileSampleKeyTracks call; body from DDS C. */
#ifdef NON_MATCHING
extern u8 effSharedRandomState[];
extern f32 effWaveEmitterNegativeYAxis[4];
#define FILE_SLOT_FRAME_DISABLED (-2)

void effUpdateWaveEmitterSlots(bhvrHandle_t *record) {
    f32 direction[4];
    f32 radiusVector[4];
    f32 previousPosition[4];
    fileKeyBlock_t *keys;
    u32 i;
    u32 count;
    u32 flags;
    effPosture_t *slot;
    fileWaveMotion_t *motion;
    s32 duration;
    s32 prewarm;
    s32 mode;
    f32 speedDecrease;
    s32 toSpawn;
    s32 prewarmLength;
    s32 frame;
    f32 distance;
    f32 sine;
    f32 sway;
    s32 length;

    keys = (fileKeyBlock_t *)record->data0;
    count = record->instances;
    length = keys->length;
    flags = record->flags;
    slot = record->slots;
    motion = (fileWaveMotion_t *)record->instanceWork;
    if (length != 0) {
        duration = keys->emissionDuration;
        prewarmLength = keys->length;
        mode = keys->heading.track.curve.headingMode;
        speedDecrease = keys->emitter.wave.axialDeceleration;
        radiusVector[3] = 0.0f;
        SDF_VU0_LOADVECTOR(keys->orientation);
        ToMatrix();
        if (duration != 0 && (s32)record->references >= duration) {
            toSpawn = 0;
            prewarm = 0;
        } else {
            if (record->references == 0 && keys->prewarm != 0) {
                prewarm = 1;
                if (!(keys->spawnVariance > 0.0f)) {
                    toSpawn = record->instances;
                } else {
                    toSpawn = (s32)((f32)record->instances *
                        (dds3GetRandFloatA(effSharedRandomState) * (1.0999999046325684f - keys->spawnVariance)));
                }
            } else {
                prewarm = 0;
                if (keys->spawnVariance > 0.0f) {
                    record->spawnRemainder += (f32)keys->spawnRate *
                        (dds3GetRandFloatA(effSharedRandomState) * (1.0999999046325684f - keys->spawnVariance));
                } else {
                    record->spawnRemainder += (f32)keys->spawnRate;
                }
                toSpawn = (s32)fabsf(record->spawnRemainder);
                record->spawnRemainder -= (f32)toSpawn;
            }
        }
        i = 0;
        if (count != 0) {
            do {
                if (FILE_SLOT_FRAME(slot) >= length) {
                    FILE_SLOT_FRAME(slot) = duration != 0 ? FILE_SLOT_FRAME_DISABLED : FILE_SLOT_FRAME_NONE;
                    fileInvalidateSlotGroup((bhvrHandle_t *)record, slot);
                }
                frame = FILE_SLOT_FRAME(slot);
                if (frame != -2) {
                    if (frame == -1) {
                        if (toSpawn != 0) {
                            s32 localSpace = flags & 1;
                            f32 initialAmplitude;
                            f32 finalAmplitude;
                            f32 radius;

                            if (localSpace != 0) {
                                motion->axis[0] = 0.0f;
                                motion->axis[1] = -1.0f;
                                motion->axis[2] = 0.0f;
                            } else {
                                SDF_VU0_LOADVECTOR(effWaveEmitterNegativeYAxis);
                                SDF_VU0_APPLYMATRIX_XYZ();
                                SDF_VU0_STOREVECTOR(direction);
                                motion->axis[0] = direction[0];
                                motion->axis[1] = direction[1];
                                motion->axis[2] = direction[2];
                            }
                            direction[0] = (dds3GetRandFloatA(effSharedRandomState) - 0.5f) * 2.0f;
                            direction[1] = 0.0f;
                            direction[2] = (dds3GetRandFloatA(effSharedRandomState) - 0.5f) * 2.0f;
                            direction[3] = 0.0f;
                            SDF_VU0_LOADVECTOR(direction);
                            SDF_VU0_NORMALIZE();
                            if (localSpace == 0) {
                                SDF_VU0_APPLYMATRIX_XYZ();
                            }
                            SDF_VU0_STOREVECTOR(direction);
                            motion->swayDirection[0] = direction[0];
                            motion->swayDirection[1] = direction[1];
                            motion->swayDirection[2] = direction[2];
                            motion->speed = keys->emitter.wave.axialSpeed *
                                (dds3GetRandFloatA(effSharedRandomState) * keys->emitter.wave.axialSpeedRandomness +
                                 (1.0f - keys->emitter.wave.axialSpeedRandomness));
                            initialAmplitude = keys->emitter.wave.initialAmplitude *
                                (dds3GetRandFloatA(effSharedRandomState) * keys->emitter.wave.initialAmplitudeRandomness +
                                 (1.0f - keys->emitter.wave.initialAmplitudeRandomness));
                            finalAmplitude = keys->emitter.wave.finalAmplitude *
                                (dds3GetRandFloatA(effSharedRandomState) * keys->emitter.wave.finalAmplitudeRandomness +
                                 (1.0f - keys->emitter.wave.finalAmplitudeRandomness));
                            motion->amplitude = initialAmplitude;
                            motion->amplitudeStep = (finalAmplitude - initialAmplitude) / (f32)length;
                            motion->phaseStep = keys->emitter.wave.phaseStep *
                                (dds3GetRandFloatA(effSharedRandomState) * keys->emitter.wave.phaseStepRandomness +
                                 (1.0f - keys->emitter.wave.phaseStepRandomness));
                            motion->phase = dds3GetRandFloatA(effSharedRandomState) * 6.2831850051879883f;
                            motion->previousSine = sdfSin(motion->phase);
                            radius = keys->emitter.wave.initialRadius *
                                ((dds3GetRandFloatA(effSharedRandomState) - 0.5f) * 2.0f);
                            VEC3_SPLAT(radiusVector, radius);
                            if (localSpace != 0) {
                                SDF_VU0_LOADVECTOR(direction);
                                SDF_VU0_LOADVECTOR_B(radiusVector);
                                SDF_VU0_MULVECTOR();
                            } else {
                                SDF_VU0_LOADVECTOR(direction);
                                SDF_VU0_LOADVECTOR_B(radiusVector);
                                SDF_VU0_MULVECTOR();
                                SDF_VU0_LOADVECTOR_B(keys->pos);
                                SDF_VU0_ADDVECTOR();
                            }
                            SDF_VU0_STOREVECTOR(slot->pos);
                            motion->scaleMultiplier = dds3GetRandFloatA(effSharedRandomState) * keys->scale.emitter.randomness +
                                (1.0f - keys->scale.emitter.randomness);
                            if (mode != 2) {
                                motion->angleMultiplier = dds3GetRandFloatA(effSharedRandomState) * keys->heading.emitter.randomness +
                                    (1.0f - keys->heading.emitter.randomness);
                                if (mode == 1) {
                                    motion->angle = dds3GetRandFloatA(effSharedRandomState) * 6.2831850051879883f;
                                    if (dds3GetRandA(effSharedRandomState) & 1) {
                                        motion->angleMultiplier = -motion->angleMultiplier;
                                    }
                                } else {
                                    motion->angle = 0.0f;
                                }
                            } else {
                                motion->angleMultiplier = 1.0f;
                                motion->angle = 0.0f;
                            }
                            FILE_SLOT_FRAME(slot) = 0;
                            SDF_VU0_COPYVECTOR(previousPosition, slot->pos);
                            if (prewarm != 0) {
                                f32 age = (f32)(dds3GetRandA(effSharedRandomState) % prewarmLength);
                                distance = motion->speed * age;
                                distance -= speedDecrease * age * age * 0.5f;
                                slot->pos[0] += motion->axis[0] * distance;
                                slot->pos[1] += motion->axis[1] * distance;
                                slot->pos[2] += motion->axis[2] * distance;
                                motion->phase += motion->phaseStep * age;
                                motion->amplitude += motion->amplitudeStep * age;
                                sine = sdfSin(motion->phase);
                                sway = motion->amplitude * sine;
                                FILE_SLOT_FRAME(slot) = (s32)age;
                                slot->pos[0] += motion->swayDirection[0] * sway;
                                slot->pos[1] += motion->swayDirection[1] * sway;
                                slot->pos[2] += motion->swayDirection[2] * sway;
                                motion->previousSine = sine;
                                motion->phase += motion->phaseStep;
                                motion->amplitude += motion->amplitudeStep;
                            }
                            fileSampleKeyTracks(slot, keys, FILE_SLOT_FRAME(slot), previousPosition);
                            slot->scale *= motion->scaleMultiplier;
                            slot->angle *= motion->angleMultiplier;
                            slot->angle += motion->angle;
                            if (prewarm != 0) {
                                fileCopyAndInvalidateSlotGroup((bhvrHandle_t *)record, slot);
                                FILE_SLOT_FRAME(slot)++;
                            }
                            toSpawn--;
                        }
                    } else {
                        SDF_VU0_COPYVECTOR(previousPosition, slot->pos);
                        sine = sdfSin(motion->phase);
                        sway = motion->amplitude * (sine - motion->previousSine);
                        slot->pos[0] += motion->swayDirection[0] * sway;
                        slot->pos[1] += motion->swayDirection[1] * sway;
                        slot->pos[2] += motion->swayDirection[2] * sway;
                        distance = motion->speed - speedDecrease * (f32)frame;
                        slot->pos[0] += motion->axis[0] * distance;
                        slot->pos[1] += motion->axis[1] * distance;
                        slot->pos[2] += motion->axis[2] * distance;
                        motion->previousSine = sine;
                        motion->phase += motion->phaseStep;
                        motion->amplitude += motion->amplitudeStep;
                        fileSampleKeyTracks(slot, keys, frame, previousPosition);
                        slot->scale *= motion->scaleMultiplier;
                        slot->angle *= motion->angleMultiplier;
                        slot->angle += motion->angle;
                        effUpdateGridRecordSlotHistory(record, slot);
                        FILE_SLOT_FRAME(slot) = frame + 1;
                    }
                }
                motion++;
                slot++;
                i++;
            } while (i < count);
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_0021CE60", effUpdateWaveEmitterSlots);
#endif

/* Scale selected wave-emitter parameters from source to destination. */
void effScaleWaveEmitterParameters(effScaleOwner_t *owner, f32 scale) {
    effScaleSet_t *src = owner->src;
    effScaleSet_t *dst = owner->dst;
    u32 i;

    dst->unk64 = src->unk64 * scale;
    dst->unk68 = src->unk68 * scale;
    for (i = 0; i < 3; i++) {
        dst->entries[i].value = src->entries[i].value * scale;
    }
    dst->unkC8 = src->unkC8 * scale;
    dst->unkCC = src->unkCC * scale;
    dst->unkD4 = src->unkD4 * scale;
    dst->unkD8 = src->unkD8 * scale;
    dst->unkE0 = src->unkE0 * scale;
}

INCLUDE_ASM(const s32, "code_r1_0021CE60", effUpdateRadialEmitterSlots);

INCLUDE_ASM(const s32, "code_r1_0021CE60", effScaleRadialEmitterParameters);

INCLUDE_ASM(const s32, "code_r1_0021CE60", effUpdateCircularEmitterSlots);

INCLUDE_ASM(const s32, "code_r1_0021CE60", effScaleCircularEmitterParameters);

INCLUDE_ASM(const s32, "code_r1_0021CE60", effUpdateOrientedRingEmitterSlots);

INCLUDE_ASM(const s32, "code_r1_0021CE60", effScaleOrientedRingEmitterParameters);

INCLUDE_ASM(const s32, "code_r1_0021CE60", effUpdateDirectedEmitterSlots);

INCLUDE_ASM(const s32, "code_r1_0021CE60", effScaleDirectedEmitterParameters);

INCLUDE_ASM(const s32, "code_r1_0021CE60", effUpdateSectorEmitterSlots);

INCLUDE_ASM(const s32, "code_r1_0021CE60", effLeaf349);

/* NON_MATCHING: 3 of 86 words match. Register allocation and scheduling differ; retail keeps type masked in s7 and the key loads before the descriptor loads. */
#ifdef NON_MATCHING
bhvrHandle_t *fileAllocateGridRecordSlots(u16 type, u32 count, const void *configuration)
{
    const fileKeyBlock_t *key = configuration;
    effEmitterType_t *emitter;
    u32 dataBytes;
    u32 total;
    struct sdfMemHandle_t *allocation;
    bhvrHandle_t *table;
    u8 *data;
    u8 *work;

    emitter = &effEmitterTypeDescriptors[type];
    total = count * key->columns * key->rows + count;
    dataBytes = emitter->dataBytes;
    allocation = sdfMemAllocDirect(total * sizeof(effPosture_t) + FILE_SLOT_TABLE_HEADER_BYTES + emitter->slotBytes * count + dataBytes * 2);
    table = sdfMemLock(allocation);
    table->type = type;
    table->slots = (effPosture_t *)((u8 *)table + FILE_SLOT_TABLE_HEADER_BYTES);
    data = (u8 *)(table->slots + total);
    table->data0 = data;
    work = data + dataBytes;
    table->data1 = work;
    table->instanceWork = work + dataBytes;
    table->instances = count;
    table->count = total;
    table->allocation = allocation;
    table->flags = 0;
    table->references = 0;
    table->spawnRemainder = 0.0f;
    memcpy(table->data0, configuration, dataBytes);
    memcpy(table->data1, configuration, dataBytes);
    VU0_STORE_VF(vf0, table->data0);
    VU0_STORE_VF(vf0, table->data0 + 16);
    if (((fileKeyBlock_t *)table->data0)->padBC != 0) {
        table->flags |= 1;
    }
    fileResetSlotStates(table);
    return table;
}
#else
INCLUDE_ASM(const s32, "code_r1_0021CE60", fileAllocateGridRecordSlots);
#endif
INCLUDE_SDATA(const s32, "code_r1_0021CE60", fileWriteToPfsData);

INCLUDE_SDATA(const s32, "code_r1_0021CE60", fileWriteToPfsData2);

INCLUDE_SDATA(const s32, "code_r1_0021CE60", fileWriteToPfsData3);

INCLUDE_SDATA(const s32, "code_r1_0021CE60", fileAppendJobFromCommandPathData);

