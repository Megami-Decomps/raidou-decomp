#include "shared/basic/dds3WorldBasic/decls.h"

/* The walk stops at the first empty slot, so a NULL basic never matches. */
s32 dds3SetSearchNextBasicArray(dds3BasicArrayHandle_t *hArray, const dds3Basic_t *pBasic)
{
    dds3Basic_t *target;

    do {
        target = dds3GetBasicArray(hArray);
        if (target == NULL) {
            return 0;
        }
        if (pBasic == target) {
            return 1;
        }
    } while (dds3SetNextBasicArray(hArray) != 0);
    return 0;
}
