#include "shared/sdf/sdfMat/decls.h"

sdfItemArray_t *sdfMatListMake(sdfItemArray_t *aTexList, u32 *aData)
{
    u32 count = *aData;
    u8 *cursor = (u8 *)(aData + 1);
    sdfItemArray_t *list = sdfMatListNew(count >= 0x20 ? count : 0x20);

    while (count != 0) {
        sdfMat_t *mat = sdfMatNew();

        cursor = sdfMatSet(mat, aTexList, cursor);
        sdfMatListAdd(list, mat);
        count--;
    }
    return list;
}
