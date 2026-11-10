#include "shared/sdf/sdfMemory/decls.h"

/* Clear the owner's slot before releasing the handle it held. */
void sdfMemFreeHandleDirect(sdfMemHandle_t **aHandlePtr)
{
    sdfMemHandle_t *handle;

    handle = *aHandlePtr;
    if (handle != NULL) {
        *aHandlePtr = NULL;
        sdfMemFreeDirect(handle);
        return;
    }
}
