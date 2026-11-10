#include "shared/sdf/sdfVu0/decls.h"

/* vu0 routine: vf28-vf31 = vf28-vf31 * vf20-vf23 (4x4 product) */
void MultMatrixRevNoArg_C(void)
{
    VU0_MATRIX4_MUL_BANK_C_LEFT();
}
