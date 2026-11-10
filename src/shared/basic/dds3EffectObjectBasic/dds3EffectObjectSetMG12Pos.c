#include "shared/basic/dds3EffectObjectBasic/decls.h"

/* Missing parameter sources leave their existing position and radius intact. */
s32 dds3EffectObjectSetMG12Pos(dds3Basic_t *eff, dds3Basic_t *p1,
                              dds3Basic_t *p2, dds3Basic_t *p3, dds3Basic_t *p4)
{
    dds3EffectObjectBasicWorkData_t *work;
    effMagaSCArea_t *area;
    effMagaP4Point_t *points;

    if (eff->type >> 24 != EFF_OBJ_KIND) {
        return 0;
    }
    work = eff->workdata;
    switch (work->type) {
    case EFF_OBJ_STATE_MAGATUHI_TWO_ROWS:
        area = work->para;
        if (GetESPPos(p1)) {
            SDF_VU0_STOREVECTOR(&area->position[0]);
            area->radius[0] = GetESPR(p1);
        }
        if (GetESPPos(p2)) {
            SDF_VU0_STOREVECTOR(&area->position[1]);
            area->radius[1] = GetESPR(p2);
        }
        effMagaSCSetArea(work->handle, area);
        return 1;
    case EFF_OBJ_STATE_MAGATUHI_FOUR_ROWS:
        points = work->para;
        if (GetESPPos(p1)) {
            SDF_VU0_STOREVECTOR(&points->position[0]);
            points->radius[0] = GetESPR(p1);
        }
        if (GetESPPos(p2)) {
            SDF_VU0_STOREVECTOR(&points->position[1]);
            points->radius[1] = GetESPR(p2);
        }
        if (GetESPPos(p3)) {
            SDF_VU0_STOREVECTOR(&points->position[2]);
            points->radius[2] = GetESPR(p3);
        }
        if (GetESPPos(p4)) {
            SDF_VU0_STOREVECTOR(&points->position[3]);
            points->radius[3] = GetESPR(p4);
        }
        effMagaP4SetPoint(work->handle, points);
        return 1;
    }
    return 0;
}
