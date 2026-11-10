#include "shared/sdf/sdfVu1/decls.h"

/* vu0 routine: lerp of two source rows (+0x20, +0x30) by the weight at node + 0x40 */
void corePktLerpUVColorChain(sdfCorePacketPointBuff_tag *node) {
    while (node != NULL) {
        void *firstSourceRow = node->sourceA;
        void *secondSourceRow = node->sourceB;
        VU0_BLEND_NODE_VECTORS(node, firstSourceRow, secondSourceRow);
        node = node->next;
    }
}
