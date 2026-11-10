#include "shared/sdk/libkernl/decls.h"

void iInvalidDCache(void *start, void *end)
{
    _sceIDC(EE_ADDR_PTR(EE_ADDR(start) & ~63U),
             EE_ADDR_PTR(EE_ADDR(end) & ~63U));
}
