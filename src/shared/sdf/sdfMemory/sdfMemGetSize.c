#include "shared/sdf/sdfMemory/decls.h"

/* A handle's span runs to the next handle's start. */
s32 sdfMemGetSize(const sdfMemHandle_t *aHandle)
{
    const sdfMemHandle_t *next;
    saddr_t size;
    s32 intrEnabled;

    intrEnabled = DIntr();
    next = aHandle->next;
    size = EE_ADDR_S(next->ptr) - EE_ADDR_S(aHandle->ptr);
    if (intrEnabled) {
        EIntr();
    }
    return size;
}
