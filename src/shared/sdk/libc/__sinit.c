#include "shared/sdk/libc/decls.h"

void __sinit(_reent *r) {
    r->__cleanup = _cleanup_r;
    r->__sdidinit = 1;
    gcc2_compiled__2(&r->__sf[0], 4, 0, r);
    gcc2_compiled__2(&r->__sf[1], 9, 1, r);
    gcc2_compiled__2(&r->__sf[2], 0xA, 2, r);
    r->__sglue.next = NULL;
    r->__sglue.niobs = 3;
    r->__sglue.iobs = &r->__sf[0];
}
