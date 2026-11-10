#include "shared/sdk/libmc/decls.h"

s32 sceMcUnformat(s32 port, s32 slot)
{
    s32 ret;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    mcRequest.w[2] = slot;
    mcRequest.w[1] = port;
    ret = sceSifCallRpc(&mcRpcClient, 17, 1, &mcRequest, 48, &mcReplyBuf, 4, 0, 0);
    if (ret == 0) {
        mcLastCmd = 17;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}
