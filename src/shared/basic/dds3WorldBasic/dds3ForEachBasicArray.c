#include "shared/basic/dds3WorldBasic/decls.h"

s32 dds3ForEachBasicArray(dds3BasicArrayHandle_t *handle, s32 (*func)(dds3Basic_t *))
{
    u32 valid;

    if (func == NULL) {
        return 1;
    }
    if (dds3GetBasicArrayCount(handle) == 0) {
        return 1;
    }
    for (valid = dds3SetFirstBasicArray(handle); valid != 0;
         valid = dds3SetNextBasicArray(handle)) {
        if (func(dds3GetBasicArray(handle)) == 0) {
            return 0;
        }
    }
    return 1;
}
