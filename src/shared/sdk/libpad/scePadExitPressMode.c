#include "shared/sdk/libpad/decls.h"

int scePadExitPressMode(int port, int slot)
{
    if (padPortState[port][slot].open == 0) {
        return 0;
    }
    return scePadSetButtonInfo(port, slot, 0);
}
