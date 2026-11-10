#include "common.h"

INCLUDE_ASM(const s32, "sdk/libkernl/thread", topThread);

#include "shared/sdk/libkernl/InitThread.c"

INCLUDE_ASM(const s32, "sdk/libkernl/thread", iWakeupThread);

#include "shared/sdk/libkernl/iRotateThreadReadyQueue.c"

INCLUDE_ASM(const s32, "sdk/libkernl/thread", iSuspendThread);
