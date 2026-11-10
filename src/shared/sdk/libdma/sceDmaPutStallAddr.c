#include "shared/sdk/libdma/decls.h"

u32 sceDmaPutStallAddr(u32 addr)
{
    u32 *reg = D_STADR;
    u32 old = *reg;

    *reg = addr;
    return old;
}
