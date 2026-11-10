#include "shared/effect/effPCPMisc/decls.h"

void dds3RingPolyExpListDelete(dds3RingPolyExpList_tag *hData) {
    sdfMatFreeDelayed(hData->pMat);
    sdfMemFreeDirect(hData->hMem);
    sdfChipFreeDirect(hData);
}
