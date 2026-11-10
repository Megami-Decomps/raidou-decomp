#include "shared/sdk/libkernl/decls.h"

s32 _sceSifLoadModuleBuffer(void *buffer, s32 argumentSize, const void *arguments, s32 *moduleResult)
{
    s32 result;

    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    lfRequest.value = EE_ADDR_S(buffer);
    if (arguments != NULL) {
        if (argumentSize > 0xFC) {
            memcpy(lfRequest.arguments, arguments, 0xFC);
            lfRequest.moduleResult = 0xFC;
        } else {
            memcpy(lfRequest.arguments, arguments, argumentSize);
            lfRequest.moduleResult = argumentSize;
        }
    } else {
        lfRequest.moduleResult = 0;
    }
    if (sceSifCallRpc(&lfRpcClient, 6, 0, &lfRequest, 0x200,
                      &lfRequest, 8, NULL, NULL) < 0) {
        return -65537;
    }
    result = lfRequest.value;
    *moduleResult = lfRequest.moduleResult;
    return result;
}
