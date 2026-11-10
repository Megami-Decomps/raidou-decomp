#include "shared/effect/effPCPFlash/decls.h"

/* vu0 routine: the orbit offset plus a tilted disc of corner offsets for an
   accumulating particle, whose radius grows by the work record's radial step */
void effPCPFlashPolyPipePartSetVertex(effPCPFlashPolyPipe_tag *work, s32 index)
{
    effPCPFlashPolyPipePart_tag *part = &work->parts[index];
    f32 *quad = dds3FlashPolyList3GetVertex(work->hPoly, index * 2);
    sdfVector4_t offset;
    sdfVector4_t unit;
    sdfVector4_t middle;
    sdfVector4_t outer;
    sdfVector4_t inner;
    sdfVector4_t size;
    f32 center;
    f32 outerEdge;
    f32 innerEdge;
    f32 span;
    f32 sinv;
    f32 height;
    f32 *mirror;

    center = part->up;
    VEC3_SPLAT(&middle.x, center);
    outerEdge = center + part->length;
    VEC3_SPLAT(&outer.x, outerEdge);
    innerEdge = center - part->length;
    VEC3_SPLAT(&inner.x, innerEdge);
    part->up = part->up + work->radialStep;
    span = part->width_in;
    VEC3_SPLAT(&size.x, span);
    unit.x = sdfCos(part->rotate);
    unit.y = 0;
    sinv = sdfSin(part->rotate);
    unit.z = sinv;
    offset.x = unit.x * work->orbitRadius;
    offset.y = 0;
    offset.z = sinv * work->orbitRadius;
    height = work->tilt;
    effPCPFlashPolyPipeVertex[0] = unit.x * height;
    effPCPFlashPolyPipeVertex[1] = height + -1.0f;
    effPCPFlashPolyPipeVertex[2] = sinv * height;
    SDF_VU0_LOADVECTOR(effPCPFlashPolyPipeVertex);
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(&unit);
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
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(&offset);
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_LOADVECTOR_B(&size);
    SDF_VU0_STOREVECTOR(quad + 8);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad + 4);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 12);
    SDF_VU0_LOADVECTOR(&offset);
    SDF_VU0_LOADVECTOR_B(&outer);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad);
    mirror = dds3FlashPolyList3GetVertex(work->hPoly, index * 2 + 1);
    SDF_VU0_COPYVECTOR(mirror + 8, quad + 8);
    SDF_VU0_COPYVECTOR(mirror + 4, quad + 4);
    SDF_VU0_COPYVECTOR(mirror + 12, quad + 12);
    SDF_VU0_LOADVECTOR(&offset);
    SDF_VU0_LOADVECTOR_B(&inner);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(mirror);
}
