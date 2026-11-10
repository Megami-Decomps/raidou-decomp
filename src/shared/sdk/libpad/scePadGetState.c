#include "shared/sdk/libpad/decls.h"

int scePadGetState(int port, int slot)
{
    padDma *d;

    if (padPortState[port][slot].open == 0) {
        return 0x63;
    }
    d = scePadGetDmaStr(port, slot);
    if (d->state == 6) {
        if (d->reqState == 2) {
            return 5;
        }
    }
    return d->state;
}
