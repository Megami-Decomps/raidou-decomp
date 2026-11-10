#include "shared/sdk/libc/decls.h"

void *malloc(size_t size) {
    void *result;

    __malloc_lock(_impure_ptr);
    result = _malloc_r(_impure_ptr, size);
    __malloc_unlock(_impure_ptr);
    return result;
}
