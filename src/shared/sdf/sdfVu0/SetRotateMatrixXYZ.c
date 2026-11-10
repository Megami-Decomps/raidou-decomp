#include "shared/sdf/sdfVu0/decls.h"

void SetRotateMatrixXYZ(const sdfVector3_t *aAngles)
{
    SetRotateMatrixX(aAngles->x);
    SetRotateMatrixY_B(aAngles->y);
    MultMatrixRevNoArg();
    SetRotateMatrixZ_B(aAngles->z);
    MultMatrixRevNoArg();
}
