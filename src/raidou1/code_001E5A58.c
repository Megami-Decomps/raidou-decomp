#include "common.h"
#include "effEvent.h"
#include "sdfMemory.h"

effArrHdr_t *effCallMemAllocDirect(s32 number) {
    void *allocation = sdfMemAllocDirect(number * sizeof(effBezierCurve_t) + sizeof(effArrHdr_t));
    void *retainedAddress = (void *)sdfMemLock(allocation);
    u32 slotIndex = 0;
    effBezierCurve_t *slot = retainedAddress;
    u8 *headerAddress = (u8 *)slot + number * sizeof(effBezierCurve_t);

    ((effArrHdr_t *)headerAddress)->allocation = allocation;
    ((effArrHdr_t *)headerAddress)->slots = retainedAddress;
    ((effArrHdr_t *)headerAddress)->count = number;
    if (number != 0) {
        do {
            slotIndex++;
            slot->pointIndex = 0;
            slot->t = 0;
            slot->parameterStep = 0.05f;
            slot = (effBezierCurve_t *)((u8 *)slot + sizeof(effBezierCurve_t));
        } while (slotIndex < number);
    }
    return (effArrHdr_t *)headerAddress;
}

INCLUDE_ASM(const s32, "code_001E5A58", effLeaf24);

/* body: matched in smt3 @ 9401f79eb: src/smt3/effect/effDebug.c:96, match, same function 0.99 (words 81/82, calls 1.00, shape 0.99) */
s32 effLeaf25(effArrHdr_t *table, s32 index, f32 *out) {
    effBezierCurve_t *slot = &((effBezierCurve_t *)table->slots)[index];
    u32 pointIndex = slot->pointIndex;
    sdfVector4_t weights;
    f32 t;
    f32 u;
    effBezierPoint_t *controlPoints;

    if (pointIndex == 7) {
        return 0;
    }
    t = slot->t;
    u = 1.0f - t;
    controlPoints = &slot->controlPoints[pointIndex];
    weights.x = u * u * u;
    weights.y = t * (u * u) * 3.0f;
    weights.z = t * t * u * 3.0f;
    weights.w = t * t * t;
    out[0] = controlPoints[0].x * weights.x + controlPoints[1].x * weights.y + controlPoints[2].x * weights.z + controlPoints[3].x * weights.w;
    out[1] = controlPoints[0].y * weights.x + controlPoints[1].y * weights.y + controlPoints[2].y * weights.z + controlPoints[3].y * weights.w;
    out[2] = controlPoints[0].z * weights.x + controlPoints[1].z * weights.y + controlPoints[2].z * weights.z + controlPoints[3].z * weights.w;
    out[3] = 1.0f;
    t += slot->parameterStep;
    if (t > 1.0f) {
        t = 1.0f;
        pointIndex += 3;
    }
    slot->t = t;
    slot->pointIndex = pointIndex;
    return 1;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effDebug.c:129, match, same function 1.00 (words 79/79, calls 1.00, shape 1.00) */
s32 effPCPBezierEval(effBezierCurve_t *slot, f32 *out) {
    sdfVector4_t weights;
    u32 pointIndex = slot->pointIndex;
    f32 t = slot->t;
    effBezierPoint_t *controlPoints = &slot->controlPoints[pointIndex];
    f32 u = 1.0f - t;

    weights.x = u * u * u;
    weights.y = t * (u * u) * 3.0f;
    weights.z = t * t * u * 3.0f;
    weights.w = t * t * t;
    out[0] = controlPoints[0].x * weights.x + controlPoints[1].x * weights.y + controlPoints[2].x * weights.z + controlPoints[3].x * weights.w;
    out[1] = controlPoints[0].y * weights.x + controlPoints[1].y * weights.y + controlPoints[2].y * weights.z + controlPoints[3].y * weights.w;
    out[2] = controlPoints[0].z * weights.x + controlPoints[1].z * weights.y + controlPoints[2].z * weights.z + controlPoints[3].z * weights.w;
    out[3] = 1.0f;
    t += slot->parameterStep;
    if (t > 1.0f) {
        if (pointIndex < 3) {
            t -= 1.0f;
            pointIndex += 3;
        } else {
            slot->t = 1.0f;
            return 0;
        }
    }
    slot->pointIndex = pointIndex;
    slot->t = t;
    return 1;
}

/* body: matched in smt3 @ 9401f79eb: src/smt3/effect/effDebug.c:160, match, same function 0.99 (words 59/60, calls 1.00, shape 0.99) */
void effLeaf26(effBezierCurve_t *slot, f32 *out) {
    effBezierPoint_t *controlPoints = &slot->controlPoints[slot->pointIndex];
    f32 t = slot->t;
    f32 u = 1.0f - t;
    sdfVector4_t w;

    w.x = u * u * u;
    w.y = t * (u * u) * 3.0f;
    w.z = t * t * u * 3.0f;
    w.w = t * t * t;

    out[0] = controlPoints[0].x * w.x + controlPoints[1].x * w.y + controlPoints[2].x * w.z + controlPoints[3].x * w.w;
    out[1] = controlPoints[0].y * w.x + controlPoints[1].y * w.y + controlPoints[2].y * w.z + controlPoints[3].y * w.w;
    out[2] = controlPoints[0].z * w.x + controlPoints[1].z * w.y + controlPoints[2].z * w.z + controlPoints[3].z * w.w;
    out[3] = 1.0f;
}

/* body: matched in smt3 @ 9401f79eb: src/smt3/effect/effDebug.c:178, match, same function 1.00 (words 10/10, calls 1.00, shape 1.00) */
void effLeaf27(effArrHdr_t *table, s32 index) {
    effBezierCurve_t *slot = &((effBezierCurve_t *)table->slots)[index];

    slot->parameterStep = 0.05f;
    slot->pointIndex = slot->t = 0;
}

INCLUDE_ASM(const s32, "code_001E5A58", effLeaf28);

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effEvent.c:319, match, same function 1.00 (words 3/3, calls 1.00, shape 1.00) */
void effBlurUpdateSub(void) {
    Eve_BlurShow = 1;
}

void Eve_BlurOFF(void)
{
    Eve_BlurShow = 0;
}
