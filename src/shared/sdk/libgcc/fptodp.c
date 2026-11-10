#include "shared/sdk/libgcc/decls.h"

f64 fptodp(f32 x) {
    fp_number_type u;

    sdfUnpackFloat(&x, &u);
    return __make_dp(u.class, u.sign, u.normal_exp, (s64)u.fraction << 30);
}
