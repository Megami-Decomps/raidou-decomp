#include "shared/model/mdlManager/decls.h"

void mdlMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber, s32 aType,
                                     f32 aBeforeLeng, f32 aSmoothLeng) {
    sdfMotionCtrlr_t *node;
    s16 slotIndex;

    for (node = aHandle->inner->motion; node != NULL; node = node->next) {
        if (node->userId == aGroup) {
            break;
        }
    }
    if (node == NULL) {
        node = mdlCreateMotion(aHandle, aGroup);
    }
    slotIndex = node->userId2;
    aHandle->slots[slotIndex] = node;
    if (slotIndex == 0) {
        aHandle->first = node;
    }
    sdfMotionChangeBlend(node, aNumber, aType, aBeforeLeng, aSmoothLeng);
    aHandle->current.h.id = aGroup;
    aHandle->current.h.motionIndex = aNumber;
    mdlStopEffect(aHandle, slotIndex);
    mdlGenEffect(aHandle, aNumber, slotIndex);
}
