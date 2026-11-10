#include "shared/sdk/libgcc/decls.h"

u32 dptoul(f64 value) {
    fp_number_type_df number;
    sdfUnpackDouble(&value, &number);
    if (dfIsZero(&number)) {
        return 0;
    }
    if ((u32)number.class < 2) {
        return 0;
    }
    if (number.sign != 0) {
        return 0;
    }
    if (dfIsInfinity(&number)) {
        return 0xFFFFFFFFU;
    }
    if (number.normal_exp < 0) {
        return 0;
    }
    if (number.normal_exp > 31) {
        return 0xFFFFFFFFU;
    }
    if (number.normal_exp > 60) {
        return (u64)number.fraction << (number.normal_exp - 60);
    }
    return (u64)number.fraction >> (60 - number.normal_exp);
}
