#include "common.h"

#include "shared/sdf/sdfDelayedResrc/sdfDelayedResrcInitCB.c"

#include "shared/sdf/sdfDelayedResrc/sdfDelayedResrcEntry.c"

#include "shared/sdf/sdfDelayedResrc/sdfDelayedResrcClose.c"

#include "shared/sdf/sdfDelayedResrc/sdfDelayedResrcRelease.c"

INCLUDE_ASM(const s32, "sdf/sdfDelayedResrc", sdfDelayedResrcProc);

#include "shared/sdf/sdfDelayedResrc/sdfDelayedResrcCheckFunc.c"

#include "shared/sdf/sdfDelayedResrc/sdfDelayedResrcCheck.c"

INCLUDE_SDATA(const s32, "sdf/sdfDelayedResrc", D_004E42B4);

