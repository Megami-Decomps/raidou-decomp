#include "shared/effect/effPCPScatter/decls.h"

void effPCPWwSpherePolyPartMove(effPCPWwSpherePoly_tag *pWwSpherePoly, s32 index)
{
    f32 *vertex = dds3WhirlwindPolyListGetVertex(pWwSpherePoly->hPoly, index);
    effPCPWwSpherePolyPart_tag *ring;
    f32 angle;
    f32 radius;
    f32 step;
    f32 height;
    f32 s;
    u32 count;
    u32 i;

    dds3WhirlwindPolyListGetUV(pWwSpherePoly->hPoly, index);
    ring = &pWwSpherePoly->pPart[index];
    count = pWwSpherePoly->hPoly->vectorsPerParticle >> 1;
    angle = ring->radian + SDF_FRAME_STEP(ring->radian_s);
    radius = ring->radius;
    step = ring->radian_add;
    radius += SDF_FRAME_STEP(ring->radius_s);
    if (ring->frame >= SDF_FRAMES(pWwSpherePoly->param.radius_aw)) {
        ring->radius_s *= SDF_FRAME_DECAY(pWwSpherePoly->param.radius_a);
    }
    SetRotateMatrixXYZ(ring->rot);
    ring->radius = radius;
    ring->radian = angle;
    ring->radian_s *= SDF_FRAME_DECAY(pWwSpherePoly->param.roll_a);
    height = ring->width;
    for (i = 0; i < count; i++) {
        vertex[0] = sdfCos(angle) * radius;
        vertex[1] = 0;
        s = sdfSin(angle);
        vertex[4] = vertex[0];
        vertex[5] = -height;
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
