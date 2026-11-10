#include "shared/sdk/libkernl/decls.h"

/* The IOP block key and query results travel as 32-bit words. */
u32 sceSifQueryBlockTopAddress(u32 block)
{
    if (iopHeapStatus < 0) {
        return 0;
    }
    iopHeapArgs[0] = block;
    if (sceSifCallRpc(iopHeapRpcClient, 8, 0, iopHeapArgs, 4,
                      &iopHeapResult, 4, NULL, NULL) < 0) {
        return 0xFFFFFFFF;
    }
    return iopHeapResult;
}
