#include "common.h"

INCLUDE_ASM(const s32, "sdk/libkernl/iopheap", sceSifInitIopHeap);

INCLUDE_ASM(const s32, "sdk/libkernl/iopheap", sceSifAllocIopHeap);

INCLUDE_ASM(const s32, "sdk/libkernl/iopheap", sceSifAllocSysMemory);

INCLUDE_ASM(const s32, "sdk/libkernl/iopheap", sceSifFreeSysMemory);

INCLUDE_ASM(const s32, "sdk/libkernl/iopheap", sceSifFreeIopHeap);

INCLUDE_ASM(const s32, "sdk/libkernl/iopheap", sceSifLoadIopHeap);

INCLUDE_ASM(const s32, "sdk/libkernl/iopheap", sceSifQueryMemSize);

INCLUDE_ASM(const s32, "sdk/libkernl/iopheap", sceSifQueryMaxFreeMemSize);

INCLUDE_ASM(const s32, "sdk/libkernl/iopheap", sceSifQueryTotalFreeMemSize);

INCLUDE_ASM(const s32, "sdk/libkernl/iopheap", sceSifQueryBlockTopAddress);

INCLUDE_ASM(const s32, "sdk/libkernl/iopheap", sceSifQueryBlockSize);
