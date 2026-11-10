#include "shared/effect/polyManager/decls.h"

void dds3SpPoly_SphereRingBuffInit(dds3SpPoly_SphereRing_t3 *pSphereRing, s32 number) {
    dds3SpPoly_SphereRingPhysics_t *record = pSphereRing->pPhysics;
    f32 spread = pSphereRing->radius_r;

    record += number;
    record->frame = 0;
    record->radius = pSphereRing->radius * (dds3GetRandFloatA(dds3EffectBase) * spread + (1.0f - spread));
}
