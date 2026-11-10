#include "shared/effect/polyManager/decls.h"

void dds3TrackPolyListInit(dds3TrackPoly_t *hTrackPoly, s32 number)
{
    dds3TrackPolyList_t *list = (dds3TrackPolyList_t *)(number * 20 + EE_ADDR(hTrackPoly->trackList));

    list->color = 0x80808080;
    list->rec = 0;
    list->count = 0;
}
