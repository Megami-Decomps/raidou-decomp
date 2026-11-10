#include "common.h"

#include "shared/sdk/libc/__sfmoreglue.c"

#include "shared/sdk/libc/__sfp.c"

#include "shared/sdk/libc/_cleanup_r.c"

#include "shared/sdk/libc/_cleanup.c"

INCLUDE_ASM(const s32, "sdk/libc/findfp", __sinit);
