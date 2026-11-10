#include "shared/sdk/libmc/decls.h"

s32 sceMcGetInfo(s32 a, s32 b, s32 *c, s32 *d, s32 *e)
{
    s32 ret;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    mcRequest.w[1] = a;
    mcRequest.w[2] = b;
    mcRequest.w[7] = EE_ADDR_S(mcEndBuf);
    if (c != NULL) {
        mcRequest.w[5] = 1;
    } else {
        mcRequest.w[5] = 0;
    }
    if (d != NULL) {
        mcRequest.w[4] = 1;
    } else {
        mcRequest.w[4] = 0;
    }
    if (e != NULL) {
        mcRequest.w[3] = 1;
    } else {
        mcRequest.w[3] = 0;
    }
    mcReplyDst0 = EE_ADDR_S(c);
    mcReplyDst1 = EE_ADDR_S(d);
    mcReplyDst2 = EE_ADDR_S(e);
    sceSifWriteBackDCache(mcEndBuf, 0xC0);
    ret = sceSifCallRpc(&mcRpcClient, 1, 1, &mcRequest, 48, &mcReplyBuf, 4,
                        mceGetInfoApdx, mcEndBuf);
    if (ret == 0) {
        mcLastCmd = 1;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}
