#include "shared/sdk/libc/decls.h"

s32 atexit(void (*fn)(void)) {
    _atexit *p;

    p = _impure_ptr->_atexit;
    if (p == NULL) {
        p = &_impure_ptr->_atexit0;
        _impure_ptr->_atexit = p;
    }
    if (p->_ind >= 32) {
        if ((p = malloc(sizeof(_atexit))) == NULL) {
            return -1;
        }
        p->_ind = 0;
        p->_next = _impure_ptr->_atexit;
        _impure_ptr->_atexit = p;
    }
    p->_fns[p->_ind++] = fn;
    return 0;
}
