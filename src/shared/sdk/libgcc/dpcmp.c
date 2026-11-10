#include "shared/sdk/libgcc/decls.h"

s32 dpcmp(double a, double b) {
    fp_number_type_df ua;
    fp_number_type_df ub;

    sdfUnpackDouble(&a, &ua);
    sdfUnpackDouble(&b, &ub);
    return __fpcmp_parts_df(&ua, &ub);
}
