#include "shared/sdk/libkernl/decls.h"

void VSync(void)
{
    s32 enabled;

    enabled = DIntr();
    *(volatile u32 *)0x1000F000 = 4;
    EE_SYNC();
    if (enabled != 0) {
        EIntr();
    }
    while ((*(volatile u32 *)0x1000F000 & 4) == 0) {
    }
    enabled = DIntr();
    *(volatile u32 *)0x1000F000 = 4;
    EE_SYNC();
    if (enabled != 0) {
        EIntr();
    }
}
