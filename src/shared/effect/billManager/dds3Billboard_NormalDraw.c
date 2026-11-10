#include "shared/effect/billManager/decls.h"

void dds3Billboard_NormalDraw(billHandle_t *hData) {
    dds3BillboardTagLink(hData, hData->u.child);
}
