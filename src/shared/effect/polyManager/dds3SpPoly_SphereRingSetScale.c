#include "shared/effect/polyManager/decls.h"

void dds3SpPoly_SphereRingSetScale(dds3SpPoly_SphereRing_t3 *hData, f32 scale) {
    hData->radius = hData->radius * scale;
}
