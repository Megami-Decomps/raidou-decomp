#include "shared/effect/effEvent/decls.h"

/* Copy xyz, lower y by half the aim height, clear w and publish the record. */
void effSimpleBEDSetPosition(effSimpleBED_t *hSimpleBED, f32 *pPos) {
    hSimpleBED->pParams.center[0] = pPos[0];
    hSimpleBED->pParams.center[1] = pPos[1] - hSimpleBED->pParams.h * 0.5f;
    hSimpleBED->pParams.center[2] = pPos[2];
    hSimpleBED->pParams.center[3] = 0;
    effEveBEDCutSetParams(hSimpleBED->hCut, &hSimpleBED->pParams);
}
