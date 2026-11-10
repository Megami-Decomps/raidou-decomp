#include "shared/effect/billManager/decls.h"

/* vu0 routine: 1 - |axis . normalized view direction|. */
f32 dds3BillboardGetDirectionScale(void *pPos, f32 *pVec) {
    f32 dot;

    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(pVec);
    SDF_VU0_INNERPRODUCT(dot);
    dot = sdfFabs(dot);
    return 1.0f - dot;
}
