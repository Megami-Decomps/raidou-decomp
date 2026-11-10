#include "shared/sdf/sdfDevFile/decls.h"

sdfItemArray_t *sdfTexSetTB(const sdfTextureOffsetListHeader_t *aData) {
    s32 i = 0;
    s32 count = aData->textureCount;
    sdfItemArray_t *handle = sdfTexListNew(count);
    const sdfTextureOffsetListEntry_t *entry;
    if (count != i) {
        entry = (const sdfTextureOffsetListEntry_t *)((const u8 *)aData + sizeof(*aData));
        do {
            i++;
            sdfTexListAdd(handle, sdfTexSet((TMX_HEADER_t *)(
                (void *)((const u8 *)aData + entry->textureHeaderOffset))));
            entry++;
        } while (i != count);
    }
    return handle;
}
