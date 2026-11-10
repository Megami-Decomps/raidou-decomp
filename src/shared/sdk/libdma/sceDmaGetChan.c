#include "shared/sdk/libdma/decls.h"

sceDmaChan *sceDmaGetChan(u32 channel)
{
    if (channel < 10) {
        return sceDmaChanRegs[channel];
    }
    return NULL;
}
