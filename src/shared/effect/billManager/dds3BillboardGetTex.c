#include "shared/effect/billManager/decls.h"

saddr_t dds3BillboardGetTex(billHandle_t *hBillboard) {
    if (hBillboard->type == 0) {
        return EE_ADDR_S(hBillboard->u.child->hTex);
    }
    return 0;
}
