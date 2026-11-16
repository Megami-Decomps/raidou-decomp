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
#include "scrTraceCode.h"

INCLUDE_ASM(const s32, "code_00113F20", scrGetCodePointer);

INCLUDE_ASM(const s32, "code_00113F20", scrSetCodePointer);

INCLUDE_ASM(const s32, "code_00113F20", scrGetScriptTimer);

/* body: matched in smt3 @ 9401f79e: src/smt3/script/scrTraceCode.c:1681, found, same function 1.00 (words 3/3, calls 1.00, shape 1.00) */
s32 scrGetCommandTimer(void)
{
    return nowpwork->CommandTimer;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/script/scrTraceCode.c:1686, found, same function 1.00 (words 3/3, calls 1.00, shape 1.00) */
s32 scrGetMessageHandle(void)
{
    return nowpwork->MesHandle;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/script/scrTraceCode.c:1691, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
scrProcessWork_s *scrGetScriptHandle(void)
{
    return nowpwork;
}

INCLUDE_ASM(const s32, "code_00113F20", scrGetData);
