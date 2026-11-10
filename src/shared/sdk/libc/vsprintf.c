#include "shared/sdk/libc/decls.h"

s32 vsprintf(char *str, const char *fmt, va_list ap) {
    s32 result;
    FILE f;

    f._flags = 0x208;
    f._bf._base = f._p = (u8 *)str;
    f._bf._size = f._w = 0x7FFFFFFF;
    f._data = _impure_ptr;
    result = vfprintf(&f, fmt, ap);
    *f._p = '\0';
    return result;
}
