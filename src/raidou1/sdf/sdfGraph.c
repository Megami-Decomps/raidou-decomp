#include "common.h"

#include "shared/sdf/sdfGraph/sdfGrSetFrameSetMode.c"

#include "shared/sdf/sdfGraph/sdfGrSetScreen.c"

#include "shared/sdf/sdfGraph/sdfGrFreeFrameSet.c"

#include "shared/sdf/sdfGraph/sdfGrMakeFrameSet.c"

#include "shared/sdf/sdfGraph/sdfGrSetDisp.c"

#include "shared/sdf/sdfGraph/sdfGrGsReset.c"

#include "shared/sdf/sdfGraph/sdfDrawThIntHandler.c"

#include "shared/sdf/sdfGraph/sdfGrReqCapture.c"

#include "shared/sdf/sdfGraph/sdfGrCapture.c"

#include "shared/sdf/sdfGraph/sdfDeliverDrawStart.c"

INCLUDE_ASM(const s32, "sdf/sdfGraph", sdfDeliverDrawEnd);

INCLUDE_ASM(const s32, "sdf/sdfGraph", sdfDrawThread);

INCLUDE_ASM(const s32, "sdf/sdfGraph", sdfGrInit);

INCLUDE_ASM(const s32, "sdf/sdfGraph", func_00358128);
INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_0046F278);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_0046F279);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_0046F27A);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_0046F27C);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_0046F280);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_0046F281);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_0046F284);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_0046F288);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_0046F28C);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_0046F290);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_0046F292);

