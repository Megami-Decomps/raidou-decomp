#include "shared/basic/dds3AreaBasic/decls.h"

s32 AreaBasicConstructor(dds3Basic_t *pBasic)
{
    dds3AreaBasicWorkData_t *work;
    u32 i;

    work = sdfChipAllocDirect(sizeof(dds3AreaBasicWorkData_t));
    pBasic->workdata = work;
    for (i = 0; i < 2; i++) {
        work->items[i] = dds3GetBasicArrayHandle(0);
    }
    return 1;
}
