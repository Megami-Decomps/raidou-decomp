#include "shared/sdk/libmc/decls.h"

s32 sceMcGetEntSpace(s32 port, s32 slot, char *name)
{
    s32 ret;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    if (name == NULL || *name == 0) {
        SignalSema(mcSema);
        return -210;
    }
    mcNameRequest.port = port;
    mcNameRequest.slot = slot;
    strncpy(mcNameRequest.name, name, 0x3FF);
    mcNameRequest.name[0x3FF] = 0;
    ret = sceSifCallRpc(&mcRpcClient, 18, 1, &mcNameRequest, 0x414, &mcReplyBuf, 4, 0, 0);
    if (ret == 0) {
        mcLastCmd = 18;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}
