#include "shared/sdf/sdfMemory/decls.h"

void *sdfMemLock(sdfMemHandle_t *aHandle)
{
    aHandle->mlock = aHandle->mlock + 1;
    return aHandle->ptr;
}
