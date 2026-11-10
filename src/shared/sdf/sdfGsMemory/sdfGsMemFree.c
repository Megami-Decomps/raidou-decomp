#include "shared/sdf/sdfGsMemory/decls.h"

void sdfGsMemFree(sdfGsMemSlice_t *aHandle)
{
    s32 reenable;

    if (aHandle == NULL) {
        return;
    }
    reenable = DIntr();
    aHandle->type = 0;
    sdfGsMemCheckMerge(aHandle);
    if (aHandle->prev != NULL) {
        if (aHandle->prev->type == 0) {
            sdfGsMemCheckMerge(aHandle->prev);
        }
    }
    if (reenable != 0) {
        EIntr();
    }
}
