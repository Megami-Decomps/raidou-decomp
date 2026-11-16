#include "common.h"
#include "kernel.h"
#include "sdfChip.h"
#include "dds3AdminiProcess.h"
#include "dds3KernelCore.h"

/* Named scheduler slots: retail strides entries by 0x44 bytes. */
typedef struct
{
    s32 kind;
    char name[32];
    dds3ProcessID_t *task;
    dds3ProcessFunc_t update;
    dds3ProcessFunc_t destroy;
    void (*setState)(s32);
    s32 enabled;
    void *userValue;
    s32 previous;
    s32 next;
} fclNamedSlot_t;

extern fclNamedSlot_t fclEncycTable[32];
extern s32 fclEncycHead;
extern s32 fclEncycTail;
extern s32 fclEncycCount;
extern s32 fclEncycFree;
extern const char dds3DebugOwnerProcessName[];
extern char dds3AdminiProcessName[];

#include "sdk_libc.h"

extern dds3AdminiList_t dds3AdminiList[];



dds3AdminiData_t *GetAdminiProcessData(void);
void dds3AdminiCallSequence(s32 sequence, void *data, u32 datasize, s32 restore);
saddr_t dds3AdminiStartSequence(dds3ProcessID_t *id);
saddr_t dds3AdminiNeoProcess(dds3ProcessID_t *id);
saddr_t dds3AdminiEndCheck(dds3ProcessID_t *id);
void dds3AdminiNeoShutdown(dds3ProcessID_t *id);
s32 fclEncycFreeEntry(const char *name);
void dds3AdminiOverwriteSequence(s32 sequence, void *data, u32 datasize);
void dds3AdminiExitSequence(void);
void dds3InitAdmini(void);
extern const char dds3DebugOwnerProcessName[24];
extern char dds3AdminiProcessName[8];
extern u8 dds3PadInitDone;

dds3AdminiData_t *GetAdminiProcessData(void)
{
    return dds3GetProcessData(dds3SearchProcessName(dds3AdminiProcessName));
}

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", dds3AdminiCallSequence2);

/* Requests retain byte-sized sequence/size storage: 256 bytes records zero.
 * Release the old payload before copying the new one. */
void dds3AdminiCallSequence(s32 sequence, void *data, u32 datasize, s32 restore)
{
    dds3AdminiData_t *work;
    void *previousData;
    u32 flag;

    if (data == NULL || datasize <= 0x100) {
        work = GetAdminiProcessData();
        previousData = work->CallData;
        work->NextSequence = sequence;
        flag = work->Stat;
        flag |= 1;
        flag &= ~8;
        flag &= ~0x10000;
        work->Stat = flag;
        work->CallCount = 2;
        if (previousData != NULL) {
            sdfChipFreeDirect(previousData);
            work->CallData = NULL;
            work->CallDataSize = 0;
        }
        if (data != NULL) {
            work->CallData = sdfChipAllocDirect(datasize);
            memcpy(work->CallData, data, datasize);
            work->CallDataSize = datasize;
        } else {
            work->CallData = NULL;
            work->CallDataSize = 0;
        }
        if (restore != 0) {
            work->Stat |= 4;
        } else {
            work->Stat &= ~4;
        }
    }
}

void dds3AdminiOverwriteSequence(s32 sequence, void *data, u32 datasize)
{
    dds3AdminiData_t *work;

    dds3AdminiCallSequence(sequence, data, datasize, 0);
    work = GetAdminiProcessData();
    work->Stat |= 8;
}

void dds3AdminiExitSequence(void)
{
    dds3AdminiData_t *work;

    work = GetAdminiProcessData();
    work->Stat |= 2;
}

s8 dds3AdminiGetActiveSequence(void)
{
    return GetAdminiProcessData()->NowSequence;
}

s8 dds3AdminiGetNextSequence(void)
{
    return GetAdminiProcessData()->NextSequence;
}

