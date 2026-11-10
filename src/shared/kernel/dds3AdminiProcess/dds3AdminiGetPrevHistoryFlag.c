#include "shared/kernel/dds3AdminiProcess/decls.h"

s32 dds3AdminiGetPrevHistoryFlag(void)
{
    dds3AdminiData_t *work;

    work = GetAdminiProcessData();
    return work->FlagHistory[(work->HistoryPointer + 7) & 7];
}
