#include "shared/basic/dds3MatterObjectBasic/decls.h"

void dds3MatterObjectBasicDestructor(dds3Basic_t *aPtr) {
    dds3MatterObjectBasicWorkData_t *work;

    dds3FreePoseSet(aPtr);
    work = aPtr->workdata;
    dds3ObjectBaseFreeControlWorkData(work->pObjectBaseControlWorkData);
    sdfChipFreeDirect(work);
}
