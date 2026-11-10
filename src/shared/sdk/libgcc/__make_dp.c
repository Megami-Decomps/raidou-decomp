#include "shared/sdk/libgcc/decls.h"

f64 __make_dp(s32 kind, s32 sign, s32 exp, s64 mant) {
    fp_number_type_df u;

    u.class = kind;
    u.sign = sign;
    u.normal_exp = exp;
    u.fraction = mant;
    return __pack_df(&u);
}
