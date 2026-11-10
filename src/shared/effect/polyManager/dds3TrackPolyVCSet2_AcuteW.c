#include "shared/effect/polyManager/decls.h"

void dds3TrackPolyVCSet2_AcuteW(dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out) {
    s32 cellCount = hTrackPoly->number;
    s32 perCell = (hTrackPoly->point / 3) >> 1;
    u32 firstAlpha = abgr_in & 0xFF000000;
    u32 secondAlpha = abgr_out & 0xFF000000;
    u32 firstStep = firstAlpha / perCell;
    u32 secondStep = secondAlpha / perCell;
    s32 i;

    abgr_in &= 0x00FFFFFF;
    abgr_out &= 0x00FFFFFF;

    for (i = 0; i < cellCount; i++) {
        u32 *vc = hTrackPoly->trackList[i].pVC;
        u32 rampFirst = 0;
        u32 rampSecond = 0;
        s32 j;

        for (j = 0; j < perCell; j++) {
            vc[1] = abgr_in | (rampFirst & 0xFF000000);
            vc[2] = abgr_out | (rampSecond & 0xFF000000);
            vc[0] = abgr_out | (rampSecond & 0xFF000000);
            rampFirst += firstStep;
            rampSecond += secondStep;
            vc += 3;
        }
        rampFirst = firstAlpha;
        rampSecond = secondAlpha;
        for (j = 0; j < perCell; j++) {
            rampFirst -= firstStep;
            rampSecond -= secondStep;
            vc[1] = abgr_in | (rampFirst & 0xFF000000);
            vc[2] = abgr_out | (rampSecond & 0xFF000000);
            vc[0] = abgr_out | (rampSecond & 0xFF000000);
            vc += 3;
        }
    }
}
