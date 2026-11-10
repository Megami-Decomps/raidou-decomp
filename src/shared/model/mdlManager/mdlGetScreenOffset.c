#include "shared/model/mdlManager/decls.h"

/* vu0 routine: project the point through the view and model matrices into vf10. */
void mdlGetScreenOffset(mdlObj_t *aHandle, void *aOfs)
{
    SDF_VU0_LOADMATRIX(&D_002FA240);
    MultMatrixRev(&dds3ScreenParam.TM);
    SDF_VU0_COPYMATRIX_A_TO_B();
    SDF_VU0_LOADMATRIX(&aHandle->inner->tm);
    SDF_VU0_SCALE_XYZ();
    MultMatrixRevNoArg();
    SDF_VU0_LOADVECTOR_BARRIER(aOfs);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.trans);
    SDF_VU0_ADDVECTOR();
}
