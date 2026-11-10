#include "shared/sdf/sdfDraw/decls.h"

void sdfGrSetClearPacket(sdfGrClearPacket_t *aClear, s32 aW, s32 aH, s32 aPixelFormat, s32 aZBuffFormat) {
    u32 lowerBounds = ((SDF_GS_CENTER_BIAS - aH) << 19) | ((SDF_GS_CENTER_BIAS - aW) << 3);
    u32 upperBounds = ((aH + SDF_GS_CENTER_BIAS) << 19) | ((aW + SDF_GS_CENTER_BIAS) << 3);

    aClear->primitive.reg = SDF_GS_PRIM;
    aClear->xyz2[0].reg = SDF_GS_XYZ2;
    aClear->test.data = SDF_GS_CENTERED_VIEW_BOUNDS_TEST;
    aClear->test.reg = SDF_GS_TEST_1;
    aClear->primitive.data = SDF_GS_PRIMITIVE_SPRITE;
    aClear->rgbaq.data = (u64)0xFE00 << 46;
    aClear->rgbaq.reg = SDF_GS_RGBAQ;
    aClear->xyz2[0].data = lowerBounds;
    aClear->xyz2[1].data = upperBounds;
    aClear->xyz2[1].reg = SDF_GS_XYZ2;
}
