#include "shared/basic/dds3WorldBasic/decls.h"

u16 dds3GetBasicArrayCount(const dds3BasicArrayHandle_t *hndl)
{
    u16 count;

    count = 0;
    if (hndl != NULL) {
        count = hndl->count;
    }
    return count;
}
