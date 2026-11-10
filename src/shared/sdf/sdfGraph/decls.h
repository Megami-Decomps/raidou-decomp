/* Declarations of smt3-decomp src/smt3/sdf/sdfGraph.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFGRAPH_H
#define SHARED_SDF_SDFGRAPH_H

#include "common.h"

#include "eeregs.h"

#include "libsdf.h"

#include "draw.h"

#include "ps2asm.h"

typedef struct
{
    u64 pmode;
    u64 smode2;
    u32 dispfb;
    u32 dispfbHi;
    u64 display;
    u64 bgcolor;
} sceGsDispEnv;

extern void sceGsResetGraph(s16 aMode, s16 aInter, s16 aOmode, s16 aFfmode);

extern void sceGsSetDefDispEnv(sceGsDispEnv *aEnv, s32 aPsm, s32 aWidth, s32 aHeight, s32 aDx,
                               s32 aDy);

extern void sceGsPutDispEnv(sceGsDispEnv *aEnv);

extern void sdfGsMemFree(sdfGsMemSlice_t *aHandle);

extern sdfGsMemSlice_t *sdfGsAllocFrameBuff(s32 aW, s32 aH, s32 aPixelFormat);

extern sdfGrScreenDef_t sdfGrFrameModes[4];

extern sdfGrFrameSet_t sdfGrFrameSet;

extern sceGsDispEnv sdfGrDispEnv;

extern u8 reqMakeFrameBuff;

extern u8 sdfGrScreenMode;

extern u8 sdfGrGsResetDone;

extern s8 sdfDispFrameId[2];

extern u8 sdfFrameEvenOdd;

extern volatile u8 sdfReqDrawDelayCount;

extern s32 WakeupThread(s32 thread);

extern void sdfWaitVblank(s32 mode);

extern volatile s8 sdfGrDrawBusy;

extern s32 sdfGrCaptureReq;

/* The capture hook, held as an address and called through a cast. */
typedef void (*sdfGrCaptureFunc_t)(u8 *);

extern s32 sdfGrCaptureTarget;

extern s8 sdfGrCaptureBuff;

void sdfGrSetFrameSetMode(sdfGrFrameSet_t *aFrame, s32 aMode, s32 aReqHandleClear);

void sdfGrFreeFrameSet(sdfGrFrameSet_t *aFrame);

/* GS store-image packet and tracked-thread node are only passed to the SDK. */
typedef struct sceGsStoreImage sceGsStoreImage;

extern sceGsStoreImage sdfGrStoreImage;

extern sdfThCB_t sdfDispThreadCB;

extern u8 sdfDrawThreadStack[0x8000];

extern u8 sdfDispThreadStack[0x1000];

extern volatile u8 sdfDispThreadFlag;

extern volatile u8 sdfFrameId;

extern s8 reqGsReset;

extern volatile s32 dds3GsSema;

extern s32 dds3GsThreadId;

extern s32 sdfDrawThreadId;

extern volatile u8 sdfDrawSyncFlag;

extern volatile u8 sdfDrawFrameFlag;

extern u16 sdfDrawFrameCounter;

extern s32 iWakeupThread(s32 thread);

extern s32 DmacAddHandler(s32 type, s32 channel, s32 (*callback)(s32), s32 priority, s32 options);

extern s32 EnableDmac(s32 channel);

extern s32 GetThreadId(void);

extern s32 CancelWakeupThread(s32 thread);

extern s32 SleepThread(void);

extern s32 WaitSema(s32 sema);

extern s32 SignalSema(s32 sema);

extern u32 sdfGetRunTimeCountBase(void);

extern u32 sdfGetRunTimeCount(u32 aBaseTime);

extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

extern void *sdfMemLock(sdfMemHandle_t *aHandle);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern s32 sceGsSetDefStoreImage(sceGsStoreImage *, s16, s16, s16, s16, s16, s16, s16);

extern s32 sceGsExecStoreImage(sceGsStoreImage *, void *);

extern s32 sceGsSyncPath(s32 mode, s32 timeout);

extern void sceGsResetPath(void);

extern void *memcpy(void *a, const void *b, size_t c);

extern void sdfGrDrawFrame(void);

extern void sdfGrMakePacketBuff(s32 aPacketBuffSize);

extern void sdfGrInitDraw(void);

extern void sdfTexInit(void);

extern s32 sdfCreateSema(s32 initCount, s32 maxCount, s32 option);

extern s32 CreateThreadSimple(void *entry, void *stack, s32 stackSize, s32 priority);

#include "sdk_libkernl.h"

extern void sdfDispThread(void);

extern void sdfGrSetupPacketBuff(s32 aFrameId);

extern void sdfCoreDrawReset(void);

void sdfGrSetFrameSetMode(sdfGrFrameSet_t *aFrame, s32 aMode, s32 aReqHandleClear);

void sdfGrSetScreen(s32 aMode);

void sdfGrFreeFrameSet(sdfGrFrameSet_t *aFrame);

void sdfGrMakeFrameSet(sdfGrFrameSet_t *aFrame);

void sdfGrSetDisp(s32 aEvenOdd);

void sdfGrGsReset(void);

s32 sdfDrawThIntHandler(s32 channel);

void sdfGrReqCapture(s32 aBuff, s32 aCallback);

void sdfGrCapture(void);

void sdfDeliverDrawStart(s32 frameId, s32 buffId);

void sdfDeliverDrawEnd(s32 mode, s32 frameId, s32 halfOffset);

void sdfDrawThread(void);

void sdfDispThread(void);

void sdfGrInit(s32 aPacketBuffSize);

void sdfGrInitNewFrame(void);

#endif
