#include "common.h"
#include "sdk_libkernl.h"
#include "sdk_libcdvd.h"

extern char sdfHddDevPfs[];
extern char sdfHddDevHdd[];

/* body: matched in smt3 @ 9401f79e: src/smt3/sdf/sdfHdd.c:98, rework, same function 0.91 (words 33/32, calls 0.89, shape 0.86) */
void sdfHddPowerOffTh(s32 aSemaId)
{
    s32 result;

    while (1) {
        WaitSema(aSemaId);
        sceDevctl(sdfHddDevPfs, 0x5003, NULL, 0, NULL, 0);
        sceDevctl(sdfHddDevHdd, 0x4806, NULL, 0, NULL, 0);
        sceCdPowerOff(&result);
    }
}

void sdfHddPowerOffHandler(void *aArg)
{
    iSignalSema(EE_ADDR_S(aArg));
}

INCLUDE_ASM(const s32, "sdf/sdfHdd", sdfHddInit);
INCLUDE_SDATA(const s32, "sdf/sdfHdd", sdfHddDevPfs);

INCLUDE_SDATA(const s32, "sdf/sdfHdd", sdfHddDevHdd);

INCLUDE_SDATA(const s32, "sdf/sdfHdd", sdfHddMountPoint);

