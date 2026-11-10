#include "shared/sdf/sdfVu0/decls.h"

/* vu0 routine: rigid inverse of vf28-vf31 (transpose the 3x3, translation = -(R^T * t)) */
void ReverseMatrix(void) {
    VU0_MATRIX4_INVERT_RIGID();
}
