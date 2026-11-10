#include "shared/sdk/libpad/decls.h"

/* The IOP writes the newer frame counter into whichever half it filled last,
 * so the reader picks the half with the higher count. */
padDma *scePadGetDmaStr(int port, int slot)
{
    padDma *d = padPortState[port][slot].dma;
    int newer;

    SyncDCache(d, d + 2);
    newer = d[0].frame < d[1].frame;
    return (padDma *)((u8 *)d + (newer << 7));
}
