#include "common.h"

#include "shared/sdk/libkernl/sceSifResetIop.c"

#include "shared/sdk/libkernl/sceSifIsAliveIop.c"

#include "shared/sdk/libkernl/sceSifSyncIop.c"

INCLUDE_ASM(const s32, "sdk/libkernl/iopreset", sceSifRebootIop);
