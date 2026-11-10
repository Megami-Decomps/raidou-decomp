#include "shared/model/mdlEffect/decls.h"

void *mdlEffectDup_D3P(mdlEffectResrcItem_t *efcResPtr) {
    mdlEffect_t *clone = dds3EffectCopy(efcResPtr->handle);
    efcResPtr->count = efcResPtr->count + 1;
    return clone;
}
