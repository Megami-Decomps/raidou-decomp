#include "shared/effect/parManager/decls.h"

void dds3Particle_BlurDraw(dds3Particle_BlurBuff_t *hBlur) {
    billHandle_t *billboard;
    dds3Particle_BlurBuffList_t *slot;
    s32 remainingSlots;
    s32 pointCount;
    s32 remainingPoints;
    u32 color;
    u32 alpha;
    u32 alphaStep;
    u128 *point;

    billboard = *hBlur->pBill_Handle;
    slot = hBlur->blurList;

    if (hBlur->number != 0) {
        remainingSlots = hBlur->number;

        do {
            pointCount = slot->count;
            color = slot->abgr;
            point = slot->pPos;
            alpha = color & 0xFF000000;
            alphaStep = alpha / (pointCount + 1);

            dds3BillboardSetScale(billboard, slot->scale, slot->scale);
            color &= 0xFFFFFF;
            if (pointCount != 0) {
                remainingPoints = pointCount;
                do {
                    SDF_VU0_COPYVECTOR(billboard, point);
                    alpha -= alphaStep;
                    --remainingPoints;
                    billboard->abgr = color | (alpha & 0xFF000000);
                    ++point;
                    dds3BillboardDraw(billboard);
                } while (remainingPoints != 0);
            }
            --remainingSlots;
            ++slot;
        } while (remainingSlots != 0);
    }
}
