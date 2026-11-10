#include "shared/effect/effMath/decls.h"

s32 dds3Bezier4PPartRun(effBezier4P_t *table, s32 slotIndex, f32 *out) {
    dds3Bezier4PPart_t *slot = &((dds3Bezier4PPart_t *)table->pPart)[slotIndex];
    sdfVector4_t w;
    f32 t = slot->t;
    f32 u = 1.0f - t;

    w.x = u * u * u;
    w.y = t * (u * u) * 3.0f;
    w.z = t * t * u * 3.0f;
    w.w = t * t * t;
    out[0] = slot->controlPoints[0][0] * w.x + slot->controlPoints[1][0] * w.y + slot->controlPoints[2][0] * w.z + slot->controlPoints[3][0] * w.w;
    out[1] = slot->controlPoints[0][1] * w.x + slot->controlPoints[1][1] * w.y + slot->controlPoints[2][1] * w.z + slot->controlPoints[3][1] * w.w;
    out[2] = slot->controlPoints[0][2] * w.x + slot->controlPoints[1][2] * w.y + slot->controlPoints[2][2] * w.z + slot->controlPoints[3][2] * w.w;
    t += SDF_FRAME_STEP(slot->step);
    if (t > 1.0f) {
        slot->t = 1.0f;
        return 0;
    }
    slot->t = t;
    return 1;
}
