#include "shared/model/mdlManager/decls.h"

void mdlSetEffectColor(mdlObj_t *aHandle, u32 aColor)
{
    dds3ModelEffectCtrlr_t *item;

    for (item = aHandle->resourceItems; item != NULL; item = item->next)
    {
        mdlSetEffectColorSub(aHandle, item, aColor);
    }
}
