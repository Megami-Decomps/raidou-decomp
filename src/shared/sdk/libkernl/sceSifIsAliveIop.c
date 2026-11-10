#include "shared/sdk/libkernl/decls.h"

s32 sceSifIsAliveIop(void)
{
    s32 alive = 0;

    if (sceSifGetReg(4) & 0x10000) {
        alive = 1;
    }
    return alive;
}
