#include "shared/model/mdlManager/decls.h"

void mdlFreeMotion(mdlObj_t *aHandle, s32 aGroup) {
    sdfMotionCtrlr_t *node = aHandle->inner->motion;

    while (node != NULL) {
        if (node->userId == aGroup) {
            mdlFreeMotionCtrlr(aHandle, node);
            break;
        }
        node = node->next;
    }
}
