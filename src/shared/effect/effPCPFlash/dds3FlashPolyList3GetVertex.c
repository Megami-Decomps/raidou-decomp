#include "shared/effect/effPCPFlash/decls.h"

void *dds3FlashPolyList3GetVertex(dds3FlashPolyList_t *group, s32 groupIndex) {
    return group->pVertex + groupIndex * EFF_QUAD_POSITION_BYTES;
}
