#include "shared/sdf/sdf3D/decls.h"

void sdf3DSetCamera(sdf3DScreenParam_t *aParam, void *aCamera) {
    SDF_VU0_LOADMATRIX(aCamera);
    SDF_VU0_STOREMATRIX(sdfCameraTm);
    MultMatrixRev(aParam->TM);
    SDF_VU0_LOADVECTOR(aParam->cameraPos);
    SDF_VU0_STOREMATRIX(sdf3DScreenTm);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_STOREVECTOR(sdf3DEyeOffset);
    memcpy(sdf3DScreenParam, aParam, sizeof(*aParam));
}
