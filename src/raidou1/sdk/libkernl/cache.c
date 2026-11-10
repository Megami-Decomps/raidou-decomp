#include "common.h"

INCLUDE_ASM(const s32, "sdk/libkernl/cache", _sceSDC);

INCLUDE_ASM(const s32, "sdk/libkernl/cache", SyncDCache);

#include "shared/sdk/libkernl/iSyncDCache.c"

INCLUDE_ASM(const s32, "sdk/libkernl/cache", _sceIDC);

INCLUDE_ASM(const s32, "sdk/libkernl/cache", InvalidDCache);

#include "shared/sdk/libkernl/iInvalidDCache.c"
