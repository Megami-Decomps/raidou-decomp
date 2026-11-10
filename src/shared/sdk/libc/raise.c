#include "shared/sdk/libc/decls.h"

s32 raise(s32 sig) {
    return _raise_r(_impure_ptr, sig);
}
