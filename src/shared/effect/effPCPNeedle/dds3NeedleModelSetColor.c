#include "shared/effect/effPCPNeedle/decls.h"

void dds3NeedleModelSetColor(dds3NeedleModel_tag *hNModel, s32 index, u32 abgr)
{
    hNModel->pPart[index].abgr = abgr;
}
