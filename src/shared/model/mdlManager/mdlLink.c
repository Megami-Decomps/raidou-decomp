#include "shared/model/mdlManager/decls.h"

void mdlLink(mdlObj_t *aHandle, void *aModelOt)
{
    sdfMotionCtrlr_t **slot;
    sdfModelRootNode_t *inner;
    dds3ModelEffectCtrlr_t *item;
    s32 i;

    slot = aHandle->slots;
    for (i = 0; i != 4; i++)
    {
        if (*slot != NULL)
        {
            if ((*slot)->stat != 0)
            {
                sdfMotionNext(*slot);
            }
        }
        slot++;
    }
    if (aHandle->flags & 1)
    {
        return;
    }
    inner = aHandle->inner;
    sdfModelEvalTm(inner);
    sdfModelLinkOt(aModelOt, inner);
    if (aHandle->flags & 2)
    {
        return;
    }
    if (aHandle->flags & 4)
    {
        if ((inner->flags & 0x10) == 0)
        {
            return;
        }
    }
    for (item = aHandle->resourceItems; item != NULL; item = item->next)
    {
        mdlLinkEffect(aHandle, item);
    }
    if (aHandle->devList == NULL)
    {
        return;
    }
    mdlLinkBlur(aHandle, aModelOt);
}
