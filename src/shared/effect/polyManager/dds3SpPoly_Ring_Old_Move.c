#include "shared/effect/polyManager/decls.h"

void dds3SpPoly_Ring_Old_Move(dds3SpPoly_Ring_Old_t3 *pRing, s32 number) {
    dds3TrackPoly_t *strip = pRing->hTrackPoly;
    dds3TrackPolyList_t *entry = &strip->trackList[number];
    sdfVector4_t scale;
    f32 *p;
    s32 pairs;
    s32 i;

    p = (f32 *)entry->pVertex;
    pairs = strip->point / 2;
    scale.x = scale.y = scale.z = SDF_FRAME_STEP(pRing->pairDisplacement);
    SDF_VU0_LOADVECTOR_C(&scale);
    for (i = 0; i < pairs; i++) {
        SDF_VU0_LOADVECTOR(p + 4);
        SDF_VU0_LOADVECTOR_B(p);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
        SDF_VU0_COPYVECTOR_C_TO_B();
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(p);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(p);
        SDF_VU0_LOADVECTOR(p + 4);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(p + 4);
        p += 8;
    }
}
