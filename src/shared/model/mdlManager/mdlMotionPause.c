#include "shared/model/mdlManager/decls.h"

void mdlMotionPause(mdlObj_t *aHandle)
{
    sdfMotionCtrlr_t *node;

    for (node = aHandle->inner->motion; node != NULL; node = node->next)
    {
        sdfMotionPause(node);
    }
}
