#include "shared/sdf/sdfVu0/decls.h"

void SetRotateMatrixZ_B(f32 aAngle) {
    volatile f32 m[4][4] __attribute__((aligned(16)));
    f32 c = cosf(aAngle);
    f32 s = sinf(aAngle);

    SDF_VU0_STOREIDENTITY((f32 *)m);
    m[0][0] = c;
    m[0][1] = s;
    m[1][0] = -s;
    m[1][1] = c;
    SDF_VU0_LOADMATRIX_B((f32 *)m);
}
