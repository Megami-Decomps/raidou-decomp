#include "shared/sdk/libc/decls.h"

s32 _init_signal(void) {
    return _init_signal_r(_impure_ptr);
}
