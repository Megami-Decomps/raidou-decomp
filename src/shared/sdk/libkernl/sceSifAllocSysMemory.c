#include "shared/sdk/libkernl/decls.h"

void *sceSifAllocSysMemory(s32 mode, s32 size, void *address)
{
    if (iopHeapStatus < 0) {
        return NULL;
    }
    iopHeapArgs[0] = size;
    iopHeapArgs[1] = mode;
    iopHeapArgs[2] = EE_ADDR(address);
    if (sceSifCallRpc(iopHeapRpcClient, 4, 0, iopHeapArgs, 12,
                      &iopHeapResult, 4, NULL, NULL) >= 0) {
        return EE_ADDR_AS(void *, iopHeapResult);
    }
    return NULL;
}
