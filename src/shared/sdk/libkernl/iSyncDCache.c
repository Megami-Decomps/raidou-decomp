#include "shared/sdk/libkernl/decls.h"

void iSyncDCache(void *start, void *end)
{
    _sceSDC(EE_ADDR_PTR(EE_ADDR(start) & ~63U),
             EE_ADDR_PTR(EE_ADDR(end) & ~63U));
}
