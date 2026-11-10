#include "shared/basic/dds3EventObjectBasic/decls.h"

u32 EventObjectBasicConstructor(dds3Basic_t *pBasic)
{
    dds3EventObjectBasicWorkData_t *work;
    dds3ObjectBaseControlWorkData_t *control;

    dds3InitPoseSet(pBasic);
    work = sdfChipAllocDirect(sizeof(dds3EventObjectBasicWorkData_t));
    pBasic->workdata = work;
    control = dds3ObjectBaseInitControlWorkData(pBasic);
    work->pObjectBaseControlWorkData = control;
    return 1;
}
