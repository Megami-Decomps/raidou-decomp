#include "shared/effect/polyManager/decls.h"

void dds3SpPoly_RingAllBuffInit(dds3SpPoly_Ring_t3 *band) {
    u32 count = band->head.number;
    dds3SpPoly_RingPhysics_t *record = band->pPhysics;
    u32 delayStep = SDF_FRAMES(band->spawnDelayStep);
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
