#include "shared/kernel/dds3AdminiProcess/decls.h"

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
