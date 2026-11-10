#include "shared/sdk/libc/decls.h"

s32 vfprintf(FILE *fp, const char *fmt, va_list ap)
{
    /* Keep attachment and initialization as one phase, as in __swsetup. */
    do {
        if (fp->_data == NULL) {
            fp->_data = _impure_ptr;
        }
        if (fp->_data->__sdidinit == 0) {
            __sinit(fp->_data);
        }
    } while (0);
    return func_002D6678(fp->_data, fp, fmt, ap);
}
