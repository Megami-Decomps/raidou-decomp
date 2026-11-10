#include "shared/sdk/libkernl/decls.h"

u32 sceSifQueryTotalFreeMemSize(void)
{
    if (iopHeapStatus < 0) {
        return 0;
    }
    if (sceSifCallRpc(iopHeapRpcClient, 7, 0, NULL, 0,
                      &iopHeapResult, 4, NULL, NULL) < 0) {
        return 0xFFFFFFFF;
    }
    return iopHeapResult;
}
