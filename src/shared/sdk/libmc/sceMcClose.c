#include "shared/sdk/libmc/decls.h"

s32 sceMcClose(s32 a)
{
    s32 ret;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    mcRequest.w[0] = a;
    ret = sceSifCallRpc(&mcRpcClient, 3, 1, &mcRequest, 48, &mcReplyBuf, 4, 0, 0);
    if (ret == 0) {
        mcLastCmd = 3;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}
