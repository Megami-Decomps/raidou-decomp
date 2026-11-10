#include "shared/effect/effEvent/decls.h"

/* vu0 routine: Jitter four control points normal to the path and viewing direction.
 * Reuse the final edge normal for the endpoint; retain XYZ-only scale initialization. */
void effAPPoint4PartInit(effAPPoint4_tag *pAPPoint4, u32 index) {
    sdfVector4_t jitterScale;
    sdfVector4_t lastNormal;
    sdfVector4_t viewDirection;
    sdfVector4_t jitteredPoint;
    effAPPoint4Part_tag *record = pAPPoint4->pPart + index;
    effCubicBezierSlot_t *bezierSlot;
    f32 cursorStep;
    f32 centeredRandom;

    SDF_VU0_LOADVECTOR(aViewEye);
    SDF_VU0_LOADVECTOR_B(aViewTarget);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(&viewDirection);
    cursorStep = 1.0f / (f32)SDF_FRAMES(pAPPoint4->param.steps);
    bezierSlot = dds3Bezier4PGetPart(pAPPoint4->pBezier4P, index);
    bezierSlot->t = 0;
    bezierSlot->step = cursorStep;

    centeredRandom = dds3GetRandFloatA(dds3EffectBase) - 0.5f;
    jitterScale.x = pAPPoint4->param.jitter[0] * (centeredRandom + centeredRandom);
    jitterScale.y = jitterScale.x;
    jitterScale.z = jitterScale.x;
    SDF_VU0_LOADVECTOR(pAPPoint4->param.controlPoints[0]);
    SDF_VU0_LOADVECTOR_B(pAPPoint4->param.controlPoints[1]);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(&viewDirection);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(&jitterScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(&jitteredPoint);
    bezierSlot->controlPoints[0][0] = jitteredPoint.x;
    bezierSlot->controlPoints[0][1] = jitteredPoint.y;
    bezierSlot->controlPoints[0][2] = jitteredPoint.z;

    centeredRandom = dds3GetRandFloatA(dds3EffectBase) - 0.5f;
    jitterScale.x = pAPPoint4->param.jitter[1] * (centeredRandom + centeredRandom);
    jitterScale.y = jitterScale.x;
    jitterScale.z = jitterScale.x;
    SDF_VU0_LOADVECTOR(pAPPoint4->param.controlPoints[1]);
    SDF_VU0_LOADVECTOR_B(pAPPoint4->param.controlPoints[2]);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(&viewDirection);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(&jitterScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(&jitteredPoint);
    bezierSlot->controlPoints[1][0] = jitteredPoint.x;
    bezierSlot->controlPoints[1][1] = jitteredPoint.y;
    bezierSlot->controlPoints[1][2] = jitteredPoint.z;

    centeredRandom = dds3GetRandFloatA(dds3EffectBase) - 0.5f;
    jitterScale.x = pAPPoint4->param.jitter[2] * (centeredRandom + centeredRandom);
    jitterScale.y = jitterScale.x;
    jitterScale.z = jitterScale.x;
    SDF_VU0_LOADVECTOR(pAPPoint4->param.controlPoints[2]);
    SDF_VU0_LOADVECTOR_B(pAPPoint4->param.controlPoints[3]);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(&viewDirection);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&lastNormal);
    SDF_VU0_LOADVECTOR_B(&jitterScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(&jitteredPoint);
    bezierSlot->controlPoints[2][0] = jitteredPoint.x;
    bezierSlot->controlPoints[2][1] = jitteredPoint.y;
    bezierSlot->controlPoints[2][2] = jitteredPoint.z;

    centeredRandom = dds3GetRandFloatA(dds3EffectBase) - 0.5f;
    jitterScale.x = pAPPoint4->param.jitter[3] * (centeredRandom + centeredRandom);
    jitterScale.y = jitterScale.x;
    jitterScale.z = jitterScale.x;
    SDF_VU0_LOADVECTOR(&lastNormal);
    SDF_VU0_LOADVECTOR_B(&jitterScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(pAPPoint4->param.controlPoints[3]);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&jitteredPoint);
    bezierSlot->controlPoints[3][0] = jitteredPoint.x;
    bezierSlot->controlPoints[3][1] = jitteredPoint.y;
    bezierSlot->controlPoints[3][2] = jitteredPoint.z;
    dds3EffectPartSetColor(record->hPart, 0);
}
