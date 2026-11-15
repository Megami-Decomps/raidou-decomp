#include "common.h"
#include "event.h"
#include "kernel.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "mdlManager.h"
#include "sdfTex.h"
#include "dds3SceneBasic.h"
#include "evtStage.h"
#include "evtSound.h"
#include "dds3WorldBasic.h"
#include "evtEventPack.h"
#include "effEvent.h"
#include "dds3KernelCore.h"

dds3ProcessID_t *evtSearchEventPack(u32 eventno);

typedef u8 sdfTextureClampMode_t;

void *dds3GetProcessData(dds3ProcessID_t *id);

typedef struct evtPackEntry_t {
    s32 kind;
    s32 dataSize;
    s32 cueCount;
    u32 dataOffset;
    s32 secondaryResourceId;
    s32 resourceId;
    u8 pad18[4];
    s32 motionId;
} evtPackEntry_t;

typedef struct evtPackHeader_t {
    u8 pad00[0x10];
    s32 entryCount;
    u8 pad14[0xC];
    evtPackEntry_t entries[0];
} evtPackHeader_t;

typedef struct evtPackProcessData_t {
    s32 eventno;
    s32 stat;
    void *pendingRequest;
    struct sdfMemHandle_t *resourceAllocation;
    u8 *payload;
    evtPackHeader_t *header;
    evtPackEntry_t *entries;
    u8 *entryPoint;
    u8 pad20[4];
    s32 sceneAllocation1;
    u8 pad28[8];
    s32 sceneAllocation2;
    u8 pad34[4];
    struct sdfTexHandle_t *effect72;
    s32 effect71;
    s32 effect76;
    s32 effect75;
} evtPackProcessData_t;

s32 evtGetEventPackStat(u32 eventno);
evtPackProcessData_t *evtGetPackProcessData(u32 eventno);
void *evtGetResorceData(u32 eventno, s32 rid);
extern s32 evtMotionSeWork;

#include "sdk_libc.h"

extern s32 evtSetRegistUnit(s32 major, s32 minor);


extern void fldFileAddrResetMode(u32 mode);
extern u32 dds3DrawFlag;

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

typedef struct evtMotionSeTaskParams_t {
    s32 modelKey;
    s32 eventTaskId;
    s32 resourceId;
} evtMotionSeTaskParams_t;

typedef struct beMotionSeTable_s {
    s32 Frame;
    s32 Se;
    u8 unk08[8];
} beMotionSeTable_s;


struct FileRequest;

struct sdfMemHandle_t *fileGetMemHandle(const struct FileRequest *fileHandle);
s32 fileHandleFree(struct FileRequest *fileHandle);
s32 pacFileCheckEnd(const struct FileRequest *pacFileHandle);

enum {
    EVT_PACK_ENTRY_POINT_KIND = 0,
    EVT_PACK_LOAD_COMPLETE = 2
};

extern char evtLoadDoneMessage[];
s32 evtCheckScriptDone(dds3ProcessID_t *task);
void evtWaitEventPackLoad(dds3ProcessID_t *task);
extern void fileSyncAll(void);
extern void Eve_DistortionBlurSetDefTex(void);
extern void Eve_PartialMultiBlurSetDefTex(void);


dds3ProcessID_t *evtLoadEventPack(s32 eventno);
void evtKillEventPack(s32 eventno);
s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);
dds3ProcessID_t *evtLoadModelSe(s32 modelKey, s32 eventTaskId, s32 resourceId);
struct FileRequest *fileLoad(const char *filename);
extern char evtLoadStartMessage[];
extern char evtEventPackScriptPath[];

/* Rows of 0x310 bytes; only the size field the bed scale reads is named. */
typedef struct evtBedModelParam_t {
    u8 pad00[0x10];
    f32 formsize;
    u8 pad14[0x310 - 0x14];
} evtBedModelParam_t;

s32 evtInitMotionSeProcess(s32 eventId, s32 resourceId);
s32 evtRegistBEField(s32 eventno, s32 rid);
s32 evtMotionSeProcess1(dds3ProcessID_t *id);
void evtFreeScriptProcessData(dds3ProcessID_t *task);
extern const char evtMotionSeNameFormat[0x10] __attribute__((aligned(8)));
extern const char evtEventScriptPathFormat[] __attribute__((aligned(8)));
void evtLoadEventScript(evtPackProcessData_t *state);
void evtCheckEventScriptLoaded(evtPackProcessData_t *state);

typedef struct evtCmbMenuCursor_t {
    u8 pad00[0x21];
    u8 openCount;
    u8 pad22[0x16];
} evtCmbMenuCursor_t;

typedef struct evtCmbWork_t {
    u8 pad00[0x17640];
    evtCmbMenuCursor_t menuCursor;
} evtCmbWork_t;

