#include "shared/effect/parManager/decls.h"

/* vu0 routine: compose the supplied matrix with the emitter's source matrix. */
void dds3ParticleSetRotateMatrix(dds3Particle_Basic_t *hData, void *pMatrix) {
    SDF_VU0_LOADMATRIX(pMatrix);
    SDF_VU0_LOADMATRIX_B(hData->rot_b);
    MultMatrixNoArg();
    SDF_VU0_STOREMATRIX(hData->rot);
}
