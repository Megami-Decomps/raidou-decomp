#include "common.h"

INCLUDE_ASM(const s32, "sdk/libkernl/thread", topThread);

INCLUDE_ASM(const s32, "sdk/libkernl/thread", InitThread);

INCLUDE_ASM(const s32, "sdk/libkernl/thread", iWakeupThread);

#include "shared/sdk/libkernl/iRotateThreadReadyQueue.c"

INCLUDE_ASM(const s32, "sdk/libkernl/thread", iSuspendThread);
