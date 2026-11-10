#include "shared/sdf/sdfMemory/decls.h"

u16 sdfMemGetType(sdfMemHandle_t *aHandle)
{
    return aHandle->stat;
}
