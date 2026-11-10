#include "shared/effect/billManager/decls.h"

void dds3BillboardSetAnimColorAdvMode(billHandle_t *hBillboard) {
    if (hBillboard->type == 1) {
        hBillboard->modeFlags |= 0x1000000;
    }
}
