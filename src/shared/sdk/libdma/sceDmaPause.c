#include "shared/sdk/libdma/decls.h"

int sceDmaPause(sceDmaChan *chan)
{
    int intr = DIntr();
    u32 enable = *D_ENABLER;
    u32 ctrl;
    u32 chcr;

    if ((enable & 0x10000) == 0) {
        *D_ENABLEW = enable | 0x10000;
    }
    ctrl = *VD_CTRL;
    chcr = CHCR(chan);
    CHCR(chan) = chcr & ~0x100;
    *D_ENABLEW = enable;
    if (intr) {
        EIntr();
    }
    return chcr;
}
