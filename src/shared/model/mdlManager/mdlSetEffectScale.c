#include "shared/model/mdlManager/decls.h"

void mdlSetEffectScale(mdlObj_t *aHandle, f32 aScale)
{
    dds3ModelEffectCtrlr_t *item;

    for (item = aHandle->resourceItems; item != NULL; item = item->next)
    {
        mdlSetEffectScaleSub(aHandle, item, aScale);
    }
}
