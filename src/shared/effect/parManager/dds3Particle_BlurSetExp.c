#include "shared/effect/parManager/decls.h"

void dds3Particle_BlurSetExp(dds3Particle_BlurBuff_t *hBlur, s32 index, const f32 *pPosBase, u32 abgr,
                   dds3ParOffsetBuff_t *pOffBuff, f32 scale) {
    sdfVector4_t localOrigin;
    dds3ParOffsetBuffParts_t *history;
    dds3Particle_BlurBuffList_t *slot;
    f32 (*destination)[4];
    s32 pointCount;
    s32 pointCapacity;
    s32 pointIndex;

    SDF_VU0_COPYVECTOR(&localOrigin, pPosBase);
    history = &pOffBuff->pParts[index];
    slot = &hBlur->blurList[index];
    pointCount = history->count;
    pointCapacity = pOffBuff->length;
    slot->count = pointCount;
    destination = (f32 (*)[4])slot->pPos;
    pointIndex = history->point;
    if (pointCount > 0) {
        f32 (*points)[3] = history->points;
        s32 lastIndex = pointCapacity - 1;
        s32 remaining = pointCount;

        do {
            --remaining;
            pointIndex = pointIndex == 0 ? lastIndex : pointIndex - 1;
            (*destination)[0] = localOrigin.x + points[pointIndex][0];
            (*destination)[1] = localOrigin.y + points[pointIndex][1];
            (*destination)[2] = localOrigin.z + points[pointIndex][2];
            ++destination;
        } while (remaining != 0);
    }
    slot->abgr = abgr;
    slot->scale = scale;
}
