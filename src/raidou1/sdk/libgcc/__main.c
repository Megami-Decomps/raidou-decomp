#include "common.h"

INCLUDE_ASM(const s32, "sdk/libgcc/__main", __do_global_dtors);

INCLUDE_ASM(const s32, "sdk/libgcc/__main", __do_global_ctors);

INCLUDE_ASM(const s32, "sdk/libgcc/__main", __main);
