#include "shared/basic/dds3PathBasic/decls.h"

void dds3PathBasicControlGetPos(dds3PathBasicControl_t *pControl)
{
    u32 segment;
    f32 weight;
    fldResourcePathData_t *keys;
    f32 (*points)[3];

    if (pControl->flag & DDS3_PATH_POSITION_CHANNEL) {
        keys = pControl->pTransrate;
        dds3PathEvalSpline(&segment, &weight, keys, pControl->frame);
        points = keys->data.position;
        SDF_VU0_SETVECTOR_X(points[segment + 1][0]);
        SDF_VU0_SETVECTOR_Y(points[segment + 1][1]);
        SDF_VU0_SETVECTOR_Z(points[segment + 1][2]);
        SDF_VU0_SCALEVECTOR_V0_BARRIER(weight);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_SETVECTOR_X(points[segment][0]);
        SDF_VU0_SETVECTOR_Y(points[segment][1]);
        SDF_VU0_SETVECTOR_Z(points[segment][2]);
        SDF_VU0_SCALEVECTOR_V0_BARRIER(1.0f - weight);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_VECTOR_W_SET_ZERO();
    } else {
        SDF_VU0_LOADIDENTITYVECTOR();
    }
}
