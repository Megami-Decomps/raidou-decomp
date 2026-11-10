#include "shared/sdf/sdfMemory/decls.h"

/* NULL is a sentinel and is never queued. */
void sdfMemFreeDelayedDirect(sdfMemHandle_t *aHandle)
{
    if (aHandle != NULL) {
        sdfDelayedResrcEntry(&sdfMemDelayedResrcCB, aHandle);
    }
}
