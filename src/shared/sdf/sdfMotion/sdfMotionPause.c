#include "shared/sdf/sdfMotion/decls.h"

void sdfMotionPause(sdfMotionCtrlr_t *aCtrlr)
{
    u8 stat;

    stat = aCtrlr->stat;
    if (stat != SDF_MOTION_STAT_PAUSE) {
        aCtrlr->statBeforePause = stat;
        aCtrlr->stat = SDF_MOTION_STAT_PAUSE;
    }
}
