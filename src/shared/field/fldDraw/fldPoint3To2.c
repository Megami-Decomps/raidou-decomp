#include "shared/field/fldDraw/decls.h"

void fldPoint3To2(f32 *x3, f32 *y3, f32 z3, f32 x2, f32 y2) {
    sdfVector4_t vec = { z3, x2, y2, 1.0f };
    sdfVector4_t result;

    SDF_VU0_LOADMATRIX(D_002FA240);
    MultMatrixRev(&dds3ScreenParam.TM);
    SDF_VU0_COPYMATRIX_A_TO_B();
    SDF_VU0_LOADVECTOR_MEMOPS(*(const u128 *)&vec);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.trans);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&result);
    *x3 = result.x;
    *y3 = result.y;
}
