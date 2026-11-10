#include "shared/sdk/libcdvd/decls.h"

s32 sceCdPowerOff(s32 *result)
{
    s32 ret;

    if (!_sceCd_scmd_prechk(33)) {
        return 0;
    }
    if (sceSifCallRpc(&cdvdScmdClient, 33, 0, 0, 0, cdvdScmdReply, 8, 0, 0) < 0) {
        SignalSema(cdvdScmdLockSema);
        return 0;
    }
    *result = UNCACHED(cdvdScmdReply + 1)[0];
    ret = UNCACHED(cdvdScmdReply)[0];
    SignalSema(cdvdScmdLockSema);
    return ret;
}
