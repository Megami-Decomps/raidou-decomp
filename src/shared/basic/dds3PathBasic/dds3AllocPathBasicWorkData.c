#include "shared/basic/dds3PathBasic/decls.h"

dds3PathBasicControl_t *dds3AllocPathBasicWorkData(dds3Basic_t *basic)
{
    dds3PathCurveTable_t *table = (dds3PathCurveTable_t *)basic->workdata;
    dds3PathCurveEntry_t *entry;
    dds3PathBasicControl_t *work;
    u32 last;
    u32 i;
    u32 count;

    if (table->count == 0) {
        return NULL;
    }
    work = sdfChipAllocDirect(sizeof(*work));
    entry = table->entries;
    memset(work, 0, sizeof(*work));
    work->direction = 0;
    work->frame = 0.0f;
    work->frameMax = 0.0f;
    count = table->count;
    for (i = 0; i < count; i++, entry++) {
        fldResourcePathData_t *keys = entry->keys;

        switch (entry->kind) {
        case DDS3_PATH_CURVE_SCALAR:
            work->pFovy = keys;
            work->flag |= DDS3_PATH_SCALAR_CHANNEL;
            count = table->count;
            break;
        case DDS3_PATH_CURVE_POSITION:
            {
                f32 (*points)[3] = keys->data.position;
                s32 spline;
                work->pTransrate = keys;
                work->flag |= DDS3_PATH_POSITION_CHANNEL;
                spline = dds3AllocSplineHandle(points, keys->count, 1);
                count = table->count;
                work->pSplineHandle = spline;
            }
            break;
        case DDS3_PATH_CURVE_ROTATION:
            work->pRotation = keys;
            work->flag |= DDS3_PATH_ROTATION_CHANNEL;
            count = table->count;
            break;
        case DDS3_PATH_CURVE_WORLD_TRANSFORM:
            work->pLight = keys;
            work->flag |= DDS3_PATH_WORLD_TRANSFORM_CHANNEL;
            count = table->count;
            break;
        }
        last = keys->frames[keys->count - 1];
        if (work->frameMax < (f32)last) {
            work->frameMax = (f32)last;
        }
    }
    return work;
}
