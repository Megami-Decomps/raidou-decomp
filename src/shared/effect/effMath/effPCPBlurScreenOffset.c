#include "shared/effect/effMath/decls.h"

/* vu0 routine: measure the projected camera-right offset. */
s32 effPCPBlurScreenOffset(f32 scale) {
    sdfVector4_t scaleVector;
    sdfVector4_t projectedEnd;
    sdfVector4_t projectedStart;
    sdfVector4_t original;
    f32 dx;
    f32 dy;
    f32 distance;

    SDF_VU0_STOREVECTOR_NOMEM(&original);
    effScreenOffset();
    SDF_VU0_STOREVECTOR(&projectedStart);

    scaleVector.x = scaleVector.y = scaleVector.z = scale;
    SDF_VU0_LOADVECTOR(&original);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(&aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(&aViewDown);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B_BARRIER(&scaleVector);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_ADDVECTOR();

    effScreenOffset();
    SDF_VU0_STOREVECTOR(&projectedEnd);
    dx = projectedEnd.x - projectedStart.x;
    dy = projectedEnd.y - projectedStart.y;
    SDF_VU0_LOADVECTOR(&projectedStart);
    distance = fsqrtf(dx * dx + dy * dy);
    return (s32)distance;
}
