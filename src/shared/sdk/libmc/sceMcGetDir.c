#include "shared/sdk/libmc/decls.h"

s32 sceMcGetDir(s32 a, s32 b, char *c, s32 d, s32 e, void *f)
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
    mcNameRequest.slot = b;
    mcNameRequest.flag = d;
    mcNameRequest.maxent = e;
    mcNameRequest.pwd = f;
    strncpy(mcNameRequest.name, c, 0x3FF);
    mcNameRequest.name[0x3FF] = 0;
    if (e >= 0) {
        sceSifWriteBackDCache(f, e << 6);
    }
    ret = sceSifCallRpc(&mcRpcClient, 13, 1, &mcNameRequest, 0x414, &mcReplyBuf, 4, 0, 0);
    if (ret == 0) {
        mcLastCmd = 13;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}
