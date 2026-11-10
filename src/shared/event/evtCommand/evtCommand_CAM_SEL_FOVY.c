#include "shared/event/evtCommand/decls.h"

u32 evtCommand_CAM_SEL_FOVY(void)
{
    dds3Basic_t *camera;

    if (scrGetIntPara(0) < 0) {
        camera = fldGetUnitFollowCamera();
    } else {
        camera = evtSearchBasic(4, scrGetIntPara(0));
        *(u32 *)((u8 *)camera->workdata + 0x88) |= 1;
    }
    if (camera == NULL) {
        return 1;
    }
    dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), camera);
    return 1;
}
