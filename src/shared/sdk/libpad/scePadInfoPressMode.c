#include "shared/sdk/libpad/decls.h"

int scePadInfoPressMode(int port, int slot)
{
    if (padPortState[port][slot].open == 0) {
        return 0;
    }
    return scePadGetButtonMask(port, slot) == 0x3FFFF;
}
