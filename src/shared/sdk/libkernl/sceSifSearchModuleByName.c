#include "shared/sdk/libkernl/decls.h"

s32 sceSifSearchModuleByName(const char *name)
{
    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    strncpy(lfRequest.name, name, 0xFC);
    lfRequest.name[0xFB] = 0;
    if (sceSifCallRpc(&lfRpcClient, 9, 0, &lfRequest, 512,
                      &lfRequest, 4, NULL, NULL) < 0) {
        return -65537;
    }
    return lfRequest.value;
}
