#include "shared/effect/billManager/decls.h"

/* Retain a cloned billboard's shared data, create a fresh render asset, and reset both matrices. */
dds3P2APolygon_tag *dds3P2APolygonCopy(const dds3P2APolygon_tag *source) {
    dds3P2APolygon_tag *instance = sdfChipAllocDirect(sizeof(dds3P2APolygon_tag));

    instance->hBillboard = dds3BillboardCopy(
        source->hBillboard);
    instance->pMat = sdfMatNew();
    sdfMatSetConstant(instance->pMat, 1.0f);
    sdfMatSetDiffuse(instance->pMat, 0x80808080);
    sdfMatSetAmbient(instance->pMat, 0x80808080);
    sdfMatSetSpecular(instance->pMat, 0x80808080);
    SDF_VU0_STOREIDENTITY(instance->rot);
    SDF_VU0_STOREIDENTITY(instance->dir);
    return instance;
}
