#include "shared/sdk/libkernl/decls.h"

s32 sceDread(s32 descriptor, void *directoryEntry)
{
    rpcSemaParam param;
    s32 result;
    fsDreadRequest_t *request = &fsRequest.dread;
    fsRequestEntry_t *entry;
    s32 sema;
    s32 accepted;

    entry = get_iob(descriptor);
    _sceFsWaitS(11);
    if (fsInitialized == 0) {
        _sceFsSigSema();
        return -1;
    }
    if (entry == NULL || entry->active == 0) {
        _sceFsSigSema();
        return -9;
    }
    request->entry = directoryEntry;
    request->descriptor = entry->word0;
    param.maxCount = 1;
    param.initCount = 0;
    param.option = 0;
    sema = CreateSema(&param);
    fsRequest.dread.sema = sema;
    request->result = &result;
    request->resultSize = 4;
    if (sceSifCallRpc(&fsRpcClient, 11, 0, request, 0x20,
                      fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        /* The failure path waits before releasing the filesystem lock. */
        WaitSema(sema);
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
