/* Declarations of smt3-decomp src/smt3/event/evtEventPack.c, for its bodies in this directory. */
#ifndef SHARED_EVENT_EVTEVENTPACK_H
#define SHARED_EVENT_EVTEVENTPACK_H

#include "common.h"

#include "event.h"

#include "kernel.h"

dds3ProcessID_t *dds3SearchProcessName(const char *namestr);

dds3ProcessID_t *evtSearchEventPack(u32 eventno);

void evtMakeEventPackName(s32 taskId, char *buffer);

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

void sdfTexFreeDelayed(sdfTexHandle_t *aHandle);

struct sdfTexHandle_t *sdfTexSet(const TMX_HEADER_t *aTex);

extern void Eve_DistortionBlurSetTex(sdfTexHandle_t *);

extern s32 evtMotionSeWork;

#include "sdk_libc.h"

extern void mdlPackDecode(s32, s32, s32, void *, s32);

extern s32 evtSetRegistUnit(s32 major, s32 minor);

extern s32 evtLoadArea_FromMemory(s32 fld, s32 area, saddr_t pF1, saddr_t pF2,
                                            const sdfTextureOffsetListHeader_t *pTB, s32 pBF);

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

dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *pBasic, u32 serialID, s32 type);

struct dds3Basic_t *dds3GetActiveScene(void);

extern sdfMotionCtrlr_t *mdlGetMotionCtrlr(mdlObj_t *aHandle, s32 aGroup);

extern s32 evtPlaySe(s32 eventno, s32 seno);

void sdfChipFreeDirect(void *aPtr);

u32 sdfMemLock(struct sdfMemHandle_t *aHandle);

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

void sdfMemFreeDirect(struct sdfMemHandle_t *aHandle);

void sdfMemFreeDelayedDirect(struct sdfMemHandle_t *aHandle);

void evtWaitEventPackLoad(dds3ProcessID_t *task);

extern void fileSyncAll(void);

extern void Eve_DistortionBlurSetDefTex(void);

extern void Eve_PartialMultiBlurSetDefTex(void);

extern void Eve_RippleBlurSetDefTex(void);

extern void Eve_PartialMultiFilterSetDefTex(void);

dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                         s32 v3, dds3ProcessFunc_t drawExit,
                         dds3ProcessFunc_t deleteExit, void *p);

dds3ProcessID_t *evtLoadEventPack(s32 eventno);

void *sdfChipAllocDirect(s32 aSize);

extern void *memset(void *a, s32 b, size_t c);

void evtKillEventPack(s32 eventno);

s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);

dds3ProcessID_t *dds3SearchProcessPriority(u32 priority);

dds3ProcessID_t *evtLoadModelSe(s32 modelKey, s32 eventTaskId, s32 resourceId);

struct FileRequest *fileLoad(const char *filename);

extern char evtLoadStartMessage[];

extern char evtEventPackScriptPath[];

dds3ProcessID_t *evtSearchEventPack(u32 eventno);

/* Return this event task's loaded state, or -1 when it does not exist. */
s32 evtGetEventPackStat(u32 eventno);

evtPackProcessData_t *evtGetPackProcessData(u32 eventno);

void *evtGetResorceData(u32 eventno, s32 rid);

void func_001FA310(s32 taskId, s32 key);

extern f32 D_003494C0[];

/* Rows of 0x310 bytes; only the size field the bed scale reads is named. */
typedef struct evtBedModelParam_t {
    u8 pad00[0x10];
    f32 formsize;
    u8 pad14[0x310 - 0x14];
} evtBedModelParam_t;

extern evtBedModelParam_t datModelParamTable[];

extern void *dds3AddBEDEffectObjectBasic2(void *data, void *pos, void *rot);

extern void dds3EffectObjectBasicStatOn(void *basic, s32 s);

extern void effEveBEDCutSetScale(void *hEveBEDCut, f32 scale);

void *evt_1FA398(s32 taskId, s32 key, s32 modelId);

/* Decode one retained unit payload and return its new viewer-object key. */
s32 evtInitMotionSeProcess(s32 eventId, s32 resourceId);

/* Start a field BE from the task's resource table when all four payloads exist. */
s32 evtRegistBEField(s32 eventno, s32 rid);

s32 evtMotionSeProcess1(dds3ProcessID_t *id);

/* Free the three-word parameter block owned by the motion-SE task. */
void evtFreeScriptProcessData(dds3ProcessID_t *task);

extern const char evtMotionSeNameFormat[0x10] __attribute__((aligned(8)));

/* Allocate a three-word task parameter block and format its "mse_..." name. */
dds3ProcessID_t *evtLoadModelSe(s32 modelKey, s32 eventTaskId, s32 resourceId);

extern const char evtEventScriptPathFormat[0x28] __attribute__((aligned(8)));

/* Resolve the event's script path ("/event/eNNN/eNNN/scr/eNNN.be", grouped by tens) and start loading it. */
void evtLoadEventScript(evtPackProcessData_t *state);

/* Retain a ready pack and select its first entry-point record.
 * Clearing the request handle and marking completion occur on separate calls;
 * no entry-point record leaves the existing entryPoint value untouched. */
void evtCheckEventScriptLoaded(evtPackProcessData_t *state);

s32 evtCheckScriptDone(dds3ProcessID_t *task);

/* Release the event task's owned handles, then free its state.
 * File I/O is waited on even when the user-value handle is zero. */
void evtWaitEventPackLoad(dds3ProcessID_t *task);

/* Schedule the event-pack process only if it does not already exist. */
dds3ProcessID_t *evtLoadEventPack(s32 eventno);

void evtKillEventPack(s32 eventno);

/* Drain every event-pack process at the priority used during creation. */
void evt_1FAD40(void);

typedef struct evtCmbMenuCursor_t {
    u8 pad00[0x21];
    u8 openCount;
    u8 pad22[0x16];
} evtCmbMenuCursor_t;

typedef struct evtCmbWork_t {
    u8 pad00[0x17640];
    evtCmbMenuCursor_t menuCursor;
} evtCmbWork_t;

extern evtCmbWork_t *CMB_GBWK;

extern u8 cmbMenuEntryTable[8];

extern s32 evtBitCheck(s32 no);

extern void cmpSetCursorFlag(void *cur, saddr_t flag);

void cmbEnableMenuEntry2(void);

void cmbEnableMenuEntry6(void);

#endif