s8 dds3AdminiGetPrevSequence(void)
{
    dds3AdminiData_t *work;

    work = GetAdminiProcessData();
    return work->SequenceHistory[(work->HistoryPointer + 7) & 7];
}

s32 dds3AdminiGetPrevHistoryFlag(void)
{
    dds3AdminiData_t *work;

    work = GetAdminiProcessData();
    return work->FlagHistory[(work->HistoryPointer + 7) & 7];
}

saddr_t dds3AdminiStartSequence(dds3ProcessID_t *id)
{
    dds3AdminiData_t *work;
    s32 (*startfunc)(s32 restored, void *reqdata);
    u32 flag;
    u32 nextflag;
    s32 restored;

    work = dds3GetProcessData(id);
    flag = work->Stat;
    if (flag & 1) {
        if (work->NextSequence >= 0) {
            if (work->CallCount != 0) {
                work->CallCount--;
                return 0;
            }
            work->NowSequence = work->NextSequence;
            work->NextSequence = -1;
            nextflag = flag;
            nextflag &= ~1;
            nextflag &= ~2;
            work->Stat = nextflag;
            work->RunTimer = 0;
            if (flag & 4) {
                work->FlagHistory[work->HistoryPointer] |= 1;
                work->Stat &= ~4;
            }
            if (work->Stat & 0x10000) {
                restored = 1;
                work->Stat &= ~0x10000;
                work->HistoryPointer = (work->HistoryPointer + 7) & 7;
            } else {
                restored = 0;
                if (work->Stat & 8) {
                    work->Stat &= ~8;
                } else {
                    work->HistoryPointer = (work->HistoryPointer + 1) & 7;
                }
            }
            work->SequenceHistory[work->HistoryPointer] = work->NowSequence;
            work->FlagHistory[work->HistoryPointer] = 0;
            startfunc = dds3AdminiList[work->NowSequence].start;
            if (startfunc != NULL) {
                startfunc(restored, work->CallData);
            }
        }
    }
    return EE_ADDR_S(dds3AdminiNeoProcess);
}

saddr_t dds3AdminiEndCheck(dds3ProcessID_t *id)
{
    dds3AdminiData_t *work = dds3GetProcessData(id);
    s32 sequence = work->NowSequence;
    s32 (*endfunc)(void);
    s32 wait;

    if (sequence >= 0) {
        endfunc = dds3AdminiList[sequence].end;
        if (endfunc != NULL) {
            wait = endfunc();
            if (wait < 0) {
                return 0;
            }
            work->CallCount = wait + 1;
            work->NowSequence = -1;
        }
    }
    return EE_ADDR_S(dds3AdminiStartSequence);
}

saddr_t dds3AdminiNeoProcess(dds3ProcessID_t *id)
{
    dds3AdminiData_t *work;
    s32 (*mainfunc)(void);
    s32 index;

    work = dds3GetProcessData(id);
    if (!(work->Stat & 2)) {
        if (work->NowSequence >= 0) {
            mainfunc = dds3AdminiList[work->NowSequence].check;
            if (mainfunc != NULL) {
                if (mainfunc() != 0) {
                    work->Stat |= 2;
                }
            }
            work->RunTimer++;
        }
    }
    if (work->Stat & 2) {
        if (work->NowSequence >= 0) {
            index = (work->HistoryPointer + 7) & 7;
            if (work->SequenceHistory[index] >= 0 && (work->FlagHistory[index] & 1)) {
                work->Stat |= 0x10001;
                work->NextSequence = work->SequenceHistory[index];
                work->CallCount = 2;
                work->Stat &= ~4;
                if (work->CallData != NULL) {
                    sdfChipFreeDirect(work->CallData);
                    work->CallData = NULL;
                    work->CallDataSize = 0;
                }
            }
            if (!(work->Stat & 1)) {
                dds3AdminiCallSequence(0, NULL, 0, 0);
            }
            work->Stat &= ~2;
        }
    }
    if (work->Stat & 1) {
        if (work->NextSequence >= 0) {
            return EE_ADDR_S(dds3AdminiEndCheck);
        }
    }
    return 0;
}

