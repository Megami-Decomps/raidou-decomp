#include "shared/sdk/libkernl/decls.h"

void *SetTLBHandler(void *handler)
{
    tlbHandler = handler;
    SetVTLBRefillHandler(1, _kTLBException);
    SetVTLBRefillHandler(2, _kTLBException);
    SetVTLBRefillHandler(3, _kTLBException);
    return handler;
}
