#include "shared/sdk/libkernl/decls.h"

void supplement_crt0(void)
{
    rpcSemaParam first;
    rpcSemaParam second;

    first.maxCount = 1;
    first.initCount = 1;
    second.maxCount = 1;
    second.initCount = 1;
    mallocSema = CreateSema(&first);
    D_0052C37C = CreateSema(&second);
}
