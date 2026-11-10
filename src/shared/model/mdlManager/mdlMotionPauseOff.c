#include "shared/model/mdlManager/decls.h"

void mdlMotionPauseOff(mdlObj_t *aHandle)
{
    sdfMotionCtrlr_t *node;

    for (node = aHandle->inner->motion; node != NULL; node = node->next)
    {
        sdfMotionPauseOff(node);
    }
}
