#include "shared/effect/effPCPNeedle/decls.h"

dds3NeedleModel_tag *dds3NeedleModelCreate(u32 number)
{
    struct effAllocation_t *allocation = sdfMemAllocDirect(number * sizeof(dds3NeedleModelPart_tag) + sizeof(dds3NeedleModel_tag));
    dds3NeedleModel_tag *work = (void *)sdfMemLock(allocation);
    dds3NeedleModelPart_tag *entry;
    u32 entryIndex;

    work->blend = 2;
    work->pPart = (dds3NeedleModelPart_tag *)(work + 1);
    work->hMem = allocation;
    work->number = number;
    work->scale[0] = 1.0f;
    work->scale[1] = 1.0f;
    work->scale[2] = 1.0f;
    work->hVNMem = NULL;
    SDF_VU0_STOREIDENTITY(work->matrix);
    work->pMat = sdfMatNew();
    sdfMatSetConstant(work->pMat, 1.0f);
    entry = work->pPart;
    for (entryIndex = 0; entryIndex < number; entryIndex++, entry++) {
        entry->abgr = 0x80808080;
    }
    memset(&dds3NeedleModelPrimParam, 0, sizeof(dds3NeedleModelPrimParam));
    dds3NeedleModelPrimParam.primitive = 0x4000;
    return work;
}
