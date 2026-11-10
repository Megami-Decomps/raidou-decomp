#include "shared/sdf/sdfVu0/decls.h"

/* vu0 routine: inverse of vf28-vf31 with per-axis scale removed (transpose, rows / |row|^2, translation = -(R^T * t)) */
void InversMatrix(void) {
    VU0_MATRIX4_INVERT_SCALED();
}
