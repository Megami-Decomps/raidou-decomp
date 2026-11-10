#include "shared/sdk/libkernl/decls.h"

s32 sceDevctl(const char *device, s32 command, const void *args, u32 argSize,
              void *output, u32 outputSize)
{
    rpcSemaParam param;
    s32 result;
    fsDevctlRequest_t *request;
    s32 length;
    u32 argIndex;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    _sceFsWaitS(23);
    request = &fsRequest.devctl;
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (length = 0; length < 0x400 &&
         (request->name[length] = device[length]) != 0; length++) {
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
    }
    if (argSize > 0x400 || outputSize > 0x400) {
        _sceFsSigSema();
        return -22;
    }
    for (argIndex = 0; argIndex < argSize; argIndex++) {
        request->args[argIndex] = ((const u8 *)args)[argIndex];
    }
    request->argSize = argSize;
    request->command = command;
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
    sceSifWriteBackDCache(packet, 0x81C);
    if (sceSifCallRpc(&fsRpcClient, 23, 0, packet,
                      0x81C, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
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
