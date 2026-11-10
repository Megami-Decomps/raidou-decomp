#include "common.h"

INCLUDE_ASM(const s32, "sdk/libkernl/intr", DisableIntc);

INCLUDE_ASM(const s32, "sdk/libkernl/intr", EnableIntc);

INCLUDE_ASM(const s32, "sdk/libkernl/intr", DisableDmac);

INCLUDE_ASM(const s32, "sdk/libkernl/intr", EnableDmac);

INCLUDE_ASM(const s32, "sdk/libkernl/intr", iEnableIntc);

INCLUDE_ASM(const s32, "sdk/libkernl/intr", iDisableIntc);

INCLUDE_ASM(const s32, "sdk/libkernl/intr", iEnableDmac);

INCLUDE_ASM(const s32, "sdk/libkernl/intr", iDisableDmac);
