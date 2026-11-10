#include "shared/sdk/libkernl/decls.h"

u64 VSync2(void)
{
    /* Kernel-published outputs: keep the result read after interrupt restore. */
    volatile u32 flag = 0;
    volatile u64 result;
    s32 enabled;

    SetVSyncFlag(&flag, &result);
    enabled = DIntr();
    *(volatile u32 *)0x1000F000 = 4;
    EE_SYNC();
    if (enabled != 0) {
        EIntr();
    }
    while ((*(volatile u32 *)0x1000F000 & 4) == 0 && flag == 0) {
    }
    enabled = DIntr();
    *(volatile u32 *)0x1000F000 = 4;
    EE_SYNC();
    if (enabled != 0) {
        EIntr();
    }
    return result;
}
