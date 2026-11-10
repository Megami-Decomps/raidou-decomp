#include "shared/basic/dds3PathBasic/decls.h"

void dds3PathBasicControlGetRot(dds3PathBasicControl_t *pControl)
{
    u32 segment;
    f32 weight;
    fldResourcePathData_t *keys;
    f32 (*quaternions)[4];
    f32 (*first)[4];
    f32 (*second)[4];

    if (pControl->flag & DDS3_PATH_ROTATION_CHANNEL) {
        keys = pControl->pRotation;
        dds3PathEvalSpline(&segment, &weight, keys, pControl->frame);
        quaternions = keys->data.rotation;
        first = &quaternions[segment];
        SDF_VU0_LOADVECTOR(first);
        second = &quaternions[segment] + 1;
        SDF_VU0_LOADVECTOR_B(second);
        Lerp(weight);
    } else {
        SDF_VU0_LOADIDENTITYVECTOR();
    }
}
