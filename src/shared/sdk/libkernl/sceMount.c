#include "shared/sdk/libkernl/decls.h"

s32 sceMount(const char *device, const char *blockDevice, s32 mode, const void *args, s32 argSize)
{
    rpcSemaParam param;
    s32 result;
    fsMountRequest_t *request = &fsRequest.mount;
    s32 deviceLength;
    s32 blockLength;
    s32 argIndex;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    _sceFsWaitS(20);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (deviceLength = 0; deviceLength < 0x400 &&
         (request->device[deviceLength] = device[deviceLength]) != 0; deviceLength++) {
    }
    if (deviceLength == 0x400) {
        request->device[0x3FF] = 0;
    }
    for (blockLength = 0; blockLength < 0x400 &&
         (request->blockDevice[blockLength] = blockDevice[blockLength]) != 0; blockLength++) {
    }
    if (blockLength == 0x400) {
        request->blockDevice[0x3FF] = 0;
    }
    if (argSize > 0x400) {
        _sceFsSigSema();
        return -7;
    }
    for (argIndex = 0; argIndex < argSize; argIndex++) {
        request->args[argIndex] = ((const u8 *)args)[argIndex];
    }
    request->argSize = argSize;
    request->mode = mode;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    packet = &fsRequest;
    sema = CreateSema(&param);
    request->sema = sema;
    request->result = &result;
    request->resultSize = 4;
    sceSifWriteBackDCache(packet, 0xC14);
    if (sceSifCallRpc(&fsRpcClient, 20, 0, packet,
                      0xC14, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
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
