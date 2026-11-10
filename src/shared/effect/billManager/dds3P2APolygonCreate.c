#include "shared/effect/billManager/decls.h"

/* Create the indexed billboard and render asset; only the second matrix is initialized. */
dds3P2APolygon_tag *dds3P2APolygonCreate(s32 entryIndex) {
    dds3P2APolygon_tag *instance = sdfChipAllocDirect(sizeof(dds3P2APolygon_tag));

    instance->hBillboard = dds3BillboardCreate(1, entryIndex);
    instance->pMat = sdfMatNew();
    sdfMatSetConstant(instance->pMat, 1.0f);
    SDF_VU0_STOREIDENTITY(instance->rot);
    return instance;
}
