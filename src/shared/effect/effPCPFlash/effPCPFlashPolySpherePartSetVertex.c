#include "shared/effect/effPCPFlash/decls.h"

void effPCPFlashPolySpherePartSetVertex(effPCPFlashPolySphere_tag *work, s32 index, void *view)
{
    effPCPFlashPolySpherePart_tag *part = &work->pPart[index];
    f32 *quad = (f32 *)dds3FlashPolyListGetVertex(work->hPoly, index);
    sdfVector4_t center;
    sdfVector4_t scale;
    sdfVector4_t across;
    sdfVector4_t up;
    f32 ratio;
    f32 size;
    f32 acrossLen;
    f32 upLen;

    size = part->radius;
    ratio = size / part->radius_max;
    VEC3_SPLAT(&scale.x, size);
    acrossLen = part->width_out * ratio;
    VEC3_SPLAT(&across.x, acrossLen);
    upLen = part->width_in * ratio;
    VEC3_SPLAT(&up.x, upLen);
    center.x = part->position[0];
    center.y = part->position[1];
    center.z = part->position[2];
    SDF_VU0_LOADVECTOR(&center);
    SDF_VU0_LOADVECTOR_B(view);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(&across);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&across);
    SDF_VU0_LOADVECTOR(&up);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&up);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(&scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(effPCPFlashPolySphereVertex);
    SDF_VU0_LOADVECTOR_B(&up);
    SDF_VU0_STOREVECTOR(quad + 12);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad + 8);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 16);
    SDF_VU0_LOADVECTOR(effPCPFlashPolySphereVertex);
    SDF_VU0_LOADVECTOR_B(&across);
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 4);
}
