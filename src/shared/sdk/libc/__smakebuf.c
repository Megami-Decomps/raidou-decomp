#include "shared/sdk/libc/decls.h"

void __smakebuf(FILE *fp)
{
    struct stat st;
    u8 *buf;
    s32 size;
    s32 couldBeTty;

    if (fp->_flags & 2) {
        fp->_bf._base = fp->_p = fp->_nbuf;
        fp->_bf._size = 1;
        return;
    }
    if (fp->_file < 0 || _fstat_r(fp->_data, fp->_file, &st) < 0) {
        couldBeTty = 0;
        size = 1024;
        fp->_flags |= 0x800;
    } else {
        couldBeTty = (st.st_mode & 0xF000) == 0x2000;
        size = 1024;
        if ((st.st_mode & 0xF000) == 0x8000 && fp->_seek == __sseek) {
            fp->_flags |= 0x400;
            fp->_blksize = size;
        } else {
            fp->_flags |= 0x800;
        }
    }
    buf = _malloc_r(fp->_data, size);
    if (buf == NULL) {
        fp->_flags |= 2;
        fp->_bf._base = fp->_p = fp->_nbuf;
        fp->_bf._size = 1;
    } else {
        fp->_data->__cleanup = _cleanup_r;
        fp->_flags |= 0x80;
        fp->_bf._base = fp->_p = buf;
        fp->_bf._size = size;
        if (couldBeTty && isatty(fp->_file)) fp->_flags |= 1;
    }
}