void dds3AdminiNeoShutdown(dds3ProcessID_t *id)
{
    dds3AdminiData_t *work = dds3GetProcessData(id);
    s32 sequence = work->NowSequence;

    if (sequence >= 0) {
        s32 (*endfunc)(void) = dds3AdminiList[sequence].end;

        if (endfunc != NULL) {
            endfunc();
        }
    }
    if (work->CallData != NULL) {
        sdfChipFreeDirect(work->CallData);
    }
    sdfChipFreeDirect(dds3GetProcessData(id));
}

void dds3InitAdmini(void)
{
    dds3AdminiData_t *work;
    s32 i;

    if (dds3SearchProcessName(dds3AdminiProcessName) == NULL) {
        work = sdfChipAllocDirect(sizeof(dds3AdminiData_t));
        work->Stat = 0;
        work->RunTimer = 0;
        work->NowSequence = -1;
        work->NextSequence = -1;
        work->HistoryPointer = 0;
        for (i = 0; i < 8; i++) {
            work->SequenceHistory[i] = -1;
            work->FlagHistory[i] = 0;
        }
        work->CallData = NULL;
        work->CallDataSize = 0;
        dds3StartProcess(dds3AdminiProcessName, DDS3_PRIORITY_ADMINI, 0, 1, dds3AdminiNeoProcess,
                         (dds3ProcessFunc_t)dds3AdminiNeoShutdown, work);
    }
}

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", fclEncycFindFreeSlot);

s32 fclEncycFreeEntry(const char *name)
{
    s32 slotIndex = fclEncycHead;
    s32 characterIndex;

    while (slotIndex >= 0) {
        characterIndex = 0;
        while (fclEncycTable[slotIndex].name[characterIndex] != '\0') {
            if (name[characterIndex] != fclEncycTable[slotIndex].name[characterIndex]) {
                break;
            }
            characterIndex++;
        }
        if (fclEncycTable[slotIndex].name[characterIndex] == '\0' && name[characterIndex] == '\0') {
            return slotIndex;
        }
        slotIndex = fclEncycTable[slotIndex].next;
    }
    return -1;
}

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", fclEncycFreeEntries);

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", dds3CallEncycFindFreeSlot);

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", fclEncycAllocEntry);

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", dds3CallEncycFindFreeSlot2);

