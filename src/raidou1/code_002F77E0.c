#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "lmap.h"
#include "jmsMisc.h"
#include "TitleMain.h"
#include "fldProcess.h"
#include "sndAt3.h"
#include "mviManager.h"
#include "scrCommand.h"
#include "evtEventViewer.h"
#include "evtPolygonMovie.h"
#include "code_r1_002CAFD8.h"
#include "dds3PadManager.h"
#include "code_r1_001CC1D0.h"
#include "sdfMovie.h"
#include "dds3KernelDraw.h"
#include "rstResult.h"
#include "code_r1_002C7ED0.h"
#include "sdfSound.h"
#include "sdk_libc.h"
#include "cfg.h"
#include "dat.h"
#include "dds3AdminiProcess.h"
#include "code_002F77E0.h"

void rstLeaf(void) {
    s32 i;
    u32 *color = gGlobal->color;
    u32 value = 0x808080;

    i = 3;
    do {
        i--;
        *color = value;
        color++;
    } while (i >= 0);
}

INCLUDE_ASM(const s32, "code_002F77E0", rstCallSetOt);

INCLUDE_ASM(const s32, "code_002F77E0", rstCallSetOt2);

INCLUDE_ASM(const s32, "code_002F77E0", rstCallSetOt3);

INCLUDE_ASM(const s32, "code_002F77E0", rstCallSetOt4);

INCLUDE_ASM(const s32, "code_002F77E0", rstCallSin);

INCLUDE_ASM(const s32, "code_002F77E0", rstLeaf2);

INCLUDE_ASM(const s32, "code_002F77E0", rstCallSin2);

INCLUDE_ASM(const s32, "code_002F77E0", rstLeaf3);

INCLUDE_ASM(const s32, "code_002F77E0", rstCallDevLoadFileEx);

INCLUDE_ASM(const s32, "code_002F77E0", rstCallTexSet);

INCLUDE_ASM(const s32, "code_002F77E0", rstFunc2);

INCLUDE_ASM(const s32, "code_002F77E0", rstCallMemFreeDirect);

INCLUDE_ASM(const s32, "code_002F77E0", rstCallGlobalWorkUserInit2);

INCLUDE_ASM(const s32, "code_002F77E0", rstCallPlaySE);

void rstCallPlaySE2(void)
{
    if ((dds3PadButtonState[0x26] & 2) != 0) {
        gGlobal->cursor--;
        p2sdPlaySE(3, 0x7F, 0x3F);
    }
    if ((dds3PadButtonState[0x27] & 2) != 0) {
        gGlobal->cursor++;
        p2sdPlaySE(3, 0x7F, 0x3F);
    }
    if (gGlobal->cursor < 0) {
        gGlobal->cursor = 1;
    }
    if (gGlobal->cursor >= 2) {
        gGlobal->cursor = 0;
    }
}
