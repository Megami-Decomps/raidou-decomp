#include "common.h"
#include "mdlEffect.h"

s32 mdlPibGetVersion(pibHeaderChunk_t *record)
{
    return record->payload.word;
}

u16 mdlPibGetChunkCount(pibHeaderChunk_t *record) {
    return record->parameter.part.count;
}
