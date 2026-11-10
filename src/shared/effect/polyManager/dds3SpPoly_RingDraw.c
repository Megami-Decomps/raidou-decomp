#include "shared/effect/polyManager/decls.h"

void dds3SpPoly_RingDraw(dds3SpPoly_Ring_t3 *hData) {
    s32 recordIndex = 0;
    u32 completed = 0;
    u8 loop;
    dds3TrackPoly_t *strip;
    s32 count;
    dds3SpPoly_RingPhysics_t *record;
    s32 duration;
    u32 color;
    dds3TrackPolyList_t *entry;

    strip = hData->hTrackPoly;
    count = hData->head.number;
    record = hData->pPhysics;
    duration = SDF_FRAMES(hData->head.frame);
    loop = hData->head.loop;
    color = hData->head.abgr;
    entry = strip->trackList;

    if (count > 0) {
        do {
            s32 age = record->frame;

            if (age == POLY_INACTIVE_ENTRY_AGE) {
                dds3SpPoly_RingBuffInit(hData, recordIndex);
                age = record->frame;
            }
            if (age < 0) {
                age++;
            } else {
                if ((u32)age < SDF_FRAMES(hData->head.alpha_s)) {
                    entry->color = dds3SpPoly_Color(age, SDF_FRAMES(hData->head.alpha_s), color);
                } else if (age <= duration &&
                           (u32)age >= duration - SDF_FRAMES(hData->head.alpha_e)) {
                    entry->color = dds3SpPoly_Color(duration - age, SDF_FRAMES(hData->head.alpha_e), color);
                }

                if (entry->color & 0xFF000000) {
                    dds3SpPoly_RingMove(hData, recordIndex);
                    age++;
                } else {
                    entry->count = 0;
                    age++;
                }
            }

            if (age >= duration) {
                if (loop != 0) {
                    age = POLY_INACTIVE_ENTRY_AGE;
                } else {
                    completed++;
                    if (completed >= (u32)count) {
                        hData->head.state = 0;
                    }
                }
                entry->count = 0;
            }

            record->frame = age;
            recordIndex++;
            entry++;
            record++;
        } while (recordIndex < count);
    }
    dds3TrackPolyDraw(hData->hTrackPoly);
}
