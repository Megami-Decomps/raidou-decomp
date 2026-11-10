#include "shared/effect/parManager/decls.h"

/* Triangle-cell variant of dds3Particle_TrackSetExp: three vertices per history segment. */
void dds3Particle_Track2SetExp(parSystem_t *hTrackPoly, s32 index, const f32 *pPos, u32 abgr, dds3ParOffsetBuff_t *pOffBuff) {
    dds3ParOffsetBuffParts_t *history = &pOffBuff->pParts[index];
    parCell_t *cell = &hTrackPoly->cells[index];
    u128 *vertex = cell->history;
    u32 count = history->count;
    sdfVector4_t point;
    sdfVector4_t localOrigin;

    if (count >= 2) {
        s32 segments;
        s32 segmentIndex;
        s32 lastIndex;
        s32 ring;

        SDF_VU0_COPYVECTOR(&localOrigin, pPos);
        segments = count - 1;
        cell->vertexCount = segments * 3;
        ring = history->point;
        lastIndex = pOffBuff->length - 1;
        ring = ring == 0 ? lastIndex : ring - 1;
        point.x = localOrigin.x + history->points[ring][0];
        point.y = localOrigin.y + history->points[ring][1];
        point.z = localOrigin.z + history->points[ring][2];
        point.w = 0;
        for (segmentIndex = 0; segmentIndex < segments; segmentIndex++) {
            SDF_VU0_LOADVECTOR(&point);
            ring = ring == 0 ? lastIndex : ring - 1;
            point.x = localOrigin.x + history->points[ring][0];
            point.y = localOrigin.y + history->points[ring][1];
            point.z = localOrigin.z + history->points[ring][2];
            SDF_VU0_LOADVECTOR_C(&point);
            SDF_VU0_COPYVECTOR_C_TO_B();
            SDF_VU0_SUBVECTOR();
            SDF_VU0_LOADVECTOR_B(parLook);
            SDF_VU0_OUTERPRODUCT();
            SDF_VU0_NORMALIZE();
            SDF_VU0_LOADVECTOR_B(parTrack_w);
            SDF_VU0_MULVECTOR();
            SDF_VU0_SWAPVECTOR_A_C();
            SDF_VU0_STOREVECTOR_NOMEM(vertex + 1);
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_ADDVECTOR_A_C();
            SDF_VU0_STOREVECTOR_NOMEM(vertex);
            SDF_VU0_COPYVECTOR_C_TO_A();
            SDF_VU0_SUBVECTOR_B_A();
            SDF_VU0_STOREVECTOR_B_NOMEM(vertex + 2);
            vertex += 3;
        }
        dds3TrackPolyVCRefresh_ParType2(hTrackPoly, index);
    }
    dds3TrackPolyColorSet(hTrackPoly, index, abgr);
}
