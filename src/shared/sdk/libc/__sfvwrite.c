#include "shared/sdk/libc/decls.h"

s32 __sfvwrite(FILE *fp, __suio *uio)
{
    u32 length;
    char *p;
    __siov *iov;
    s32 written;
    s32 available;
    s32 newlineKnown;
    s32 newlineDistance;
    char *newline;

    /* One byte count spans the entry and vector phases; see levers/cross-block-locals.md. */
    length = uio->uio_resid;
    if (length == 0)
        return 0;
    if ((fp->_flags & 8) == 0 || fp->_bf._base == 0)
        if (__swsetup(fp) != 0)
            return -1;
    iov = uio->uio_iov;
    length = 0;
    if (fp->_flags & 2) {
        do {
            while (length == 0) {
                p = iov->iov_base;
                length = iov->iov_len;
                iov++;
            }
            written = fp->_write(fp->_cookie, p, length > 1024 ? 1024 : length);
            if (written <= 0)
                goto error;
            p += written;
            length -= written;
        } while ((uio->uio_resid -= written) != 0);
    } else if ((fp->_flags & 1) == 0) {
        do {
            while (length == 0) {
                p = iov->iov_base;
                length = iov->iov_len;
                iov++;
            }
            written = fp->_w;
            if (fp->_flags & 0x200) {
                if (length < written)
                    written = length;
                memmove(fp->_p, p, written);
                fp->_w -= written;
                fp->_p += written;
                written = length;
            } else if (fp->_p > fp->_bf._base && length > written) {
                memmove(fp->_p, p, written);
                fp->_p += written;
                if (fflush(fp) != 0)
                    goto error;
            } else if (length >= (written = fp->_bf._size)) {
                written = fp->_write(fp->_cookie, p, written);
                if (written <= 0)
                    goto error;
            } else {
                written = length;
                memmove(fp->_p, p, written);
                fp->_w -= written;
                fp->_p += written;
            }
            p += written;
            length -= written;
        } while ((uio->uio_resid -= written) != 0);
    } else {
        newlineKnown = 0;
        do {
            if (length == 0) {
                newlineKnown = 0;
                while (length == 0) {
                    p = iov->iov_base;
                    length = iov->iov_len;
                    iov++;
                }
            }
            if (!newlineKnown) {
                newline = memchr(p, '\n', length);
                newlineDistance = newline ? newline + 1 - p : length + 1;
                newlineKnown = 1;
            }
            available = length < newlineDistance ? length : newlineDistance;
            written = fp->_w + fp->_bf._size;
            if (fp->_p > fp->_bf._base && written < available) {
                memmove(fp->_p, p, written);
                fp->_p += written;
                if (fflush(fp) != 0)
                    goto error;
            } else if (available >= (written = fp->_bf._size)) {
                written = fp->_write(fp->_cookie, p, written);
                if (written <= 0)
                    goto error;
            } else {
                written = available;
                memmove(fp->_p, p, written);
                fp->_w -= written;
                fp->_p += written;
            }
            if ((newlineDistance -= written) == 0) {
                if (fflush(fp) != 0)
                    goto error;
                newlineKnown = 0;
            }
            p += written;
            length -= written;
        } while ((uio->uio_resid -= written) != 0);
    }
    return 0;
error:
    fp->_flags |= 0x40;
    return -1;
}
