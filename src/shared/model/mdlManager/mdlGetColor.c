#include "shared/model/mdlManager/decls.h"

u32 mdlGetColor(mdlObj_t *aHandle)
{
    return aHandle->inner->color;
}
