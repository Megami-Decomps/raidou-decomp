#include "shared/basic/dds3CameraObjectBasic/decls.h"

dds3Basic_t *dds3AllocCameraObjectBasic(u32 serialID)
{
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_CAMERAOBJECT);

    basic->serialID = serialID;
    dds3ObjectBaseCreateMover(basic);
    return basic;
}
