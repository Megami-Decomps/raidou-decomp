#include "shared/model/mdlManager/decls.h"

void mdlFree(mdlObj_t *aHandle)
{
    sdfModelRootNode_t *inner;
    dds3ModelEffectCtrlr_t *item;
    dds3ModelEffectCtrlr_t *next;

    inner = aHandle->inner;
    while (inner->motion != NULL)
    {
        sdfMotionCtrlrFree(inner->motion);
    }
    sdfTexListFree(inner->texList, 1);
    for (item = aHandle->resourceItems; item != NULL; item = next)
    {
        next = item->next;
        mdlFreeEffect(item);
    }
    mdlFreeBlur(aHandle);
    sdfModelRootFree(inner, 1, 1);
    mdlUnlinkResrc(aHandle);
    sdfChipFreeDirect(aHandle);
}
