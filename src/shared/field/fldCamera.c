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

/* body: matched in smt3 @ 9401f79e: src/smt3/field/fldCamera.c:88, near, same function 0.98 (words 106/107, calls 1.00, shape 0.95) */
f32 fldCamCalcSlopeOffset(f32 x, f32 z, f32 *xs, f32 *ys, f32 *zs) {
    s32 i, j, k;
    f32 xi, yi, zi, xj, yj, zj, xk, yk, zk;
    f32 dx, dz, lenA, lenB, travelA, travelB;
    f32 zA, zB, yA, yB, width, deltaZ, height;
    if (xs[0] == xs[1]) { i = 2; j = 0; k = 1; }
    else if (xs[0] == xs[2]) { i = 1; j = 0; k = 2; }
    else { i = 0; j = 1; k = 2; }
    xi = xs[i]; yi = ys[i]; zi = zs[i];
    xj = xs[j]; yj = ys[j]; zj = zs[j];
    xk = xs[k]; yk = ys[k]; zk = zs[k];
    dx = xj - xi; dz = zj - zi;
    lenA = fsqrtf(dx * dx + dz * dz);
    dx = xk - xi; dz = zk - zi;
    lenB = fsqrtf(dx * dx + dz * dz);
    travelA = lenA * (x - xi) / (xj - xi);
    travelB = lenB * (x - xi) / (xk - xi);
    zA = (zj - zi) * travelA / lenA + zi;
    zB = (zk - zi) * travelB / lenB + zi;
    yA = (yj - yi) * travelA / lenA + yi;
    yB = (yk - yi) * travelB / lenB + yi;
    width = zB - zA;
    deltaZ = z - zA;
    if (width < 0.001f && -0.001f < width) {
        height = yA;
    } else {
        height = (yB - yA) * deltaZ / width + yA;
    }
    return height;
}
