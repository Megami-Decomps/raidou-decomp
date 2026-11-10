#include "shared/sdk/libc/decls.h"

void *signal(s32 sig, void *handler) {
    return _signal_r(_impure_ptr, sig, handler);
}
