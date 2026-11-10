#include "shared/effect/effMagatuhi/decls.h"

/* Replace control-point xyz and jitter amplitudes without changing point w. */
void effMagaP4SetPoint(effMagatuhiCallback_t *hEffect, effMagatuhiRowsSrc_t *pP4Point) {
    effMagaPoint4_tag *dst = dds3MagatuhiGetHandle(hEffect->effect);
    u32 controlPointIndex;

    for (controlPointIndex = 0; controlPointIndex < EFF_MAGATUHI_CONTROL_POINT_COUNT; controlPointIndex++) {
        dst->param.point[controlPointIndex][0] = pP4Point->controlPoints[controlPointIndex][0];
        dst->param.point[controlPointIndex][1] = pP4Point->controlPoints[controlPointIndex][1];
        dst->param.point[controlPointIndex][2] = pP4Point->controlPoints[controlPointIndex][2];
        dst->param.jitterScales[controlPointIndex] = pP4Point->jitterScales[controlPointIndex];
    }
}
