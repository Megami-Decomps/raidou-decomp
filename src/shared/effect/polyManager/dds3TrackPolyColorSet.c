#include "shared/effect/polyManager/decls.h"

void dds3TrackPolyColorSet(dds3TrackPoly_t *hTrackPoly, s32 number, u32 color) {
    hTrackPoly->trackList[number].color = color;
}
