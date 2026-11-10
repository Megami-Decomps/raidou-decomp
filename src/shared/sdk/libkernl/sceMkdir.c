#include "shared/sdk/libkernl/decls.h"

s32 sceMkdir(const char *path, s32 mode)
{
    rpcSemaParam param;
    s32 result;
    fsMkdirRequest_t *request = &fsRequest.mkdir;
    s32 length;
    s32 sema;
    s32 accepted;

    _sceFsWaitS(7);
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
    request->mode = mode;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    sema = CreateSema(&param);
    request->result = &result;
    request->sema = sema;
    request->resultSize = 4;
    if (sceSifCallRpc(&fsRpcClient, 7, 0, &fsRequest,
                      length + 0x11, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
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
