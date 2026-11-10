#include "shared/sdk/libdma/decls.h"

int sceDmaWatch(sceDmaChan *chan, u32 addr, int mode, int timeout)
{
    if (mode == 1) {
        return chan->madr < addr;
    }
    WatchDma(chan, addr, timeout);
    return 0;
}
