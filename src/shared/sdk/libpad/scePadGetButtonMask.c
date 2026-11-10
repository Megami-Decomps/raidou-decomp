#include "shared/sdk/libpad/decls.h"

u32 scePadGetButtonMask(int port, int slot)
{
    padDma *d;

    if (padPortState[port][slot].open == 0) {
        return 0;
    }
    d = scePadGetDmaStr(port, slot);
    if (d->ok != 1) {
        return 0;
    }
    if (d->ok2 < 2) {
        return 0;
    }
    if (d->f66 < 2) {
        return 0;
    }
    return (long)d->mask[0] + ((long)d->mask[1] << 8) + ((long)d->mask[2] << 16) +
           ((long)d->mask[3] << 24);
}
