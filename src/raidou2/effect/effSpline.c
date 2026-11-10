#include "common.h"

INCLUDE_ASM(const s32, "effect/effSpline", dds3AllocSplineHandle);

#include "shared/effect/effSpline/dds3FreeSplineHandle.c"

INCLUDE_ASM(const s32, "effect/effSpline", func_0020D230);

INCLUDE_ASM(const s32, "effect/effSpline", func_0020D2D0);

INCLUDE_ASM(const s32, "effect/effSpline", func_0020D400);

INCLUDE_ASM(const s32, "effect/effSpline", func_0020D530);

INCLUDE_ASM(const s32, "effect/effSpline", func_0020D540);

#include "shared/effect/effSpline/dds3AllocSplinePoints.c"

#include "shared/effect/effSpline/dds3BuildSplinePoints.c"

#include "shared/effect/effSpline/SolveTridiagonal.c"

#include "shared/effect/effSpline/dds3BuildSplineCoefs.c"

#include "shared/effect/effSpline/dds3BuildSplineCoefsSkipDup.c"
