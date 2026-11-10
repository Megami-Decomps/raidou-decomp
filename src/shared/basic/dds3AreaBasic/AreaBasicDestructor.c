#include "shared/basic/dds3AreaBasic/decls.h"

void AreaBasicDestructor(dds3Basic_t *pBasic)
{
    dds3AreaBasicWorkData_t *work;
    u32 i;

    work = pBasic->workdata;
    dds3ForEachBasicArray(work->items[0], CB_ReleaseObjectBasic);
    dds3ForEachBasicArray(work->items[1], CB_ReleaseObjectBasic);
    for (i = 0; i < 2; i++) {
        dds3FreeBasicArrayHandle(work->items[i]);
    }
    sdfChipFreeDirect(work);
}
