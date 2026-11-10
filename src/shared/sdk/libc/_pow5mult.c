#include "shared/sdk/libc/decls.h"

_Bigint *_pow5mult(_reent *r, _Bigint *b, s32 n)
{
    _Bigint *power;
    _Bigint *nextPower;
    _Bigint *product;
    s32 remainder;

    remainder = n & 3;
    if (remainder != 0) {
        b = _multadd(r, b, pow5MultTable[remainder], 0);
    }
    n >>= 2;
    if (n == 0) {
        return b;
    }
    power = r->_p5s;
    if (power == NULL) {
        power = r->_p5s = _i2b(r, 625);
        power->next = NULL;
    }
    for (;;) {
        if (n & 1) {
            product = _multiply(r, b, power);
            _Bfree(r, b);
            b = product;
        }
        n >>= 1;
        if (n == 0) {
            break;
        }
        if ((nextPower = power->next) == NULL) {
            nextPower = power->next = _multiply(r, power, power);
            nextPower->next = NULL;
        }
        power = nextPower;
    }
    return b;
}
