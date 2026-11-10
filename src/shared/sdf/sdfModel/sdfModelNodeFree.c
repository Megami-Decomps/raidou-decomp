#include "shared/sdf/sdfModel/decls.h"

void sdfModelNodeFree(sdfModelNode_t *aNode)
{
    sdfModelPacketFree(aNode);
    sdfChipFreeDirect(aNode->tmPacket);
    aNode->tmPacket = NULL;
    sdfChipFreeDirect(aNode);
}