extern char evtLoadStartMessage[8];
extern char evtLoadDoneMessage[8];

dds3ProcessID_t *evtSearchEventPack(u32 eventno) {
    char taskName[32];

    evtMakeEventPackName(eventno, taskName);
    return dds3SearchProcessName(taskName);
}

/* Return this event task's loaded state, or -1 when it does not exist. */
s32 evtGetEventPackStat(u32 eventno) {
    dds3ProcessID_t *task;

    task = evtSearchEventPack(eventno);
    if (task != 0) {
        evtPackProcessData_t *state = (evtPackProcessData_t *)dds3GetProcessData(task);
        return state->stat;
    }
    return -1;
}

evtPackProcessData_t *evtGetPackProcessData(u32 eventno) {
    dds3ProcessID_t *task;

    task = evtSearchEventPack(eventno);
    if (task != 0) {
        return (evtPackProcessData_t *)dds3GetProcessData(task);
    }
    return NULL;
}

void *evtGetResorceData(u32 eventno, s32 rid) {
    s32 i;
    evtPackProcessData_t *data;
    dds3ProcessID_t *task;

    task = evtSearchEventPack(eventno);
    if (task != 0) {
        data = (evtPackProcessData_t *)dds3GetProcessData(task);
        if (data->stat != 2) {
            return 0;
        }
        for (i = 0; i < data->header->entryCount; i++) {
            if (data->entries[i].secondaryResourceId == rid) {
                return data->payload + data->entries[i].dataOffset;
            }
        }
        return 0;
    }
    return NULL;
}

INCLUDE_ASM(const s32, "event/evtEventPack", evtCallGetPackProcessData);

INCLUDE_ASM(const s32, "event/evtEventPack", evtCallMemset9);

/* Decode one retained unit payload and return its new viewer-object key. */
s32 evtInitMotionSeProcess(s32 eventId, s32 resourceId) {
    evtPackProcessData_t *packState;
    evtPackEntry_t *entry;
    s32 packEntryIndex;
    s32 entrySize;
    u8 *modelPayload;
    s32 viewerObjectKey;

    packState = evtGetPackProcessData(eventId);
    packEntryIndex = 0;
    if (packState->header->entryCount > 0) {
        entry = packState->entries;
        do {
            if (entry->secondaryResourceId == resourceId) {
                switch (entry->kind) {
                case 5:
                    entrySize = entry->dataSize;
                    modelPayload = packState->payload + entry->dataOffset;
                    printf("unit decode rid:%d, size:%d\n", resourceId, entrySize);
                    switch (eventId) {
                    case 0x2A4:
                    case 0x2AC:
                    case 0x2AE:
                    case 0x2B0:
                        mdlPackDecode(3, evtMotionSeWork, 0x101, modelPayload, entrySize);
                        break;
                    default:
                        mdlPackDecode(3, evtMotionSeWork, 0x103, modelPayload, entrySize);
                        break;
                    }
                    viewerObjectKey = evtSetRegistUnit(3, evtMotionSeWork);
                    evtMotionSeWork++;
                    if (evtMotionSeWork >= 0x7D0) {
                        evtMotionSeWork = 0x3E8;
                    }
                    printf("BE regist unit %d < %d >\n", evtMotionSeWork, entrySize);
                    return viewerObjectKey;
                }
            }
            entry++;
        } while (++packEntryIndex < packState->header->entryCount);
    }
    return -1;
}

/* Start a field BE from the task's resource table when all four payloads exist. */
s32 evtRegistBEField(s32 eventno, s32 rid) {
    evtPackProcessData_t *packState;
    evtPackEntry_t *entry;
    s32 entryCount;
    s32 remainingEntryCount;
    s32 *areaRoomIds;
    s32 *resource0Data;
    s32 *resource1Data;
    const sdfTextureOffsetListHeader_t *textureOffsets;

    packState = evtGetPackProcessData(eventno);
    areaRoomIds = NULL;
    resource1Data = NULL;
    resource0Data = NULL;
    textureOffsets = NULL;
    entryCount = packState->header->entryCount;
    if (entryCount > 0) {
        entry = packState->entries;
        remainingEntryCount = entryCount;
        do {
            if (entry->resourceId == rid) {
                switch (entry->kind) {
                case 2:
                    resource0Data = (s32 *)(packState->payload + entry->dataOffset);
                    break;
                case 3:
                    resource1Data = (s32 *)(packState->payload + entry->dataOffset);
                    break;
                case 4:
                    textureOffsets = (const sdfTextureOffsetListHeader_t *)(packState->payload + entry->dataOffset);
                    break;
                }
            }
            if (entry->secondaryResourceId == rid && entry->kind == 1) {
                areaRoomIds = (s32 *)(packState->payload + entry->dataOffset);
            }
            entry++;
        } while (--remainingEntryCount != 0);
    }

    if (areaRoomIds != NULL && resource0Data != NULL && resource1Data != NULL && textureOffsets != NULL) {
        evtLoadArea_FromMemory(areaRoomIds[0], areaRoomIds[1], EE_ADDR_S(resource0Data), EE_ADDR_S(resource1Data), textureOffsets, 0);
        fldFileAddrResetMode(1);
        dds3DrawFlag |= 0x02000000;
        return 1;
    }
    return 0;
}

