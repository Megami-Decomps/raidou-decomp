#include "common.h"

#include "shared/sdk/libkernl/SetTLBHandler.c"

#include "shared/sdk/libkernl/SetDebugHandler.c"

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", Copy_2);

#include "shared/sdk/libkernl/kCopy_3.c"

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", GetEntryAddress_1);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", setup_0);

#include "shared/sdk/libkernl/InitTLBFunctions.c"

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", func_0039C910);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", func_0039C920);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", _SetTLBEntry);

#include "shared/sdk/libkernl/iPutTLBEntry.c"

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", func_0039C970);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", func_0039C980);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", func_0039C990);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", func_0039C9A0);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", func_0039C9B0);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", func_0039C9C0);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", InitTLB);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", InitTLB32MB);
