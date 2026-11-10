#include "shared/sdf/sdfVu0/decls.h"

/* vu0 routine: vf28-vf31 = vf28-vf31 * vf24-vf27 (4x4 product) */
void MultMatrixRevNoArg(void)
{
        VU0_MATRIX4_MUL_BANK_B_LEFT();
}
