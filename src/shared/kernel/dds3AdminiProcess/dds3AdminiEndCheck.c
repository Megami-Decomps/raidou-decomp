#include "shared/kernel/dds3AdminiProcess/decls.h"

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
