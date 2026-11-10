#include "shared/basic/dds3Basic/decls.h"

/* vu0 routine: compose scale, quaternion rotation and translation in vf28-31. */
void dds3MakeMatrix(const sdfVector4_t *scale, const sdfVector4_t *quaternion,
                    const sdfVector4_t *transrate)
{
    SDF_VU0_LOADVECTOR(quaternion);
    ToMatrix();
    SDF_VU0_LOADVECTOR(scale);
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(transrate);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
}
