#include "shared/effect/effPCPMisc/decls.h"

effPCPSparkleSphere_tag *effPCPSparkleSphereCreate(effPCPSparkleSphereParam_t *params) {
    effPCPSparkleSphere_tag *billboard = (effPCPSparkleSphere_tag *)sdfChipAllocDirect(sizeof(effPCPSparkleSphere_tag));

    billboard->param = params->abgr;
    billboard->hBill = dds3BillboardGeneral(2);
    dds3BillboardSetMode(billboard->hBill, 2);
    billboard->abgr = 0x80808080;
    billboard->area = 100.0f;
    return billboard;
}
