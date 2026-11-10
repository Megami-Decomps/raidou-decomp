#include "shared/sdf/sdfVu0/decls.h"

void SetRotateMatrixX(f32 aAngle) {
    volatile f32 m[4][4] __attribute__((aligned(16)));
    f32 c = cosf(aAngle);
    f32 s = sinf(aAngle);

    SDF_VU0_STOREIDENTITY((f32 *)m);
    m[1][1] = c;
    m[1][2] = s;
    m[2][1] = -s;
    m[2][2] = c;
    SDF_VU0_LOADMATRIX((f32 *)m);
}