s32 CallDebug(s32 restored, void *reqdata);
s32 ExitDebug(void);
s32 CheckDebug(void);
s32 CallBootMc(s32 restored, void *reqdata);
s32 ExitBootMc(void);
s32 CheckBootMc(void);
s32 CallTitle(s32 restored, void *reqdata);
s32 ExitTitle(void);
s32 CheckTitle(void);
s32 CallLoad(s32 restored, void *reqdata);
s32 ExitLoad(void);
s32 CheckLoad(void);
s32 CallSave(s32 restored, void *reqdata);
s32 ExitSave(void);
s32 CheckSave(void);
s32 CallEndSave(s32 restored, void *reqdata);
s32 ExitEndSave(void);
s32 CheckEndSave(void);
s32 CallEpSave(s32 restored, void *reqdata);
s32 ExitEpSave(void);
s32 CheckEpSave(void);
s32 CallField(s32 restored, void *reqdata);
s32 ExitField(void);
s32 CheckField(void);
s32 CallFieldMcLoad(s32 restored, void *reqdata);
s32 ExitFieldMcLoad(void);
s32 CheckFieldMcLoad(void);
s32 CallEvent(s32 restored, void *reqdata);
s32 ExitEvent(void);
s32 CheckEvent(void);
s32 CallCamp(s32 restored, void *reqdata);
s32 ExitCamp(void);
s32 CheckCamp(void);
s32 CallModeSelect(s32 restored, void *reqdata);
s32 dds3CallShowSceneModels(void);
s32 dds3Func14(void);
s32 dds3Leaf24(s32 restored, void *reqdata);
s32 dds3CallProcessEndRstInit(void);
s32 dds3CallProcessCheckRstInit(void);
s32 CallNewBattle(s32 restored, void *reqdata);
s32 ExitNewBattle(void);
s32 CheckNewBattle(void);
s32 CallStaff(s32 restored, void *reqdata);
s32 ExitStaff(void);
s32 CheckStaff(void);
s32 CallBattleFieldTest(s32 restored, void *reqdata);
s32 ExitBattleFieldTest(void);
s32 CheckBattleFieldTest(void);
s32 datLeaf2(s32 restored, void *reqdata);
s32 datCallShutDown(void);
s32 CheckLmap(void);
s32 CallJimusyo(s32 restored, void *reqdata);
s32 ExitJimusyo(void);
s32 CheckJimusyo(void);
s32 datLeaf37(s32 restored, void *reqdata);
s32 datFunc20(void);
s32 datFunc21(void);
s32 CallNewBattleTest(s32 restored, void *reqdata);
s32 ExitNewBattleTest(void);
s32 CheckNewBattleTest(void);
s32 sysLeaf24(s32 restored, void *reqdata);
s32 sysLeaf25(void);
s32 evtKeepBattleStageTestModePending(void);
s32 evtCreateTestAndSkyTasks(s32 restored, void *reqdata);
s32 evtInitializeSelectionScene(void);
s32 evtKeepTestSkyModePending(void);
s32 evtClearSecondaryWorldAndViewer(s32 restored, void *reqdata);
s32 evtInitializeSelectionScreen(void);
s32 sysLeaf26(void);
s32 sysLeaf27(s32 restored, void *reqdata);
s32 sysLeaf28(void);
s32 sysLeaf29(void);
s32 sysLeaf30(s32 restored, void *reqdata);
s32 sysFunc22(void);
s32 sysFunc23(void);
s32 sysLeaf31(s32 restored, void *reqdata);
s32 sysFunc24(void);
s32 sysFunc25(void);
s32 sysCallInitSky(s32 restored, void *reqdata);
s32 sysCallExitSky(void);
s32 sysLeaf32(void);
s32 sysLeaf33(s32 restored, void *reqdata);
s32 sysCallProcessEndRstInit(void);
s32 sysCallProcessCheckRstInit(void);
s32 sysLeaf34(s32 restored, void *reqdata);
s32 sysLeaf35(void);
s32 sysLeaf36(void);
s32 sysLeaf37(s32 restored, void *reqdata);
s32 sysLeaf38(void);
s32 sysLeaf39(void);
s32 sysLeaf40(s32 restored, void *reqdata);
s32 sysFunc26(void);
s32 sysFunc27(void);
s32 sysLeaf41(s32 restored, void *reqdata);
s32 sysCallFinishMovieViewer(void);
s32 sysLeaf42(void);
s32 sysLeaf43(s32 restored, void *reqdata);
s32 sysFunc28(void);
s32 sysLeaf44(void);
s32 datLeaf32(s32 restored, void *reqdata);
s32 datCallShutDown2(void);
s32 datLeaf33(void);
s32 datLeaf34(s32 restored, void *reqdata);
s32 datCallInit(void);
s32 datLeaf35(void);
s32 sysLeaf(s32 restored, void *reqdata);
s32 ExitLmap(void);
s32 datLeaf(void);
s32 datLeaf36(s32 restored, void *reqdata);
s32 datFunc18(void);
s32 datFunc19(void);
s32 datLeaf3(s32 restored, void *reqdata);
s32 datFunc(void);
s32 datLeaf4(void);
s32 datLeaf38(s32 restored, void *reqdata);
s32 ExitHanafudaTest(void);
s32 CheckHanafudaTest(void);
s32 datLeaf5(s32 restored, void *reqdata);
s32 datFunc2(void);
s32 datFunc3(void);
s32 CallGomaden(s32 restored, void *reqdata);
s32 ExitGomaden(void);
s32 datLeaf39(void);
s32 datRefDDS3GBWK(s32 restored, void *reqdata);
s32 datCallDebugProcessEnd(void);
s32 datCallDebugProcessCheck(void);
s32 datLeaf40(s32 restored, void *reqdata);
s32 datFunc22(void);
s32 datFunc23(void);
s32 datLeaf6(s32 restored, void *reqdata);
s32 datCallInit2(void);
s32 datLeaf7(void);
s32 dds3CallExitSky(void);
s32 evtCallTestProcess3(s32 restored, void *reqdata);
s32 evtCallTestProcess32(s32 restored, void *reqdata);
#ifdef VERSION_RAIDOU1
dds3AdminiList_t dds3AdminiList[44] = {
    { CallDebug, ExitDebug, CheckDebug },
    { CallBootMc, ExitBootMc, CheckBootMc },
    { CallTitle, ExitTitle, CheckTitle },
    { CallLoad, ExitLoad, CheckLoad },
    { CallSave, ExitSave, CheckSave },
    { CallEndSave, ExitEndSave, CheckEndSave },
    { CallEpSave, ExitEpSave, CheckEpSave },
    { CallField, ExitField, CheckField },
    { CallFieldMcLoad, ExitFieldMcLoad, CheckFieldMcLoad },
    { CallEvent, ExitEvent, CheckEvent },
    { CallCamp, ExitCamp, CheckCamp },
    { CallModeSelect, dds3CallShowSceneModels, dds3Func14 },
    { dds3Leaf24, dds3CallProcessEndRstInit, dds3CallProcessCheckRstInit },
    { CallNewBattle, ExitNewBattle, CheckNewBattle },
    { CallStaff, ExitStaff, CheckStaff },
    { CallBattleFieldTest, ExitBattleFieldTest, CheckBattleFieldTest },
    { datLeaf2, datCallShutDown, CheckLmap },
    { CallJimusyo, ExitJimusyo, CheckJimusyo },
    { datLeaf37, datFunc20, datFunc21 },
    { CallNewBattleTest, ExitNewBattleTest, CheckNewBattleTest },
    { sysLeaf24, sysLeaf25, evtKeepBattleStageTestModePending },
    { evtCreateTestAndSkyTasks, evtInitializeSelectionScene, evtKeepTestSkyModePending },
    { evtClearSecondaryWorldAndViewer, evtInitializeSelectionScreen, sysLeaf26 },
    { sysLeaf27, sysLeaf28, sysLeaf29 },
    { sysLeaf30, sysFunc22, sysFunc23 },
    { sysLeaf31, sysFunc24, sysFunc25 },
    { sysCallInitSky, sysCallExitSky, sysLeaf32 },
    { sysLeaf33, sysCallProcessEndRstInit, sysCallProcessCheckRstInit },
    { sysLeaf34, sysLeaf35, sysLeaf36 },
    { sysLeaf37, sysLeaf38, sysLeaf39 },
    { sysLeaf40, sysFunc26, sysFunc27 },
    { sysLeaf41, sysCallFinishMovieViewer, sysLeaf42 },
    { sysLeaf43, sysFunc28, sysLeaf44 },
    { datLeaf32, datCallShutDown2, datLeaf33 },
    { datLeaf34, datCallInit, datLeaf35 },
    { sysLeaf, ExitLmap, datLeaf },
    { datLeaf36, datFunc18, datFunc19 },
    { datLeaf3, datFunc, datLeaf4 },
    { datLeaf38, ExitHanafudaTest, CheckHanafudaTest },
    { datLeaf5, datFunc2, datFunc3 },
    { CallGomaden, ExitGomaden, datLeaf39 },
    { datRefDDS3GBWK, datCallDebugProcessEnd, datCallDebugProcessCheck },
    { datLeaf40, datFunc22, datFunc23 },
    { datLeaf6, datCallInit2, datLeaf7 },
};

