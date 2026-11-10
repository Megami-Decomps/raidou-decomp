#include "shared/sdk/libkernl/decls.h"

s32 _sceSifLoadModule(const char *path, s32 argumentSize, const void *arguments,
                      s32 *moduleResult, s32 command)
{
    s32 result;

    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    strncpy(lfRequest.name, path, 0xFC);
    lfRequest.name[0xFB] = 0;
    if (arguments != NULL) {
        if (argumentSize > 0xFC) {
            memcpy(lfRequest.arguments, arguments, 0xFC);
            lfRequest.value = 0xFC;
        } else {
            memcpy(lfRequest.arguments, arguments, argumentSize);
            lfRequest.value = argumentSize;
        }
    } else {
        lfRequest.arguments[0] = 0;
        lfRequest.value = 0;
    }
    if (sceSifCallRpc(&lfRpcClient, command, 0, &lfRequest, 0x200,
                      &lfRequest, 8, NULL, NULL) < 0) {
        return -65537;
    }
    result = lfRequest.value;
    *moduleResult = lfRequest.moduleResult;
    return result;
}
