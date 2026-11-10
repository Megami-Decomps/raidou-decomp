#include "shared/sdk/libc/decls.h"

s32 _fstat_r(_reent *r, s32 fd, struct stat *buf) {
    s32 result;

    D_00552178 = 0;
    result = fstat(fd, buf);
    if (result == -1 && D_00552178 != 0) {
        r->_errno = D_00552178;
    }
    return result;
}
