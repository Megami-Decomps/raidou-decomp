#include "shared/sdf/sdfMemory/decls.h"

/* Unlink the successor, not the handle itself, and reconnect both links. */
void sdfMemFreeMerge(sdfMemHandle_t *aHandle)
{
    sdfMemHandle_t *following = aHandle->next->next;

    following->prev = aHandle;
    aHandle->next = following;
}
