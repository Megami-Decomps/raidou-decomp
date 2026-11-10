#include "shared/sdk/libc/decls.h"

double _mprec_log10(s32 dig) {
    double v = 1.0;

    if (dig < 24) {
        return pow10Table[dig];
    }
    while (dig > 0) {
        v *= 10.0;
        dig--;
    }
    return v;
}
