#include "shared/sdk/libkernl/decls.h"

s32 sceSifGetIopAddr(void *address, void *output, u32 kind)
{
    if (_lf_bind() < 0) {
        return -65536;
    }
    if (kind >= 3) {
        return -65538;
    }
    lfRequest.value = EE_ADDR_S(address);
    lfRequest.moduleResult = kind;
    if (sceSifCallRpc(&lfRpcClient, 3, 0, &lfRequest, 32,
                      &lfRequest, 32, NULL, NULL) < 0) {
        return -65537;
    }
    if (kind == 0) {
        *(u8 *)output = *(u8 *)&lfRequest.value;
    } else if (kind == 1) {
        *(u16 *)output = *(u16 *)&lfRequest.value;
    } else if (kind == 2) {
        *(u32 *)output = lfRequest.value;
    } else {
        return -65538;
    }
    return 0;
}
