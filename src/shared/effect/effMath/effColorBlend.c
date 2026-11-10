#include "shared/effect/effMath/decls.h"

/* vu0 routine: blend two colours, `amount` of `from` */
u32 effColorBlend(u32 abgr0, u32 abgr1, f32 rate)
{
    effColorWork_t work[3];
    u32 blended;
    u32 unit;

    if (rate >= 1.0f) {
        return abgr1;
    }
    unit = EFF_RGBA_UNIT;
    work[0].word = abgr1;
    MMI_RGBA_UNPACK(&work[0], unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    work[1].word = abgr0;
    MMI_RGBA_UNPACK(&work[1], unit);
    SDF_VU0_SCALEVECTOR(1.0f - rate);
    SDF_VU0_SCALEVECTOR_B(rate);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_dds3ColorFtoI(blended, 128.0f);
    work[2].word = blended;
    return blended;
}
