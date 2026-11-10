#include "shared/effect/effPCPMisc/decls.h"

/* vu0 routine: place three staggered groups in the camera-relative basis. */
void effPCPAgiraoFireDraw(effPCPAgiraoFire_tag *hData) {
    f32 position[4] __attribute__((aligned(16)));
    f32 basis[4][4] __attribute__((aligned(16)));
    s32 frame = hData->frame;
    u32 color = hData->color;
    u32 group;
    u32 handleIndex;

    LookAtMatrix(aViewTarget, aViewEye, aViewDown);
    SDF_VU0_STOREMATRIX(basis);
    for (group = 0; group < 3; group++) {
        s32 start = hData->head.groupStartFrame[group];
        if (frame >= start) {
            SDF_VU0_LOADVECTOR_B(hData->head.origin);
            SDF_VU0_LOADMATRIX(basis);
            position[0] = hData->head.offset[group][0];
            position[1] = hData->head.offset[group][1];
            position[2] = 0;
            SDF_VU0_LOADVECTOR(position);
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(position);
            for (handleIndex = 0; handleIndex < 7; handleIndex++) {
                if (frame >= (s32)((u32)hData->head.handleDelay[handleIndex] + (u32)start)) {
                    dds3EffectSetRGBA((struct effHandle_t *)hData->handles[group * 7 + handleIndex], color);
                    dds3EffectSetPosition((struct effHandle_t *)hData->handles[group * 7 + handleIndex], position);
                    dds3EffectDraw((struct effHandle_t *)hData->handles[group * 7 + handleIndex]);
                }
            }
        }
    }
    hData->frame = (s32)((u32)hData->frame + 1);
}
