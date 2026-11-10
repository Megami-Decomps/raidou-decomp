#include "shared/sdf/sdfMemory/decls.h"

void *sdfMemGetPtr(sdfMemHandle_t *aHandle)
{
    return aHandle->ptr;
}
