#include "shared/effect/polyManager/decls.h"

void dds3SpPoly_SphereRingAllBuffInit(dds3SpPoly_SphereRing_t3 *arc) {
    u32 count = arc->head.number;
    dds3SpPoly_SphereRingPhysics_t *record = arc->pPhysics;
    u32 delayStep = SDF_FRAMES(arc->spawnDelayStep);
    s32 age = POLY_INACTIVE_ENTRY_AGE;
    u32 index;

    index = 0;
    if (count != 0) {
        do {
            index++;
            record->frame = age;
            age -= delayStep;
            record++;
        } while (index < count);
    }
}
