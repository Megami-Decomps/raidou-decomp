#include "shared/sdk/libpad/decls.h"

int scePadGetReqState(int port, int slot)
{
    if (padPortState[port][slot].open == 0) {
        return 0;
    }
    return scePadGetDmaStr(port, slot)->reqState;
}
