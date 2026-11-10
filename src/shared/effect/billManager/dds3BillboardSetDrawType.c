#include "shared/effect/billManager/decls.h"

void dds3BillboardSetDrawType(billHandle_t *hBillboard, s32 type) {
    s32 maskedValue = type & BILL_VARIANT_MASK;

    switch (hBillboard->type) {
    case 0:
        hBillboard->u.child->type = (u16)maskedValue;
        break;
    case 1:
        hBillboard->pair.cameraFacingMode = maskedValue;
        break;
    }
}
