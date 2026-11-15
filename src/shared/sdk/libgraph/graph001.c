#include "common.h"
#include "ps2asm.h"
#include "sdk_libgraph.h"
#include "sdk_libkernl.h"

/* Syscall 2 has the SetGsCrt void interface. */
extern void SetGsCrt(s16 interlace, s16 mode, s16 fieldMode);

/* Reference implementation; see docs/matching/sdk/libgraph.md. */

void sceGsResetGraph(s16 mode, s16 interlace, s16 outputMode, s16 fieldMode)
{
    GParam *param;

    switch (mode) {
    case 0:
        param = sceGsGetGParam();
        *(volatile u64 *)0x12001000 = 0x200;
        param->interlace = interlace;
        param->mode = outputMode;
        param->version = (*(volatile u64 *)0x12001000 >> 16) & 0xFF;
        GsPutIMR(0xFF00);
        param->fieldMode = fieldMode != 0;
        if (param->intcHandlerEnabled != 0) {
            DisableIntc(2);
            RemoveIntcHandler(2, param->intcHandlerId);
            param->intcHandlerEnabled = 0;
            param->intcHandlerId = 0;
        }
        SetGsCrt(interlace & 1, outputMode & 0xFF, fieldMode & 1);
        break;
    case 1:
        *(volatile u64 *)0x12001000 = 0x100;
        break;
    case 5:
        param = sceGsGetGParam();
        param->fieldMode = fieldMode != 0;
        param->interlace = interlace;
        param->mode = outputMode;
        param->version = (*(volatile u64 *)0x12001000 >> 16) & 0xFF;
        SetGsCrt(interlace & 1, outputMode & 0xFF, fieldMode & 1);
        break;
    }
}

GParam *sceGsGetGParam(void)
{
    return &sceGsGParam;
}
