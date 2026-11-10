#include "shared/sdf/sdfCoreDraw/decls.h"

void sdfCoreDrawScanOneSkinNodes(sdfModelRootNode_t *root, sdfCorePacket_t *p) {
    sdfModelNode_t **objects = root->nodeList->items;
    u16 *indices = (u16 *)(p + 1);
    if ((p->prim.packetBits & SDF_PACKETBIT_BLENDVERT) != 0) {
        s32 count = *indices;
        if (count != 0) {
            indices++;
            do {
                sdfModelAllocTmPacket(objects[*indices++]);
            } while (--count != 0);
        }
    }
}
