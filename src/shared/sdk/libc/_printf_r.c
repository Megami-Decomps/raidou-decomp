#include "shared/sdk/libc/decls.h"

s32 _printf_r(_reent *r, const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    return func_002D6678(r, r->_stdout, fmt, ap);
}
