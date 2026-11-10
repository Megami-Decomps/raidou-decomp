#include "shared/effect/polyManager/decls.h"

void dds3TrackPolySet2(dds3TrackPoly_t *hTrackPoly, s32 number, const u128 *pV) {
    dds3TrackPolyList_t *cell = &hTrackPoly->trackList[number];
    u128 *vertex;
    s32 shiftCount;
    s32 i;

    if (cell->rec == 0) {
        shiftCount = hTrackPoly->point - 3;
        vertex = cell->pVertex + shiftCount;
        for (i = 0; i < shiftCount; i++) {
            vertex--;
            SDF_VU0_COPYVECTOR(vertex + 3, vertex);
        }
        cell->rec = SDF_FRAMES(hTrackPoly->rec + 1) - 1;
        if (cell->count < shiftCount + 3) {
            cell->count += 3;
        }
    } else {
        cell->rec--;
        vertex = cell->pVertex;
    }
    SDF_VU0_COPYVECTOR(vertex, pV);
    SDF_VU0_COPYVECTOR(vertex + 1, pV + 1);
    SDF_VU0_COPYVECTOR(vertex + 2, pV + 2);
}
