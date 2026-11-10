#include "shared/sdf/sdfSystem/decls.h"

void sdfSetMainThreadPriority(void)
{
    s32 thid = GetThreadId();

    sdfMainThreadId = thid;
    ChangeThreadPriority(thid, 0x7C);
    SleepThread();
}
