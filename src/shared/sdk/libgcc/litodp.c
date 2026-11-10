#include "shared/sdk/libgcc/decls.h"

f64 litodp(s32 n) {
    fp_number_type_df value;

    value.class = 3;
    value.sign = (u32)n >> 31;
    if (n == 0) {
        value.class = 2;
    } else {
        value.normal_exp = 60;
        if (value.sign != 0) {
            if (n == (-2147483647 - 1)) {
                return -2147483648.0;
            }
            value.fraction = -n;
        } else {
            value.fraction = n;
        }
        while ((u64)value.fraction <= 0x0FFFFFFFFFFFFFFFULL) {
            value.fraction = (u64)value.fraction << 1;
            value.normal_exp--;
        }
    }
    return __pack_df(&value);
}
