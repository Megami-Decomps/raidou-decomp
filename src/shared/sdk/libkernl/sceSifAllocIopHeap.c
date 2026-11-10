#include "shared/sdk/libkernl/decls.h"

s32 sceSifAllocIopHeap(s32 size)
{
    if (iopHeapStatus < 0) {
        return 0;
    }
    iopHeapArgs[0] = size;
    if (sceSifCallRpc(iopHeapRpcClient, 1, 0, iopHeapArgs, 4,
                      &iopHeapResult, 4, NULL, NULL) >= 0) {
        return iopHeapResult;
    }
    return 0;
}
