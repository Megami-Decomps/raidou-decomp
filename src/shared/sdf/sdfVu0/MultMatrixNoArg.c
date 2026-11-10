#include "shared/sdf/sdfVu0/decls.h"

/* vu0 routine: vf28-vf31 = vf24-vf27 * vf28-vf31 (4x4 product) */
void MultMatrixNoArg(void)
{
    VU0_MATRIX4_MUL_BANK_B_RIGHT();
}
