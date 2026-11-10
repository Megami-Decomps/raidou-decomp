#include "shared/sdk/libkernl/decls.h"

s32 sceSifFreeIopHeap(void *address)
{
    return sceSifFreeSysMemory(address);
}
