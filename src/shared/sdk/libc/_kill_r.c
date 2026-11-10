#include "shared/sdk/libc/decls.h"

s32 _kill_r(_reent *r, s32 pid, s32 sig) {
    s32 result;

    D_00552178 = 0;
    result = kill(pid, sig);
    if (result == -1 && D_00552178 != 0) {
        r->_errno = D_00552178;
    }
    return result;
}
