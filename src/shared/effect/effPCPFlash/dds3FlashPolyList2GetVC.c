#include "shared/effect/effPCPFlash/decls.h"

void *dds3FlashPolyList2GetVC(dds3FlashPolyList_t *group, s32 groupIndex) {
    return group->pVC + groupIndex * EFF_TRIANGLE_COLOR_BYTES;
}
