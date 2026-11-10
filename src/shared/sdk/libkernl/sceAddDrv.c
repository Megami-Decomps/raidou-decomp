#include "shared/sdk/libkernl/decls.h"

s32 sceAddDrv(void *driver)
{
    rpcSemaParam param;
    s32 result;
    fsCloseRequest_t *request = &fsRequest.close;
    s32 sema;
    s32 accepted;

    _sceFsWaitS(15);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    request->descriptor = EE_ADDR(driver);
    param.maxCount = 1;
    param.initCount = 0;
    param.option = 0;
    sema = CreateSema(&param);
    fsRequest.close.sema = sema;
    request->result = &result;
    request->resultSize = 4;
    if (sceSifCallRpc(&fsRpcClient, 15, 0, request, 16,
                      fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -1;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -1;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}
