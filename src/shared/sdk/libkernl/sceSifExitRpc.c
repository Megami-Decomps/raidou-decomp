#include "shared/sdk/libkernl/decls.h"

void sceSifExitRpc(void)
{
    sceSifExitCmd();
    sifRpcInitialized = 0;
}
