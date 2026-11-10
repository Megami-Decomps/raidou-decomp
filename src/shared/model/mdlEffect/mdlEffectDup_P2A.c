#include "shared/model/mdlEffect/decls.h"

void *mdlEffectDup_P2A(mdlEffectResrcItem_t *efcResPtr) {
    struct mdlBillboard_t *clone = dds3BillboardCopy(efcResPtr->handle);
    efcResPtr->count = efcResPtr->count + 1;
    return clone;
}
