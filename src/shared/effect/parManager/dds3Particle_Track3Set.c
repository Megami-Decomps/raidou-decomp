#include "shared/effect/parManager/decls.h"

void dds3Particle_Track3Set(effTrackPolyList_t *hTrackPoly, s32 i, u32 abgr) {
    u128 axis[2];
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(parLook);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(parTrack_w);
    SDF_VU0_MULVECTOR();
    SDF_VU0_SWAPVECTOR_A_C();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(&axis[0]);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_SUBVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B(&axis[1]);
    Eff_ModelTrackPolyListVertexSet(hTrackPoly, i, axis);
    Eff_ModelTrackPolyListColorSet(hTrackPoly, i, (abgr & 0xFF000000) | 0x808080);
}
