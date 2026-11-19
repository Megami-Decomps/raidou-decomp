#include "common.h"
#include "kernel.h"
#include "sdfFrame.h"
#include "dds3KernelMain.h"
#include "sdfDraw.h"
#include "dds3WorldBasic.h"
#include "dds3AdminiProcess.h"
#include "dds3KernelDraw.h"
#include "dds3Randomize.h"
#include "dds3PadManager.h"
#include "mviManager.h"
#include "dds3KernelCore.h"
#include "fileManager.h"
#include "code_r1_00128378.h"
#include "mdlManager.h"
#include "sdfDevCons.h"
#include "sdfSystem.h"
#include "itf.h"
#include "sdk_libkernl.h"
#include "sdfFileIO.h"
#include "sdf_PadManager.h"
#include "cmpFile.h"

extern u32 dds3RunCount;
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
extern char dds3FirstProcessName[];
extern char dds3DrawFirstProcessName[];

#include "sdk_libc.h"

/* body: matched in smt3 @ 9401f79e: src/smt3/kernel/dds3KernelMain.c:120, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void KernelDebug(void) {
}

s32 dds3BgSetScrollSub(void)
{
    return 0;
}

s32 dds3LinkBoxZSub(void)
{
    return -1;
}

/* body: matched in raidou @ 88f0d54: src/shared/kernel/dds3KernelCore.c:814, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void dds3Leaf20(void) {
}

extern s32 dds3BasicMainProcess(dds3ProcessID_t *id);
extern s32 dds3BasicDrawProcess(dds3ProcessID_t *id);
extern s32 frFlipPacketBuf(dds3ProcessID_t *id);
extern void frDestroyFontTable(dds3ProcessID_t *id);
extern s32 dds3DebugDevDumpProcess(dds3ProcessID_t *id);
extern char dds3DrawProcessName[];
extern char dds3BasicMainProcessName[];
extern char dds3BasicDrawProcessName[];
extern char dds3FontSysProcessName[];
extern char dds3DebugDevDumpProcessName[];
extern s8 dds3SoftResetFlag;
extern s32 dds3FrameCount;

extern s32 dds3FirstProcess(dds3ProcessID_t *id);
extern s32 dds3DrawFirstProcess(dds3ProcessID_t *id);
extern s32 dds3DrawProcess(dds3ProcessID_t *id);
extern void dds3InitDrawEnv(void);
extern void frInitFontTable(s32 fontCount, s32 glyphCount);
extern void sdfFirstInitSub(void);
extern void itfMesMngInitializeManager(void);
extern void sndInit(void);
extern void dds3EffectManagerInit(void);
extern void dds3InitDebugSystem(void);
#ifdef VERSION_RAIDOU1
extern void sbtlCallBtlMapInit(void);
#else
extern void LoadOnMemoryData_sbtl(void);
extern void cmpFirstInitSub(void);
extern void cmpLoadSpriteToMem(void);
extern void fldGetSceneIndexedValue(void);
extern void jmsFirstInitSub(void);
extern void dds3FirstInitSub(void);
extern void sbtlBattleTmx(void);
extern void slFirstInitSub(void);
#endif
extern void fldSkyDefaultLoad(void);
extern void dds3GlobalWorkInit(void);
extern void evtMoonInitProcess(void);
extern void itfMoneyProcessStart(void);
extern void encInitProcess(void);
extern void mapFadeProc(void);
extern void fclMapAllShadow(void);
extern void fldBgTaskInit(void);
extern void cmbRefDDS3GBWK(void);
extern void fclFirstInitSub(void);
extern void effFirstInitSub(void);
extern void nbRefDDS3GBWK14(void);
extern void Init_effStat(void);
extern void cmpLoadScript(void);
extern void itfHanyoMenu(void);

void dds3FirstInit(void)
{
    dds3InitDrawEnv();
    dds3PadInitialize();
    dds3StartProcess(dds3FirstProcessName, 0, 0, 0, dds3FirstProcess, NULL, NULL);
    dds3StartProcess(dds3DrawFirstProcessName, 0x2710, 0, 0, dds3DrawFirstProcess, NULL, NULL);
    dds3StartProcess(dds3DrawProcessName, 0x4E20, 0, 0, dds3DrawProcess, NULL, NULL);
    dds3StartProcess(dds3BasicMainProcessName, 0x3FA, 1, 1, dds3BasicMainProcess, NULL, NULL);
    dds3StartProcess(dds3BasicDrawProcessName, 0x2B07, 1, 1, dds3BasicDrawProcess, NULL, NULL);
    frInitFontTable(0x100, 0x200);
    dds3StartProcess(dds3FontSysProcessName, 0x4E1F, 0, 0, frFlipPacketBuf, (dds3ProcessFunc_t)frDestroyFontTable, NULL);
    fileSysInit();
    sdfFirstInitSub();
    itfMesMngInitializeManager();
    sndInit();
    sdfDevConsInit();
    mdlInitManager();
    dds3EffectManagerInit();
    dds3StartMovieManager();
    dds3SoftResetFlag = 0;
    dds3FrameCount = 0;
    dds3AddWorld(0x2710);
    dds3InitRandCB();
    dds3InitAdmini();
    dds3AdminiCallSequence(0, NULL, 0, 0);
    dds3InitDebugSystem();
    dds3StartProcess(dds3DebugDevDumpProcessName, 0x5213, 0, 0, dds3DebugDevDumpProcess, NULL, NULL);
    fldSkyDefaultLoad();
    dds3FadeClear();
#ifdef VERSION_RAIDOU1
    sbtlCallBtlMapInit();
#else
    LoadOnMemoryData_sbtl();
#endif
    dds3GlobalWorkInit();
#ifdef VERSION_RAIDOU2
    itfCallLoadIconSprite2();
#endif
    evtMoonInitProcess();
    itfMoneyProcessStart();
    encInitProcess();
    mapFadeProc();
#ifdef VERSION_RAIDOU1
    fldFirstInit();
#else
    fclMapAllShadow();
#endif
    fldBgTaskInit();
#ifdef VERSION_RAIDOU1
    mphCallMemAllocDirect();
#endif
    cmbRefDDS3GBWK();
    fclFirstInitSub();
    effFirstInitSub();
    nbRefDDS3GBWK14();
    Init_effStat();
#ifdef VERSION_RAIDOU1
    itfCallLoadIconSprite2();
    pipeCampCAMPPACKLB();
#else
    cmpFirstInitSub();
    cmpLoadSpriteToMem();
#endif
    cmpLoadScript();
    itfHanyoMenu();
#ifdef VERSION_RAIDOU2
    fldGetSceneIndexedValue();
    jmsFirstInitSub();
    dds3FirstInitSub();
    sbtlBattleTmx();
    slFirstInitSub();
#endif
    dds3AdminiCallSequence(1, NULL, 0, 0);
}

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3NewGameStart);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3SoftReset);

/* body: matched in smt3 @ 9401f79e: src/smt3/kernel/dds3KernelMain.c:128, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
u32 dds3GetKernelCount(void)
{
    return dds3FrameCount;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/kernel/dds3KernelMain.c:133, adapt, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
u32 dds3GetFrameID(void)
{
    return dds3FrameID;
}

#ifdef VERSION_RAIDOU1
/* body: matched in raidou @ 88f0d54: src/shared/kernel/dds3KernelMain.c:189, match, same function 1.00 (words 5/5, calls 1.00, shape 1.00) */
#else
void dds3PauseOn(void);
void dds3PauseOff(void);
void dds3Stop(u32 frame);
void dds3SlowOn(u32 aSlowTime, u32 slowcnt);
void dds3SlowOff(void);
void dds3ScreenCaptureOn(void);
void dds3ScreenCaptureOff(void);
void dds3SoftResetOn(void);
void dds3SoftResetOff(void);
extern u32 dds3DebugSign;
extern char dds3FirstProcessName[8];
extern char dds3DrawFirstProcessName[8];
extern u8 dds3DrawProcessFlag;

