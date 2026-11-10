#include "shared/sdk/libc/decls.h"

void free(void *ptr) {
    __malloc_lock(_impure_ptr);
    _free_r(_impure_ptr, ptr);
    __malloc_unlock(_impure_ptr);
}
