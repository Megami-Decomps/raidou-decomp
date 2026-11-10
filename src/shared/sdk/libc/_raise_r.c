#include "shared/sdk/libc/decls.h"

s32 _raise_r(_reent *r, s32 sig)
{
    void (*handler)(s32);

    if ((u32)sig >= 32) {
        r->_errno = 22;
        return -1;
    }
    if (r->_sig_func == NULL && _init_signal_r(r) != 0) {
        return -1;
    }
    handler = r->_sig_func[sig];
    if (handler == NULL) {
        return _kill_r(r, _getpid_r(r), sig);
    }
    if (handler == (void (*)(s32))1) {
        return 0;
    }
    if (handler == (void (*)(s32))-1) {
        r->_errno = 22;
        return 1;
    }
    r->_sig_func[sig] = NULL;
    handler(sig);
    return 0;
}
