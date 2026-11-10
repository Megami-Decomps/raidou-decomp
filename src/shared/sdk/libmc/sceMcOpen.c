#include "shared/sdk/libmc/decls.h"

s32 sceMcOpen(s32 a, s32 b, char *c, s32 d)
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
    strncpy(mcNameRequest.name, c, 0x3FF);
    mcNameRequest.port = a;
    mcNameRequest.flag = d;
    mcNameRequest.slot = b;
    mcNameRequest.name[0x3FF] = 0;
    ret = sceSifCallRpc(&mcRpcClient, 2, 1, &mcNameRequest, 0x414, &mcReplyBuf, 4, 0, 0);
    if (ret == 0) {
        mcLastCmd = 2;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}
