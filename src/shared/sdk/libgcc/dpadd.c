#include "shared/sdk/libgcc/decls.h"

f64 dpadd(double a, double b) {
    fp_number_type_df ua;
    fp_number_type_df ub;
    fp_number_type_df r;

    sdfUnpackDouble(&a, &ua);
    sdfUnpackDouble(&b, &ub);
    return __pack_df(libc_2D1590(&ua, &ub, &r));
}
