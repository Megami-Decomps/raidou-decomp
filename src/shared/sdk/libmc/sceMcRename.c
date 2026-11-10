#include "shared/sdk/libmc/decls.h"

s32 sceMcRename(s32 port, s32 slot, char *name, char *newname)
{
    s32 ret;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    if (name == NULL || *name == 0 || newname == NULL) {
        SignalSema(mcSema);
        return -210;
    }
    mcNameRequest.port = port;
    mcNameRequest.slot = slot;
    mcNameRequest.flag = 16;
    strncpy(mcNameRequest.name, name, 0x3FF);
    mcNameRequest.name[0x3FF] = 0;
    strncpy(mcEntInfoBuf.name, newname, 0x20);
    mcEntInfoBuf.name[0x1F] = 0;
    mcNameRequest.pwd = &mcEntInfoBuf;
    FlushCache(0);
    ret = sceSifCallRpc(&mcRpcClient, 14, 1, &mcNameRequest, 0x414, &mcReplyBuf, 4, 0, 0);
    if (ret == 0) {
        mcLastCmd = 19;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}
