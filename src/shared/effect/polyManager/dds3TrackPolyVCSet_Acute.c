#include "shared/effect/polyManager/decls.h"

void dds3TrackPolyVCSet_Acute(dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out) {
    u32 startCarrier[4];
    u32 cellCarrier[4];
    u32 packed[4];
    u32 perCell = hTrackPoly->point >> 1;
    u32 cellCount = hTrackPoly->number;
    s32 i;

    startCarrier[0] = abgr_in & 0xFF000000;
    MMI_RGBA_UNPACK(startCarrier, 1.0f / 128.0f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(dds3TrackPolyVCSetAcuteParam);
    VU0_LERP_VF10_VF11(1.0f / (f32)perCell);
    SDF_VU0_COPYVECTOR_A_TO_C();
    for (i = 0; i < cellCount; i++) {
        u32 *vc = hTrackPoly->trackList[i].pVC;
        s32 j;

        cellCarrier[0] = abgr_in;
        MMI_RGBA_UNPACK_NOMEM(cellCarrier, 0x3C000000);
        for (j = 0; j < perCell; j++) {
            u32 color;

            SDF_VU0_COPYVECTOR_A_TO_B();
            MMI_RGBA_PACK_UNIT(color, 0x43000000);
            packed[0] = color;
            vc[0] = color;
            vc[1] = color;
            SDF_VU0_SUBVECTOR_B_C();
            SDF_VU0_COPYVECTOR_B_TO_A();
            vc += 2;
        }
    }
}
