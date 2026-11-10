#include "shared/sdf/sdfMotion/decls.h"

void sdfMotionPauseOff(sdfMotionCtrlr_t *aCtrlr)
{
    if (aCtrlr->stat == SDF_MOTION_STAT_PAUSE) {
        aCtrlr->stat = aCtrlr->statBeforePause;
    }
}
