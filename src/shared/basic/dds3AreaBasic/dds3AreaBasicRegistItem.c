#include "shared/basic/dds3AreaBasic/decls.h"

void dds3AreaBasicRegistItem(dds3Basic_t *pAreaBasic, dds3Basic_t *pItemBasic)
{
    dds3BasicArrayHandle_t *handle;

    handle = dds3AreaBasicReleaseItem(pAreaBasic, dds3AreaBasicTypeHasItems(pItemBasic->type >> 24));
    dds3SetFirstBasicArray(handle);
    if (dds3SetSearchNextBasicArray(handle, pItemBasic) != 1) {
        dds3AddBasicArray(handle, 1);
        dds3SetBasicArray(handle, pItemBasic);
    }
}
