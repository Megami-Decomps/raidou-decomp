#include "shared/model/mdlManager/decls.h"

sdfMotionCtrlr_t *mdlCreateMotion(mdlObj_t *aHandle, s32 aGroup)
{
    sdfMotionPlayBook_t *motionTable;
    s16 motionSlotIndex;
    sdfMotionCtrlr_t *object;

    motionTable = aHandle->sub->playbooks[aGroup].ptr;
    motionSlotIndex = aHandle->sub->playbooks[aGroup].playGroup;
    object = sdfMotionCtrlrNew(aHandle->inner, motionTable);
    object->userId = aGroup;
    aHandle->slots[motionSlotIndex] = object;
    object->userId2 = motionSlotIndex;
    if (motionSlotIndex == 0)
    {
        aHandle->first = object;
    }
    return object;
}
