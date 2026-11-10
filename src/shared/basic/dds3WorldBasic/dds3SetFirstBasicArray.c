#include "shared/basic/dds3WorldBasic/decls.h"

u32 dds3SetFirstBasicArray(dds3BasicArrayHandle_t *hndl)
{
    hndl->nowindex = hndl->firstindex;
    return hndl->firstindex >= 0;
}
