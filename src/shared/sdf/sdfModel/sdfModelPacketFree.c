#include "shared/sdf/sdfModel/decls.h"

void sdfModelPacketFree(sdfModelNode_t *aNode)
{
    sdfModelPacketHeader_tag **chain = aNode->drawPacket;
    s32 i = 0;
    s32 count = 2;

    do {
        sdfModelPacketHeader_tag *packet = *chain;

        while (packet != NULL) {
            sdfModelPacketHeader_tag *next = packet->next;

            if (packet->memHandle != NULL) {
                sdfMemFreeDirect(packet->memHandle);
            } else {
                sdfChipFreeDirect(packet);
            }
            packet = next;
        }
        i++;
        chain++;
    } while (i != count);
}
