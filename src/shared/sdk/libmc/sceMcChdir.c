#include "shared/sdk/libmc/decls.h"

s32 sceMcChdir(s32 a, s32 b, char *c, char *d)
{
    s32 ret;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    if (c == NULL || *c == 0) {
        SignalSema(mcSema);
        return -210;
    }
    mcNameRequest.port = a;
    mcNameRequest.pwd = mcPwdBuf;
    mcNameRequest.slot = b;
    strncpy(mcNameRequest.name, c, 0x3FF);
    mcNameRequest.name[0x3FF] = 0;
    sceSifWriteBackDCache(mcPwdBuf, 0x400);
    ret = sceSifCallRpc(&mcRpcClient, 12, 1, &mcNameRequest, 0x414, &mcReplyBuf, 4,
                        mceStorePwd, d);
    if (ret == 0) {
        mcLastCmd = 12;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}
