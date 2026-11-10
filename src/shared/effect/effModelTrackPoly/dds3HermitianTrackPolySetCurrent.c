#include "shared/effect/effModelTrackPoly/decls.h"

void dds3HermitianTrackPolySetCurrent(dds3HermitianTrackPoly_tag *hPoly, s32 frame) {
    s32 pos = hPoly->current + ((hPoly->split - 1) * (frame - 1) + frame) * -2;

    if (pos < 2) {
        pos += hPoly->point - 2;
    }
    hPoly->current = pos;
}
