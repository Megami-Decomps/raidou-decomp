#include "shared/effect/effMagatuhi/decls.h"

/* Jitter control points perpendicular to the path and camera viewing direction. */
void effMagaPoint4PartInit(effMagaPoint4_tag *pMagaPoint4, s32 index) {
    f32 scale[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 lastNormal[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 viewDirection[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 point[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    effCubicBezierSlot_t *slot;
    f32 step;
    f32 random;

    SDF_VU0_LOADVECTOR(aViewEye);
    SDF_VU0_LOADVECTOR_B(aViewTarget);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(viewDirection);
    step = 1.0f / (f32)SDF_FRAMES(pMagaPoint4->param.life);
    slot = dds3Bezier4PGetPart(pMagaPoint4->pBezier4P, index);
    slot->t = 0;
    slot->step = step;

    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    scale[0] = pMagaPoint4->param.jitterScales[0] * (random + random);
    scale[1] = scale[0];
    scale[2] = scale[0];
    SDF_VU0_LOADVECTOR(pMagaPoint4->param.point[0]);
    SDF_VU0_LOADVECTOR_B(pMagaPoint4->param.point[1]);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(viewDirection);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(point);
    slot->controlPoints[0][0] = point[0];
    slot->controlPoints[0][1] = point[1];
    slot->controlPoints[0][2] = point[2];

    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    scale[0] = pMagaPoint4->param.jitterScales[1] * (random + random);
    scale[1] = scale[0];
    scale[2] = scale[0];
    SDF_VU0_LOADVECTOR(pMagaPoint4->param.point[1]);
    SDF_VU0_LOADVECTOR_B(pMagaPoint4->param.point[2]);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(viewDirection);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(point);
    slot->controlPoints[1][0] = point[0];
    slot->controlPoints[1][1] = point[1];
    slot->controlPoints[1][2] = point[2];

    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    scale[0] = pMagaPoint4->param.jitterScales[2] * (random + random);
    scale[1] = scale[0];
    scale[2] = scale[0];
    SDF_VU0_LOADVECTOR(pMagaPoint4->param.point[2]);
    SDF_VU0_LOADVECTOR_B(pMagaPoint4->param.point[3]);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(viewDirection);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(lastNormal);
    SDF_VU0_LOADVECTOR_B(scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(point);
    slot->controlPoints[2][0] = point[0];
    slot->controlPoints[2][1] = point[1];
    slot->controlPoints[2][2] = point[2];

    /* The fourth point reuses the third segment's normalized side direction. */
    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    scale[0] = pMagaPoint4->param.jitterScales[3] * (random + random);
    scale[1] = scale[0];
    scale[2] = scale[0];
    SDF_VU0_LOADVECTOR(lastNormal);
    SDF_VU0_LOADVECTOR_B(scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(pMagaPoint4->param.point[3]);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(point);
    slot->controlPoints[3][0] = point[0];
    slot->controlPoints[3][1] = point[1];
    slot->controlPoints[3][2] = point[2];
    effMagaSmokeInitPart(pMagaPoint4->pMagatuhi->valueWork, index);
    effMagaCylinderSetPart(pMagaPoint4->pMagatuhi->valueWork, index, 0);
}