s32 evtMotionSeProcess1(dds3ProcessID_t *id) {
    evtMotionSeTaskParams_t *motionSeParams;
    dds3Basic_t *motionNode;
    evtPackProcessData_t *packState;
    evtUnit_t *unit;
    u16 motionId;
    s32 motionFrame;
    s32 entryIndex;
    s32 cueIndex;
    beMotionSeTable_s *cues;

    motionSeParams = (evtMotionSeTaskParams_t *)dds3GetProcessData(id);
    if (dds3GetActiveScene() == NULL) {
        return -1;
    }
    motionNode = dds3SearchSceneBasic(dds3GetActiveScene(), motionSeParams->modelKey, EFF_WORLD_KIND_FOLLOW_MODEL);
    if (motionNode == NULL) {
        return -1;
    }
    packState = evtGetPackProcessData(motionSeParams->eventTaskId);
    if (packState == NULL) {
        return -1;
    }
    if (motionSeParams->eventTaskId < 600) {
        return 0;
    }
    unit = ((dds3UnitObjectBasicWorkData_t *)motionNode->workdata)->hUnit;
    if (unit == NULL) {
        return -1;
    }
    motionId = mdlGetMotionCtrlr(unit->owner, 0)->cutNumber;
    motionFrame = unit->owner->first->frame;
    for (entryIndex = 0; entryIndex < packState->header->entryCount; entryIndex++) {
        if (packState->entries[entryIndex].kind == 6 &&
            packState->entries[entryIndex].resourceId == motionSeParams->resourceId &&
            packState->entries[entryIndex].motionId == motionId) {
            cues = (beMotionSeTable_s *)(packState->payload + packState->entries[entryIndex].dataOffset);
            for (cueIndex = 0; cueIndex < packState->entries[entryIndex].cueCount; cueIndex++) {
                if (cues[cueIndex].Frame == motionFrame) {
                    evtPlaySe(motionSeParams->eventTaskId, cues[cueIndex].Se);
                    printf("EVE_SE motno = %d, frame = %d\n", motionId, motionFrame);
                }
            }
        }
    }
    return 0;
}

/* Free the three-word parameter block owned by the motion-SE task. */
void evtFreeScriptProcessData(dds3ProcessID_t *task)
{
    sdfChipFreeDirect(EE_ADDR_PTR(dds3GetProcessData(task)));
}

/* Allocate a three-word task parameter block and format its "mse_..." name. */
dds3ProcessID_t *evtLoadModelSe(s32 modelKey, s32 eventTaskId, s32 resourceId) {
    char taskName[0x20];
    evtMotionSeTaskParams_t *params;

    sprintf(taskName, evtMotionSeNameFormat, eventTaskId, resourceId);
    params = sdfChipAllocDirect(0xC);
    memset(params, 0, 0xC);
    params->modelKey = modelKey;
    params->eventTaskId = eventTaskId;
    params->resourceId = resourceId;
    return dds3StartProcess(taskName, 0x3EC, 0, 0, evtMotionSeProcess1, (dds3ProcessFunc_t)evtFreeScriptProcessData, params);
}

/* Resolve the event's script path ("/event/eNNN/eNNN/scr/eNNN.be", grouped by tens) and start loading it. */
void evtLoadEventScript(evtPackProcessData_t *state) {
    s32 eventId;
    s32 eventDirectoryId;
    struct FileRequest *request;

    printf(evtLoadStartMessage);
    eventId = state->eventno;
    eventDirectoryId = eventId - eventId % 10;
    sprintf(evtEventPackScriptPath, evtEventScriptPathFormat, eventDirectoryId, eventId, eventId);
    request = fileLoad(evtEventPackScriptPath);
    state->pendingRequest = request;
    state->stat = 1;
}

/* Retain a ready pack and select its first entry-point record.
 * Clearing the request handle and marking completion occur on separate calls;
 * no entry-point record leaves the existing entryPoint value untouched. */
