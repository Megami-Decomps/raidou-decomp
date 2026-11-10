#include "shared/basic/dds3EffectObjectBasic/decls.h"

void dds3EffectObjectBasicDestructor(dds3Basic_t *aPtr) {
    dds3EffectObjectBasicWorkData_t *data;

    data = aPtr->workdata;
    FreeEffectData(data);
    dds3FreePoseSet(aPtr);
    dds3ObjectBaseFreeControlWorkData(data->pObjectBaseControlWorkData);
    sdfChipFreeDirect(aPtr->workdata);
    aPtr->workdata = NULL;
}
