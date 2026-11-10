#include "shared/sdf/sdfModel/decls.h"

void sdfModelNodeSetShape(sdfModelNode_t *aNode, void *aData, s32 aType)
{
    s32 pass;

    aNode->shape = aData;
    sdfModelPacketFree(aNode);
    sdfModelAllocTmPacket(aNode);
    sdfModelMakeLocalTm(aNode);
    for (pass = 0; pass != 2; pass++) {
        sdfModelLinkDrawPacket(aNode, aData, aType, 0, pass);
    }
}
