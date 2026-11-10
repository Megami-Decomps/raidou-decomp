#include "common.h"

INCLUDE_ASM(const s32, "sdf/sdfCoreDraw", sdfCorePacketGen);

#include "shared/sdf/sdfCoreDraw/sdfCoreDrawScanOneSkinNodes.c"

#include "shared/sdf/sdfCoreDraw/sdfCoreDrawSetLight.c"

#include "shared/sdf/sdfCoreDraw/sdfCoreDrawSetFog.c"

#include "shared/sdf/sdfCoreDraw/sdfCoreDrawSetZBias.c"

INCLUDE_ASM(const s32, "sdf/sdfCoreDraw", func_003681B0);

#include "shared/sdf/sdfCoreDraw/sdfCoreDrawInit.c"
