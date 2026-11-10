#ifndef DDS_SDF_VU_MATRIX_H

#define DDS_SDF_VU_MATRIX_H
#include "common.h"

struct Matrix4;

/* Build the camera basis from the global view target, eye, and up vectors.
 * The resulting basis rows replace VU0 registers vf28-vf31 in place. */
void vuBuildLookAtBasis(void);

#endif /* DDS_SDF_VU_MATRIX_H */
