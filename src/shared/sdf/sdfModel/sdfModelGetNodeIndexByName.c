#include "shared/sdf/sdfModel/decls.h"

s32 sdfModelGetNodeIndexByName(sdfModelRootNode_t *aRoot, const char *aName)
{
    sdfModelChunkHeader_t *chunk = sdfModelGetExtraChunk(aRoot, 0x4D4E444E);
    u8 *entry;
    u8 *end;
    u32 len;

    if (chunk == NULL) {
        return -1;
    }
    entry = (u8 *)chunk + 8;
    end = (u8 *)chunk + chunk->size;
    len = strlen(aName);
    do {
        u32 entryLen = strlen((char *)entry);
        u8 *id = EE_ADDR_AS(u8 *, ((EE_ADDR((entry + entryLen + 4))) & ~3U));

        if (entryLen == len && memcmp(entry, aName, len) == 0) {
            return *(s32 *)id;
        }
        entry = id + 4;
    } while (entry < end);
    return -1;
}
