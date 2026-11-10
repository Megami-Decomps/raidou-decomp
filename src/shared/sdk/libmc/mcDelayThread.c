#include "shared/sdk/libmc/decls.h"

void mcDelayThread(u16 ticks)
{
    SetAlarm(ticks, mcHearAlarm, EE_ADDR_AS(void *, GetThreadId()));
    SleepThread();
}
