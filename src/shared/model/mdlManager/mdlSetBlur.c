#include "shared/model/mdlManager/decls.h"

void mdlSetBlur(mdlObj_t *aHandle, s32 interval, s32 count,
                         u32 startColor, u32 endColor) {
    dds3ModelBlurCb_t *deviceList;
    f32 inverseSpan;
    /* Both retail versions home the packed colors 16 bytes apart. */
    u32 startCarrier[4];
    u32 endCarrier[4];

    mdlFreeBlur(aHandle);
    if (count > 0) {
        deviceList = sdfChipAllocZeroDirect(sizeof(dds3ModelBlurCb_t));
        deviceList->intervalCount = 1;
        aHandle->devList = deviceList;
        deviceList->spawnCount = count;
        deviceList->count = count;
        deviceList->interval = SDF_FRAMES(interval);
        /* Final argument use: see docs/idioms.md, "Final argument use: mdlSetBlur". */
        deviceList->keepCount = count * SDF_FRAMES(interval);
        inverseSpan = 1.0f / deviceList->keepCount;
        startCarrier[0] = endColor;
        EE_MMI_RGBA_UNPACK(startCarrier, 0.0078125f);
        SDF_VU0_STOREVECTOR(deviceList->colorBase);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        endCarrier[0] = startColor;
        EE_MMI_RGBA_UNPACK(endCarrier, 0.0078125f);
        SDF_VU0_SUBVECTOR_EXTENDED();
        SDF_VU0_SCALEVECTOR_V0(inverseSpan);
        SDF_VU0_STOREVECTOR(deviceList->colorDelta);
    }
}
