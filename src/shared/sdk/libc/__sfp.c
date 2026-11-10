#include "shared/sdk/libc/decls.h"

FILE *__sfp(_reent *r)
{
    _glue *glue;
    FILE *fp;
    s32 i;

    if (r->__sdidinit == 0) {
        __sinit(r);
    }
    glue = &r->__sglue;
    for (;;) {
        i = glue->niobs;
        fp = glue->iobs;
        for (i--; i >= 0; i--, fp++) {
            if (fp->_flags == 0) {
                /* Keep initialization outside the short search loop. */
                goto found;
            }
        }
        if (glue->next == NULL) {
            glue->next = __sfmoreglue(r, 4);
            if (glue->next == NULL) {
                break;
            }
        }
        glue = glue->next;
    }
    r->_errno = 12;
    return NULL;

found:
    fp->_flags = 1;
    fp->_file = -1;
    fp->_data = r;
    fp->_p = NULL;
    fp->_w = 0;
    fp->_r = 0;
    fp->_bf._base = NULL;
    fp->_bf._size = 0;
    fp->_lbfsize = 0;
    fp->_ub._base = NULL;
    fp->_ub._size = 0;
    fp->_lb._base = NULL;
    fp->_lb._size = 0;
    return fp;
}
