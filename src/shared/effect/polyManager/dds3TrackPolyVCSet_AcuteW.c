#include "shared/effect/polyManager/decls.h"

void dds3TrackPolyVCSet_AcuteW(dds3TrackPoly_t *hTrackPoly, u32 color) {
    u32 words = hTrackPoly->point;
    u32 count = hTrackPoly->number;
    u32 quarter = words >> 2;
    u32 i;
    u32 j;
    u32 **cell;
    u32 *vertex;
    s32 alpha[4];
    s32 start[4];
    s32 rise[4];
    s32 fall[4];
    u32 base;
    u32 odd;
    u32 packed;

    alpha[0] = color & 0xFF000000;
    MMI_RGBA_UNPACK(alpha, 1.0f / 128.0f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(dds3TrackPolyVCSetAcuteWParam);
    SDF_VU0_INTERPOLATE(1.0f / quarter);
    SDF_VU0_COPYVECTOR_A_TO_C();
    i = 0;
    if (count != 0) {
        base = color & 0x00FFFFFF;
        odd = (words >> 1) & 1;
        cell = (u32 **)&hTrackPoly->trackList[0].pVC;
        do {
            vertex = *cell;
            start[0] = base;
            MMI_RGBA_UNPACK(start, 1.0f / 128.0f);
            /* Keep each packed ramp value in its output lane before copying it to both vertex words. */
            for (j = 0; j < quarter; j++) {
                SDF_VU0_COPYVECTOR_A_TO_B();
                MMI_RGBA_PACK_UNIT(packed, 128.0f);
                rise[0] = packed;
                vertex[0] = rise[0];
                vertex[1] = vertex[0];
                SDF_VU0_ADDVECTOR_B_C();
                SDF_VU0_COPYVECTOR_B_TO_A();
                vertex += 2;
            }
            if (odd) {
                vertex[0] = color;
                vertex[1] = color;
                vertex += 2;
            }
            for (j = 0; j < quarter; j++) {
                SDF_VU0_COPYVECTOR_A_TO_B();
                MMI_RGBA_PACK_UNIT(packed, 128.0f);
                fall[0] = packed;
                vertex[0] = fall[0];
                vertex[1] = vertex[0];
                SDF_VU0_SUBVECTOR_B_C();
                SDF_VU0_COPYVECTOR_B_TO_A();
                vertex += 2;
            }
            i++;
            cell = (u32 **)((u8 *)cell + sizeof(dds3TrackPolyList_t));
        } while (i < count);
    }
}
