#include "common.h"

INCLUDE_ASM(const s32, "sdk/libkernl/kprintf", printfloat);

INCLUDE_RODATA(const s32, "sdk/libkernl/kprintf", D_00469088);

INCLUDE_RODATA(const s32, "sdk/libkernl/kprintf", D_00469090);

INCLUDE_RODATA(const s32, "sdk/libkernl/kprintf", D_00469098);

INCLUDE_ASM(const s32, "sdk/libkernl/kprintf", _printf);

#include "shared/sdk/libkernl/kprintf.c"

#include "shared/sdk/libkernl/scePrintf.c"
