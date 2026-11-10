#include "shared/sdk/libcdvd/decls.h"

void sceCdDelayThread(u16 ticks)
{
    sdfSemaParam_t param;
    s32 sema;

    param.maxCount = 1;
    param.initCount = 0;
    param.option = 0;
    sema = CreateSema(&param);
    SetAlarm(ticks, gcc2_compiled__6, EE_ADDR_AS(void *, sema));
    WaitSema(sema);
    DeleteSema(sema);
}
