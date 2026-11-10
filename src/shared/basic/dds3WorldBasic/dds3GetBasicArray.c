#include "shared/basic/dds3WorldBasic/decls.h"

dds3Basic_t *dds3GetBasicArray(const dds3BasicArrayHandle_t *hndl)
{
    if (hndl->count == 0) {
        return NULL;
    }
    if (hndl->nowindex < 0) {
        return NULL;
    }
    return ((dds3WorldBasic_t *)dds3TheWorld->workdata)->basicarray[hndl->nowindex].target;
}