char dds3AdminiProcessName[8] = "Admini";

s32 fclEncycHead = -1;

INCLUDE_RODATA(const s32, "kernel/dds3AdminiProcess", dds3DebugOwnerProcessName);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", fclEncycTail);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", fclEncycCount);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", fclEncycFree);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", dds3PadInitDone);
#else
dds3AdminiList_t dds3AdminiList[42] = {
    { CallDebug, ExitDebug, CheckDebug },
    { CallBootMc, ExitBootMc, CheckBootMc },
    { CallTitle, ExitTitle, CheckTitle },
    { CallLoad, ExitLoad, CheckLoad },
    { CallSave, ExitSave, CheckSave },
    { CallEndSave, ExitEndSave, CheckEndSave },
    { CallEpSave, ExitEpSave, CheckEpSave },
    { CallField, ExitField, CheckField },
    { CallFieldMcLoad, ExitFieldMcLoad, CheckFieldMcLoad },
    { CallEvent, dds3CallExitSky, CheckEvent },
    { CallCamp, ExitCamp, CheckCamp },
    { CallModeSelect, dds3CallShowSceneModels, dds3Func14 },
    { dds3Leaf24, dds3CallProcessEndRstInit, dds3CallProcessCheckRstInit },
    { CallNewBattle, ExitNewBattle, CheckNewBattle },
    { CallStaff, ExitStaff, CheckStaff },
    { CallBattleFieldTest, ExitBattleFieldTest, CheckBattleFieldTest },
    { datLeaf32, datCallShutDown2, datLeaf33 },
    { CallJimusyo, ExitJimusyo, CheckJimusyo },
    { datLeaf37, datFunc20, datFunc21 },
    { evtCallTestProcess3, datCallShutDown, CheckLmap },
    { evtCallTestProcess32, datCallShutDown, CheckLmap },
    { CallNewBattleTest, ExitNewBattleTest, CheckNewBattleTest },
    { sysLeaf24, sysLeaf25, evtKeepBattleStageTestModePending },
    { evtCreateTestAndSkyTasks, evtInitializeSelectionScene, evtKeepTestSkyModePending },
    { evtClearSecondaryWorldAndViewer, evtInitializeSelectionScreen, sysLeaf26 },
    { sysLeaf27, sysLeaf28, sysLeaf29 },
    { sysLeaf30, sysFunc22, sysFunc23 },
    { sysLeaf31, sysFunc24, sysFunc25 },
    { sysCallInitSky, sysCallExitSky, sysLeaf32 },
    { sysLeaf33, sysCallProcessEndRstInit, sysCallProcessCheckRstInit },
    { sysLeaf34, sysLeaf35, sysLeaf36 },
    { sysLeaf37, sysLeaf38, sysLeaf39 },
    { sysLeaf40, sysFunc26, sysFunc27 },
    { sysLeaf41, sysCallFinishMovieViewer, sysLeaf42 },
    { sysLeaf43, sysFunc28, sysLeaf44 },
    { datLeaf34, datCallInit, datLeaf35 },
    { datLeaf36, datFunc18, datFunc19 },
    { datLeaf38, ExitHanafudaTest, CheckHanafudaTest },
    { CallGomaden, ExitGomaden, datLeaf39 },
    { datRefDDS3GBWK, datCallDebugProcessEnd, datCallDebugProcessCheck },
    { datLeaf40, datFunc22, datFunc23 },
    { NULL, NULL, NULL },
};

char dds3AdminiProcessName[8] = "Admini";

s32 fclEncycHead = -1;

INCLUDE_RODATA(const s32, "kernel/dds3AdminiProcess", dds3DebugOwnerProcessName);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", fclEncycTail);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", fclEncycCount);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", fclEncycFree);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", dds3PadInitDone);
#endif

