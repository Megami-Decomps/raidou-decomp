#include "shared/effect/effPCPNeedle/decls.h"

void dds3NeedleModelDelete(dds3NeedleModel_tag *hNModel)
{
    sdfMatFreeDelayed(hNModel->pMat);
    dds3NeedleModelVNCListDelete(hNModel);
    sdfMemFreeDirect(hNModel->hMem);
}
