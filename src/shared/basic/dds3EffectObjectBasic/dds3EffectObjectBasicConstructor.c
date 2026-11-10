#include "shared/basic/dds3EffectObjectBasic/decls.h"

s32 dds3EffectObjectBasicConstructor(dds3Basic_t *aPtr) {
    dds3EffectObjectBasicWorkData_t *data;

    dds3InitPoseSet(aPtr);
    aPtr->workdata = sdfChipAllocDirect(0x50);
    memset(aPtr->workdata, 0, 0x50);
    data = aPtr->workdata;
    data->pObjectBaseControlWorkData = dds3ObjectBaseInitControlWorkData(aPtr);
    dds3ObjectBaseOnFlag(aPtr, 0x60);
    return 1;
}