void evtCheckEventScriptLoaded(evtPackProcessData_t *state) {
    evtPackHeader_t *header;
    s32 entryPointIndex;

    if (state->pendingRequest != NULL) {
        if (pacFileCheckEnd(state->pendingRequest) != 0) {
            state->resourceAllocation = fileGetMemHandle((struct FileRequest *)state->pendingRequest);
            fileHandleFree(state->pendingRequest);
            state->pendingRequest = NULL;
            header = EE_ADDR_AS(evtPackHeader_t *, sdfMemLock(state->resourceAllocation));
            state->payload = (u8 *)header;
            state->header = header;
            state->entries = header->entries;
            for (entryPointIndex = 0; entryPointIndex < state->header->entryCount; entryPointIndex++) {
                if (state->entries[entryPointIndex].kind == EVT_PACK_ENTRY_POINT_KIND) {
                    state->entryPoint = state->payload + state->entries[entryPointIndex].dataOffset;
                    break;
                }
            }
        }
    } else {
        printf(evtLoadDoneMessage);
        state->stat = EVT_PACK_LOAD_COMPLETE;
    }
}

s32 evtCheckScriptDone(dds3ProcessID_t *task) {
    evtPackProcessData_t *packState = (evtPackProcessData_t *)dds3GetProcessData(task);

    switch (packState->stat) {
    default:
        if (packState->stat < 2) {
            if (packState->stat == 0) {
                evtLoadEventScript(packState);
            }
        }
        break;
    case 1:
        evtCheckEventScriptLoaded(packState);
        break;
    }
    return 0;
}

/* Release the event task's owned handles, then free its state.
 * File I/O is waited on even when the user-value handle is zero. */
void evtWaitEventPackLoad(dds3ProcessID_t *task) {
    void *stateHandle = dds3GetProcessData(task);
    evtPackProcessData_t *packState = (evtPackProcessData_t *)stateHandle;

    fileSyncAll();
    if (stateHandle != 0) {
        if (packState->effect72 != 0) {
            Eve_DistortionBlurSetDefTex();
            sdfTexFreeDelayed(packState->effect72);
        }
        if (packState->effect71 != 0) {
            Eve_PartialMultiBlurSetDefTex();
            sdfTexFreeDelayed(EE_ADDR_AS(struct sdfTexHandle_t *, packState->effect71));
        }
        if (packState->effect76 != 0) {
            Eve_RippleBlurSetDefTex();
            sdfTexFreeDelayed(EE_ADDR_AS(struct sdfTexHandle_t *, packState->effect76));
        }
        if (packState->effect75 != 0) {
            Eve_PartialMultiFilterSetDefTex();
            sdfTexFreeDelayed(EE_ADDR_AS(struct sdfTexHandle_t *, packState->effect75));
        }
        if (packState->pendingRequest != NULL) {
            fileHandleFree(packState->pendingRequest);
        }
        if (packState->resourceAllocation != NULL) {
            sdfMemFreeDelayedDirect(packState->resourceAllocation);
        }
        if (packState->sceneAllocation1 != 0) {
            sdfMemFreeDirect(EE_ADDR_AS(struct sdfMemHandle_t *, (u32)(packState->sceneAllocation1)));
        }
        if (packState->sceneAllocation2 != 0) {
            sdfMemFreeDirect(EE_ADDR_AS(struct sdfMemHandle_t *, (u32)(packState->sceneAllocation2)));
        }
    }
    sdfChipFreeDirect((void *)stateHandle);
}

/* Schedule the event-pack process only if it does not already exist. */
dds3ProcessID_t *evtLoadEventPack(s32 eventno) {
    char taskName[0x20];
    evtPackProcessData_t *taskData;
    dds3ProcessID_t *task;

    task = evtSearchEventPack(eventno);
    if (task == 0) {
        evtMakeEventPackName(eventno, taskName);
        taskData = sdfChipAllocDirect(sizeof(evtPackProcessData_t));
        memset(taskData, 0, sizeof(evtPackProcessData_t));
        taskData->eventno = eventno;
        taskData->stat = 0;
        task = dds3StartProcess(taskName, DDS3_PRIORITY_EVENT_PACK, 1, 1, evtCheckScriptDone, (dds3ProcessFunc_t)evtWaitEventPackLoad, taskData);
    }
    return task;
}

void evtKillEventPack(s32 eventno) {
    dds3ProcessID_t *taskHandle;

    taskHandle = evtSearchEventPack(eventno);
    if (taskHandle != 0) {
        dds3KillProcess(taskHandle, 0);
        return;
    }
}

INCLUDE_ASM(const s32, "event/evtEventPack", evtCallKillProcess);

const char evtMotionSeNameFormat[0x10] __attribute__((aligned(8))) = "mse_%d_%d";

s32 evtMotionSeWork = 0x3E8;

char evtLoadStartMessage[8] = "LOAD..\n";

char evtLoadDoneMessage[8] = "OK!!\n";

const char evtEventScriptPathFormat[0x20] __attribute__((aligned(8))) = "/event/e%03d/e%03d/scr/e%03d.be";
