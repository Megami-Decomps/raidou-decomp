#include "shared/sdk/libgcc/decls.h"

s32 dptoli(f64 value) {
    fp_number_type_df number;
    s32 result;

    sdfUnpackDouble(&value, &number);
    if (dfIsZero(&number)) {
        return 0;
    }
    if ((u32)number.class < 2) {
        return 0;
    }
    if (dfIsInfinity(&number)) {
        return number.sign ? (s32)0x80000000 : 0x7FFFFFFF;
    }
    if (number.normal_exp < 0) {
        return 0;
    }
    if (number.normal_exp > 30) {
        return number.sign ? (s32)0x80000000 : 0x7FFFFFFF;
    }
    result = (u64)number.fraction >> (60 - number.normal_exp);
    return number.sign ? -result : result;
}
