#include "shared/effect/billManager/decls.h"

sdfTexHandle_t *dds3BillboardGetGeneralTex(s32 number) {
    return hGeneralBillboard[number]->u.child->hTex;
}
