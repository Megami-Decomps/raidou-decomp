#include "common.h"

#include "shared/sdk/libkernl/SetTLBHandler.c"

#include "shared/sdk/libkernl/SetDebugHandler.c"

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", Copy_2);

#include "shared/sdk/libkernl/kCopy_3.c"

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", GetEntryAddress_1);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", setup_0);

#include "shared/sdk/libkernl/InitTLBFunctions.c"

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", func_00415790);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", func_004157A0);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", _SetTLBEntry);

#include "shared/sdk/libkernl/iPutTLBEntry.c"

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", func_004157F0);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", func_00415800);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", func_00415810);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", func_00415820);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", func_00415830);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", func_00415840);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", InitTLB);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", InitTLB32MB);
