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

extern void fclShopProcessStart(void *pPlace);

#include "sdk_libc.h"

void CallShopTest(s32 flg, void *data);

#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "fileManager.h"
#include "newdata.h"
#include "sdk_libkernl.h"
#include "dds/btl.h"
#include "dds/sdf_dev_state.h"

/* Convert three Euler angle components to the VU0 quaternion output.
 * Providers: DDS1 game/code_002E7C20.c and DDS2 game/code_00340AC8.c. */

/* Multiply the quaternions in vf10 and vf11, writing the product to vf10. */

/* Convert the quaternion in vf10 into the primary matrix bank vf28-vf31.
 * Providers: DDS1 game/code_002E7C20.c and DDS2 game/code_00340AC8.c. */
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

#include "dds/eff_surface_node.h"
#include "dds/file_job_type.h"
#include "dds/eff_emitter_type.h"
#include "dds/cam_aim_record.h"
#include "dds/eff_scale_params.h"
#include "dds/ee_mmi.h"
#include "dds/file_selection_api.h"
#include "dds/file_save_record.h"
#include "dds/mnu_list.h"
#include "dds/pcp_vu0.h"
#include "dds/eff_expanded_list.h"

typedef s32 (*FileMenuContinuationCallback)(void);

struct EffectSlotSet;

enum { FILE_CONFIG_SET, FILE_CONFIG_FRAME, FILE_CONFIG_X, FILE_CONFIG_Y };

typedef struct MenuWork {
    FileSavePreviewRecord header;
    s8 applyLoadResult;
    s8 hardInitSelected;
    u16 flowConditionMask;
    u16 startBranchFlag; /* 0x34: selects one of two initial menu flows */
    u16 flowRowCursor;
    u16 unk38;
    u16 unk3A;
    struct sdfMemHandle_t *unk3C;
} MenuWork;

typedef struct MdlFlagPair {
    s32 flag;
    s32 unk4;
} MdlFlagPair;

/* Eight-byte story-flag rows select a unit and a six-byte bonus record. */
typedef struct FileStatBonus {
    s32 flag;
    u16 unitId;
    u8 bonusIndex;
    u8 pad07;
} FileStatBonus;

#include "dds/fpu.h"

struct MenuListNode;

struct sdfMemHandle_t;

extern u32 D_00437D18;
extern u32 D_00437D1C;
extern u32 D_00437D20;
extern struct EffExpandedList *effCacheTexCreate(u32);
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
typedef struct EffDispatchExtra {
    void (*func)(void *, f32 vector[4]); /* 0x00 vector handler, may be NULL */
    u8 pad04[0x24];
} EffDispatchExtra;

typedef struct EffDispatchScale {
    void (*func)(void *, f32); /* 0x00 handler, may be NULL */
    u8 pad04[0x24];
} EffDispatchScale;

typedef struct EffDispatchColor {
    void (*func)(void *, u32); /* 0x00 handler, may be NULL */
    u8 pad04[0x24];
} EffDispatchColor;

typedef struct LoadMirror {
    u32 current;
    u32 previous;
} LoadMirror;

extern void effCreate(EffectQueuedSurfaceNode *node, u32 entryId, void *resource);
extern void BillboardSet(EffectQueuedSurfaceNode *node, u32 resourceId);
extern void BillboardGeneralSet(EffectQueuedSurfaceNode *node, u32 resourceId);
extern void fileLoadObjectOpenNamedDevice(EffectQueuedSurfaceNode *node, u32 resourceId);
extern void fileReplaceEffectSurfaceJobs(EffectQueuedSurfaceNode *node, FileJobPayload *job);
extern void PartsListSet(EffectQueuedSurfaceNode *node, FileQueue *queue);
extern void fileReplaceReferenceHolder(EffectQueuedSurfaceNode *node, u32 resource);
extern void fileJobFreePrimaryBuffer(FileJobPayload *job);
extern void fileJobFreeSecondaryBuffer(FileJobPayload *job);
extern void fileQueueAppend(FileQueue *queue, FileJob *job);
extern FileJob *fileJobCreate(void);
extern FileJob *fileQueueFindById(FileQueue *queue, u32 id);
extern s32 fileFindQueuedJobIndex(FileQueue *queue, FileJob *target);
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
typedef struct Init374E0 {
    s8 mode[3];      /* 0x00 */
    u8 pad3;         /* 0x03 */
    s32 pos[3][2];   /* 0x04 */
    s32 counter[3];  /* 0x1C */
    u8 pad28[0xC];   /* 0x28 */
    s32 alpha[3];    /* 0x34 */
} Init374E0;

typedef struct FileFlowEntry {
    u16 state;
    u16 mask;
} FileFlowEntry;

typedef struct FileMultiFlagBonus {
    s32 flags[4];
    u8 bonusIndex;
    u8 pad11[3];
} FileMultiFlagBonus;

extern u8 aViewEye[];

/* vu0 routine: point at t between p[1] and p[2] of a Catmull-Rom (Hermite, 0.5 tangents) spline, left in vf10 */
void vuCatmullRomPoint(f32 (*p)[4], f32 t);
extern u8 aViewTarget[];

