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
extern dds3AdminiList_t dds3AdminiList[43];
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

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", StartSequence);

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

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", dds3AdminiNeoProcess);

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

char dds3AdminiProcessName[8] = "Admini";

s32 fclEncycHead = -1;

INCLUDE_RODATA(const s32, "kernel/dds3AdminiProcess", dds3DebugOwnerProcessName);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", fclEncycTail);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", fclEncycCount);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", fclEncycFree);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", dds3PadInitDone);

