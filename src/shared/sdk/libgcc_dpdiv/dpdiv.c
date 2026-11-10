#include "shared/sdk/libgcc_dpdiv/decls.h"

f64 dpdiv(f64 a, f64 b)
{
    fp_number_type_df numerator;
    fp_number_type_df denominator;
    const fp_number_type_df *result;

    sdfUnpackDouble(&a, &numerator);
    sdfUnpackDouble(&b, &denominator);
    result = dfDivideParts(&numerator, &denominator);
    return __pack_df(result);
}
