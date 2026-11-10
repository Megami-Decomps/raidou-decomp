#include "shared/effect/polyManager/decls.h"

u32 dds3SpPoly_Color(u32 frame, u32 frame_end, u32 abgr) {
    f32 ratio = 1.0f;
    s32 tint[4];
    u8 unused[16]; /* SMT3 and DDS1 twin both reserve 0x20 bytes,
                      * but neither touches the spare 0x10-byte local. */
    u32 packed;

    if (frame < frame_end) {
        ratio = (f32)frame / (f32)frame_end;
    }
    SDF_VU0_LOADVECTOR(dds3SpPolyColorFrom);
    SDF_VU0_LOADVECTOR_B(dds3SpPolyColorTo);
    SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
    SDF_VU0_SCALEVECTOR_B_BITS(ratio);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_B();
    tint[0] = abgr;
    MMI_RGBA_UNPACK(tint, 1.0f / 128.0f);
    SDF_VU0_MULVECTOR();
    MMI_RGBA_PACK_UNIT(packed, 128.0f);
    return packed;
}
