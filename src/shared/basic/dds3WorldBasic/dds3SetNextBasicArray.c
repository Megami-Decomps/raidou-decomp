#include "shared/basic/dds3WorldBasic/decls.h"

u32 dds3SetNextBasicArray(dds3BasicArrayHandle_t *hndl)
{
    if (hndl->nowindex < 0) {
        return 0;
    }
    hndl->nowindex =
        ((dds3WorldBasic_t *)dds3TheWorld->workdata)->basicarray[hndl->nowindex].next;
    return hndl->nowindex >= 0;
}
