#include "shared/sdk/libc/decls.h"

char *cvt(_reent *r, f64 value, s32 precision, s32 flags, char *sign,
          s32 *decimalPoint, s32 format, s32 *length)
{
    union {
        f64 value;
        u64 bits;
    } input;
    s32 mode = 3;
    s32 dtoaSign;
    char *end;
    char *digits;
    char *limit;

    if (format != 'f') {
        if (format == 'e' || format == 'E') {
            precision++;
        }
        mode = 2;
    }
    input.value = value;
    if ((s32)(input.bits >> 32) < 0) {
        value = 0.0 - value;
        *sign = '-';
    } else {
        *sign = 0;
    }
    digits = _dtoa_r(r, value, mode, precision, decimalPoint, &dtoaSign, &end);
    if ((format != 'g' && format != 'G') || (flags & 1)) {
        limit = digits + precision;
        if (format == 'f') {
            if (*digits == '0' && value != 0.0) {
                *decimalPoint = 1 - precision;
            }
            limit += *decimalPoint;
        }
        if (value == 0.0) {
            end = limit;
        }
        while (end < limit) {
            *end++ = '0';
        }
    }
    *length = end - digits;
    return digits;
}
