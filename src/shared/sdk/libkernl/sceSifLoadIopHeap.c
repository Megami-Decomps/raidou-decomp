#include "shared/sdk/libkernl/decls.h"

s32 sceSifLoadIopHeap(const char *path, u32 address)
{
    s32 length;

    if (iopHeapStatus < 0) {
        return 0;
    }
    for (length = 0; length < 0xFC &&
         (iopHeapLoadRequest.name[length] = path[length]) != 0; length++) {
    }
    if (length == 0xFC) {
        iopHeapLoadRequest.name[length - 1] = 0;
        length--;
    }
    iopHeapLoadRequest.address = address;
    iopHeapLoadRequest.name[0xFB] = 0;
    if (sceSifCallRpc(iopHeapRpcClient, 3, 0, &iopHeapLoadRequest, length + 5,
                      &iopHeapResult, 4, NULL, NULL) >= 0) {
        return iopHeapResult;
    }
    return -1;
}
