#include "shared/sdk/libkernl/decls.h"

void _sceFsSemInit(void)
{
    rpcSemaParam param;
    if (fsCommandSema == -1) {
        /* Only these descriptor fields are initialized by the native helper. */
        param.initCount = 1;
        param.maxCount = 1;
        param.option = 0;
        fsCommandSema = CreateSema(&param);
    }
}
