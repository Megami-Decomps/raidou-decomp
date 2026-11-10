#include "shared/effect/effModelTrackPoly/decls.h"

void dds3HermitianTrackPolyGetVertex(dds3HermitianTrackPoly_tag *hPoly, void *pVertex0, void *pVertex1, s32 frame)
{
    s32 index = hPoly->current - 2 * (hPoly->split * (frame - 1) + frame);
    u8 *vertex;

    if (index < 2) {
        index += hPoly->point - 2;
    }
    vertex = (u8 *)hPoly->pVertex + index * 16;
    SDF_VU0_COPYVECTOR(pVertex0, vertex);
    SDF_VU0_COPYVECTOR(pVertex1, vertex + 16);
}
