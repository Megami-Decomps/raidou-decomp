#include "shared/basic/dds3AreaBasic/decls.h"

/* Remove the first matching entry from the list selected by the item's type. */
void dds3AreaBasicGetItemArray(dds3Basic_t *pAreaBasic, dds3Basic_t *itemType)
{
    dds3BasicArrayHandle_t *handle =
        dds3AreaBasicReleaseItem(pAreaBasic, dds3AreaBasicTypeHasItems(itemType->type >> 24));

    dds3SearchDeleteBasicArray(handle, itemType, DDS3_BASICARRAY_SERACHDELETE_MODE_FIRST);
    /* Retail and DDS1 dds3RemoveObjectFromOwnerWorldIndex retain a JAL tail.
     * The consumed list cursor is no longer needed after removal. */
    handle = NULL;
}
