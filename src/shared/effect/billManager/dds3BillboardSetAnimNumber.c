#include "shared/effect/billManager/decls.h"

void dds3BillboardSetAnimNumber(billHandle_t *hBillboard, u32 number) {
    if (hBillboard->type == 1 && hBillboard->animationEntryIndex != number) {
        dds3Billboard_P2ASetNumber(hBillboard, number);
    }
}
