#include "shared/sdf/sdfMemory/decls.h"

/* Coalesce adjacent free handles, then recycle this handle or mark it free. */
void sdfMemFreeDirect(sdfMemHandle_t *aHandle)
{
    sdfMemHandle_t *next;
    s32 intrEnabled;

    if (aHandle == NULL) {
        return;
    }
    intrEnabled = DIntr();
    next = aHandle->next;
    if (next->stat == SDF_MEM_STAT_FREE) {
        sdfMemFreeMerge(aHandle);
        sdfChipFreeDirect(next);
    }
    if (aHandle->prev->stat == SDF_MEM_STAT_FREE) {
        sdfMemFreeMerge(aHandle->prev);
        sdfChipFreeDirect(aHandle);
    } else {
        aHandle->stat = SDF_MEM_STAT_FREE;
        aHandle->mlock = 0;
    }
    if (intrEnabled != 0) {
        EIntr();
    }
}
