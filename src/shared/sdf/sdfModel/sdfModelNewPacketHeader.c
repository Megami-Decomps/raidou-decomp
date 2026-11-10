#include "shared/sdf/sdfModel/decls.h"

sdfModelPacketHeader_tag *sdfModelNewPacketHeader(sdfModelNode_t *aNode, s32 aOtIndex, s32 aId)
{
    sdfModelPacketHeader_tag *packet = sdfChipAllocDirect(sizeof(*packet));
    sdfModelPacketHeader_tag **head =
        (sdfModelPacketHeader_tag **)((u32)aId * sizeof(*aNode->drawPacket) + EE_ADDR(aNode) +
                                    (u32)&((sdfModelNode_t *)0)->drawPacket);
    sdfModelPacketHeader_tag *next = *head;

    packet->otIndex = aOtIndex;
    *head = packet;
    packet->size = 1;
    packet->memHandle = NULL;
    packet->next = next;
    return packet;
}
