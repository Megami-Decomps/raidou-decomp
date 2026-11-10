#include "shared/sdk/libkernl/decls.h"

s32 sceClose(s32 descriptor)
{
    rpcSemaParam param;
    s32 result;
    fsCloseRequest_t *request = &fsRequest.close;
    fsRequestEntry_t *entry;
    s32 sema;
    s32 accepted;
    s32 status;
    s32 threshold;

    entry = get_iob(descriptor);
    _sceFsWaitS(1);
    if (fsInitialized == 0) {
        _sceFsSigSema();
        return -1;
    }
    if (entry == NULL || entry->active == 0) {
        _sceFsSigSema();
        return -9;
    }
    request->descriptor = entry->word0;
    request->slot = entry - fsIobTable;
    param.maxCount = 1;
    param.initCount = 0;
    param.option = 0;
    sema = CreateSema(&param);
    fsRequest.close.sema = sema;
    request->result = &result;
    request->resultSize = 4;
    if (sceSifCallRpc(&fsRpcClient, 1, 0, request, 0x14,
                      fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    entry->active = 0;
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    status = result;
    threshold = -1;
    /* Keep the real native signed threshold in a general register for its
     * comparison. No instruction, fixed register or memory effect is added. */
    FAKEMATCH_ASM_LIVE(threshold);
    if (threshold < status) {
        status = 0;
    }
    return status;
}
