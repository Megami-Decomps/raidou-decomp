#include "shared/sdf/sdfGsMemory/decls.h"

s32 sdfGsMemCheckMerge(sdfGsMemSlice_t *aHandle)
{
    sdfGsMemSlice_t *below = aHandle->next;

    if (below == NULL || below->type != 0) {
        return 0;
    }
    aHandle->size += below->size;
    if ((aHandle->next = below->next) != NULL) {
        below->next->prev = aHandle;
    } else {
        gsMemHandleLinkTail = aHandle;
    }
    sdfChipFreeDirect(below);
    return 1;
}
