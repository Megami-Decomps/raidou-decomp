#include "shared/model/mdlManager/decls.h"

void mdlFreeMotionCtrlr(mdlObj_t *aHandle, sdfMotionCtrlr_t *aCtrlr)
{
    if (aHandle->slots[aCtrlr->userId2] == aCtrlr)
    {
        aHandle->slots[aCtrlr->userId2] = NULL;
    }
    sdfMotionCtrlrFree(aCtrlr);
}
