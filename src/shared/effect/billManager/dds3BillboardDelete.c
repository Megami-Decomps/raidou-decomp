#include "shared/effect/billManager/decls.h"

void dds3BillboardDelete(billHandle_t *hData) {
    dds3BillboardFuncTable[hData->type].destroy(hData);
}
