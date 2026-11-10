#include "shared/sdf/sdfModel/decls.h"

u32 sdfModelGetMarkPosCount(sdfModelRootNode_t *aRoot)
{
    const sdfModelChunkHeader_t *chunk = sdfModelGetExtraChunk(aRoot, 0x534F504D);

    if (chunk != NULL) {
        return (chunk->size - 0x10) >> 6;
    }
    return 0;
}
