#include "shared/sdf/sdfMemory/decls.h"

/* The address must belong to an existing used handle: the end-sentinel path
 * does not end this scan. */
sdfMemHandle_t *sdfMemGetHandleByPtr(void *aPtr)
{
    sdfMemHandleCB_tag *cb = &sdfMemCB;
    sdfMemHandle_t *handle;
    s32 intrEnabled;

    intrEnabled = DIntr();
    for (handle = cb->headGuard.next;; handle = handle->next) {
        if (handle->stat != SDF_MEM_STAT_USED) {
            if (handle->stat == SDF_MEM_STAT_END) {
                if (intrEnabled != 0) {
                    EIntr();
                }
            }
        } else if (handle->ptr == aPtr) {
            if (intrEnabled != 0) {
                EIntr();
            }
            return handle;
        }
    }
}
