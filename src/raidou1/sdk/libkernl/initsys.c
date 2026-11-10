#include "common.h"

INCLUDE_ASM(const s32, "sdk/libkernl/initsys", supplement_crt0);

INCLUDE_ASM(const s32, "sdk/libkernl/initsys", Copy);

INCLUDE_ASM(const s32, "sdk/libkernl/initsys", kCopy);

INCLUDE_ASM(const s32, "sdk/libkernl/initsys", kFindAddress);

INCLUDE_ASM(const s32, "sdk/libkernl/initsys", FindAddress);

INCLUDE_ASM(const s32, "sdk/libkernl/initsys", GetSystemCallEntry);

INCLUDE_ASM(const s32, "sdk/libkernl/initsys", GetSystemCallTableEntry);

INCLUDE_ASM(const s32, "sdk/libkernl/initsys", setup);

INCLUDE_ASM(const s32, "sdk/libkernl/initsys", _InitSys);
