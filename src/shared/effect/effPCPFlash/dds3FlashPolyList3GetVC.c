#include "shared/effect/effPCPFlash/decls.h"

void *dds3FlashPolyList3GetVC(dds3FlashPolyList_t *group, s32 groupIndex) {
    return group->pVC + groupIndex * EFF_QUAD_COLOR_BYTES;
}
