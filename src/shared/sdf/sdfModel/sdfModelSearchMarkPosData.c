#include "shared/sdf/sdfModel/decls.h"

sdfModelMarkPos_t *sdfModelSearchMarkPosData(sdfModelRootNode_t *aRoot, s32 aId)
{
    sdfModelChunkHeader_t *chunk = sdfModelGetExtraChunk(aRoot, 0x534F504D);
    sdfModelMarkPos_t *data;
    u8 *end;

    if (chunk == NULL) {
        return NULL;
    }
    data = (sdfModelMarkPos_t *)((u8 *)chunk + 0x10);
    end = (u8 *)chunk + chunk->size;
    while ((u8 *)data < end) {
        if (data->id == aId) {
            return data;
        }
        data++;
    }
    return NULL;
}
