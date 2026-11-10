#include "shared/effect/billManager/decls.h"

void dds3P2APolygonSetScale(s32 hData, float scale) {
    dds3BillboardSetScale((EE_ADDR_AS(dds3P2APolygon_tag *, hData))->hBillboard, scale, scale);
}
