/* Declarations of smt3-decomp src/smt3/sdf/sdfSystem.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFSYSTEM_H
#define SHARED_SDF_SDFSYSTEM_H

#include "fpu.h"

#include "common.h"

#include "eeregs.h"

#include "pipe.h"

#include "ps2asm.h"

#include "basic.h"

#include "motion.h"

/* Native 0x30-byte EE thread descriptor passed to CreateThread. */
typedef struct {
    s32 status;
    void (*entry)(void *);
    void *stack;
    s32 stackSize;
    void *gp;
    s32 priority;
    u8 unk18[0x18];
} sdfKernelThreadParam_t;

typedef struct sdfHandlerNode_t {
    struct sdfHandlerNode_t *next;
    s32 handlerKind;
    s32 channel;
    s32 handlerId;
} sdfHandlerNode_t;

struct pipeTextureFile_t;

extern dds3Basic_t *pipe_Camera;

extern s32 _gp, sdfThreadSema, sdfVblankFlag, sdfCoreDrawBuffSize;

extern u8 sdfPadManagerMode, sdfSystemThreadStack[], sdfVblankThreadStack[];

extern void (*D_00551828)(void);

extern sdfThCB_t *sdfVblankThreadId;

extern sdfThCB_t sdfSystemThreadCB;

extern s32 CreateThread(sdfKernelThreadParam_t *parameter);

extern s32 AddIntcHandler2(s32 channel, s32 handler, s32 argument, s32 next);

#include "sdk_libkernl.h"

extern s32 AddSbusIntcHandler(s32 channel, s32 handler);

extern void DisableIntc(s32 channel);

extern void RemoveIntcHandler(s32 channel, s32 handlerId);

extern void RemoveDmacHandler(s32 channel, s32 handlerId);

extern void RemoveSbusIntcHandler(s32 channel);

extern s32 SetAlarm(u16 delay, void (*callback)(s32, s32, s32), void *argument);

extern s32 WaitSema(s32 semaphore);

extern s32 SignalSema(s32 semaphore);

extern void ExitDeleteThread(void);

extern void TerminateThread(s32 threadId);

extern void DeleteThread(s32 threadId);

extern void EnableIntc(s32 channel);

extern void WakeupThread(s32 threadId);

extern s32 InitThread(void);

extern void sdfPadManager(void);

extern void sdfPadWork(void);

extern void sdfFileCmdThreadCheckPrio(void);

extern void sdfChipInit(u32 aSize);

extern void sdfMemInit(u32 aSize);

extern void sdfGsMemInit(void);

extern void sdfImageSetInit(void);

extern void sdfTexInit(void);

extern void sdfPadInitManager(void);

extern void sdfGrInit(u32 aPacketBuffSize);

extern void sdf3DInit(s32 aNestLevelLimit);

extern void sdfCoreDrawInit(s32 buffSize);

extern void sdfRandomizeByTime(void *aCB);

extern void *memset(void *a, s32 b, size_t c);

void FatalError(const char *format, ...) __attribute__((noreturn));

s32 CreateThreadSimple(void *entryAddress, void *workspace, s32 stackBytes, s32 priority);

void *sdfAllocAuto(s32 bytes);

s32 sdfFreeAuto(void *allocation);

void sdfAlarmWakeup(s32 unused0, s32 unused1, s32 threadId);

void sdfWaitVblank(s32 count);

s32 sdfInitThreadSystem(void);

s32 VblankIntcHandler(s32 cause);

typedef struct sdfSemaParam_t {
    s32 count;
    s32 maxCount;
    s32 initCount;
    s32 waitThreads;
    u32 attr;
    u32 option;
} sdfSemaParam_t;

typedef struct tagpipeParts {
    s32 offset;
    u8 id;
    u8 unk05;
    u8 mdl;
    s8 resv[5];
} tagpipeParts;

extern tagpipeParts *PartsWork;

extern u16 myStatus;

extern u8 camMode, CameraAction, offsetStep, offsetStepCount;

extern s32 D_00551738, D_0055173C, D_00551744, D_00551748;

extern void *pipeObjList, *pipeObjTail;

extern u8 attackCount;

typedef struct PIPE_EFF_t {
    sdfMemHandle_t *mhFile;
    void *ptr;
    s32 size;
    void *d3p;
    f32 x;
    f32 y;
    f32 z;
    f32 pos2[2];
    s32 offset;
    s32 count;
} PIPE_EFF_t;

