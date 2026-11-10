#include "shared/sdk/libmc/decls.h"

/* SetAlarm delivers the common argument in its third callback register. */
void mcHearAlarm(s32 cause, s32 address, s32 thread)
{
    iWakeupThread(thread);
    EE_SYNC_EI();
}
