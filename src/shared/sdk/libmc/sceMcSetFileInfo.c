#include "shared/sdk/libmc/decls.h"

s32 sceMcSetFileInfo(s32 port, s32 slot, char *name, mcEntInfo *info, s32 flags)
{
    s32 ret;
    /* Keep the live flags separate from the request cursor in $16. This
     * localized compiler constraint reconstructs the native allocation; it
     * does not establish the historical source spelling. $17 is callee-saved. */
    FAKEMATCH_ASM_REG(s32, fileFlags, "$17") = flags;

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
    mcNameRequest.flag = fileFlags & 7;
    mcEntInfoBuf = *info;
    mcNameRequest.pwd = &mcEntInfoBuf;
    strncpy(mcNameRequest.name, name, 0x3FF);
    mcNameRequest.name[0x3FF] = 0;
    FlushCache(0);
    ret = sceSifCallRpc(&mcRpcClient, 14, 1, &mcNameRequest, 0x414, &mcReplyBuf, 4, 0, 0);
    if (ret == 0) {
        mcLastCmd = 14;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}
