/* Declarations of smt3-decomp src/smt3/kernel/dds3KernelMain.c, for its bodies in this directory. */
#ifndef SHARED_KERNEL_DDS3KERNELMAIN_H
#define SHARED_KERNEL_DDS3KERNELMAIN_H

#include "common.h"

#include "kernel.h"

#include "sdfFrame.h"

extern u32 dds3FrameCount;

extern u32 dds3FrameID;

extern u32 dds3RunCount;

extern s8 dds3SoftResetFlag;

extern u8 dds3SoftResetDisableFlag;

extern u32 dds3PauseSign;

extern u32 dds3PauseTime;

extern u32 dds3SlowSign;

extern u32 dds3SlowTime;

extern u32 dds3SlowRunCount;

extern u32 dds3SlowEndCount;

extern u16 dds3VblanksPerFrame;

extern s16 dds3FrameRunTime;

extern s16 dds3LastDrawFrameCount;

extern s8 dds3FileCmdLoPrio;

extern u16 sdfReqDrawDelaySleepCount;

extern u16 sdfDrawFrameCounter;

extern char dds3HardResetMessage[];

extern char dds3ResetExecPath[];

extern char dds3FirstProcessName[];

extern char dds3DrawFirstProcessName[];

extern char dds3DrawProcessName[];

extern char dds3BasicMainProcessName[];

extern char dds3BasicDrawProcessName[];

extern char dds3FontSysProcessName[];

extern char dds3DebugDevDumpProcessName[];

extern void dds3InitDrawEnv(void);

extern void dds3PadInitialize(void);

extern dds3ProcessID_t *dds3StartProcess(const char *v1, s32 dDS3_PRIORITY_DRAW_PB,
    s32 v2, s32 v3, dds3ProcessFunc_t drawExit,
    dds3ProcessFunc_t deleteExit, s32 p);

extern s32 dds3FirstProcess(dds3ProcessID_t *id);

extern s32 dds3DrawFirstProcess(dds3ProcessID_t *id);

extern s32 dds3DrawProcess(dds3ProcessID_t *id);

extern s32 dds3BasicMainProcess(dds3ProcessID_t *id);

extern s32 dds3BasicDrawProcess(dds3ProcessID_t *id);

extern s32 dds3DebugDevDumpProcess(dds3ProcessID_t *proc_id);

extern s32 frFlipPacketBuf(dds3ProcessID_t *id);

extern void frDestroyFontTable(dds3ProcessID_t *id);

extern void frInitFontTable(s32 GsFontNums, s32 TextNums);

extern void fileSysInit(void);

extern void itfMesMngInitializeManager(void);

extern void sndInit(void);

extern void sdfDevConsInit(void);

extern void mdlInitManager(void);

extern void dds3EffectManagerInit(void);

extern void fldFirstInit(void);

extern void nbPnlLoad(void);

extern void dds3LoadBootFile(void);

extern void effBTLSystemInit(void);

extern void dds3StartMovieManager(void);

extern void evtPicturePriProcessInit(void);

extern void dds3GlobalWorkInit(void);

extern void dds3AddWorld(s32 bufcnt);

extern void dds3InitRandCB(void);

extern void dds3InitAdmini(void);

extern void dds3AdminiCallSequence(s32 sequence, void *data, u32 datasize, s32 restore);

extern void dds3InitDebugSystem(void);

extern void cmpTexLoad(void);

extern void dds3FadeClear(void);

extern void evtInitMoonProcess(void);

extern void encInitProcess(void);

extern u32 sdfGetRunTimeCountBase(void);

extern u32 sdfGetRunTimeCount(u32 aBaseTime);

extern void sdfWaitVblank(s32 count);

extern void sdfGrReqDrawDelay(s32 count);

extern void sdfPadWork(void);

extern void sdfFileCmdThreadSetHiPrio(void);

extern void sdfFileCmdThreadSetLoPrio(void);

extern void dds3RunProcessList(void);

extern s32 dds3ProcessMain(void);

extern void LoadExecPS2(const char *path, s32 argc, char **argv);

#include "sdk_libc.h"

void dds3FirstInit(void);

void KernelDebug(void);

void dds3SoftReset(void);

u32 dds3GetKernelCount(void);

u32 dds3GetFrameID(void);

void dds3PauseOn(void);

void dds3PauseOff(void);

void dds3Stop(u32 frame);

void dds3SlowOn(u32 aSlowTime, u32 slowcnt);

void dds3SlowOff(void);

void dds3ScreenCaptureOn(void);

void dds3ScreenCaptureOff(void);

void dds3SoftResetOn(void);

void dds3SoftResetOff(void);

void dds3KernelMain(void);

#endif
