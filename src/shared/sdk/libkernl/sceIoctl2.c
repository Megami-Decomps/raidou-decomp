#include "shared/sdk/libkernl/decls.h"

s32 sceIoctl2(s32 descriptor, s32 command, const void *args, u32 argSize,
              void *output, u32 outputSize)
{
    rpcSemaParam param;
    s32 result;
    fsIoctl2Request_t *request = &fsRequest.ioctl2;
    fsRequestEntry_t *entry;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    entry = get_iob(descriptor);
    _sceFsWaitS(26);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    if (entry == NULL || entry->active == 0) {
        _sceFsSigSema();
        return -9;
    }
    if (argSize > 0x400 || outputSize > 0x400) {
        _sceFsSigSema();
        return -22;
    }
    if (args == NULL) {
        request->argSize = 0;
    } else {
        memcpy(request->args, args, argSize);
    }
    request->descriptor = entry->word0;
    request->command = command;
    request->argSize = argSize;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    packet = &fsRequest;
    sema = CreateSema(&param);
    request->outputSize = outputSize;
    request->result = &result;
    request->resultSize = 4;
    request->output = output;
    request->sema = sema;
    sceSifWriteBackDCache(packet, 0x420);
    if (sceSifCallRpc(&fsRpcClient, 26, 0, packet,
                      0x420, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
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
