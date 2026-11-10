#include "shared/sdk/libdma/decls.h"

u32 CheckAddress(addr_t addr)
{
    if ((addr >> 28) == 7) {
        addr &= 0x0FFFFFFF;
        addr |= 0x80000000;
    }
    return addr;
}
