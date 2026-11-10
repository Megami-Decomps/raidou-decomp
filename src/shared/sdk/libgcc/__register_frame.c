#include "shared/sdk/libgcc/decls.h"

void __register_frame(void *fn) {
    __register_frame_info(fn, malloc(0x18));
}
