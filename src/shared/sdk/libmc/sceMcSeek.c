#include "shared/sdk/libmc/decls.h"

s32 sceMcSeek(s32 fd, s32 offset, s32 origin)
{
    s32 ret;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    mcRequest.w[0] = fd;
    mcRequest.w[4] = offset;
    mcRequest.w[5] = origin;
    ret = sceSifCallRpc(&mcRpcClient, 4, 1, &mcRequest, 48, &mcReplyBuf, 4, 0, 0);
    if (ret == 0) {
        mcLastCmd = 4;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}
