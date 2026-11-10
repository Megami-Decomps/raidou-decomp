#include "shared/sdf/sdfTex/decls.h"

u32 sdfTexGetCLUTAddr(sdfTexHandle_t *aHandle)
{
    sdfGsMemSlice_t *slice;

    slice = aHandle->clutGsHandle;
    if (slice == NULL) {
        return 0;
    }
    return slice->addr;
}
