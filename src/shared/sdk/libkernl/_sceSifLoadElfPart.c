#include "shared/sdk/libkernl/decls.h"

s32 _sceSifLoadElfPart(const char *path, const char *section, u32 *result, s32 command)
{
    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    strncpy(lfRequest.name, path, 0xFC);
    lfRequest.name[0xFB] = 0;
    strncpy((char *)lfRequest.arguments, section, 0xFC);
    lfRequest.arguments[0xFB] = 0;
    if (sceSifCallRpc(&lfRpcClient, command, 0, &lfRequest, 0x200,
                      &lfRequest, 16, NULL, NULL) < 0) {
        return -65537;
    }
    if (lfRequest.value == 0) {
        return -65539;
    }
    result[0] = lfRequest.value;
    result[1] = lfRequest.moduleResult;
    return 0;
}
