#include "shared/sdk/libpad/decls.h"

int scePadEnterPressMode(int port, int slot)
{
    if (padPortState[port][slot].open == 0) {
        return 0;
    }
    return scePadSetButtonInfo(port, slot, 0xFFF);
}
