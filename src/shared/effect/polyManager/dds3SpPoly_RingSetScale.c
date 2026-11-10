#include "shared/effect/polyManager/decls.h"

void dds3SpPoly_RingSetScale(dds3SpPoly_Ring_t3 *hData, f32 scale) {
    hData->radialWidth = hData->radialWidth * scale;
    hData->liftStep = hData->liftStep * scale;
    hData->initialRadius = hData->initialRadius * scale;
    hData->targetRadius = hData->targetRadius * scale;
}
