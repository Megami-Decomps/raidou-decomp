#include "shared/sdk/libkernl/decls.h"

s32 sceSifSetIopAddr(void *address, const void *input, u32 kind)
{
    if (_lf_bind() < 0) {
        return -65536;
    }
    lfRequest.value = EE_ADDR_S(address);
    lfRequest.moduleResult = kind;
    if (kind == 0) {
        *(u8 *)lfRequest.name = *(const u8 *)input;
    } else if (kind == 1) {
        *(u16 *)lfRequest.name = *(const u16 *)input;
    } else if (kind == 2) {
        *(u32 *)lfRequest.name = *(const u32 *)input;
    } else {
        return -65538;
    }
    {
        s32 status = sceSifCallRpc(&lfRpcClient, 2, 0, &lfRequest, 32,
                               &lfRequest, 16, NULL, NULL);
        /* Keep the selected post-RPC return value in its ABI register.
         * This is a target-specific matching aid, not an original-source claim. */
        FAKEMATCH_ASM_REG(s32, result, "v0") = status < 0 ? -65537 : 0;
        return result;
    }
}
