#include "common.h"

/* Declarations from smt3-decomp src/smt3/kernel/dds3AdminiProcess.c */
#include "common.h"

#include "kernel.h"

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

extern char dds3AdminiHistoryOpenStr[];

extern char dds3AdminiHistoryCursorStr[];

extern char dds3AdminiHistoryEndStr[];

extern const char dds3AdminiHistoryEntryFmt[];

extern const char dds3AdminiRestoreMsg[];

#include "sdk_libc.h"

extern dds3AdminiList_t dds3AdminiList[];

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern dds3ProcessID_t *dds3SearchProcessName(char *namestr);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);

extern void *sdfChipAllocDirect(u32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern void *memcpy(void *a, const void *b, size_t c);

extern s32 dds3ExistProcess(dds3ProcessID_t *id);

extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);

dds3AdminiData_t *GetAdminiProcessData(void);

void dds3AdminiCallSequence(s32 sequence, void *data, u32 datasize, s32 restore);

saddr_t dds3AdminiStartSequence(dds3ProcessID_t *id);

saddr_t dds3AdminiNeoProcess(dds3ProcessID_t *id);

saddr_t dds3AdminiEndCheck(dds3ProcessID_t *id);

void dds3AdminiNeoShutdown(dds3ProcessID_t *id);

s32 fclEncycFreeEntry(const char *name);

dds3AdminiData_t *GetAdminiProcessData(void);

u32 dds3_102828(void);

/* Requests retain byte-sized sequence/size storage: 256 bytes records zero.
 * Release the old payload before copying the new one. */
void dds3AdminiCallSequence(s32 sequence, void *data, u32 datasize, s32 restore);

void dds3AdminiOverwriteSequence(s32 sequence, void *data, u32 datasize);

void dds3AdminiExitSequence(void);

s8 dds3AdminiGetActiveSequence(void);

s8 dds3AdminiGetNextSequence(void);

s8 dds3AdminiGetPrevSequence(void);

s32 dds3AdminiGetPrevHistoryFlag(void);

saddr_t dds3AdminiStartSequence(dds3ProcessID_t *id);

saddr_t dds3AdminiEndCheck(dds3ProcessID_t *id);

saddr_t dds3AdminiNeoProcess(dds3ProcessID_t *id);

void dds3AdminiNeoShutdown(dds3ProcessID_t *id);

void dds3InitAdmini(void);

s32 fclEncycFreeEntry(const char *name);

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

s32 CheckLoad_dds3SequenceList(void);

s32 CallLoad2(s32 restored, void *reqdata);

s32 ExitLoad2(void);

s32 CheckLoad2(void);

s32 CallEndSave(s32 restored, void *reqdata);

s32 ExitEndSave(void);

s32 CheckEndSave(void);

s32 CallField(s32 restored, void *reqdata);

s32 ExitField(void);

s32 CheckField(void);

s32 CallEvent(s32 restored, void *reqdata);

s32 ExitEvent(void);

s32 CheckEvent(void);

s32 CallCamp(s32 restored, void *reqdata);

s32 ExitCamp(void);

s32 CheckCamp(void);

s32 CallCombine(s32 restored, void *reqdata);

s32 ExitCombine(void);

s32 CheckCombine(void);

s32 CallShop(s32 restored, void *reqdata);

s32 ExitShop(void);

s32 CheckShop(void);

s32 CallRag(s32 restored, void *reqdata);

s32 ExitRag(void);

s32 CheckRag(void);

s32 CallRecover(s32 restored, void *reqdata);

s32 ExitRecover(void);

s32 CheckRecover(void);

s32 CallTerminal(s32 restored, void *reqdata);

s32 ExitTerminal(void);

s32 CheckTerminal(void);

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

s32 CallPipe(s32 restored, void *reqdata);

s32 ExitPipe(void);

s32 CheckPipe(void);

s32 CallNewBattleTest(s32 restored, void *reqdata);

s32 ExitNewBattleTest(void);

s32 CheckNewBattleTest(void);

s32 CallBattleFieldTest(s32 restored, void *reqdata);

s32 ExitBattleFieldTest(void);

s32 CheckBattleFieldTest(void);

s32 CallKernelTest(s32 restored, void *reqdata);

s32 ExitKernelTest(void);

s32 CheckKernelTest(void);

s32 CallEventTest(s32 restored, void *reqdata);

