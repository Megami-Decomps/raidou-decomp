#include "shared/sdk/libdma/decls.h"

int sceDmaRestart(sceDmaChan *chan, u32 chcr)
{
    u32 old = chan->chcr;

    chan->chcr = chcr;
    return (old >> 8) & 1;
}
