#include "shared/model/mdlEffect/decls.h"

void mdlCreateEffectResrc_IPU(sdfItemArray_t *list, u8 *sourceAddress, sdfMemHandle_t *backingAllocation, s32 sourceSize) {
    mdlEffectResrc_IPU_t *node = sdfChipAllocZeroDirect(sizeof(*node));
    mdlEffectResrcItem_t *entry = &((mdlEffectResrcItem_t *)list->items)[list->count];

    node->dataSize = sourceSize;
    node->data = EE_ADDR(sourceAddress);
    node->dataMemHandle = backingAllocation;
    entry->type = MDL_PART_OBJECT;
    entry->count = 0;
    entry->handle = node;
    list->count += 1;
}