s32 ExitEventTest(void);

s32 CheckEventTest(void);

s32 CallEventViewer(s32 restored, void *reqdata);

s32 ExitEventViewer(void);

s32 CheckEventViewer(void);

s32 CallInterfaceTest(s32 restored, void *reqdata);

s32 ExitInterfaceTest(void);

s32 CheckInterfaceTest(void);

s32 CallMessageScriptTest(s32 restored, void *reqdata);

s32 ExitMessageScriptTest(void);

s32 CheckMessageScriptTest(void);

s32 CallCampTest(s32 restored, void *reqdata);

s32 ExitCampTest(void);

s32 CheckCampTest(void);

s32 CallCombineTest(s32 restored, void *reqdata);

s32 ExitCombineTest(void);

s32 CheckCombineTest(void);

s32 CallShopTest(s32 restored, void *reqdata);

s32 ExitShopTest(void);

s32 CheckShopTest(void);

s32 CallRagTest(s32 restored, void *reqdata);

s32 ExitRagTest(void);

s32 CheckRagTest(void);

s32 CallRecoverTest(s32 restored, void *reqdata);

s32 ExitRecoverTest(void);

s32 CheckRecoverTest(void);

s32 CallTerminalTest(s32 restored, void *reqdata);

s32 ExitTerminalTest(void);

s32 CheckTerminalTest(void);

s32 CallResult2Test(s32 restored, void *reqdata);

s32 ExitResult2Test(void);

s32 CheckResult2Test(void);

s32 CallNameTest(s32 restored, void *reqdata);

s32 ExitNameTest(void);

s32 CheckNameTest(void);

s32 CallModelViewerTest(s32 restored, void *reqdata);

s32 ExitModelViewerTest(void);

s32 CheckModelViewerTest(void);

s32 CallSoundTest(s32 restored, void *reqdata);

s32 ExitSoundTest(void);

s32 CheckSoundTest(void);

s32 CallBillboardTest(s32 restored, void *reqdata);

s32 ExitBillboardTest(void);

s32 CheckBillboardTest(void);

s32 CallFontTest(s32 restored, void *reqdata);

s32 ExitFontTest(void);

s32 CheckFontTest(void);

s32 CallPuzzleBoy(s32 restored, void *reqdata);

s32 ExitPuzzleBoy(void);

s32 CheckPuzzleBoy(void);

s32 CallMapEditor(s32 restored, void *reqdata);

s32 ExitMapEditor(void);

s32 CheckMapEditor(void);

s32 CallMovieViewerTest(s32 restored, void *reqdata);

s32 ExitMovieViewerTest(void);

s32 CheckMovieViewerTest(void);

s32 CallModeSelect(s32 restored, void *reqdata);

s32 ExitModeSelect(void);

s32 CheckModeSelect(void);

s32 CallTgs(s32 restored, void *reqdata);

s32 ExitTgs(void);

s32 CheckTgs(void);

extern dds3AdminiList_t dds3AdminiList[43];

extern const char dds3AdminiHistoryEntryFmt[16];

extern const char dds3AdminiRestoreMsg[24];

extern const char dds3DebugOwnerProcessName[24];

extern char dds3AdminiProcessName[8];

extern char dds3AdminiHistoryOpenStr[8];

extern char dds3AdminiHistoryCursorStr[8];

extern char dds3AdminiHistoryEndStr[4];

extern s32 fclEncycHead;

extern s32 fclEncycTail;

extern s32 fclEncycCount;

extern s32 fclEncycFree;

extern u8 D_0054E8F4;

extern u8 D_0054E8F5;

extern u8 D_0054E8F6;

extern u8 dds3PadInitDone;
/* End of declarations from src/smt3/kernel/dds3AdminiProcess.c */

dds3AdminiData_t *GetAdminiProcessData(void)
{
    return dds3GetProcessData(dds3SearchProcessName(dds3AdminiProcessName));
}

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", func_001029E0);

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

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", func_00102BF8);

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

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", func_00102DF0);

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

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", func_00103518);

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", fclEncycAllocEntry);

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", func_00103658);

INCLUDE_RODATA(const s32, "kernel/dds3AdminiProcess", D_004A6F98);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", D_004DE290);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", D_004DE298);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", D_004DE29C);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", D_004DE2A0);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", D_004DE2A4);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", D_004DE2A7);