/* The quaternion result is returned in vf10 for the VU transform routines. */
void sdfVuMatrixToQuaternion(const f32 matrix[4][4]);

#define FILE_JOB_LOCATION_AS_JOB_RELATIVE_OFFSET(payload) ((payload).offset)

#define FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(payload) ((payload).offset)

#define FILE_JOB_LOCATION_CLEAR(payload) ((payload).offset = 0)

void fileJobFreePrimaryBuffer(FileJobPayload *job);
void fileJobFreeSecondaryBuffer(FileJobPayload *job);
void fileJobNotifyPair(FileJobPayload *left, FileJobPayload *right);
u32 fileJobSerializedSize(FileJobPayload *job);
void fileJobResetAndInitTransform(FileJob *job);
void fileQueueAppend(FileQueue *queue, FileJob *job);
void fileQueueInsertAfter(FileQueue *queue, FileJob *after, FileJob *job);
void fileQueueRemove(FileQueue *queue, FileJob *job);
FileJob *fileJobCreate(void);
void fileDestroyJob(FileJob *job);
void camFollowOffsetVec(FileJob *job, void *dst);

struct CamAim;

extern void PoseRotBillboard(struct CamAim *obj, void *dst);
FileJob *fileAppendJob(FileQueue *queue, u32 id);
FileJob *fileDuplicateAndAppendJob(FileQueue *queue, FileJobPayload *source);
extern u8 fileAppendJobFromCommandPathData[];
extern FileJob *fileQueueFindBySector(FileQueue *queue, u32 sector);
extern FileJob *fileQueueFindFlaggedById(FileQueue *queue, u32 id);

