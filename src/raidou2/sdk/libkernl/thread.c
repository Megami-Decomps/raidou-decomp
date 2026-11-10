#include "common.h"

INCLUDE_ASM(const s32, "sdk/libkernl/thread", topThread);

INCLUDE_ASM(const s32, "sdk/libkernl/thread", InitThread);

INCLUDE_ASM(const s32, "sdk/libkernl/thread", iWakeupThread);

INCLUDE_ASM(const s32, "sdk/libkernl/thread", iRotateThreadReadyQueue);

INCLUDE_ASM(const s32, "sdk/libkernl/thread", iSuspendThread);
