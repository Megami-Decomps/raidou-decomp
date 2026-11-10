#include "shared/model/mdlManager/decls.h"

s32 mdlGetMajorNum(mdlObj_t *aHandle)
{
    return aHandle->sub->major;
}
