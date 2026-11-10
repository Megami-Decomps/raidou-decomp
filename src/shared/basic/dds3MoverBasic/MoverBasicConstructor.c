#include "shared/basic/dds3MoverBasic/decls.h"

s32 MoverBasicConstructor(dds3Basic_t *pBasic)
{
    dds3MoverBasicWorkData_t *work;

    work = sdfChipAllocDirect(sizeof(dds3MoverBasicWorkData_t));
    pBasic->workdata = work;
    memset(work, 0, sizeof(dds3MoverBasicWorkData_t));
    return 1;
}
