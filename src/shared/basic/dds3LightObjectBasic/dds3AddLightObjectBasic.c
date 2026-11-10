#include "shared/basic/dds3LightObjectBasic/decls.h"

dds3Basic_t *dds3AddLightObjectBasic(u32 serial)
{
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_LIGHTOBJECT);

    basic->serialID = serial;
    dds3ObjectBaseCreateMover(basic);
    return basic;
}
