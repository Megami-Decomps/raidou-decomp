#include "shared/sdf/sdfVu1/decls.h"

/* vu0 routine: lerp of the XY pair at +0x30 by the weight at node + 0x40 */
void corePktLerpUVChain(sdfCorePacketPointBuff_tag *node) {
    while (node != NULL) {
        void *firstSourceRow = node->sourceA;
        void *secondSourceRow = node->sourceB;
        VU0_BLEND_NODE_XY(node, firstSourceRow, secondSourceRow);
        node = node->next;
    }
}
