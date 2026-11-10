#include "shared/sdf/sdfSystem/decls.h"

void sdfDelayThread(u32 delay) {
    s32 threadId = GetThreadId();
    CancelWakeupThread(threadId);
    SetAlarm(delay & 0xFFFF, sdfAlarmWakeup, EE_ADDR_AS(void *, threadId));
    SleepThread();
}
