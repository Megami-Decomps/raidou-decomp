#include "shared/model/mdlManager/decls.h"

sdfMotionCtrlr_t *mdlGetMotionCtrlr(mdlObj_t *aHandle, s32 aGroup)
{
    sdfMotionCtrlr_t *node;

    for (node = aHandle->inner->motion; node != NULL; node = node->next)
    {
        if (node->userId == aGroup)
        {
            return node;
        }
    }
    return NULL;
}
