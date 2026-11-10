#include "shared/model/mdlManager/decls.h"

void mdlSetColorWithEffect(mdlObj_t *aHandle, u32 aColor)
{
    aHandle->inner->color = aColor;
    mdlSetEffectColor(aHandle, aColor);
}
