#include "shared/sdf/sdfModel/decls.h"

u8 *sdfModelAllocTmPacket(sdfModelNode_t *aNode)
{
    u8 *packet = aNode->tmPacket;

    if (packet == NULL) {
        packet = sdfChipAllocDirect(0x100);
        aNode->tmPacket = packet;
    }
    return packet;
}