#endif
void dds3PauseOn(void)
{
    dds3PauseTime = 0;
    dds3PauseSign = 1;
    dds3SlowSign = 0;
}

#ifdef VERSION_RAIDOU1
/* body: matched in raidou @ 88f0d54: src/shared/kernel/dds3KernelMain.c:196, match, same function 1.00 (words 4/4, calls 1.00, shape 1.00) */
#endif
void dds3PauseOff(void)
{
    dds3PauseSign = 0;
    dds3PauseTime = 0;
    dds3SlowSign = 0;
}

#ifdef VERSION_RAIDOU1
/* body: matched in raidou @ 88f0d54: src/shared/kernel/dds3KernelMain.c:203, match, same function 1.00 (words 5/5, calls 1.00, shape 1.00) */
#endif
void dds3Stop(u32 frame)
{
    dds3PauseTime = SDF_FRAMES(frame);
    dds3PauseSign = 1;
    dds3SlowSign = 0;
}

#ifdef VERSION_RAIDOU1
/* body: matched in raidou @ 88f0d54: src/shared/kernel/dds3KernelMain.c:210, match, same function 1.00 (words 9/9, calls 1.00, shape 1.00) */
#endif
void dds3SlowOn(u32 aSlowTime, u32 slowcnt)
{
    u32 runCount = dds3RunCount;

    dds3SlowSign = 1;
    dds3SlowTime = aSlowTime;
    dds3SlowRunCount = runCount;
    dds3SlowEndCount = slowcnt;
    dds3PauseSign = 0;
    dds3PauseTime = 0;
}

