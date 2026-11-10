#include "shared/sdk/libkernl/decls.h"

s32 sceSifSyncIop(void)
{
    if (sceSifGetReg(4) & 0x40000) {
        sceResetttyinit();
        return 1;
    }
    return 0;
}
