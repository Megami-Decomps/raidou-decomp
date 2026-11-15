#include "common.h"
#include "ps2asm.h"
#include "sdk_libgraph.h"
#include "sdk_libkernl.h"

/* The GS interrupt mask register, through the kernel's syscall stubs. */

/* Reference implementation; see docs/matching/sdk/libgraph.md. */

u64 sceGsPutIMR(u64 imr)
{
    u64 old = GsGetIMR();

    GsPutIMR(imr);
    return old;
}
