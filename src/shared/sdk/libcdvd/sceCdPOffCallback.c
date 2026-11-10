#include "shared/sdk/libcdvd/decls.h"

cdvdPowerCallback_t sceCdPOffCallback(cdvdPowerCallback_t callback, void *argument)
{
    cdvdPowerCallback_t old;
    s32 intr;

    if (cdvdScmdState[0] < 0) {
        PowerOffCB();
    }
    intr = DIntr();
    old = cdvdPowerCallback;
    cdvdPowerArgument = argument;
    cdvdPowerCallback = callback;
    if (intr) {
        EIntr();
    }
    return old;
}
