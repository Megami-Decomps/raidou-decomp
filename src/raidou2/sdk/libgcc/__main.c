#include "common.h"

#include "shared/sdk/libgcc/__do_global_dtors.c"

INCLUDE_ASM(const s32, "sdk/libgcc/__main", __do_global_ctors);

#include "shared/sdk/libgcc/__main.c"
