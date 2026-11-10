#include "shared/kernel/dds3AdminiProcess/decls.h"

s8 dds3AdminiGetPrevSequence(void)
{
    dds3AdminiData_t *work;

    work = GetAdminiProcessData();
    return work->SequenceHistory[(work->HistoryPointer + 7) & 7];
}
