#include "shared/sdk/libdma/decls.h"

void sceDmaSend(sceDmaChan *chan, void *tag)
{
    u32 addr = CheckAddress(EE_ADDR(tag));

    WaitDma(chan);
    if (chan->tadr != 0xFFFFFFFF) {
        chan->tadr = addr;
    }
    chan->qwc = 0;
    chan->chcr = ((int)chan->chcr & ~0xC) | 0x105;
}
