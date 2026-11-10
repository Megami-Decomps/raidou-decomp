#include "shared/sdk/libkernl/decls.h"

s32 sceChstat(const char *path, const fsStat_t *stat, u32 mask)
{
    rpcSemaParam param;
    s32 result;
    fsChstatRequest_t *request = &fsRequest.chstat;
    s32 length;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    _sceFsWaitS(13);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (length = 0; length < 0x400 &&
         (request->name[length] = path[length]) != 0; length++) {
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
        length = 0x3FF;
    }
    request->stat = *stat;
    request->mask = mask;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    packet = &fsRequest;
    sema = CreateSema(&param);
    request->sema = sema;
    request->result = &result;
    request->resultSize = 4;
    sceSifWriteBackDCache(packet, 0x450);
    if (sceSifCallRpc(&fsRpcClient, 13, 0, packet,
                      length + 0x51, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}
