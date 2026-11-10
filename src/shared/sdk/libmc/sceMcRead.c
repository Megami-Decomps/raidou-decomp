#include "shared/sdk/libmc/decls.h"

s32 sceMcRead(s32 a, void *b, s32 c)
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
    mcRequest.w[7] = EE_ADDR_S(mcEndBuf);
    mcRequest.w[6] = EE_ADDR_S(b);
    mcRequest.w[3] = c;
    sceSifWriteBackDCache(b, c);
    sceSifWriteBackDCache(mcEndBuf, 0xC0);
    ret = sceSifCallRpc(&mcRpcClient, 5, 1, &mcRequest, 48, &mcReplyBuf, 4,
                        mceIntrReadFixAlign, mcEndBuf);
    if (ret == 0) {
        mcLastCmd = 5;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}
