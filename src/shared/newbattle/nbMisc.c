#include "common.h"
#include "dds3CameraObjectBasic.h"
#include "kernel_draw.h"
#include "sdfVu0.h"
#include "sdk_libgcc.h"
#include "ps2asm.h"

s32 nbClipEffectNum(void)
{
    f32 viewDistance;
    f32 forwardDot;

    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(&aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(viewDistance);
    if (dpcmp(fptodp(viewDistance), 16.0) <= 0) {
        return 0;
    }
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADMATRIX(&sdfViewMatrix);
    SDF_VU0_COPYMATRIX_A_TO_B();
    ReverseMatrix();
    SDF_VU0_GET_ROW2();
    SDF_VU0_NORMALIZE();
    SDF_VU0_INNERPRODUCT(forwardDot);
    if (forwardDot <= 0.25f) {
        return 0;
    }
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_COPYMATRIX_B_TO_A();
    MultMatrixRev(&dds3ScreenParam.TM);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.trans);
    SDF_VU0_ADDVECTOR();
    return 1;
}
