#include "common.h"
#include "sdfMemory.h"
#include "sdk_libc.h"
#include "kernel.h"
#include "dds3KernelCore.h"
#include "dds3KernelMain.h"
#include "include_asm.h"
#include "code_r1_002C7ED0.h"
#include "nbDebugProcess.h"
#include "jmsMisc.h"
#include "TitleMain.h"
#include "code_r1_002C8188.h"
#include "fclMisc.h"
#include "evtEventViewer.h"

extern void cfgCopyFlagGblToBak(void);
extern char TitleInitData[];
extern const char TitleInitStrAtlusLgSpr[];
extern const char TitleInitStrTitle000Spr[];
extern const char TitleInitStrTitle001Spr[];
extern const char TitleInitStrTitle002Spr[];
extern const char TitleProcessStartStrTitleCalc[];
extern const char TitleProcessStartStrTitleDraw[];

titleData_t *TitleInit(s32 mode)
{
    sdfMemHandle_t *handle;
    titleData_t *data;

    handle = sdfMemAllocDirect(sizeof(titleData_t));
    data = sdfMemLock(handle);
    memset(data, 0, sizeof(titleData_t));
    data->hMem = handle;
    data->mode = mode;
    cfgCopyFlagGblToBak();
    mnuClearPanelTransitionState(&data->panel);
    data->spriteList = InitDebugProcessData(0);
    effAppendListEntry(data->spriteList, TitleInitData, TitleInitStrAtlusLgSpr, 0, &data->sprite[0]);
    effAppendListEntry(data->spriteList, TitleInitData, TitleInitStrTitle000Spr, 0, &data->sprite[1]);
    effAppendListEntry(data->spriteList, TitleInitData, TitleInitStrTitle001Spr, 0, &data->sprite[2]);
    effAppendListEntry(data->spriteList, TitleInitData, TitleInitStrTitle002Spr, 0, &data->sprite[3]);
    data->flag |= 1;
#ifdef VERSION_RAIDOU2
    dds3CallEventSub(0);
#else
    dds3KernelDebugSign = 0;
#endif
    return data;
}

#ifdef VERSION_RAIDOU1
void TitleDestroy(dds3ProcessID_t *PID) {
    titleData_t *data;
    u32 i;

    data = dds3GetProcessData(PID);
    for (i = 0; i < 4; i++) {
        if (data->sprite[i] != NULL) {
            effDestroyResourceSlotSet(data->sprite[i]);
        }
    }
    if (data->PmvPID != NULL) {
        evtEnd(data->PmvPID);
        data->PmvPID = NULL;
    }
    evtCallStop(data);
    fclFilterOFF();
    effDestroyEffectList(data->spriteList);
    dds3KernelDebugSign = 1;
    sdfMemFreeDirect(data->hMem);
}
#else
INCLUDE_ASM(const s32, "code_r1_002F7568", TitleDestroy);
#endif

void TitleProcessStart(s32 mode)
{
    titleData_t *data;

    data = TitleInit(mode);
    dds3StartProcess(TitleProcessStartStrTitleCalc, 1030, 0, 0, TitleFirstCalcProcess3, NULL, data);
    dds3StartProcess(TitleProcessStartStrTitleDraw, 11031, 0, 0, TitleFirstDrawProcess, (dds3ProcessFunc_t)TitleDestroy, data);
}

void TitleProcessEnd(void)
{
    dds3KillProcessName(TitleProcessStartStrTitleCalc, 0);
    dds3KillProcessName(TitleProcessStartStrTitleDraw, 0);
}

s32 TitleProcessCheck(void)
{
    return dds3SearchProcessName(TitleProcessStartStrTitleCalc) != NULL;
}
