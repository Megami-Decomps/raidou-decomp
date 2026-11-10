#include "shared/effect/effPCPFlash/decls.h"

/* vu0 routine: two quads of corner offsets for a flash particle (a strip and its mirror), turned around the view axis by the particle's angle */
void effPCPFlashPolySphere3PartSetVertex(effPCPFlashPolySphere3_tag *work, s32 index, void *view)
{
    effPCPFlashPolySphere3Part_tag *part = &work->parts[index];
    f32 *quad = (f32 *)dds3FlashPolyList3GetVertex(work->hPoly, index * 2);
    sdfVector4_t base;
    sdfVector4_t size;
    sdfVector4_t spare;
    sdfVector4_t middle;
    sdfVector4_t outer;
    sdfVector4_t inner;
    f32 center;
    f32 outerEdge;
    f32 innerEdge;
    f32 span;
    f32 *mirror;

    center = part->move;
    VEC3_SPLAT(&middle.x, center);
    outerEdge = center + part->radius;
    VEC3_SPLAT(&outer.x, outerEdge);
    innerEdge = center - part->radius;
    VEC3_SPLAT(&inner.x, innerEdge);
    span = part->width;
    VEC3_SPLAT(&size.x, span);
    SetRotateMatrixAxis(part->look_r, (const sdfVector3_t *)view);
    base.x = 0;
    base.y = 1.0f;
    base.z = 0;
    SDF_VU0_LOADVECTOR(&base);
    SDF_VU0_LOADVECTOR_B(view);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_COPYVECTOR_A_TO_C();
    /* retail multiplies the (never written) `spare` slot here and stores it back */
    SDF_VU0_LOADVECTOR_B(&spare);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&spare);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(view);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(&size);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&size);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(&outer);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&outer);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(&inner);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&inner);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(&middle);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(&size);
    SDF_VU0_STOREVECTOR(quad + 8);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad + 4);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 12);
    SDF_VU0_LOADVECTOR(&outer);
    SDF_VU0_STOREVECTOR(quad);
    mirror = (f32 *)dds3FlashPolyList3GetVertex(work->hPoly, index * 2 + 1);
    SDF_VU0_COPYVECTOR(mirror + 8, quad + 8);
    SDF_VU0_COPYVECTOR(mirror + 4, quad + 4);
    SDF_VU0_COPYVECTOR(mirror + 12, quad + 12);
    SDF_VU0_LOADVECTOR(&inner);
    SDF_VU0_STOREVECTOR(mirror);
}
