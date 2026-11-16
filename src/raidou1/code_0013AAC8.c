#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "dds3KernelDraw.h"
#include "event.h"
#include "scr.h"
#include "dds3SceneBasic.h"
#include "field.h"
#include "fldFileResolver_00132080.h"
#include "dds3Basic.h"
#include "dds3EventBasic.h"
#include "dds3PathBasic.h"
#include "dds3PointBasic.h"
#include "dds3CameraObjectBasic.h"
#include "dds3ObjectBase.h"
#include "dds3obj.h"
#include "include_asm.h"
#include "kernel_draw.h"
#include "sdf_packet_list.h"
#include "sdf_resource.h"
#include "sdk_libc.h"
#include "billboard.h"
#include "sdf_types.h"
#include "sdf_lighting.h"
#include "model.h"
#include "motion.h"
#include "draw.h"
#include "sdf_movie_types.h"
#include "ps2asm.h"
#include "fpu.h"
#include "draw3d.h"
#include "effPCPMisc_00224D10.h"
#include "fldFileResolver.h"
#include "datCalc.h"
#include "fld.h"
#include "fldProcess.h"
#include "mapData.h"
#include "mapLbWork.h"
#include "mdlManager.h"
#include "fldDbg.h"
#include "sdfQuat.h"
#include "sdfMath.h"
#include "sdk_libm.h"
#include "sdfMemory.h"
#include "fldDraw.h"

/* NON_MATCHING: 72 of 228 words match.  */
#ifdef NON_MATCHING
f32 mapGetScreenDir(f32 cx, f32 cy, f32 cz, f32 dir2d)
{
    f32 x0;
    f32 y0;
    f32 x1;
    f32 y1;
    f32 ang;
    f32 best;
    f32 bestDiff;
    f32 diff;
    f32 back;
    f32 rad;
    s32 z;

    while (dir2d >= 360.0f) {
        dir2d -= 360.0f;
    }
    while (dir2d < 0.0f) {
        dir2d += 360.0f;
    }
    best = 900.0f;
    fldPoint3To2(&x0, &y0, cx, cy, cz);
    bestDiff = 900.0f;
    for (ang = 0.0f; ang < 360.0f; ang += 2.8125f) {
        rad = (-ang * 3.14f) / 180.0f;
        fldPoint3To2(&x1, &y1, cx - sdfSin(rad) * 100.0f, cy, cz - sdfCos(rad) * 100.0f);
        diff = R360_LR(dir2d, sdfAtan2(x1 - x0, (y1 - y0) * 1.16f * 2.0f) * (180.0f / 3.14f));
        if (diff < 0.0f) {
            diff = -diff;
        }
        if (diff < bestDiff) {
            bestDiff = diff;
            best = ang;
        }
    }
    while (best >= 360.0f) {
        best -= 360.0f;
    }
    while (best < 0.0f) {
        best += 360.0f;
    }
    back = best + 180.0f;
    rad = (-back * 3.14f) / 180.0f;
    z = fldPoint3ToZ(cx - sdfSin(rad) * 100.0f, cy, cz - sdfCos(rad) * 100.0f);
    rad = (-best * 3.14f) / 180.0f;
    if (fldPoint3ToZ(cx - sdfSin(rad) * 100.0f, cy, cz - sdfCos(rad) * 100.0f) < z) {
        best = back;
        while (best >= 360.0f) {
            best -= 360.0f;
        }
        while (best < 0.0f) {
            best += 360.0f;
        }
    }
    return best;
}
#else
INCLUDE_ASM(const s32, "code_0013AAC8", mapGetScreenDir);
#endif

INCLUDE_ASM(const s32, "code_0013AAC8", mapCallSetOt5);

void mapBgDrawOnOff(s32 on)
{
    fldGlobalBase.bgDrawOn = on;
}

void mapDrawSBtlSceneSub(void)
{
}
