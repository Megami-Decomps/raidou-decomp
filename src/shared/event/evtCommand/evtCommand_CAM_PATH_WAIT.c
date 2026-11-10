#include "shared/event/evtCommand/decls.h"

s32 evtCommand_CAM_PATH_WAIT(void)
{
    dds3Basic_t *camera;
    s32 stat;

    if (scrGetIntPara(0) < 0) {
        camera = fldGetUnitFollowCamera();
    } else {
        camera = evtSearchBasic(4, scrGetIntPara(0));
    }
    if (camera == NULL) {
        return 1;
    }
    stat = evtCheckPathStat(camera);
    switch (stat) {
    case 0:
        return 1;
    case 1:
        return 0;
    case -1:
        return 1;
    default:
        return 0;
    }
}
