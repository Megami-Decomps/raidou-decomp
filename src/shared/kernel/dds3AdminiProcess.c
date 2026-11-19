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

/* One initializer for the three slot kinds and for unregistering. */
static inline void fclEncycSetEntry(s32 index, s32 kind, dds3ProcessFunc_t update, dds3ProcessFunc_t destroy,
                                    void (*setState)(s32), void *userValue) {
    fclEncycTable[index].kind = kind;
    fclEncycTable[index].task = NULL;
    fclEncycTable[index].update = update;
    fclEncycTable[index].destroy = destroy;
    fclEncycTable[index].setState = setState;
    fclEncycTable[index].enabled = 0;
    fclEncycTable[index].userValue = userValue;
}

/* NON_MATCHING: 111 of 113 words match.  */
#ifdef NON_MATCHING
s32 fclEncycFindFreeSlot(const char *name) {
    s32 slotIndex;
    s32 characterIndex;
    s32 previous;

    if (dds3SearchProcessName(dds3DebugOwnerProcessName) == NULL) {
        return -1;
    }
    if (fclEncycFreeEntry(name) >= 0) {
        return -1;
    }
    for (slotIndex = 0; slotIndex < 32; slotIndex++) {
        if (fclEncycTable[slotIndex].kind == 0) {
            break;
        }
    }
    if (slotIndex == 32) {
        return -1;
    }
    fclEncycTable[slotIndex].kind = 0;
    characterIndex = 0;
    while ((fclEncycTable[slotIndex].name[characterIndex] = name[characterIndex]) != '\0') {
        if (characterIndex == 31) {
            fclEncycTable[slotIndex].name[characterIndex] = '\0';
            break;
        }
        characterIndex++;
    }
    fclEncycTable[slotIndex].task = NULL;
    fclEncycTable[slotIndex].update = NULL;
    fclEncycTable[slotIndex].destroy = NULL;
    fclEncycTable[slotIndex].setState = NULL;
    fclEncycTable[slotIndex].enabled = 0;
    fclEncycTable[slotIndex].userValue = NULL;
    fclEncycTable[slotIndex].previous = -1;
    fclEncycTable[slotIndex].next = -1;
    previous = fclEncycTail;
    if (previous < 0) {
        fclEncycHead = slotIndex;
        fclEncycTail = slotIndex;
        fclEncycFree = slotIndex;
    } else {
        fclEncycTail = slotIndex;
        fclEncycTable[previous].next = slotIndex;
        fclEncycTable[slotIndex].previous = previous;
    }
    fclEncycCount++;
    return slotIndex;
}
#else
INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", fclEncycFindFreeSlot);
#endif

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

/* NON_MATCHING: 120 of 122 words match.  */
#ifdef NON_MATCHING
s32 fclEncycFreeEntries(const char *name) {
    s32 slotIndex;

    if (dds3SearchProcessName(dds3DebugOwnerProcessName) == NULL) {
        return 0;
    }
    slotIndex = fclEncycFreeEntry(name);
    if (slotIndex < 0) {
        return 0;
    }
    if (fclEncycTable[slotIndex].kind > 0) {
        if (fclEncycTable[slotIndex].kind < 3) {
            if (dds3ExistProcess(fclEncycTable[slotIndex].task) == 1) {
                dds3KillProcess(fclEncycTable[fclEncycFree].task, 1);
            }
        }
    }
    fclEncycTable[slotIndex].name[0] = 0;
    fclEncycSetEntry(slotIndex, 0, NULL, NULL, NULL, NULL);
    if (fclEncycTable[slotIndex].previous < 0) {
        fclEncycHead = fclEncycTable[slotIndex].next;
    } else {
        fclEncycTable[fclEncycTable[slotIndex].previous].next = fclEncycTable[slotIndex].next;
    }
    if (fclEncycTable[slotIndex].next < 0) {
        fclEncycTail = fclEncycTable[slotIndex].previous;
    } else {
        fclEncycTable[fclEncycTable[slotIndex].next].previous = fclEncycTable[slotIndex].previous;
    }
    fclEncycTable[slotIndex].previous = -1;
    fclEncycTable[slotIndex].next = -1;
    if (fclEncycFree == slotIndex) {
        fclEncycFree = fclEncycHead;
    }
    fclEncycCount--;
    return 1;
}
#else
INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", fclEncycFreeEntries);
#endif

/* NON_MATCHING: 30 of 37 words match.  */
#ifdef NON_MATCHING
s32 dds3CallEncycFindFreeSlot(const char *name, void (*setState)(s32)) {
    s32 index = fclEncycFindFreeSlot(name);
    if (index < 0) {
        return 0;
    }
    fclEncycSetEntry(index, 3, NULL, NULL, setState, 0);
    return 1;
}
#else
INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", dds3CallEncycFindFreeSlot);
#endif

