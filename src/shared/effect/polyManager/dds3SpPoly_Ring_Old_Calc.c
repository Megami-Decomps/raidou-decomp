#include "shared/effect/polyManager/decls.h"

void dds3SpPoly_Ring_Old_Calc(dds3SpPoly_Ring_Old_t3 *pRing) {
    u16 index;
    u16 completed;
    u16 count;
    u8 loop;
    s32 duration;
    f32 alphaStep;
    dds3TrackPolyList_t *entry;
    s32 *ages;

    if (pRing->active != 0) {
        duration = SDF_FRAMES(pRing->duration);
        completed = 0;
        count = pRing->entryCount;
        entry = pRing->hTrackPoly->trackList;
        alphaStep = 128.0f / (f32)duration;
        ages = pRing->ages;
        loop = pRing->loop;
        for (index = 0; index < count; index++) {
            s32 age = *ages;

            if (age == POLY_INACTIVE_ENTRY_AGE) {
                dds3SpPoly_Ring_Old_New(pRing, index);
                *ages = 0;
            } else if (age >= 0) {
                entry->color = ((u32)(alphaStep * (f32)(duration - age)) << 24) | 0x808080;
                dds3SpPoly_Ring_Old_Move(pRing, index);
            }
            if (age >= duration) {
                entry->color = 0x808080;
                if (loop != 0) {
                    *ages = POLY_INACTIVE_ENTRY_AGE;
                } else {
                    completed++;
                    if (completed >= count) {
                        entry->color = 0;
                        pRing->active = 0;
                    }
                }
            } else {
                (*ages)++;
            }
            ages++;
            entry++;
        }
    }
}
