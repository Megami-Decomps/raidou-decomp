#include "shared/interface/frFont/decls.h"

/* Read the packed table count, then skip the table and its trailing metadata
 * word to reach the encoded text. The native parser copies that metadata even
 * though this lookup does not use its value. */
u8 *MessageNumSearch(s32 Index, frTextBank_t *pData, s32 Size) {
    u32 offset;
    u32 tableCount;
    u32 textMetadata;

    if (Index >= pData->count) {
        return NULL;
    }
    offset = pData->entries[Index].offset + 0x38;
    tableCount = 0;
    memcpy(&tableCount, (u8 *)pData + offset, 2);
    offset += 4;
    offset += tableCount * 4;
    memcpy(&textMetadata, (u8 *)pData + offset, sizeof(textMetadata));
    offset += sizeof(textMetadata);
    return (u8 *)pData + offset;
}
