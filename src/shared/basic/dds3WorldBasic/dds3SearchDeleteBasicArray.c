#include "shared/basic/dds3WorldBasic/decls.h"

s32 dds3SearchDeleteBasicArray(dds3BasicArrayHandle_t *hArray, dds3Basic_t *pBasic, s32 mode)
{
    s32 deleted;

    deleted = 0;
    if (dds3GetBasicArrayCount(hArray) != 0) {
        dds3SetFirstBasicArray(hArray);
        do {
            if (dds3SetSearchNextBasicArray(hArray, pBasic) != 1) {
                break;
            }
            dds3DeleteBasicArray(hArray);
            deleted = 1;
        } while (mode != 0);
    }
    return deleted;
}
