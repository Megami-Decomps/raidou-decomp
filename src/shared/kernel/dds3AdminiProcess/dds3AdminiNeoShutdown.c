#include "shared/kernel/dds3AdminiProcess/decls.h"

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
