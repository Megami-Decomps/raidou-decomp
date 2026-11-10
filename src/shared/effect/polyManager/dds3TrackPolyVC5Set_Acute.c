#include "shared/effect/polyManager/decls.h"

void dds3TrackPolyVC5Set_Acute(dds3TrackPoly_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2) {
    s32 cellCount = hTrackPoly->number;
    s32 perCell = hTrackPoly->point / 5;
    u32 firstAlpha = abgr_0 & 0xFF000000;
    u32 secondAlpha = abgr_1 & 0xFF000000;
    u32 thirdAlpha = abgr_2 & 0xFF000000;
    u32 firstStep = firstAlpha / perCell;
    u32 secondStep = secondAlpha / perCell;
    u32 thirdStep = thirdAlpha / perCell;
    s32 i;

    abgr_0 &= 0x00FFFFFF;
    abgr_1 &= 0x00FFFFFF;
    abgr_2 &= 0x00FFFFFF;

    for (i = 0; i < cellCount; i++) {
        u32 *vc = hTrackPoly->trackList[i].pVC;
        u32 rampFirst = firstAlpha;
        u32 rampSecond = secondAlpha;
        u32 rampThird = thirdAlpha;
        s32 j;

        for (j = 0; j < perCell; j++) {
            rampFirst -= firstStep;
            rampSecond -= secondStep;
            rampThird -= thirdStep;
            vc[0] = abgr_2 | (rampThird & 0xFF000000);
            vc[1] = abgr_1 | (rampSecond & 0xFF000000);
            vc[2] = abgr_0 | (rampFirst & 0xFF000000);
            vc[3] = abgr_1 | (rampSecond & 0xFF000000);
            vc[4] = abgr_2 | (rampThird & 0xFF000000);
            vc += 5;
        }
    }
}