#ifdef VERSION_RAIDOU1
/* body: matched in raidou @ 88f0d54: src/shared/kernel/dds3KernelMain.c:222, match, same function 1.00 (words 6/6, calls 1.00, shape 1.00) */
#endif
void dds3SlowOff(void)
{
    dds3PauseSign = 0;
    dds3SlowTime = 4;
    dds3PauseTime = 0;
    dds3SlowSign = 0;
}

#ifdef VERSION_RAIDOU1
/* body: matched in raidou @ 88f0d54: src/shared/kernel/dds3KernelMain.c:230, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
#endif
void dds3ScreenCaptureOn(void) {
}

#ifdef VERSION_RAIDOU1
/* body: matched in raidou @ 88f0d54: src/shared/kernel/dds3KernelMain.c:233, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
#endif
void dds3ScreenCaptureOff(void) {
}

#ifdef VERSION_RAIDOU1
/* body: matched in raidou @ 385e897: src/shared/kernel/dds3KernelMain.c:203, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
#endif
void dds3SoftResetOn(void)
{
    dds3SoftResetDisableFlag = 0;
}

#ifdef VERSION_RAIDOU1
/* body: matched in raidou @ 385e897: src/shared/kernel/dds3KernelMain.c:208, match, same function 1.00 (words 3/3, calls 1.00, shape 1.00) */
#endif
#ifdef VERSION_RAIDOU1
void dds3SoftResetOff(void)
{
    dds3SoftResetDisableFlag = 1;
}

void dds3KernelMain(void)
{
    u32 base;
    s32 time;
    s32 sign;

    dds3FirstInit();
    dds3FrameID = 0;
    goto top;
frame:
    dds3FrameCount++;
    dds3FrameRunTime = sdfGetRunTimeCount(base) - sdfReqDrawDelaySleepCount;
    dds3LastDrawFrameCount = sdfDrawFrameCounter;
    dds3FrameID ^= 1;
    goto runcount;
pause:
    time = dds3PauseTime;
    sign = dds3PauseSign;
    if (time > 0) {
        time--;
        if (time == 0) {
            sign = 0;
        }
        dds3PauseTime = time;
        dds3PauseSign = sign;
    }
runcount:
    dds3RunCount++;
    if (dds3FileCmdLoPrio == 0) {
        sdfFileCmdThreadSetHiPrio();
    }
top:
    if (dds3SoftResetFlag != 0) {
        goto done;
    }
    sdfWaitVblank(dds3VblanksPerFrame);
    sdfGrReqDrawDelay(dds3VblanksPerFrame - 1);
    sdfPadWork();
    if (dds3FileCmdLoPrio != 0) {
        sdfFileCmdThreadSetLoPrio();
    }
    if (dds3PauseSign != 0) {
        goto pause;
    }
    base = sdfGetRunTimeCountBase();
    dds3RunProcessList();
    if (dds3ProcessMain() == 0) {
        goto done;
    }
    goto frame;
done:
    LoadExecPS2(dds3ResetExecPath, 0, 0);
}
#else
void dds3SoftResetOff(void)
{
    dds3SoftResetDisableFlag = 1;
}

