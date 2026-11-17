#include "common.h"
#include "sdfChip.h"

struct sdfTexPacket_t *sdfTexGetPacket(sdfTexHandle_t *aHandle)
{
    return aHandle->packet;
}
