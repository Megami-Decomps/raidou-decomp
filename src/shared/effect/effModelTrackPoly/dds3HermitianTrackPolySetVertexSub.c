#include "shared/effect/effModelTrackPoly/decls.h"

void dds3HermitianTrackPolySetVertexSub(dds3HermitianTrackPoly_tag *hPoly, const u128 *pVertex) {
    s32 position = hPoly->current;
    u128 *points = hPoly->pVertex;
    s32 count;

    SDF_VU0_COPYVECTOR(&points[position], pVertex);
    SDF_VU0_COPYVECTOR(&points[position + 1], pVertex + 1);
    position += 2;
    count = hPoly->point;
    hPoly->current = position;
    if (position == count) {
        SDF_VU0_COPYVECTOR(&points[0], pVertex);
        SDF_VU0_COPYVECTOR(&points[1], pVertex + 1);
        hPoly->current = 2;
    }
    if (hPoly->count < count - 2) {
        hPoly->count += 2;
    }
}
