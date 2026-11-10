#include "shared/sdk/libkernl/decls.h"

void *SetDebugHandler(s32 index, void *handler)
{
    void *previous;

    if (((u32)index - 1) >= 13) {
        return (void *)-1;
    }

    previous = debugHandlers[index];
    debugHandlers[index] = handler;
    if (((u32)index - 1) < 3) {
        SetVTLBRefillHandler(index, _kDebugException);
    } else {
        SetVCommonHandler(index, _kDebugException);
    }
    return previous;
}
