#include "shared/sdk/libcdvd/decls.h"

s32 sceCdNcmdDiskReady(void)
{
    s32 ret;

    if (!_sceCd_ncmd_prechk(2)) {
        return 0;
    }
    if (sceSifCallRpc(&cdvdNcmdClient, 14, 0, 0, 0, &cdvdNcmdReply, 4, 0, 0) < 0) {
        SignalSema(cdvdNcmdSema);
        return 0;
    }
    ret = UNCACHED(&cdvdNcmdReply)[0];
    SignalSema(cdvdNcmdSema);
    return ret;
}
