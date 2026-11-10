#include "shared/event/evtCommand/decls.h"

s32 evtCommand_CAM_CREATE(void)
{
    sdfVector4_t pos;
    sdfVector4_t rot;
    s32 id;

    memset(&pos, 0, 0x10);
    memset(&rot, 0, 0x10);
    rot.w = 1.0f;
    id = dds3AllocObjectId();
    dds3GetRadiusPoseSet(dds3AddCameraObjectBasicTypeFree(id, &pos.x, &rot.x), 1.0f);
    scrSetIntReturnValue(id);
    return 1;
}
