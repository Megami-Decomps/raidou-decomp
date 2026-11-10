#include "shared/effect/billManager/decls.h"

u16 dds3BillboardGetAnimState(const billHandle_t *hBillboard) {
    if (hBillboard->type == 1) {
        return hBillboard->animationActive;
    }
    return 0;
}
