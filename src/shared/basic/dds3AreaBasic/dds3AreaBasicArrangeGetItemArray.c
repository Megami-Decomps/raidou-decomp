#include "shared/basic/dds3AreaBasic/decls.h"

dds3BasicArrayHandle_t *dds3AreaBasicArrangeGetItemArray(dds3Basic_t *area, u32 itemType, dds3BasicProc_t check)
{
    dds3BasicArrayHandle_t *src;
    dds3BasicArrayHandle_t *dst;
    dds3Basic_t *basic;

    src = dds3AreaBasicReleaseItem(area, itemType);
    if (dds3GetBasicArrayCount(src) == 0) {
        return NULL;
    }

    dst = dds3GetBasicArrayHandle(0);
    dds3SetFirstBasicArray(src);
    do {
        basic = dds3GetBasicArray(src);
        if (check == NULL || check(basic) != 0) {
            dds3AddBasicArray(dst, 1);
            dds3SetBasicArray(dst, basic);
        }
    } while (dds3SetNextBasicArray(src) != 0);

    if (dds3GetBasicArrayCount(dst) == 0) {
        dds3FreeBasicArrayHandle(dst);
        return NULL;
    }
    return dst;
}
