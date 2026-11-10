#include "shared/basic/dds3PathBasic/decls.h"

s32 dds3PathBasicControlIsFinished(dds3PathBasicControl_t *work)
{
    s32 result = 1;

    if (work->flag & DDS3_PATHBASIC_FLAG_STOP) {
        return 1;
    }
    if (work->direction == DDS3_PATHBASIC_CONTROL_FORWARD) {
        if (work->frameMax > work->frame) {
            work->frame = SDF_FRAME_STEP_UP(work->frame, 1.0f, work->frameMax);
        } else if (work->flag & DDS3_PATHBASIC_FLAG_LOOP) {
            work->frame = 0.0f;
        } else {
            result = 0;
        }
    } else if (work->direction == DDS3_PATHBASIC_CONTROL_BACK) {
        if (work->frame > 0.0f) {
            work->frame = SDF_FRAME_STEP_DOWN(work->frame, 1.0f, 0.0f);
        } else if (work->flag & DDS3_PATHBASIC_FLAG_LOOP) {
            work->frame = work->frameMax;
        } else {
            result = 0;
        }
    }
    return result;
}
