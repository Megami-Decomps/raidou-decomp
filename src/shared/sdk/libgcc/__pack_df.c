#include "shared/sdk/libgcc/decls.h"

f64 __pack_df(const fp_number_type_df *u) {
    sdfDoubleBits_t result;
    u64 fraction = u->fraction;
    u32 exponent = 0;
    u32 sign = u->sign;

    if (dfIsNaN(u)) {
        fraction |= (u64)1 << 51;
        exponent = 0x7FF;
    } else if (dfIsInfinity(u)) {
        exponent = 0x7FF;
        fraction = 0;
    } else if (dfIsZero(u)) {
        fraction = 0;
    } else if (fraction != 0) {
        if (u->normal_exp < -1022) {
            fraction = 0;
        } else if (u->normal_exp > 1023) {
            exponent = 0x7FF;
            fraction = 0;
        } else {
            exponent = u->normal_exp + 1023;
            fraction >>= 8;
        }
    }
    result.fields.fraction = fraction;
    result.fields.exponent = exponent;
    result.fields.sign = sign;
    return result.value;
}
