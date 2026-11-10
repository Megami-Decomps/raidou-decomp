#include "common.h"

#include "shared/sdk/libkernl/supplement_crt0.c"

INCLUDE_ASM(const s32, "sdk/libkernl/initsys", Copy);

#include "shared/sdk/libkernl/kCopy.c"

#include "shared/sdk/libkernl/kFindAddress.c"

INCLUDE_ASM(const s32, "sdk/libkernl/initsys", FindAddress);

#include "shared/sdk/libkernl/GetSystemCallEntry.c"

#include "shared/sdk/libkernl/GetSystemCallTableEntry.c"

INCLUDE_ASM(const s32, "sdk/libkernl/initsys", setup);

#include "shared/sdk/libkernl/_InitSys.c"
