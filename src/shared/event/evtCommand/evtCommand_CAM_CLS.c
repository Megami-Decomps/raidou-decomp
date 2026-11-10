#include "shared/event/evtCommand/decls.h"

s32 evtCommand_CAM_CLS(void)
{
    s32 key;
    dds3Basic_t *camera;

    key = scrGetIntPara(0);
    camera = evtSearchBasic(4, key);
    if (camera == NULL) {
        return 1;
    }
    dds3DeleteSceneBasicData(camera);
    return 1;
}
