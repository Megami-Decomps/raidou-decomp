#include "common.h"

INCLUDE_ASM(const s32, "sdk/libkernl/kprintf", printfloat);

INCLUDE_RODATA(const s32, "sdk/libkernl/kprintf", D_004DBA58);

INCLUDE_RODATA(const s32, "sdk/libkernl/kprintf", D_004DBA60);

INCLUDE_RODATA(const s32, "sdk/libkernl/kprintf", D_004DBA68);

INCLUDE_ASM(const s32, "sdk/libkernl/kprintf", _printf);

#include "shared/sdk/libkernl/kprintf.c"

#include "shared/sdk/libkernl/scePrintf.c"
