#include "shared/effect/polyManager/decls.h"

void dds3SpPoly_Ring_Old_BuffInit(dds3SpPoly_Ring_Old_t3 *hData) {
    u16 count;
    s32 age;
    s32 *ages;
    u16 i;
    dds3TrackPoly_t *system;
    dds3TrackPolyList_t *cells;

    count = hData->entryCount;
    age = POLY_INACTIVE_ENTRY_AGE;
    ages = hData->ages;
    system = hData->hTrackPoly;
    i = 0;
    if (hData->entryCount != 0) {
        cells = system->trackList;
        do {
            cells[i].rec = 0;
            cells[i].count = 0;
            cells[i].color = 0x00808080;
            *ages = age;
            ages++;
            age -= SDF_FRAMES(hData->spawnDelayStep);
            i++;
        } while (i < count);
    }
}
