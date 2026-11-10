#include "shared/effect/billManager/decls.h"

void dds3BillboardSetXYOffset(billHandle_t *hBillboard, float x, float y) {
    if (hBillboard->type == 0) {
        dds3BillResource_tag *child = hBillboard->u.child;
        child->width = x * 0.5f;
        child->height = y * 0.5f;
    }
}
