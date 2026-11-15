#include "common.h"
#include "ps2asm.h"
#include "sdk_libgraph.h"
#include "sdk_libkernl.h"

typedef struct {
    u64 pmode;
    u64 smode2;
    u64 dispfb;
    u64 display;
    u64 bgcolor;
} sceGsDispEnv;

void sceGsPutDispEnv(sceGsDispEnv *disp);

/* Reference implementation; see docs/matching/sdk/libgraph.md. */

void sceGsPutDispEnv(sceGsDispEnv *disp)
{
    GParam *gp = sceGsGetGParam();

    if (gp->version == 1) {
        *(volatile u64 *)0x12000000 = disp->pmode;
        *(volatile u64 *)0x12000070 = disp->dispfb;
        *(volatile u64 *)0x12000080 = disp->display;
        *(volatile u64 *)0x120000C0 = disp->bgcolor;
    } else {
        *(volatile u64 *)0x12000000 = disp->pmode;
        *(volatile u64 *)0x12000020 = disp->smode2;
        *(volatile u64 *)0x12000090 = disp->dispfb;
        *(volatile u64 *)0x120000A0 = disp->display;
        *(volatile u64 *)0x120000E0 = disp->bgcolor;
    }
}
