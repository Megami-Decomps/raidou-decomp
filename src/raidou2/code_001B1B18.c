#include "common.h"
#include "mdlEffect.h"

INCLUDE_ASM(const s32, "code_001B1B18", mdlPibGetVersion);

u16 mdlPibGetChunkCount(pibHeaderChunk_t *record) {
    return record->parameter.part.count;
}
