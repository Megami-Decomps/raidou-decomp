#include "shared/event/evtPolygonMovie/decls.h"

/* vu0 routine: blend two RGBA8888 colours by t */
u32 evtHokanColor(s32 hokan, f32 ratio, u32 c1, u32 c2)
{
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;

    if (hokan) {
        color1[0] = c1;
        MMI_RGBA_UNPACK(color1, 1.0f / 128.0f);
        SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
        SDF_VU0_COPYVECTOR_A_TO_B();
        color2[0] = c2;
        MMI_RGBA_UNPACK(color2, 1.0f / 128.0f);
        SDF_VU0_SCALEVECTOR_BITS(ratio);
        SDF_VU0_ADDVECTOR();
        MMI_RGBA_PACK_UNIT(packed, 128.0f);
        blended[0] = packed;
        return blended[0];
    }
    return c1;
}
