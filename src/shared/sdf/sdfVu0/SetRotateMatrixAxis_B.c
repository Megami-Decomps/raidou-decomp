#include "shared/sdf/sdfVu0/decls.h"

void SetRotateMatrixAxis_B(const sdfVector3_t* aAngle, f32 aAxis)
{
    sdfMatrix4_t matrix;
    StoreRotateMatrixAxis(aAxis, aAngle, &matrix);
    SDF_VU0_LOADMATRIX_B(&matrix);
}