void dds3CallEventSub(u8 enable)
{
    dds3CallEventSubData = enable;
}

void dds3KernelMain(void)
{
    u32 base;
    s32 time;
    s32 sign;

    dds3FirstInit();
    dds3FrameID = 0;
    goto top;
frame:
    dds3FrameCount++;
    dds3FrameRunTime = sdfGetRunTimeCount(base) - sdfReqDrawDelaySleepCount;
    dds3LastDrawFrameCount = sdfDrawFrameCounter;
    dds3FrameID ^= 1;
    goto runcount;
pause:
    time = dds3PauseTime;
    sign = dds3PauseSign;
    if (time > 0) {
        time--;
        if (time == 0) {
            sign = 0;
        }
        dds3PauseTime = time;
        dds3PauseSign = sign;
    }
runcount:
    dds3RunCount++;
    if (dds3FileCmdLoPrio == 0) {
        sdfFileCmdThreadSetHiPrio();
    }
top:
    if (dds3SoftResetFlag != 0) {
        goto done;
    }
    sdfWaitVblank(dds3VblanksPerFrame);
    sdfGrReqDrawDelay(dds3VblanksPerFrame - 1);
    sdfPadWork();
    if (dds3FileCmdLoPrio != 0) {
        sdfFileCmdThreadSetLoPrio();
    }
    if (dds3PauseSign != 0) {
        goto pause;
    }
    base = sdfGetRunTimeCountBase();
    dds3RunProcessList();
    if (dds3ProcessMain() == 0) {
        goto done;
    }
    goto frame;
done:
    LoadExecPS2(dds3ResetExecPath, 0, 0);
}
#endif
#ifdef VERSION_RAIDOU2
INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3FrameCount);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3RunCount);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3SoftResetFlag);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3SoftResetDisableFlag);

u32 dds3PauseSign = 0;

u32 dds3PauseTime = 0;

u32 dds3SlowSign = 0;

u32 dds3SlowTime = 4;

u32 dds3SlowRunCount = 0;

u32 dds3SlowEndCount = 0;
#endif

INCLUDE_RODATA(const s32, "kernel/dds3KernelMain", dds3DrawProcessName);

INCLUDE_RODATA(const s32, "kernel/dds3KernelMain", dds3BasicMainProcessName);

INCLUDE_RODATA(const s32, "kernel/dds3KernelMain", dds3BasicDrawProcessName);

INCLUDE_RODATA(const s32, "kernel/dds3KernelMain", dds3FontSysProcessName);

INCLUDE_RODATA(const s32, "kernel/dds3KernelMain", dds3DebugDevDumpProcessName);

INCLUDE_RODATA(const s32, "kernel/dds3KernelMain", dds3ResetExecPath);

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3FrameCount);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3RunCount);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3SoftResetFlag);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3SoftResetDisableFlag);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3PauseSign);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3PauseTime);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3SlowSign);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3SlowTime);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3SlowRunCount);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3SlowEndCount);

#endif
INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3DebugSign);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3FrameRunTime);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3LastDrawFrameCount);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3VblanksPerFrame);

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3KernelDebugSign);
#else
INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3CallEventSubData);
#endif

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3FileCmdLoPrio);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3FirstProcessName);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3DrawFirstProcessName);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3DrawProcessFlag);

