#include "shared/sdk/libgcc/decls.h"

f64 dpmul(f64 a, f64 b) {
    fp_number_type_df left;
    fp_number_type_df right;
    fp_number_type_df result;

    sdfUnpackDouble(&a, &left);
    sdfUnpackDouble(&b, &right);
    return __pack_df(dfMultiplyParts(&left, &right, &result));
}
