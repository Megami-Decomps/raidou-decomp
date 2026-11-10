#include "shared/basic/dds3MoverBasic/decls.h"

void MoverBasicDestructor(dds3Basic_t *pBasic)
{
    dds3MoverBasicWorkData_t *work;

    work = pBasic->workdata;
    dds3MoverBasicFreePath(pBasic);
    dds3ObjectBaseSetControlBasicType(work->pObjectBasic, NULL, 1);
    sdfChipFreeDirect(work);
}