/* NON_MATCHING: 31 of 42 words match.  */
#ifdef NON_MATCHING
s32 fclEncycAllocEntry(const char *name, dds3ProcessFunc_t update, dds3ProcessFunc_t destroy, void *userValue) {
    s32 index = fclEncycFindFreeSlot(name);
    if (index < 0) {
        return 0;
    }
    fclEncycSetEntry(index, 1, update, destroy, NULL, userValue);
    return 1;
}
#else
INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", fclEncycAllocEntry);
#endif

/* NON_MATCHING: 32 of 43 words match.  */
#ifdef NON_MATCHING
s32 dds3CallEncycFindFreeSlot2(const char *name, dds3ProcessFunc_t update, dds3ProcessFunc_t destroy, void *userValue) {
    s32 index = fclEncycFindFreeSlot(name);
    if (index < 0) {
        return 0;
    }
    fclEncycSetEntry(index, 2, update, destroy, NULL, userValue);
    return 1;
}
#else
INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", dds3CallEncycFindFreeSlot2);
#endif

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
s32 CallShop(s32 restored, void *reqdata);
s32 ExitShop(void);
s32 CheckShop(void);
s32 CallResult2(s32 restored, void *reqdata);
s32 ExitResult2(void);
s32 CheckResult2(void);
s32 CallNewBattle(s32 restored, void *reqdata);
s32 ExitNewBattle(void);
s32 CheckNewBattle(void);
s32 CallStaff(s32 restored, void *reqdata);
s32 ExitStaff(void);
s32 CheckStaff(void);
s32 CallConfig(s32 restored, void *reqdata);
s32 ExitConfig(void);
s32 CheckConfig(void);
s32 CallMahjon(s32 restored, void *reqdata);
s32 ExitMahjon(void);
s32 CheckMahjon(void);
s32 CallJimusyo(s32 restored, void *reqdata);
s32 ExitJimusyo(void);
s32 CheckJimusyo(void);
s32 CallNameEntry(s32 restored, void *reqdata);
s32 ExitNameEntry(void);
s32 CheckNameEntry(void);
s32 CallNewBattleTest(s32 restored, void *reqdata);
s32 ExitNewBattleTest(void);
s32 CheckNewBattleTest(void);
s32 CallBattleFieldTest(s32 restored, void *reqdata);
s32 ExitBattleFieldTest(void);
s32 CheckBattleFieldTest(void);
s32 CallEventTest(s32 restored, void *reqdata);
s32 ExitEventTest(void);
s32 CheckEventTest(void);
s32 CallEventViewer(s32 restored, void *reqdata);
s32 ExitEventViewer(void);
s32 CheckEventViewer(void);
s32 CallInterfaceTest(s32 restored, void *reqdata);
s32 ExitInterfaceTest(void);
s32 CheckInterfaceTest(void);
s32 CallCampTest(s32 restored, void *reqdata);
s32 ExitCampTest(void);
s32 CheckCampTest(void);
s32 sysLeaf31(s32 restored, void *reqdata);
s32 ExitShopTest(void);
s32 CheckShopTest(void);
s32 CallTerminalTest(s32 restored, void *reqdata);
s32 ExitTerminalTest(void);
s32 CheckTerminalTest(void);
s32 CallResult2Test(s32 restored, void *reqdata);
s32 ExitResult2Test(void);
s32 CheckResult2Test(void);
s32 CallModelViewerTest(s32 restored, void *reqdata);
s32 ExitModelViewerTest(void);
s32 CheckModelViewerTest(void);
s32 CallSoundTest(s32 restored, void *reqdata);
s32 ExitSoundTest(void);
s32 CheckSoundTest(void);
s32 CallFontTest(s32 restored, void *reqdata);
s32 ExitFontTest(void);
s32 CheckFontTest(void);
s32 CallMovieViewerTest(s32 restored, void *reqdata);
s32 ExitMovieViewerTest(void);
s32 CheckMovieViewerTest(void);
s32 CallSprMotEditTest(s32 restored, void *reqdata);
s32 ExitSprMotEditTest(void);
s32 CheckSprMotEditTest(void);
s32 CallMorph(s32 restored, void *reqdata);
s32 ExitMorph(void);
s32 CheckMorph(void);
s32 CallCtnTest(s32 restored, void *reqdata);
s32 ExitCtnTest(void);
s32 CheckCtnTest(void);
s32 CallLmap(s32 restored, void *reqdata);
s32 ExitLmap(void);
s32 CheckLmap(void);
s32 CallNameEntryTest(s32 restored, void *reqdata);
s32 ExitNameEntryTest(void);
s32 CheckNameEntryTest(void);
s32 CallSEffect(s32 restored, void *reqdata);
s32 ExitSEffect(void);
s32 CheckSEffect(void);
s32 CallWater(s32 restored, void *reqdata);
s32 ExitWater(void);
s32 CheckWater(void);
s32 CallHanafudaTest(s32 restored, void *reqdata);
s32 ExitHanafudaTest(void);
s32 CheckHanafudaTest(void);
s32 CallGomaden(s32 restored, void *reqdata);
s32 ExitGomaden(void);
s32 CheckGomaden(void);
s32 CallJimusyoTest(s32 restored, void *reqdata);
s32 ExitJimusyoTest(void);
s32 CheckJimusyoTest(void);
s32 CallStorySelect(s32 restored, void *reqdata);
s32 ExitStorySelect(void);
s32 CheckStorySelect(void);
s32 CallDitectiveBook(s32 restored, void *reqdata);
s32 ExitDitectiveBook(void);
s32 CheckDitectiveBook(void);
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
    { CallShop, ExitShop, CheckShop },
    { CallResult2, ExitResult2, CheckResult2 },
    { CallNewBattle, ExitNewBattle, CheckNewBattle },
    { CallStaff, ExitStaff, CheckStaff },
    { CallConfig, ExitConfig, CheckConfig },
    { CallMahjon, ExitMahjon, CheckMahjon },
    { CallJimusyo, ExitJimusyo, CheckJimusyo },
    { CallNameEntry, ExitNameEntry, CheckNameEntry },
    { CallNewBattleTest, ExitNewBattleTest, CheckNewBattleTest },
    { CallBattleFieldTest, ExitBattleFieldTest, CheckBattleFieldTest },
    { CallEventTest, ExitEventTest, CheckEventTest },
    { CallEventViewer, ExitEventViewer, CheckEventViewer },
    { CallInterfaceTest, ExitInterfaceTest, CheckInterfaceTest },
    { CallCampTest, ExitCampTest, CheckCampTest },
    { sysLeaf31, ExitShopTest, CheckShopTest },
    { CallTerminalTest, ExitTerminalTest, CheckTerminalTest },
    { CallResult2Test, ExitResult2Test, CheckResult2Test },
    { CallModelViewerTest, ExitModelViewerTest, CheckModelViewerTest },
    { CallSoundTest, ExitSoundTest, CheckSoundTest },
    { CallFontTest, ExitFontTest, CheckFontTest },
    { CallMovieViewerTest, ExitMovieViewerTest, CheckMovieViewerTest },
    { CallSprMotEditTest, ExitSprMotEditTest, CheckSprMotEditTest },
    { CallMorph, ExitMorph, CheckMorph },
    { CallCtnTest, ExitCtnTest, CheckCtnTest },
    { CallLmap, ExitLmap, CheckLmap },
    { CallNameEntryTest, ExitNameEntryTest, CheckNameEntryTest },
    { CallSEffect, ExitSEffect, CheckSEffect },
    { CallWater, ExitWater, CheckWater },
    { CallHanafudaTest, ExitHanafudaTest, CheckHanafudaTest },
    { CallGomaden, ExitGomaden, CheckGomaden },
    { CallJimusyoTest, ExitJimusyoTest, CheckJimusyoTest },
    { CallStorySelect, ExitStorySelect, CheckStorySelect },
    { CallDitectiveBook, ExitDitectiveBook, CheckDitectiveBook },
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
    { CallShop, ExitShop, CheckShop },
    { CallResult2, ExitResult2, CheckResult2 },
    { CallNewBattle, ExitNewBattle, CheckNewBattle },
    { CallStaff, ExitStaff, CheckStaff },
    { CallConfig, ExitConfig, CheckConfig },
    { CallMorph, ExitMorph, CheckMorph },
    { CallJimusyo, ExitJimusyo, CheckJimusyo },
    { CallNameEntry, ExitNameEntry, CheckNameEntry },
    { evtCallTestProcess3, ExitMahjon, CheckMahjon },
    { evtCallTestProcess32, ExitMahjon, CheckMahjon },
    { CallNewBattleTest, ExitNewBattleTest, CheckNewBattleTest },
    { CallBattleFieldTest, ExitBattleFieldTest, CheckBattleFieldTest },
    { CallEventTest, ExitEventTest, CheckEventTest },
    { CallEventViewer, ExitEventViewer, CheckEventViewer },
    { CallInterfaceTest, ExitInterfaceTest, CheckInterfaceTest },
    { CallCampTest, ExitCampTest, CheckCampTest },
    { sysLeaf31, ExitShopTest, CheckShopTest },
    { CallTerminalTest, ExitTerminalTest, CheckTerminalTest },
    { CallResult2Test, ExitResult2Test, CheckResult2Test },
    { CallModelViewerTest, ExitModelViewerTest, CheckModelViewerTest },
    { CallSoundTest, ExitSoundTest, CheckSoundTest },
    { CallFontTest, ExitFontTest, CheckFontTest },
    { CallMovieViewerTest, ExitMovieViewerTest, CheckMovieViewerTest },
    { CallSprMotEditTest, ExitSprMotEditTest, CheckSprMotEditTest },
    { CallCtnTest, ExitCtnTest, CheckCtnTest },
    { CallNameEntryTest, ExitNameEntryTest, CheckNameEntryTest },
    { CallWater, ExitWater, CheckWater },
    { CallGomaden, ExitGomaden, CheckGomaden },
    { CallJimusyoTest, ExitJimusyoTest, CheckJimusyoTest },
    { CallStorySelect, ExitStorySelect, CheckStorySelect },
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

