#include "shared/sdk/libkernl/decls.h"

s32 sceSifUnloadModule(s32 moduleId)
{
    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    lfRequest.value = moduleId;
    if (sceSifCallRpc(&lfRpcClient, 8, 0, &lfRequest, 4,
                      &lfRequest, 4, NULL, NULL) < 0) {
        return -65537;
    }
    return lfRequest.value;
}
