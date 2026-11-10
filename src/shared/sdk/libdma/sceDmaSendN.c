#include "shared/sdk/libdma/decls.h"

void sceDmaSendN(sceDmaChan *chan, void *data, u32 qwc)
{
    u32 addr = CheckAddress(EE_ADDR(data));

    WaitDma(chan);
    if (chan->madr != 0xFFFFFFFF) {
        chan->madr = addr;
    }
    chan->qwc = qwc;
    chan->chcr = ((int)chan->chcr & ~0xC) | 0x101;
}
