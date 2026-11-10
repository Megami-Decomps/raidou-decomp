#include "shared/sdf/sdfVu0/decls.h"

void SetRotateMatrixAxis(f32 aAngle, const sdfVector3_t* aAxis)
{
    sdfMatrix4_t matrix;
    StoreRotateMatrixAxis(aAngle, aAxis, &matrix);
    SDF_VU0_LOADMATRIX(&matrix);
}
