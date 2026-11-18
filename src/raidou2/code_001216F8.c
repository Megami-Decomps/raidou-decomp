#include "common.h"
#include "basic.h"

INCLUDE_ASM(const s32, "code_001216F8", dds3PointBasicGetResource);

void *dds3GetWorldNodeData(dds3Basic_t *node) {
    return node->workdata;
}