/* Makes the first job chained to owner's sector the leader and rechains the rest to it. */
static inline void fileQueueRechainSectorFollowers(FileQueue *queue, FileJob *owner, FileJob *leader) {
    FileJob *next;

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
extern s32 fileQueueCountLinkedJobs(FileQueue *queue);

/* Sixteen-byte preamble of a saved file queue. */
typedef struct FileQueueImageHeader {
    s32 version;
    s32 unk4;
    s32 unk8;
    f32 unkC;
} FileQueueImageHeader;

extern char fileQueueVersionedImagePaddingBytes[];
FileJob *fileQueueFindById(FileQueue *queue, u32 id);
FileJob *fileQueueFindFlaggedById(FileQueue *queue, u32 id);
FileJob *fileQueueFindBySector(FileQueue *queue, u32 sector);
s32 fileFindQueuedJobIndex(FileQueue *queue, FileJob *target);
s32 fileQueueCountLinkedJobs(FileQueue *queue);
void camFollowOffsetVec(FileJob *obj, void *dst);
void PoseRotBillboard(CamAim *obj, void *dst);
EffectQueuedSurfaceNode *fileCreateSurfaceLoaderState(s32 capacity);
EffectQueuedSurfaceNode *fileCreateGridLoaderRecord(FileKeyBlock *hdr);
void Delete_effParticle(EffectQueuedSurfaceNode *node);
void ResourceCopy(EffectQueuedSurfaceNode *dst, EffectQueuedSurfaceNode *src);
EffectQueuedSurfaceNode *Copy_effParticle(EffectQueuedSurfaceNode *owner);
void effCreate(EffectQueuedSurfaceNode *node, u32 entryId, void *resource);
void fileLoadObjectOpenNamedDevice(EffectQueuedSurfaceNode *node, u32 resourceId);
void BillboardSet(EffectQueuedSurfaceNode *node, u32 resourceId);
void BillboardGeneralSet(EffectQueuedSurfaceNode *node, u32 resourceId);
void fileReplaceEffectSurfaceJobs(EffectQueuedSurfaceNode *node, FileJobPayload *job);
void PartsListSet(EffectQueuedSurfaceNode *node, FileQueue *queue);
void fileReplaceReferenceHolder(EffectQueuedSurfaceNode *obj, u32 resource);
void fileClearLoadObjectReferences(EffectQueuedSurfaceNode *obj);

void fileSendLoadObjectRecordVector(EffectQueuedSurfaceNode *obj, const u128 *value);
void fileCopyLoadObjectRecordVector(EffectQueuedSurfaceNode *obj, const u128 *value);
void fileSetRecordWordFour(s32 record, u32 value);
void fileSetLoadObjectScale(EffectQueuedSurfaceNode *obj, f32 scale);
void fileResetSlotStates(FileSlotTable *table);


void fileInvalidateSlotGroup(FileSlotTable *table, FileSlot *slot);
void fileCopyAndInvalidateSlotGroup(FileSlotTable *table, FileSlot *source);
void effUpdateGridRecordSlotHistory(FileSlotTable *record, FileSlot *slot);
extern void effUpdateGridRecordSlotHistory(FileSlotTable *, FileSlot *);

typedef struct FileWaveMotion {
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
} FileWaveMotion;            /* 0x44, ordinary word alignment */

void effScaleWaveEmitterParameters(ScaleOwner *owner, f32 scale);

typedef struct FileSlotMotion {
    f32 direction[4];     /* 0x00; this callback initializes xyz only */
    f32 speed;            /* 0x10 */
    f32 scaleMultiplier;  /* 0x14 */
    f32 angle;            /* 0x18 */
    f32 angleMultiplier;  /* 0x1C */
} FileSlotMotion;

typedef struct FileCircularMotion {
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
} FileCircularMotion;       /* 0x44, ordinary word alignment */

typedef struct FileOrientedRingMotion {
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
} FileOrientedRingMotion;   /* 0x38, ordinary word alignment */

typedef struct FileCurveMotion {
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
} FileCurveMotion; /* 0x4C, naturally four-byte aligned */

void effPCPHelperBEDGetModel(FileSlotTable *record);

void fileJobFreeSecondaryBuffer(FileJobPayload *job) {
    struct sdfMemHandle_t *allocation = job->secondary.allocation;
    if (allocation != NULL) {
        sdfMemFreeDirect(allocation);
        FILE_JOB_LOCATION_CLEAR(job->secondary);
        job->secondary.size = 0;
        job->secondary.allocation = NULL;
    }
}

FileJobPayload *Copy_effParts(FileJobPayload *request) {
    FileJobPayload *job = fileCreateJob(request->type);
    FileTypeCallbacks *cb = &fileJobTypeOperations[job->type];

    job->option = request->option;
    job->primary.selector = request->primary.selector;
    job->data = cb->createChild(request->data, job->type);
    return job;
}

void fileJobNotifyPair(FileJobPayload *left, FileJobPayload *right) {
    void (*cb)(void *, void *) = fileJobTypeOperations[right->type].notifyPair;

    if (cb != NULL) {
        cb(left->data, right->data);
    }
}

void ScaleSet_effParts(FileJobPayload *job) {
    u16 id = job->type;

    if (fileJobTypeOperations[id].reset != NULL) {
        fileJobTypeOperations[id].reset(job->data);
    }
}

void fileJobInvokeTypeCallback(FileJobPayload *job) {
    u16 idx = job->type;
    void *data = job->data;

    fileJobTypeOperations[idx].invoke(data);
}

void PositionSet(FileJobPayload *job, const f32 position[4]) {
    u16 id = job->type;

    if (fileJobTypeOperations[id].setPosition != NULL) {
        fileJobTypeOperations[id].setPosition(job->data, position);
    }
}

void fileJobInvokeRotationCallback(FileJobPayload *job, const f32 rotation[4]) {
    u16 id = job->type;

    if (fileJobTypeOperations[id].setRotation != NULL) {
        fileJobTypeOperations[id].setRotation(job->data, rotation);
    }
}

void fileJobInvokeScaleCallback(FileJobPayload *job, f32 scale) {
    u16 id = job->type;

    if (fileJobTypeOperations[id].setScale != NULL) {
        fileJobTypeOperations[id].setScale(job->data, scale);
    }
}

void fileDispatchJobTypeCallback(FileJobPayload *job, u32 color) {
    u16 id = job->type;

    if (fileJobTypeOperations[id].setColor != NULL) {
        fileJobTypeOperations[id].setColor(job->data, color);
    }
}

void fileJobSetPrimaryData(FileJobPayload *job, const void *src, s32 size, u16 option) {
    fileJobFreePrimaryBuffer(job);
    if (src != NULL && size > 0) {
        job->primary.allocation = sdfMemAllocDirect(size);
        FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(job->primary) = sdfMemLock(job->primary.allocation);
        job->primary.size = size;
        job->option = option;
        memcpy((void *)FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(job->primary), src, size);
    }
}

void fileJobCopyCommandIntoPrimaryData(FileJobPayload *job, const char *commandPath, u16 option) {
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

void fileJobSetSecondaryData(FileJobPayload *job, const void *src, s32 size, u16 selector) {
    fileJobFreeSecondaryBuffer(job);
    if (src != NULL && size > 0) {
        job->secondary.allocation = sdfMemAllocDirect(size);
        FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(job->secondary) = sdfMemLock(job->secondary.allocation);
        job->secondary.size = size;
        job->primary.selector = selector;
        memcpy((void *)FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(job->secondary), src, size);
    }
}

void fileJobCopyCommandIntoSecondaryData(FileJobPayload *job, const char *commandPath, u16 selector) {
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

void fileWriteToPfs(FileJobPayload *job, const char *filePath) {
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
FileJobPayload *fileJobCreateFromCommandState(const char *entry) {
    sdfFileCmdHandle_t *command;
    s32 size;
    struct sdfMemHandle_t *allocation;
    void *buffer;
    FileJobPayload *job;

    command = sdfFileOpen(entry);
    if (command != 0) {
        size = sdfFileLength(command);
        allocation = sdfMemAllocDirect(size);
        buffer = (void *)sdfMemLock(allocation);
        sdfFileRead(command, buffer, size);
        sdfFileClose(command);
        job = fileDuplicateJob((FileJobPayload *)buffer);
        sdfMemFreeDirect(allocation);
        return job;
    }
    return NULL;
}

u32 fileJobSerializedSize(FileJobPayload *job) {
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

void fileQueueAppend(FileQueue *queue, FileJob *job) {
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

void fileQueueInsertAfter(FileQueue *queue, FileJob *after, FileJob *job) {
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

void fileQueueRemove(FileQueue *queue, FileJob *job) {
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

void fileDestroyJob(FileJob *job) {
    sdfChipFreeDirect(job);
}

FileQueue *Create_effPartsList(FileQueue *source) {
    FileQueue *queue = fileQueueCreate();
    FileJob *entry;
    FileJob *job;
    f32 vec[4] __attribute__((aligned(16)));

    SDF_VU0_COPYVECTOR(queue->offset, source->offset);
    SDF_VU0_COPYVECTOR(queue->axis, source->offset);
    queue->scaleMultiplier = source->scaleMultiplier;
    queue->transformFlags = source->transformFlags;
    if (source->first != NULL) {
        for (entry = source->first; entry != NULL; entry = entry->next) {
            job = fileJobCreate();

            if ((entry->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) == 0) {
                job->id = (u32)Create_effParts((FileJobPayload *)entry->id);
            } else {
                FileJob *parent = fileQueueFindById(source, entry->id);
                s32 index = fileFindQueuedJobIndex(source, parent);

                parent = fileQueueGetAt(queue, index);
                job->id = (u32)Copy_effParts((FileJobPayload *)parent->id);
            }
            fileJobCopyHeader(job, entry);
            strcpy(job->name, entry->name);
            fileQueueAppend(queue, job);
        }
    } else {
        FileJob *records;

        entry = (FileJob *)((u8 *)source + (u32)source->last);
        records = entry;
        if (source->count > 0) {
            s32 count = source->count;

            do {
                job = fileJobCreate();

                if ((entry->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) == 0) {
                    FileJobPayload *request = (FileJobPayload *)((u8 *)source + entry->id);

                    if (entry->flags & FILE_JOB_FLAG_SECTOR_FOLLOWER) {
                        FileJobPayload *secondary = (FileJobPayload *)((u8 *)source + records[entry->sector].id);

                        request->secondary.size = secondary->secondary.size;
                        FILE_JOB_LOCATION_AS_JOB_RELATIVE_OFFSET(request->secondary) =
                            (u32)fileResolveSecondaryBuffer(secondary) - (u32)request;
                        job->id = (u32)Create_effParts(request);
                        FILE_JOB_LOCATION_AS_JOB_RELATIVE_OFFSET(request->secondary) = request->secondary.size = 0;
                    } else {
                        job->id = (u32)Create_effParts(request);
                    }
                } else {
                    FileJob *parent = fileQueueGetAt(queue, entry->id);

                    job->id = (u32)Copy_effParts((FileJobPayload *)parent->id);
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
void effPartsDelete(FileQueue *queue) {
    FileJob *job = queue->first;
    while (job != NULL) {
        FileJob *next = job->next;
        if ((job->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) == 0) {
            fileJobDestroy((FileJobPayload *)job->id);
        }
        fileDestroyJob(job);
        job = next;
    }
    sdfChipFreeDirect(queue);
}

FileQueue *effPartsCreateFinish(const FileQueue *source) {
    FileQueue *queue = fileQueueCreate();
    const FileJob *src;
    f32 vec[4] __attribute__((aligned(16)));

    SDF_VU0_COPYVECTOR(queue, source);
    SDF_VU0_COPYVECTOR(queue->axis, source->axis);
    queue->scaleMultiplier = source->scaleMultiplier;
    queue->transformFlags = source->transformFlags;
    for (src = source->first; src != NULL; src = src->next) {
        FileJob *job = fileJobCreate();
        job->id = (u32)Copy_effParts((FileJobPayload *)src->id);
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

void effPartsPositionSet(FileQueue *queue, const f32 vec[4])
{
    f32 rot[16];
    f32 base[4];
    f32 pos[4];
    FileJob *job;
    f32 scale;

    SDF_VU0_LOADVECTOR(vec);
    SDF_VU0_STOREVECTOR(queue->position);
    SDF_VU0_LOADVECTOR_B(queue->offset);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(base);
    SDF_VU0_LOADVECTOR(queue->quat);
    ToMatrix();
    VU0_STORE_MATRIX(rot);
    scale = queue->scale * queue->scaleMultiplier;
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
        PositionSet((FileJobPayload *)job->id, pos);
    }
}

void effPartsRotateSet(FileQueue *queue, const f32 rot[4])
{
    f32 quat[4];
    f32 pos[4];
    FileJob *job;

    SDF_VU0_LOADVECTOR(rot);
    SDF_VU0_STOREVECTOR(queue->quat);
    SDF_VU0_STOREVECTOR(quat);
    ToMatrix();
    SDF_VU0_LOADVECTOR(queue->axis);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR(queue->offset);
    for (job = queue->first; job != NULL; job = job->next) {
        SDF_VU0_LOADVECTOR(job->quat);
        SDF_VU0_LOADVECTOR_B(quat);
        Mult();
        SDF_VU0_STOREVECTOR_NOMEM(pos);
        fileJobInvokeRotationCallback((FileJobPayload *)job->id, pos);
    }
    effPartsPositionSet(queue, queue->position);
}

void effPartsScaleSet(FileQueue *queue, f32 scale)
{
    s128 pos;
    FileJob *job;
    f32 total;
    f32 jobScale;

    queue->scale = scale;
    total = scale * queue->scaleMultiplier;
    for (job = queue->first; job != NULL; job = job->next) {
        jobScale = job->scale;
        if (job->updateFlags & FILE_JOB_UPDATE_FLAG_APPLY_QUEUE_SCALE) {
            jobScale = jobScale * total;
        }
        fileJobInvokeScaleCallback((FileJobPayload *)job->id, jobScale);
        if (job->xformFlags & 0x80) {
            SDF_VU0_LOADVECTOR(queue->position);
            SDF_VU0_LOADVECTOR_B(queue->offset);
            SDF_VU0_ADDVECTOR();
            if (job->xformFlags & 4) {
                VU0_SCALAR_OP(-5.0f, "vaddx.y vf10, vf0, vf2x");
            }
            SDF_VU0_LOADVECTOR_B(job->offset);
            VU0_SCALAR_OP(total, "vmulx.xyzw vf11, vf11, vf2x");
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR_NOMEM(&pos);
            PositionSet((FileJobPayload *)job->id, (f32 *)&pos);
        }
    }
}

/* Store the queue's modulation colour, then send every job its own colour
 * modulated by it ((job / 128) * (queue / 128) * 128 per channel). The tint
 * store is the unclobbered SDK spelling: as a bare volatile asm it flushes the
 * CSE table, so the loop's 1/128 is rematerialized rather than copied from the
 * first unpack's FPR. Reading the payload before the packed store keeps the
 * store from feeding the argument load. */
void ColorSet(FileQueue *queue, u32 color) {
    f32 tint[4] __attribute__((aligned(16)));
    u32 queueColor[4] __attribute__((aligned(16)));
    u32 jobColor[4] __attribute__((aligned(16)));
    u32 packed[4] __attribute__((aligned(16)));
    FileJob *job;

    queue->color = color;
    queueColor[0] = color;
    EE_MMI_RGBA_UNPACK(queueColor, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(tint);
    for (job = queue->first; job != NULL; job = job->next) {
        u32 result;
        FileJobPayload *payload;

        jobColor[0] = job->color;
        EE_MMI_RGBA_UNPACK(jobColor, 1.0f / 128.0f);
        SDF_VU0_LOADVECTOR_B(tint);
        SDF_VU0_MULVECTOR();
        EE_MMI_RGBA_PACK_UNIT(result, 0x43000000);
        payload = (FileJobPayload *)job->id;
        packed[0] = result;
        fileDispatchJobTypeCallback(payload, result);
    }
}

void effPartsRotateMatrixSet(FileQueue *queue, f32 matrix[4][4]) {
    f32 vec[4] __attribute__((aligned(16)));
    sdfVuMatrixToQuaternion(matrix);
    SDF_VU0_STOREVECTOR(vec);
    effPartsRotateSet(queue, vec);
}

FileJob *fileAppendJob(FileQueue *queue, u32 id) {
    FileJob *job = fileJobCreate();

    job->id = id;
    fileQueueAppend(queue, job);
    return job;
}

FileJob *fileDuplicateAndAppendJob(FileQueue *queue, FileJobPayload *source) {
    FileJobPayload *job = fileDuplicateJob(source);
    return fileAppendJob(queue, (u32)job);
}

FileJob *fileAppendJobFromCommandPath(FileQueue *queue, const char *commandPath) {
    printf(fileAppendJobFromCommandPathData);
    return fileAppendJob(queue, (u32)fileJobCreateFromCommandState(commandPath));
}

FileJob *fileJobDuplicateAfter(FileQueue *queue, FileJob *src) {
    FileJob *job = fileJobCreate();

    memcpy(job, src, 0xC0);
    fileJobResetAndInitTransform(job);
    job->flags |= FILE_JOB_FLAG_SHARED_PAYLOAD;
    job->id = src->id;
    fileQueueInsertAfter(queue, src, job);
    return job;
}

void fileQueueLinkJobToSectorLeader(FileQueue *queue, FileJob *job, FileJob *ref) {
    FileJob *node;
    FileJob *found;
    FileJobPayload *source;
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
        fileJobFreeSecondaryBuffer((FileJobPayload *)job->id);
        source = (FileJobPayload *)ref->id;
        fileJobSetSecondaryData((FileJobPayload *)job->id,
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

void fileQueueDetachSectorFollower(FileQueue *queue, FileJob *job) {
    u32 flags = job->flags;
    FileJob *first;

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
void fileQueueSaveImage(FileQueue *queue, const char *filePath) {
    char path[0xD0];
    FileJobPayload payload;
    FileQueue header;
    FileJob record;
    s32 fd;
    s32 count;
    s32 offset;
    s32 size;
    FileJob *job;

    if (sdfHddValid != 0) {
        sprintf(path, fileWriteToPfsData, filePath);
        fd = sceOpen(path, 0x602, 0x1B6);
    } else {
        sprintf(path, fileWriteToPfsData2, sdfFileGetHostRoot(), filePath);
        fd = sceOpen(path, 0x602);
    }
    header = *queue;
    header.updateFrame = 0;
    size = sizeof(FileQueue);
    header.entryOffset = size;
    header.first = NULL;
    count = fileQueueCountLinkedJobs(queue);
    sceWrite(fd, &header, sizeof(FileQueue));
    offset = count * sizeof(FileJob) + sizeof(FileQueue);

    for (job = queue->first; job != NULL; job = job->next) {
        record = *job;
        if ((job->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) == 0) {
            if ((job->flags & FILE_JOB_FLAG_SECTOR_FOLLOWER) == 0) {
                size = fileJobSerializedSize((FileJobPayload *)job->id);
            } else {
                payload = *(FileJobPayload *)job->id;
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
        sceWrite(fd, &record, sizeof(FileJob));
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
            size = fileJobSerializedSize((FileJobPayload *)job->id);
            fileJobWriteSerializedPayload(fd, (FileJobPayload *)job->id);
        } else {
            payload = *(FileJobPayload *)job->id;
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
void fileQueueSaveVersionedImage(FileQueue *queue, const char *filePath) {
    char path[0xD0];
    FileJobPayload payload;
    FileQueue header;
    FileJob record;
    FileQueueImageHeader image;
    s32 fd;
    s32 count;
    s32 offset;
    s32 size;
    FileJob *job;

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
    size = sizeof(FileQueue);
    header.entryOffset = size;
    header.first = NULL;
    count = fileQueueCountLinkedJobs(queue);
    sceWrite(fd, &header, sizeof(FileQueue));
    offset = count * sizeof(FileJob) + sizeof(FileQueue);

    for (job = queue->first; job != NULL; job = job->next) {
        record = *job;
        if ((job->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) == 0) {
            if ((job->flags & FILE_JOB_FLAG_SECTOR_FOLLOWER) == 0) {
                size = fileJobSerializedSize((FileJobPayload *)job->id);
            } else {
                payload = *(FileJobPayload *)job->id;
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
        sceWrite(fd, &record, sizeof(FileJob));
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
            size = fileJobSerializedSize((FileJobPayload *)job->id);
            fileJobWriteSerializedPayload(fd, (FileJobPayload *)job->id);
        } else {
            payload = *(FileJobPayload *)job->id;
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

FileQueue *fileQueueCreateFromCommandState(const char *entry) {
    sdfFileCmdHandle_t *command = sdfFileOpen(entry);
    s32 size;
    struct sdfMemHandle_t *allocation;
    FileQueue *image;
    FileQueue *queue;
    FileJob *copy;
    FileJob *src;
    u32 i;

    if (command == NULL) {
        return NULL;
    }
    size = sdfFileLength(command);
    allocation = sdfMemAllocDirect(size);
    image = (FileQueue *)sdfMemLock(allocation);
    sdfFileRead(command, image, size);
    sdfFileClose(command);
    queue = fileQueueCreate();
    SDF_VU0_COPYVECTOR(queue->offset, image->offset);
    SDF_VU0_COPYVECTOR(queue->axis, image->offset);
    queue->scaleMultiplier = image->scaleMultiplier;
    queue->transformFlags = image->transformFlags;
    for (i = 0, src = (FileJob *)((u8 *)image + image->entryOffset);
         i < image->count; i++, src++) {
        if ((src->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) == 0) {
            copy = fileDuplicateAndAppendJob(queue, (FileJobPayload *)((u8 *)image + src->id));
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

FileJob *fileQueueFindById(FileQueue *queue, u32 id) {
    FileJob *job = queue->first;
    while (job != NULL) {
        if (job->id == id) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

FileJob *fileQueueFindFlaggedById(FileQueue *queue, u32 id) {
    FileJob *job = queue->first;
    while (job != NULL) {
        if ((job->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) != 0 && job->id == id) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

FileJob *fileQueueFindBySector(FileQueue *queue, u32 sector) {
    FileJob *job = queue->first;
    while (job != NULL) {
        if ((job->flags & (FILE_JOB_FLAG_SHARED_PAYLOAD | FILE_JOB_FLAG_SECTOR_FOLLOWER)) ==
            FILE_JOB_FLAG_SECTOR_FOLLOWER && job->sector == sector) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

FileJob *fileQueueGetAt(FileQueue *queue, s32 index) {
    FileJob *job = queue->first;
    while (job != NULL) {
        if (index-- == 0) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

s32 fileFindQueuedJobIndex(FileQueue *queue, FileJob *target) {
    FileJob *job = queue->first;
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

s32 fileQueueCountLinkedJobs(FileQueue *queue) {
    FileJob *job;
    s32 count;

    count = 0;
    for (job = queue->first; job != 0; job = job->next) {
        count = count + 1;
    }
    return count;
}

/* vu0 routine: dst = |pos| * unit(aViewEye - aViewTarget); flag 0x10 flattens y, then puts pos.y back */
void camFollowOffsetVec(FileJob *obj, void *dst)
{
    f32 len[4];
    f32 length;
    u32 flags = obj->xformFlags;

    if ((flags & 0x18) == 0) {
        SDF_VU0_COPYVECTOR(dst, obj->offset);
        return;
    }
    SDF_VU0_LOADVECTOR(obj->offset);
    if (obj->xformFlags & 0x10) {
        VU0_SCALAR_OP(0.0f, "vaddx.y vf10, vf0, vf2x");
    }
    SDF_VU0_GETLENGTH(length);
    VEC3_SPLAT(len, length);
    SDF_VU0_LOADVECTOR(aViewEye);
    SDF_VU0_LOADVECTOR_B(aViewTarget);
    SDF_VU0_SUBVECTOR();
    if (obj->xformFlags & 0x10) {
        VU0_SCALAR_OP(0.0f, "vaddx.y vf10, vf0, vf2x");
    }
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(len);
    SDF_VU0_MULVECTOR();
    if (obj->xformFlags & 0x10) {
        VU0_SCALAR_OP(obj->offset[1], "vaddx.y vf10, vf0, vf2x");
    }
    SDF_VU0_STOREVECTOR(dst);
}

/* dst = rotation facing unit(aViewEye - aViewTarget) (pitch and yaw); flag 0x40 composes it onto obj->quat, no flag 0x60 copies obj->quat */
void PoseRotBillboard(CamAim *obj, void *dst)
{
    f32 v[4];
    f32 pitch;
    u32 flags = obj->flags;

    if ((flags & 0x60) == 0) {
        SDF_VU0_COPYVECTOR(dst, obj->quat);
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
    if (obj->flags & 0x40) {
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(obj->quat);
        Mult();
    }
    SDF_VU0_STOREVECTOR(dst);
}

EffectQueuedSurfaceNode *fileCreateSurfaceLoaderState(s32 capacity) {
    EffectQueuedSurfaceNode *rec = (EffectQueuedSurfaceNode *)sdfChipAllocZeroDirect(sizeof(EffectQueuedSurfaceNode));
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

EffectQueuedSurfaceNode *fileCreateGridLoaderRecord(FileKeyBlock *hdr) {
    u32 rows = hdr->emissionDuration;
    u32 count = (rows != 0 ? rows : hdr->spawnRate) * (rows != 0 ? hdr->spawnRate : hdr->length);

    return fileCreateSurfaceLoaderState(count <= 0x12C ? count : 0x12C);
}

INCLUDE_ASM(const s32, "code_r1_0021CE60", PartsCreate_effParticle);

void Delete_effParticle(EffectQueuedSurfaceNode *node) {
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

EffectQueuedSurfaceNode *Copy_effParticle(EffectQueuedSurfaceNode *owner) {
    FileKeyBlock *source = (FileKeyBlock *)owner->record->data1;
    EffectQueuedSurfaceNode *result = fileCreateGridLoaderRecord(source);

    effCreate(result, owner->record->type, source);
    ResourceCopy(result, owner);
    return result;
}

INCLUDE_ASM(const s32, "code_r1_0021CE60", ResourceCopy);

void effCreate(EffectQueuedSurfaceNode *node, u32 entryId, void *resource) {
    if (node->record != 0) {
        effPCPHelperBEDGetModel(node->record);
    }
    node->record = fileAllocateGridRecordSlots((u16)entryId, node->handleCount, resource);
}

void fileLoadObjectOpenNamedDevice(EffectQueuedSurfaceNode *node, u32 resourceId) {
    struct billHandle_t *resource = node->resource;
    if (resource != 0) {
        dds3BillboardDelete(resource);
    }
    resource = dds3BillboardGeneral(resourceId);
    node->resource = resource;
    if (node->record != 0) {
        dds3BillboardSetMode(resource, (s16)((FileKeyBlock *)node->record->data0)->alphaTrack.surfaceIndex);
    }
}

void BillboardSet(EffectQueuedSurfaceNode *node, u32 resourceId) {
    struct billHandle_t *resource = node->resource;
    if (resource != 0) {
        dds3BillboardDelete(resource);
    }
    resource = dds3BillboardCreate(0, resourceId);
    node->resource = resource;
    if (node->record != 0) {
        dds3BillboardSetMode(resource, (s16)((FileKeyBlock *)node->record->data0)->alphaTrack.surfaceIndex);
    }
}

void BillboardGeneralSet(EffectQueuedSurfaceNode *node, u32 resourceId) {
    struct billHandle_t *resource = node->resource;
    if (resource != 0) {
        dds3BillboardDelete(resource);
    }
    resource = dds3BillboardCreate(1, resourceId);
    node->resource = resource;
    dds3BillboardSetAnimColorAdvMode(resource);
    if (node->record != 0) {
        dds3BillboardSetMode(node->resource, (s16)((FileKeyBlock *)node->record->data0)->alphaTrack.surfaceIndex);
    }
}

void fileReplaceEffectSurfaceJobs(EffectQueuedSurfaceNode *node, FileJobPayload *job) {
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
        node->jobs = (FileJobPayload **)sdfMemLock(node->jobAllocation);
        node->jobs[0] = Create_effParts(job);
        for (i = 1; i < count; i++) {
            node->jobs[i] = Copy_effParts(node->jobs[0]);
        }
    }
}

void PartsListSet(EffectQueuedSurfaceNode *node, FileQueue *queue) {
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

void fileReplaceReferenceHolder(EffectQueuedSurfaceNode *obj, u32 resource) {
    if (obj->resourceHolder != NULL) {
        effCacheTexDelete(obj->resourceHolder);
    }
    obj->resourceHolder = effCacheTexCreate(resource);
}

void fileClearLoadObjectReferences(EffectQueuedSurfaceNode *obj) {
    if (obj->record != 0) {
        fileClearRecordReferences(obj->record);
        return;
    }
}

INCLUDE_ASM(const s32, "code_r1_0021CE60", effLeaf348);

INCLUDE_ASM(const s32, "code_r1_0021CE60", fileDrawLoadObjectSlots);

INCLUDE_ASM(const s32, "code_r1_0021CE60", effCallSRAnimation);

void fileSendLoadObjectRecordVector(EffectQueuedSurfaceNode *obj, const u128 *value) {
    mnuRecordSetVector(obj->record, value);
}

void fileCopyLoadObjectRecordVector(EffectQueuedSurfaceNode *obj, const u128 *value) {
    fileSetRecordSecondVector(obj->record, value);
}

void fileSetRecordWordFour(s32 record, u32 value) {
    *(u32 *)(record + 4) = value;
}

void fileSetLoadObjectScale(EffectQueuedSurfaceNode *obj, f32 scale) {
    obj->opacity = scale;
    dds3DispatchIndexedCallback(obj->record, scale);
}

void fileResetSlotStates(FileSlotTable *table) {
    u32 count;
    FileSlot *slot;
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
u32 effColorAnimation(const SdfColorTrack *colorTrack,
                              const SdfAlphaTrack *alphaTrack,
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

f32 effSampleScalarCurve(const EffScalarCurve *curve, s32 frame, s32 duration) {
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

void fileInvalidateSlotGroup(FileSlotTable *table, FileSlot *slot) {
    FileKeyBlock *grid = (FileKeyBlock *)table->data0;
    s32 rows = grid->rows;
    s32 columns = grid->columns;
    s32 n = columns * rows;
    u32 index;
    FileSlot *groupSlot;
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

void fileCopyAndInvalidateSlotGroup(FileSlotTable *table, FileSlot *source) {
    FileKeyBlock *grid = (FileKeyBlock *)table->data0;
    s32 rows = grid->rows;
    s32 columns = grid->columns;
    s32 n = columns * rows;
    u32 index;
    FileSlot *slot;

    if (n != 0) {
        index = ((u32)source - (u32)table->slots) >> 5;
        slot = table->slots + (table->instances + index * n);
        *slot = *source;
        FILE_SLOT_FRAME(slot) = FILE_SLOT_FRAME_NONE;
    }
}

/* vu0 routine: shifts one slot's history and interpolates its trailing samples. */
void effUpdateGridRecordSlotHistory(FileSlotTable *record, FileSlot *slot) {
    f32 points[4][4];
    f32 color[4];
    f32 colorStep[4];
    u32 curveStartColor[4];
    u32 curveEndColor[4];
    u32 curveResultColor[4];
    u32 linearStartColor[4];
    u32 linearEndColor[4];
    u32 linearResultColor[4];
    FileKeyBlock *dims = (FileKeyBlock *)record->data0;
    u32 rows = (u32)dims->rows;
    u32 columns = (u32)dims->columns;
    u32 count = columns * rows;
    u32 remaining;
    u32 i;
    u32 historyIndex;
    s32 frame;
    FileSlot *group;
    FileSlot *shiftTarget;
    FileSlot *cursor;
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
        group = (FileSlot *)(base +
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
                        FileSlot *previous;
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
                        FileSlot *previous = group + rows;
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
                                    *(u32 *)(slotBytes + (u32)&((FileSlot *)0)->color) = packedOutput[0];
                                    *(s32 *)(slotBytes + (u32)&FILE_SLOT_FRAME((FileSlot *)0)) = frame;
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

INCLUDE_ASM(const s32, "code_r1_0021CE60", effUpdateWaveEmitterSlots);

/* Scale selected wave-emitter parameters from source to destination. */
void effScaleWaveEmitterParameters(ScaleOwner *owner, f32 scale) {
    ScaleSet *src = owner->src;
    ScaleSet *dst = owner->dst;
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

INCLUDE_ASM(const s32, "code_r1_0021CE60", fileAllocateGridRecordSlots);
INCLUDE_SDATA(const s32, "code_r1_0021CE60", fileWriteToPfsData);

INCLUDE_SDATA(const s32, "code_r1_0021CE60", fileWriteToPfsData2);

INCLUDE_SDATA(const s32, "code_r1_0021CE60", fileWriteToPfsData3);

INCLUDE_SDATA(const s32, "code_r1_0021CE60", fileAppendJobFromCommandPathData);

