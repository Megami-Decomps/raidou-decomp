#include "shared/interface/frFont/decls.h"

/* Return the zero-based highest set bit; both zero and one return zero. */
s32 frConvertVal2N(u32 Val) {
    s32 shiftCount = 0;

    if (Val == 0) {
        return 0;
    }
    do {
        Val = Val >> 1;
        shiftCount += 1;
    } while (Val != 0);
    return (shiftCount + FR_FONT_BYTE_MASK) & FR_FONT_BYTE_MASK;
}
