#include "shared/model/mdlManager/decls.h"

void mdlSetColor(mdlObj_t *aHandle, u32 aColor)
{
    aHandle->inner->color = aColor;
    mdlSetEffectColor(aHandle, (aColor & 0xFF000000) | 0x808080);
}
