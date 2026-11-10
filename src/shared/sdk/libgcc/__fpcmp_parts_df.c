#include "shared/sdk/libgcc/decls.h"

s32 __fpcmp_parts_df(const fp_number_type_df *a, const fp_number_type_df *b) {
    if (dfIsNaN(a) || dfIsNaN(b)) {
        return 1;
    }
    if (dfIsInfinity(a) && dfIsInfinity(b)) {
        return b->sign - a->sign;
    }
    if (dfIsInfinity(a)) {
        return a->sign ? -1 : 1;
    }
    if (dfIsInfinity(b)) {
        return b->sign ? 1 : -1;
    }
    if (dfIsZero(a) && dfIsZero(b)) {
        return 0;
    }
    if (dfIsZero(a)) {
        return b->sign ? 1 : -1;
    }
    if (dfIsZero(b)) {
        return a->sign ? -1 : 1;
    }
    if (a->sign != b->sign) {
        return a->sign ? -1 : 1;
    }
    if (a->normal_exp > b->normal_exp) {
        return a->sign ? -1 : 1;
    }
    if (a->normal_exp < b->normal_exp) {
        return a->sign ? 1 : -1;
    }
    if ((u64)a->fraction > (u64)b->fraction) {
        return a->sign ? -1 : 1;
    }
    if ((u64)a->fraction < (u64)b->fraction) {
        return a->sign ? 1 : -1;
    }
    return 0;
}
