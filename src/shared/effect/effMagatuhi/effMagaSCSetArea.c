#include "shared/effect/effMagatuhi/decls.h"

/* Update the first family's scattered float parameters without touching gaps. */
void effMagaSCSetArea(effMagatuhiCallback_t *hEffect, effMagatuhiFloatParams_t *pMagaSCArea) {
    effMagaSmokeConv_tag *dst = dds3MagatuhiGetHandle(hEffect->effect);

    dst->head.originX = pMagaSCArea->originX;
    dst->head.originY = pMagaSCArea->originY;
    dst->head.originZ = pMagaSCArea->originZ;
    dst->head.initialRadius = pMagaSCArea->initialRadius;
    dst->head.targetX = pMagaSCArea->targetX;
    dst->head.targetY = pMagaSCArea->targetY;
    dst->head.targetZ = pMagaSCArea->targetZ;
    dst->head.captureDistance = pMagaSCArea->captureDistance;
}
