#include "shared/sdk/libkernl/decls.h"

s32 sceSifFreeSysMemory(void *address)
{
    if (iopHeapStatus < 0) {
        return 0;
    }
    iopHeapArgs[0] = EE_ADDR(address);
    if (sceSifCallRpc(iopHeapRpcClient, 2, 0, iopHeapArgs, 4,
                      &iopHeapResult, 4, NULL, NULL) >= 0) {
        return iopHeapResult;
    }
    return -1;
}
