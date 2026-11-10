#include "shared/effect/effPCPFlash/decls.h"

void *dds3FlashPolyList2GetVertex(dds3FlashPolyList_t *group, s32 groupIndex) {
    return group->pVertex + groupIndex * EFF_TRIANGLE_POSITION_BYTES;
}
