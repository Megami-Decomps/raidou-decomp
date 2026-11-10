#include "shared/sdk/libgcc/decls.h"

void __deregister_frame(void *fn) {
    free(__deregister_frame_info(fn));
}
