#include "shared/sdk/libdma/decls.h"

int sceDmaSync(sceDmaChan *chan, int mode, int unused)
{
    if (mode == 1) {
        return (chan->chcr >> 8) & 1;
    }
    WaitDma(chan);
    return 0;
}
