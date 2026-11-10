#include "shared/sdk/libgcc/decls.h"

s32 __register_frame_table(void *fn) {
    return __register_frame_info_table(fn, malloc(0x18));
}
