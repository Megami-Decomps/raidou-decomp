#include "shared/sdk/libpad/decls.h"

int scePadSetReqState(int port, int slot, int state)
{
    if (padPortState[port][slot].open == 0) {
        return 0;
    }
    scePadGetDmaStr(port, slot)->reqState = state;
    return 1;
}
