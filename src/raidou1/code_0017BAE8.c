#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "fld.h"
#include "fldNpcPath.h"
#include "fpu.h"
#include "ps2asm.h"
#include "sdfMath.h"
#include "sdfQuat.h"
#include "code_00174790.h"
#include "fldProcess.h"
#include "mapData.h"
#include "ctn.h"
#include "neko.h"
#include "lmap.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "dds3ObjectBase.h"
#include "code_r1_00128378.h"
#include "evtUnitManager.h"
#include "sdfSound.h"
#include "dds3KernelDraw_0010A750.h"
#include "dds3UnitObjectBasic.h"
#include "dds3Randomize.h"
#include "fldFileResolver.h"
#include "fldDraw.h"
#include "EventBit.h"
#include "evtStage.h"
#include "dds3KernelMain.h"
#include "dds3KernelDraw.h"
#include "mapLbWork.h"
#include "fldDbg.h"
#include "scrTraceCode.h"
#include "code_r1_0017BBD8.h"
#include "code_r1_0021CE60.h"

INCLUDE_ASM(const s32, "code_0017BAE8", pbLeaf);

INCLUDE_ASM(const s32, "code_0017BAE8", pbLeaf2);

INCLUDE_ASM(const s32, "code_0017BAE8", pbLeaf3);

INCLUDE_ASM(const s32, "code_0017BAE8", pbLeaf4);

INCLUDE_ASM(const s32, "code_0017BAE8", pbLeaf5);

INCLUDE_ASM(const s32, "code_0017BAE8", pbLeaf6);

INCLUDE_ASM(const s32, "code_0017BAE8", pbLeaf7);

INCLUDE_ASM(const s32, "code_0017BAE8", pbLeaf8);

INCLUDE_ASM(const s32, "code_0017BAE8", pbLeaf9);

void pbLeaf10(u32 job)
{
    pbLeaf10Data2.flags &= ~0x80;
    pbLeaf10Data.flags &= ~0x80;
}

INCLUDE_ASM(const s32, "code_0017BAE8", pbLeaf11);

INCLUDE_ASM(const s32, "code_0017BAE8", pbLeaf12);

INCLUDE_ASM(const s32, "code_0017BAE8", pbLeaf13);

INCLUDE_ASM(const s32, "code_0017BAE8", pbLeaf14);

/* body: matched in smt3 @ 9401f79e: src/smt3/pipe/pipeMain.c:130, found, same function 1.00 (words 18/18, calls 1.00, shape 1.00) */
void DEBUG_PRINT(const char *str, ...) {
}
