#include "shared/effect/polyManager/decls.h"

void dds3SpPoly_RollRingAllBuffInit(dds3SpPoly_RollRing_t3 *rotatingBand) {
    u32 count = rotatingBand->head.number;
    dds3SpPoly_RollRingPhysics_t *record = rotatingBand->pPhysics;
    u32 delayStep = SDF_FRAMES(rotatingBand->spawnDelayStep);
    s32 age = POLY_INACTIVE_ENTRY_AGE;
    u32 index;

    index = 0;
    if (count != 0) {
        do {
            record->frame = age;
            record++;
            if ((index + 1) % rotatingBand->simu_num == 0) {
                age -= delayStep;
            }
            index++;
        } while (index < count);
    }
}
