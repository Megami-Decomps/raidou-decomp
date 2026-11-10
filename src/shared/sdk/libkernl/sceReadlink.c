#include "shared/sdk/libkernl/decls.h"

s32 sceReadlink(const char *path, char *buffer, u32 count)
{
    rpcSemaParam param;
    s32 result;
    fsReadlinkRequest_t *request = &fsRequest.readlink;
    s32 length;
    s32 sema;
    s32 accepted;

    _sceFsWaitS(17);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (length = 0; length < 0x400 &&
         (request->name[length] = path[length]) != 0; length++) {
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
    }
    if (count >= 0x400) {
        count = 0x3FF;
    }
    request->buffer = buffer;
    request->count = count;
    sceSifWriteBackDCache(buffer, count);
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    sema = CreateSema(&param);
    request->result = &result;
    request->sema = sema;
    request->resultSize = 4;
    /* The fixed transfer also includes the shared buffer's untouched tail. */
    if (sceSifCallRpc(&fsRpcClient, 25, 0, &fsRequest,
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
