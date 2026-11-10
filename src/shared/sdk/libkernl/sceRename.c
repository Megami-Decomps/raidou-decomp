#include "shared/sdk/libkernl/decls.h"

s32 sceRename(const char *oldPath, const char *newPath)
{
    rpcSemaParam param;
    s32 result;
    fsRenameRequest_t *request = &fsRequest.rename;
    s32 oldLength;
    s32 newLength;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    _sceFsWaitS(17);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (oldLength = 0; oldLength < 0x400 &&
         (request->oldName[oldLength] = oldPath[oldLength]) != 0; oldLength++) {
    }
    if (oldLength == 0x400) {
        request->oldName[0x3FF] = 0;
    }
    for (newLength = 0; newLength < 0x400 &&
         (request->newName[newLength] = newPath[newLength]) != 0; newLength++) {
    }
    if (newLength == 0x400) {
        request->newName[0x3FF] = 0;
    }
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    packet = &fsRequest;
    sema = CreateSema(&param);
    request->sema = sema;
    request->result = &result;
    request->resultSize = 4;
    sceSifWriteBackDCache(packet, 0x80C);
    if (sceSifCallRpc(&fsRpcClient, 17, 0, packet,
                      0x80C, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
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
