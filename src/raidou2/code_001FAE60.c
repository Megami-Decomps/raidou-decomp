#include "common.h"
#include "basic.h"

void *effPkt2DGetPrimPtr(sdfGrPacketHeader_t *pPkt)
{
    return pPkt + 1;
}
