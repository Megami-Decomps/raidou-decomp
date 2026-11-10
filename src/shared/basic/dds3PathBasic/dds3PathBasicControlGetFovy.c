#include "shared/basic/dds3PathBasic/decls.h"

f32 dds3PathBasicControlGetFovy(dds3PathBasicControl_t *pControl)
{
    u32 segment;
    f32 weight;
    fldResourcePathData_t *keys;
    f32 *values;

    if (pControl->flag & DDS3_PATH_SCALAR_CHANNEL) {
        keys = pControl->pFovy;
        dds3PathEvalSpline(&segment, &weight, keys, pControl->frame);
        values = keys->data.scalar;
        return values[segment] * (1.0f - weight) + values[segment + 1] * weight;
    }
    return 0.0f;
}
