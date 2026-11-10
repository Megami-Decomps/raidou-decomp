#include "shared/sdk/libc/decls.h"

void _cleanup(void) {
    _cleanup_r(_impure_ptr);
}
