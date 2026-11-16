#include "common.h"
#include "kernel.h"
#include "scr.h"
#include "event.h"
#include "field.h"
#include "dds3KernelCore.h"
#include "slMain.h"
#include "fld.h"
#include "dds3SequenceList.h"
#include "TitleMain.h"
#include "dds3KernelMain.h"
#include "code_r1_00128378.h"
#include "fldProcess.h"
#include "sbtlWork.h"
#include "ps2asm.h"

extern s32 ProcessCheck_rstInit(void);

extern void fmProcTrigSequence(s32 mode);
extern s32 fmProcessChk(void);
extern void titleExit(void);
extern void titleStart(u32 mode);

#include "sdk_libc.h"


extern s32 dds3StartMenuProcess(dds3ProcessID_t *id);
extern s32 fmShutDown2(void);
dds3ProcessID_t *CallDebug(void);
s32 ExitDebug(void);
s32 CheckDebug(void);
void CallBootMc(void);
s32 ExitBootMc(void);
s32 CheckBootMc(void);
void CallTitle(u32 flg, u32 *data);
s32 ExitTitle(void);
void CallStaff(void);
s32 CheckStaff(void);
void CallEndSave(void);
s32 ExitEndSave(void);
s32 CheckEndSave(void);
void CallBattleFieldTest(void);
s32 ExitBattleFieldTest(void);
s32 CheckBattleFieldTest(void);

dds3ProcessID_t *CallDebug(void)
{
    return dds3StartProcess("StartSel", 0x2AF9, 0, 0, dds3StartMenuProcess, NULL, NULL);
}

s32 ExitDebug(void)
{
    dds3KillProcessName("StartSel", 1);
    return 1;
}

s32 CheckDebug(void)
{
    return 0;
}

void CallBootMc(void)
{
    fmProcTrigSequence(3);
}

s32 ExitBootMc(void)
{
    s32 done;
    s32 ret;

    fmShutDownFade();
    fmShutDown();
    done = fmShutDown2();
    ret = -1;
    if (done != 0) {
        ret = 0;
    }
    return ret;
}

s32 CheckBootMc(void)
{
    s32 operationResult;

    operationResult = fmProcessChk();
    return operationResult == 0;
}

void CallTitle(u32 flg, u32 *data)
{
    if (data == 0) {
        titleStart(0);
        return;
    }
    titleStart(*data);
}

s32 ExitTitle(void)
{
    titleExit();
    return 0;
}

s32 CheckTitle(void)
{
    return TitleProcessCheck() == 0;
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallLoad);

s32 ExitLoad(void)
{
    fmShutDownFade();
    fmShutDown();
    if (fmShutDown2() != 0) {
        mapRetuenFromLoad();
        return 0;
    }
    return -1;
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckLoad);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallSave);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitSave);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckSave);

void CallStaff(void) {
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitStaff);

s32 CheckStaff(void)
{
    return 0;
}

void CallEndSave(void)
{
    fmProcTrigSequence(2);
}

s32 ExitEndSave(void)
{
    s32 done;
    s32 ret;

    fmShutDownFade();
    fmShutDown();
    done = fmShutDown2();
    ret = -1;
    if (done != 0) {
        ret = 0;
    }
    return ret;
}

s32 CheckEndSave(void)
{
    s32 operationResult;

    operationResult = fmProcessChk();
    return operationResult == 0;
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallEpSave);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitEpSave);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckEpSave);

void CallField(DDS3_ADMINI_CALL flg, struct fldSceneParam_t *param)
{
    if (param == NULL && flg == DDS3_ADMINI_CALL_NEW) {
        InitOoyTest();
        return;
    }
    dds3BistaIn(0);
    fldProcTrigSequence(param, flg);
}

s32 ExitField(void)
{
    fldAllShutDown();
    if (fldCallDelayedResrcCheck() != 0) {
        if (mapExitFieldSub() != 0) {
            mapRetuenFromLoad();
        }
        return 0;
    }
    return -1;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/kernel/dds3SequenceList.c:269, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
s32 CheckField(void)
{
    return 0;
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallFieldMcLoad);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitFieldMcLoad);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckFieldMcLoad);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallNewBattle);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitNewBattle);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckNewBattle);

#ifdef VERSION_RAIDOU1
void CallEvent(DDS3_ADMINI_CALL flg, s32 *data)
{
    s32 eventNo;

    dds3KernelDebugSign = 0;
    eventNo = 0;
    switch (flg) {
    case DDS3_ADMINI_CALL_NEW:
        eventNo = *data;
        break;
    case DDS3_ADMINI_CALL_RESTORE:
        eventNo = evtPendingEventSelection;
        break;
    }
    if (eventNo <= 0) {
        return;
    }
    if (eventNo < 100) {
        fldCallElvReset2(eventNo);
        return;
    }
    evtInitSky();
    evtStartEvent(eventNo);
}
#else
INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallEvent);
#endif

#ifdef VERSION_RAIDOU1
s32 ExitEvent(void)
{
    dds3KernelDebugSign = 1;
    evtExitSky();
    fldExitEventSub();
    return 0;
}
#else
INCLUDE_ASM(const s32, "kernel/dds3SequenceList", dds3CallExitSky);
#endif

/* body: matched in smt3 @ 9401f79e: src/smt3/kernel/dds3SequenceList.c:317, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
s32 CheckEvent(void)
{
    return 0;
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallCamp);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitCamp);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckCamp);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallModeSelect);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", dds3CallShowSceneModels);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", dds3Func14);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", dds3Leaf24);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", dds3CallProcessEndRstInit);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", dds3CallProcessCheckRstInit);

void CallBattleFieldTest(void) {
}

s32 ExitBattleFieldTest(void)
{
    return 0;
}

s32 CheckBattleFieldTest(void)
{
    return 0;
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallJimusyo);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitJimusyo);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckJimusyo);

void CallNewBattleTest(DDS3_ADMINI_CALL flg, SBTL_START_DATA *start)
{
    SBTL_START_DATA data;
    sdfVector4_t pos = { 0.0f, 0.0f, 0.0f, 0.0f };
    sdfVector4_t rot = { 0.0f, 0.0f, 0.0f, 1.0f };

    if (flg == 0) {
        if (start == NULL) {
            data.val28 = 0;
            SDF_VU0_LOADVECTOR(&pos);
            SDF_VU0_STOREVECTOR(&data.pos);
            SDF_VU0_LOADVECTOR(&rot);
            SDF_VU0_STOREVECTOR(&data.rot);
#ifdef VERSION_RAIDOU2
            data.mode = 1;
#endif
            data.val20 = 0;
            data.val24 = 0;
            sbtlSbattle(&data, 0);
        } else {
            sbtlSbattle(start, 1);
        }
    } else {
        dds3AdminiExitSequence();
        fldExitEventSub();
    }
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitNewBattleTest);

s32 CheckNewBattleTest(void)
{
    if (dds3SearchProcessName("sbattle") == NULL && dds3SearchProcessPriority(0x3F8) == NULL && ProcessCheck_rstInit() == 0) {
        fldExitEventSub();
        return 1;
    }
    return 0;
}

