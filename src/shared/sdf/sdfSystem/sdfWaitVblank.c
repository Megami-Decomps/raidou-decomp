#include "shared/sdf/sdfSystem/decls.h"

void sdfWaitVblank(s32 count)
{
    s32 rest = count - CancelWakeupThread(GetThreadId());

    do {
        SleepThread();
        rest--;
    } while (rest > 0);
}
