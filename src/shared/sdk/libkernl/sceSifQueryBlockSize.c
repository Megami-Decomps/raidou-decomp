#include "shared/sdk/libkernl/decls.h"

u32 sceSifQueryBlockSize(u32 block)
{
    if (iopHeapStatus < 0) {
        return 0;
    }
    iopHeapArgs[0] = block;
    if (sceSifCallRpc(iopHeapRpcClient, 9, 0, iopHeapArgs, 4,
                      &iopHeapResult, 4, NULL, NULL) < 0) {
        return 0xFFFFFFFF;
    }
    return iopHeapResult;
}
