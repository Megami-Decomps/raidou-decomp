#include "shared/sdf/sdfMat/decls.h"

void sdfTexListSetClutColor(sdfItemArray_t *aList, s32 aColor, f32 aRatio)
{
    s32 i;
    s32 count;

    if (aList == NULL) {
        return;
    }
    count = aList->count;
    for (i = 0; i < count; i++) {
        sdfTexHandle_t *handle = ((sdfTexHandle_t **)aList->items)[i];

        if (handle->numOfCLUT != 0) {
            sdfTexSetClutColor(handle, aColor, aRatio);
        }
    }
}