extern s32 SectionIndex, mapIndex, mapPrev;

extern u8 debugMode, pipeInitDone;

extern s32 offsetCount;

extern tagpipeBasic *basicMy;

extern tagpipeBasic *PipeLine, *PipeCur, *PipeCur2, *PipeRBOX, *PipeEMS, *PipeEML;

extern tagpipeBasic *PipeMaka1, *PipeZMaka, *PipeDMaka, *PipeDRBOX;

extern PIPE_EFF_t *pipe_CELL01_D3P, *pipe_BREAK01_D3P, *pipe_BREAK02_D3P;

extern PIPE_EFF_t *pipe_DAMAGE01_D3P, *pipe_DAMAGE03_D3P, *pipe_ATTACK01_D3P;

extern s32 sdfChipCheck(void *aPtr);

extern void sdfChipFreeDirect(void *aPtr);

extern void sdfChipFreeDelayedDirect(void *aPtr);

extern void sdfMemFreePtrDirect(void *aPtr);

extern void *sdfMemGetHandleByPtr(void *aPtr);

extern tagPipeObject *ObjWork;

extern tagPipeTexture *MakaTex;

extern s32 ZmakaCount;

extern void *hWorkArea;

extern sdfHandlerNode_t *sdfDmacHandlerTable;

extern void *slLoadProcId;

extern s32 sdfMainThreadId;

extern char D_00551700[];

extern char D_00551808[];

extern s32 dds3ConfigValue[2];

extern s32 playerDAMAGE;

extern s32 myMaka;

extern s32 pipeMyMakaCount;

extern s8 rBoxNum;

extern u8 sdfCdClock;

extern s32 sdfVblankSema;

extern s8 slLoadKillFlag;

extern s32 pipeBulrLimitValue;

extern void *pipeMessData;

extern void *makaWork;

extern s32 CreateSema(sdfSemaParam_t *param);

extern s32 iWakeupThread(s32 thid);

extern void sdfMemFreeDelayedDirect(void *aHandle);

extern sdfThCB_t *func_002A87F0(s32 threadId);

extern s32 GetThreadId(void);

extern void ChangeThreadPriority(s32 thid, s32 prio);

extern s32 SleepThread(void);

extern void sdfIpuRemoveHandler(sdfHandlerNode_t *handler);

extern void func_002A8758(sdfThCB_t *node);

extern void *sdfMemAllocDirect(s32 aSize);

extern void *sdfMemLock(void *aHandle);

extern void *sdfChipAllocDirect(s32 aSize);

extern s32 CancelWakeupThread(s32 thid);

void sdfDummyFunc(void);

s32 sdfCreateSema(s32 initCount, s32 maxCount, u32 option);

s32 CreateThreadSimple(void *entryAddress, void *workspace, s32 stackBytes, s32 priority);

s32 sdfCreateThreadWithStack(void (*entryAddress)(), s32 stackBytes, s32 priority);

void *sdfAllocAuto(s32 size);

s32 sdfFreeAuto(void *allocation);

s32 sdfFreeAutoDelayed(void *allocation);

s32 sdf_2A8258(void **slot);

void FatalError(const char *format, ...);

sdfHandlerNode_t *DmacAddHandler(s32 kind, s32 channel, saddr_t handler, s32 arg, s32 next);

void sdfIpuRemoveHandler(sdfHandlerNode_t *target);

void sdf_2A84D8(void);

void sdfAlarmWakeup(s32 arg0, s32 arg1, s32 thid);

void sdfDelayThread(u32 delay);

s32 sdfGetRunTimeCountBase(void);

s32 sdfGetRunTimeCount(s32 aBaseTime);

void sdfSystemThreadProc(void);

void dds3InitSubsystems(u32 heapSize, u32 initialHeapSize, u32 packetBufferBytes);

void sdfStartThreadAndWait(sdfThCB_t *node, void (*entry)(void), void *stack, s64 stackSize, s32 priority, s32 threadArgument);

void sdfWaitVblank(s32 count);

void func_002A8758(sdfThCB_t *node);

sdfThCB_t *func_002A87F0(s32 threadId);

void sdf_2A8858(void);

s32 VblankIntcHandler(s32 cause);

void VblankThreadProc(void);

s32 sdfInitThreadSystem(void);

void sdfSetMainThreadPriority(void);

#endif
