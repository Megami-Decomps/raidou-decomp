#include "shared/effect/polyManager/decls.h"

dds3TrackPoly_t *dds3TrackPolyCreate(s32 number, s32 face, s32 rec, u32 type) {
    s32 total;
    sdfMemHandle_t *allocation;
    s32 base;
    s32 cellsSize;
    dds3TrackPoly_t *system;
    s32 i;

    if (type == TRACK_TOPOLOGY_FIVE_VECTOR) {
        face = face * 5 + 5;
    } else if (type == TRACK_TOPOLOGY_FOUR_VECTOR) {
        face = face * 4 + 4;
    } else if (type == TRACK_TOPOLOGY_SIX_VECTOR) {
        face = face * 6 + 6;
    } else {
        face = face * 2;
        if (rec != 0) {
            if (face % rec != 0) {
                face = face / rec + 2;
            } else {
                face = face / rec;
            }
        }
        face &= ~1;
        face += 2;
        if (type == TRACK_TOPOLOGY_TRIANGLE) {
            face += face >> 1;
        }
    }
    total = number * face;
    cellsSize = (total + number) * 0x14;
    allocation = sdfMemAllocDirect(cellsSize + 0x2C);
    base = sdfMemLock(allocation);
    system = EE_ADDR_AS(dds3TrackPoly_t *, (base + cellsSize));
    memset(system, 0, 0x2C);
    system->pVertex = EE_ADDR_AS(u128 *, base);
    base += total * 0x10;
    system->pVC = EE_ADDR_AS(u32 *, base);
    base += total * 4;
    system->trackList = EE_ADDR_AS(dds3TrackPolyList_t *, base);
    for (i = 0; i < number; i++) {
        dds3TrackPolyList_t *cell = (dds3TrackPolyList_t *)(i * sizeof(dds3TrackPolyList_t) + EE_ADDR_S(system->trackList));
        cell->pVertex = (u128 *)((u8 *)system->pVertex + i * face * 0x10);
        cell->pVC = system->pVC + i * face;
        dds3TrackPolyListInit(system, i);
    }
    system->pMat = sdfMatNew();
    sdfMatSetConstant(system->pMat, 1.0f);
    system->type = type;
    system->number = number;
    system->mode = 2;
    system->point = face;
    system->rec = rec;
    system->hMem = allocation;
    system->next = 0;
    system->d_next = 0;
    return system;
}
