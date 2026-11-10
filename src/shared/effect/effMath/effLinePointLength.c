#include "shared/effect/effMath/decls.h"

/* vu0 routine: distance from `to` to the line through `from` along `dir` */
f32 effLinePointLength(sdfVector4_t *pDir, sdfVector4_t *pLinePos, sdfVector4_t *pPos)
{
    sdfVector4_t along;
    sdfVector4_t delta;
    f32 projection;
    f32 length;

    SDF_VU0_LOADVECTOR(pPos);
    SDF_VU0_LOADVECTOR_B(pLinePos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(&delta);
    SDF_VU0_LOADVECTOR_B(pDir);
    SDF_VU0_INNERPRODUCT(projection);
    along.x = pDir->x * projection;
    along.y = pDir->y * projection;
    along.z = pDir->z * projection;
    SDF_VU0_LOADVECTOR(&delta);
    SDF_VU0_LOADVECTOR_B(&along);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(length);
    return length;
}
