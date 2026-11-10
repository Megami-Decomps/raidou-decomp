#include "shared/sdk/libdma/decls.h"

void sceDmaRecv(sceDmaChan *chan)
{
    WaitDma(chan);
    chan->qwc = 0;
    chan->chcr = ((((int)chan->chcr & ~0xC) | 0x4) & ~1) | 0x100;
}
