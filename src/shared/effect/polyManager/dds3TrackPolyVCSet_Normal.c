#include "shared/effect/polyManager/decls.h"

void dds3TrackPolyVCSet_Normal(dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out)
{
    s32 perCell = hTrackPoly->point >> 1;
    s32 count = hTrackPoly->number;
    s32 i;

    for (i = 0; i < count; i++) {
        u32 *vc = hTrackPoly->trackList[i].pVC;
        s32 j;

        for (j = 0; j < perCell; j++) {
            vc[0] = abgr_in;
            vc[1] = abgr_out;
            vc += 2;
        }
    }
}
