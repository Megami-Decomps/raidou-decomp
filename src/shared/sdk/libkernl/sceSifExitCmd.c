#include "shared/sdk/libkernl/decls.h"

void sceSifExitCmd(void)
{
    DisableDmac(5);
    RemoveDmacHandler(5, sifDmacHandlerId);
    D_0052C2B8 = 0;
}
