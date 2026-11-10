#include "shared/sdf/sdfModel/decls.h"

sdfDmaTag_t *sdfModelSetMatRefPacket(sdfModelRootNode_t *aRoot, s32 aMatIndex, sdfDmaTag_t *aBuffer,
                                     s32 aId)
{
    return sdfMatMakePacket(aRoot->matList->items[aMatIndex], aBuffer, aId);
}
