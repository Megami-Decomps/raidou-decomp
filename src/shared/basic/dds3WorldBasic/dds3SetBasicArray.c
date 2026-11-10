#include "shared/basic/dds3WorldBasic/decls.h"

u32 dds3SetBasicArray(dds3BasicArrayHandle_t *hndl, dds3Basic_t *data)
{
    if (hndl->count == 0) {
        return 0;
    }
    if (hndl->nowindex < 0) {
        return 0;
    }
    ((dds3WorldBasic_t *)dds3TheWorld->workdata)->basicarray[hndl->nowindex].target = data;
    return 1;
}
