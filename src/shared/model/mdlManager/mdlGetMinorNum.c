#include "shared/model/mdlManager/decls.h"

s32 mdlGetMinorNum(mdlObj_t *aHandle)
{
    return aHandle->sub->minor;
}
