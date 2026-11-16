#include "common.h"
#include "effPacket.h"

void *effPkt2DGetPrimPtr(sdfGrPacketHeader_t *pPkt) {
    return pPkt + 1;
}
