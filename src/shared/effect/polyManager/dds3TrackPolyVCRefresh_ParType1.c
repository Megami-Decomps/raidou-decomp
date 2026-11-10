#include "shared/effect/polyManager/decls.h"

void dds3TrackPolyVCRefresh_ParType1(dds3TrackPoly_t *hTrackPoly, s32 index) {
    u32 startCarrier[4];
    u32 cellCarrier[4];
    u32 packed[4];
    dds3TrackPolyList_t *cell = (dds3TrackPolyList_t *)((u8 *)(index * sizeof(dds3TrackPolyList_t)) + EE_ADDR(hTrackPoly->trackList));
    u32 *vc = cell->pVC;
    u32 perCell = cell->count >> 1;
    u32 color = vc[0];
    u32 j;

    if (perCell < 2) {
        return;
    }
    startCarrier[0] = color & 0xFF000000;
    MMI_RGBA_UNPACK(startCarrier, 1.0f / 128.0f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(dds3TrackPolyVCType1Param);
    VU0_LERP_VF10_VF11(1.0f / (f32)perCell);
    SDF_VU0_COPYVECTOR_A_TO_C();
    cellCarrier[0] = color;
    MMI_RGBA_UNPACK(cellCarrier, 1.0f / 128.0f);
    for (j = 0; j < perCell; j++) {
        u32 packedColor;

        SDF_VU0_COPYVECTOR_A_TO_B();
        MMI_RGBA_PACK_UNIT(packedColor, 0x43000000);
        packed[0] = packedColor;
        vc[0] = packedColor;
        vc[1] = packedColor;
        SDF_VU0_SUBVECTOR_B_C();
        SDF_VU0_COPYVECTOR_B_TO_A();
        vc += 2;
    }
}
