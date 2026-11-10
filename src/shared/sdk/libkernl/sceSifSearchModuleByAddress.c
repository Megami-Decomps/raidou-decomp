#include "shared/sdk/libkernl/decls.h"

s32 sceSifSearchModuleByAddress(void *address)
{
    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    lfRequest.value = EE_ADDR_S(address);
    if (sceSifCallRpc(&lfRpcClient, 10, 0, &lfRequest, 4,
                      &lfRequest, 4, NULL, NULL) < 0) {
        return -65537;
    }
    return lfRequest.value;
}
