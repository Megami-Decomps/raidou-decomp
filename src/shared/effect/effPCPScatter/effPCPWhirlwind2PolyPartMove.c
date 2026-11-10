#include "shared/effect/effPCPScatter/decls.h"

void effPCPWhirlwind2PolyPartMove(effPCPWhirlwind2Poly_tag *pWhirlwind2Poly, s32 index)
{
    f32 *vertex = dds3WhirlwindPolyListGetVertex(pWhirlwind2Poly->hPoly, index);
    effPCPWhirlwindPolyPart_tag *ring;
    f32 radius;
    f32 rise;
    f32 angle;
    f32 step;
    u32 count;
    u32 i;

    dds3WhirlwindPolyListGetUV(pWhirlwind2Poly->hPoly, index);
    ring = &pWhirlwind2Poly->pPart[index];
    radius = ring->radius + SDF_FRAME_STEP(ring->radius_s);
    count = pWhirlwind2Poly->hPoly->vectorsPerParticle >> 1;
    rise = ring->up_s;
    angle = ring->radian;
    step = ring->radian_add;
    SetRotateMatrixX(ring->rot_x);
    SetRotateMatrixY_B(ring->rot_y);
    MultMatrixRevNoArg();
    ring->rot_y += SDF_FRAME_STEP(ring->radian_s);
    ring->radian_s *= SDF_FRAME_DECAY(pWhirlwind2Poly->param.roll_a);
    ring->radius = radius;
    ring->radius_s *= SDF_FRAME_DECAY(pWhirlwind2Poly->param.radius_a);
    ring->up_s = rise * SDF_FRAME_DECAY(pWhirlwind2Poly->param.up_a);
    for (i = 0; i < count; i++) {
        f32 s;

        vertex[0] = sdfCos(angle) * radius;
        vertex[1] += SDF_FRAME_STEP(rise);
        s = sdfSin(angle);
        vertex[4] = vertex[0];
        vertex[5] += SDF_FRAME_STEP(rise);
        vertex[6] = vertex[2] = s * radius;
        SDF_VU0_LOADVECTOR(vertex);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(vertex);
        SDF_VU0_LOADVECTOR(vertex + 4);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(vertex + 4);
        angle += step;
        vertex += 8;
    }
}
