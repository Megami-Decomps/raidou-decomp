#include "shared/sdk/libmc/decls.h"

s32 sceMcGetSlotMax(s32 port)
{
    s32 ret;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    mcRequest.w[1] = port;
    ret = sceSifCallRpc(&mcRpcClient, 21, 0, &mcRequest, 48, &mcReplyBuf, 4, 0, 0);
    if (ret != 0) {
        SignalSema(mcSema);
        return ret;
    }
    SignalSema(mcSema);
    return mcReplyBuf.result;
}
