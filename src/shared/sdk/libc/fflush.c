#include "shared/sdk/libc/decls.h"

s32 fflush(void *stream)
{
    FILE *fp = stream;
    u8 *cursor;
    s32 remaining;
    s32 status;

    if (fp == NULL)
        return _fwalk(_impure_ptr, fflush);
    /* Same initialization phase as __swsetup; see levers/control-flow.md. */
    do {
        if (fp->_data == NULL)
            fp->_data = _impure_ptr;
        if (fp->_data->__sdidinit == 0)
            __sinit(fp->_data);
    } while (0);
    /* Live status scratch; see levers/cross-block-locals.md. */
    status = fp->_flags;
    if ((status & 8) == 0 || (cursor = fp->_bf._base) == NULL)
        return 0;
    remaining = fp->_p - cursor;
    fp->_p = cursor;
    fp->_w = (status & 3) ? 0 : fp->_bf._size;
    for (; remaining > 0; remaining -= status, cursor += status) {
        status = fp->_write(fp->_cookie, (char *)cursor, remaining);
        if (status <= 0) {
            fp->_flags |= 0x40;
            return -1;
        }
    }
    return 0;
}
