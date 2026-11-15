#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "kernel.h"
#include "ps2asm.h"
#include "basic.h"
#include "dds3obj.h"
#include "billboard.h"
#include "draw.h"
#include "scr.h"
#include "draw3d.h"
#include "field.h"
#include "model.h"
#include "motion.h"
#include "newdata.h"
#include "event.h"
#include "effect.h"
#include "sdfFrame.h"
#include "sdfAspect.h"

f32 fldCalcAngleBetween(f32 fromX, f32 fromZ, f32 toX, f32 toZ);
f32 GetSphericalPosMove(f32 fromX, f32 fromZ, f32 toX, f32 toZ);

typedef struct {
    f32 distance;
    f32 eyeHeight;
    f32 targetHeight;
    f32 fovy;
} fldCamParamRow_t;

f32 GetSphericalPosMove(f32 fromX, f32 fromZ, f32 toX, f32 toZ) {
    s32 angle = (s32)(360.0f - fldCalcAngleBetween(fromX, fromZ, toX, toZ) + 90.0f);
    return (f32)(angle % 360);
}

INCLUDE_ASM(const s32, "field/fldCamera", fldCamCalcSlopeOffset);
