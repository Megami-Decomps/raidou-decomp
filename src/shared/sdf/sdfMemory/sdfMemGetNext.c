#include "shared/sdf/sdfMemory/decls.h"

/* The next handle, or NULL when that is the end sentinel. */
sdfMemHandle_t *sdfMemGetNext(sdfMemHandle_t *aHandle)
{
    sdfMemHandle_t *next = aHandle->next;

    if (next->stat == SDF_MEM_STAT_END) {
        return NULL;
    }
    return next;
}
