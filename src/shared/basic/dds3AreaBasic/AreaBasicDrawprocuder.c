#include "shared/basic/dds3AreaBasic/decls.h"

s32 AreaBasicDrawprocuder(dds3Basic_t *pBasic)
{
    dds3AreaBasicWorkData_t *work = pBasic->workdata;

    dds3ForEachBasicArray(work->items[0], CB_BasicDrawProcuder);
    return 1;
}
