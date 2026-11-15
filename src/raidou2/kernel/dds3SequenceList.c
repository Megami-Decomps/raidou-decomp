#include "common.h"
#include "kernel.h"
#include "scr.h"
#include "event.h"
#include "field.h"
#include "dds3KernelCore.h"

extern void fmProcTrigSequence(s32 mode);
extern s32 fmProcessChk(void);
extern void titleExit(void);
extern void titleStart(u32 mode);

#include "sdk_libc.h"


extern s32 dds3StartMenuProcess(dds3ProcessID_t *id);
extern void fmShutDownFade(void);
extern void fmShutDown(void);
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

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckTitle);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallLoad);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitLoad);

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

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallField);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitField);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckField);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallFieldMcLoad);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitFieldMcLoad);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckFieldMcLoad);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallNewBattle);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitNewBattle);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckNewBattle);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallEvent);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", dds3CallExitSky);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckEvent);

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

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallNewBattleTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitNewBattleTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckNewBattleTest);

INCLUDE_SDATA(const s32, "kernel/dds3SequenceList", CheckNewBattleTestData);

