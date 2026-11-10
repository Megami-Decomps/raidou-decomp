#include "shared/basic/dds3EventObjectBasic/decls.h"

void EventObjectBasicDestructor(dds3Basic_t *pBasic)
{
    dds3EventObjectBasicWorkData_t *work;

    dds3FreePoseSet(pBasic);
    work = pBasic->workdata;
    dds3ObjectBaseFreeControlWorkData(work->pObjectBaseControlWorkData);
    sdfChipFreeDirect(work);
}
