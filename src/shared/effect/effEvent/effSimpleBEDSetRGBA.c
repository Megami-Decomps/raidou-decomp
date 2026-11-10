#include "shared/effect/effEvent/decls.h"

/* Copy the holder's updated packed tint to its event record. */
void effSimpleBEDSetRGBA(effSimpleBED_t *hSimpleBED, u32 abgr) {
    hSimpleBED->pParams.abgr = abgr;
    effEveBEDCutSetParams(hSimpleBED->hCut, &hSimpleBED->pParams);
}
