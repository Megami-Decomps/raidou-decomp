#include "shared/sdk/libc/decls.h"

f64 _b2d(_Bigint *b, s32 *exponent)
{
    libcDoubleBits_t result;
    u32 *first;
    u32 *last;
    u32 top;
    u32 previous;
    s32 leading;

    first = b->x;
    last = first + b->wds;
    top = *--last;
    leading = _hi0bits(top);
    *exponent = 32 - leading;

    if (leading < 11) {
        u32 below;
        result.word[1] = (top >> (11 - leading)) | 0x3FF00000;
        below = first < last ? *--last : 0;
        result.word[0] = (top << (leading + 21)) | (below >> (11 - leading));
    } else {
        if (first < last) {
            last--;
            previous = *last;
        } else {
            previous = 0;
        }
        leading -= 11;
        if (leading != 0) {
            result.word[1] = (top << leading) | (previous >> (32 - leading)) |
                             0x3FF00000;
            if (first < last) {
                last--;
                top = *last;
            } else {
                top = 0;
            }
            result.word[0] = (previous << leading) | (top >> (32 - leading));
        } else {
            result.word[1] = top | 0x3FF00000;
            result.word[0] = previous;
        }
    }
    return result.value;
}
