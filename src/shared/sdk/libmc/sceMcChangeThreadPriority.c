#include "shared/sdk/libmc/decls.h"

s32 sceMcChangeThreadPriority(s32 prio)
{
    s32 ret;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    mcRequest.w[5] = prio;
    ret = sceSifCallRpc(&mcRpcClient, 20, 1, &mcRequest, 48, &mcReplyBuf, 4, 0, 0);
    if (ret == 0) {
        mcLastCmd = 20;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}
