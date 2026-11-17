#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "kernel_draw.h"
#include "dds3KernelDraw.h"
#include "sdf2D.h"
#include "sdfPrim.h"
#include "sdfPacketBuff.h"
#include "sdfDraw.h"
#include "dds3KernelMain.h"

/* body: matched in raidou @ 88f0d54: src/shared/code_r1_0010DF68.c:184, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void sysLeaf24(void) {
}

INCLUDE_ASM(const s32, "code_0010DF50", sysLeaf25);

/* body: matched in dds @ 18d1fa88f: src/dds1/kernel/dds3SequenceList.c:421, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
u32 evtKeepBattleStageTestModePending(void) {
    return 0;
}
