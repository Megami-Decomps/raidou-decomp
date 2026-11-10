#include "shared/sdk/libc/decls.h"

s32 exponent(char *output, s32 value, s32 marker)
{
    /* The reverse-digit cursor ends at stack + 0x134 in the 0x140-byte frame. */
    char digits[0x134];
    char *cursor = output;
    char *digit;

    *cursor++ = marker;
    if (value < 0) {
        value = -value;
        *cursor++ = '-';
    } else {
        *cursor++ = '+';
    }
    digit = digits + sizeof(digits);
    if (value > 9) {
        while (value > 9) {
            *--digit = value % 10 + '0';
            value /= 10;
        }
        *--digit = value + '0';
        while (digit < digits + sizeof(digits)) {
            *cursor++ = *digit++;
        }
    } else {
        *cursor++ = '0';
        *cursor++ = value + '0';
    }
    return cursor - output;
}
