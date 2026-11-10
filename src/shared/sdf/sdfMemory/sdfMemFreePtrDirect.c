#include "shared/sdf/sdfMemory/decls.h"

/* Resolve a data address to its handle before releasing it. */
void sdfMemFreePtrDirect(void *aPtr)
{
    sdfMemHandle_t *handle;

    handle = sdfMemGetHandleByPtr(aPtr);
    sdfMemFreeDirect(handle);
}
