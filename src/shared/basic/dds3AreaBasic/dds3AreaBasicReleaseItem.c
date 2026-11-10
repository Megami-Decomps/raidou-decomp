#include "shared/basic/dds3AreaBasic/decls.h"

dds3BasicArrayHandle_t *dds3AreaBasicReleaseItem(dds3Basic_t *pAreaBasic, u32 pItemBasic)
{
    dds3AreaBasicWorkData_t *work = pAreaBasic->workdata;

    return work->items[pItemBasic];
}
