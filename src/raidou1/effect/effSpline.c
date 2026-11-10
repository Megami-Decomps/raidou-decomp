#include "common.h"

INCLUDE_ASM(const s32, "effect/effSpline", dds3AllocSplineHandle);

#include "shared/effect/effSpline/dds3FreeSplineHandle.c"

INCLUDE_ASM(const s32, "effect/effSpline", func_001F5F38);

INCLUDE_ASM(const s32, "effect/effSpline", func_001F5FD8);

INCLUDE_ASM(const s32, "effect/effSpline", func_001F6108);

INCLUDE_ASM(const s32, "effect/effSpline", func_001F6238);

INCLUDE_ASM(const s32, "effect/effSpline", func_001F6248);

#include "shared/effect/effSpline/dds3AllocSplinePoints.c"

#include "shared/effect/effSpline/dds3BuildSplinePoints.c"

#include "shared/effect/effSpline/SolveTridiagonal.c"

#include "shared/effect/effSpline/dds3BuildSplineCoefs.c"

#include "shared/effect/effSpline/dds3BuildSplineCoefsSkipDup.c"
