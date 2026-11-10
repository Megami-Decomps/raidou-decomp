#include "shared/sdk/libpad/decls.h"

int scePadRead(int port, int slot, u8 *data)
{
    padDma *d;

    if (padPortState[port][slot].open == 0) {
        return 0;
    }
    d = scePadGetDmaStr(port, slot);
    memcpy(data, d, d->length);
    return d->length;
}
